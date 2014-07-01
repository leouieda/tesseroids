/*
Functions that calculate the gravitational potential and its first and second
derivatives for the tesseroid.

The gravity gradients can be calculated using the general formula of
Grombein et al. (2010). The integrals are solved using the Gauss-Legendre
Quadrature rule (Asgharzadeh et al., 2007).

The derivatives of the potential are made with respect to the local coordinate
system x->North, y->East, z->Up (away from center of the Earth).

To maintain the standard convention, ONLY for component gz the z axis is
inverted, so a positive density results in positive gz.

Example
-------

To calculate the gzz component due to a tesseroid on a regular grid:

    #include <stdio.h>
    #include "constants.h"
    #include "libtesseroid.h"

    int main()
    {
        TESSEROID tess = {1000, 44, 46, -1, 1, MEAN_EARTH_RADIUS - 100000,
                          MEAN_EARTH_RADIUS};
        GLQ *glqlon, *glqlat, *glqr;
        double lon, lat, r = MEAN_EARTH_RADIUS + 1500000, res;
        int order = 8;

        glqlon = glq_new(order, tess.w, tess.e);
        glqlat = glq_new(order, tess.s, tess.n);
        glqr = glq_new(order, tess.r1, tess.r2);

        for(lat = 20; lat <= 70; lat += 0.5)
        {
            for(lon = -25; lon <= 25; lon += 0.5)
            {
                res = tess_gzz(tess, lon, lat, r, *glqlon, *glqlat, *glqr);
                printf("%g %g %g\n", lon, lat, res);
            }
        }

        glq_free(glqlon);
        glq_free(glqlat);
        glq_free(glqr);

        return 0;
    }

Gauss-Legendre Quadrature example
---------------------------------

To integrate the cossine function from 0 to 90 degrees:

    #include <stdio.h>
    #include <stdlib.h>
    #include <math.h>
    #include "libtesseroid.h"

    int main(){
        // Create a new glq structure
        GLQ *glq;
        double result = 0, a = 0, b = 0.5*3.14;
        int i;

        glq = glq_new(5, a, b);

        if(glq == NULL){
            printf("malloc error");
            return 1;
        }

        // Calculate the integral
        for(i = 0; i < glq->order; i++)
            result += glq->weights[i]*cos(glq->nodes[i]);

        // Need to multiply by a scale factor of the integration limits
        result *= 0.5*(b - a);

        printf("Integral of cossine from 0 to 90 degrees = %lf\n", result);

        // Free allocated memory
        glq_free(glq);

        return 0;
    }

References
----------

Asgharzadeh, M.F., von Frese, R.R.B., Kim, H.R., Leftwich, T.E. & Kim, J.W.
(2007): Spherical prism gravity effects by Gauss-Legendre quadrature integration.
Geophysical Journal International, 169, 1-11.

Grombein, T.; Seitz, K.; Heck, B. (2010): Untersuchungen zur effizienten
Berechnung topographischer Effekte auf den Gradiententensor am Fallbeispiel der
Satellitengradiometriemission GOCE.
KIT Scientific Reports 7547, ISBN 978-3-86644-510-9, KIT Scientific Publishing,
Karlsruhe, Germany.
*/

#ifndef _TESSEROIDS_LIBTESSEROID_H_
#define _TESSEROIDS_LIBTESSEROID_H_


/* Needed for definition of TESSEROID */
#include "geometry.h"

/* Store the nodes and weights needed for a GLQ integration */
typedef struct glq_struct
{
    int order; /**< order of the quadrature, ie number of nodes */
    double *nodes; /**< abscissas or discretization points of the quadrature */
    double *weights; /**< weighting coefficients of the quadrature */
    double *nodes_unscaled; /**< nodes in [-1,1] interval */
} GLQ;
/* Make a new GLQ structure and set all the parameters needed
WARNING: Don't forget to free the memory malloced by this function using
glq_free()!
lower and upper are the integration limits. */
extern GLQ * glq_new(int order, double lower, double upper);
/* Free the memory allocated to make a GLQ structure */
extern void glq_free(GLQ *glq);
/* Put the GLQ nodes to the integration limits IN PLACE.
Return code:
    - 0: if everything went OK
    - 1: if invalid order
    - 2: if NULL pointer for nodes or nodes_unscaled */
extern int glq_set_limits(double lower, double upper, GLQ *glq);
/* Calculates the GLQ nodes using glq_next_root.
Nodes will be in the [-1,1] interval. To convert them to the integration limits
use glq_scale_nodes
Return code:
    - 0: if everything went OK
    - 1: if invalid order
    - 2: if NULL pointer for nodes
    - 3: if number of maximum iterations was reached when calculating the root.
         This usually means that the desired accuracy was not achieved. Default
         desired accuracy is GLQ_MAXERROR. Default maximum iterations is
         GLQ_MAXIT. */
extern int glq_nodes(int order, double *nodes);
/* Calculate the next Legendre polynomial root given the previous root found.
Uses the root-finder algorithm of:
  Barrera-Figueroa, V., Sosa-Pedroza, J. and López-Bonilla, J., 2006,
  "Multiple root finder algorithm for Legendre and Chebyshev polynomials via
  Newton's method", 2006, Annales mathematicae et Informaticae, 33, pp 3-13
Return code:
    - 0: if everything went OK
    - 1: if order is not valid
    - 2: if root_index is not valid (negative)
    - 3: if number of maximum iterations was reached when calculating the root.
         This usually means that the desired accuracy was not achieved. Default
         desired accuracy is GLQ_MAXERROR. Default maximum iterations is
         GLQ_MAXIT. */
extern int glq_next_root(double initial, int root_index, int order,
                         double *roots);
/* Calculates the weighting coefficients for the GLQ integration.
Return code:
    - 0: if everything went OK
    - 1: if order is not valid
    - 2: if nodes is a NULL pointer
    - 3: if weights is a NULL pointer */
extern int glq_weights(int order, double *nodes, double *weights);


extern double calc_tess_model(TESSEROID *model, int size, double lonp,
    double latp, double rp, GLQ *glq_lon, GLQ *glq_lat, GLQ *glq_r,
    double (*field)(TESSEROID, double, double, double, GLQ, GLQ, GLQ));
/* Adaptatively calculate the field of a tesseroid model at a given point by
splitting the tesseroids if necessary to maintain GLQ stability. */
extern double calc_tess_model_adapt(TESSEROID *model, int size, double lonp,
    double latp, double rp, GLQ *glq_lon, GLQ *glq_lat, GLQ *glq_r,
    double (*field)(TESSEROID, double, double, double, GLQ, GLQ, GLQ),
    double ratio);
extern double tess_pot(TESSEROID tess, double lonp, double latp, double rp,
                       GLQ glq_lon, GLQ glq_lat, GLQ glq_r);
extern double tess_gx(TESSEROID tess, double lonp, double latp, double rp,
                      GLQ glq_lon, GLQ glq_lat, GLQ glq_r);
extern double tess_gy(TESSEROID tess, double lonp, double latp, double rp,
                      GLQ glq_lon, GLQ glq_lat, GLQ glq_r);
extern double tess_gz(TESSEROID tess, double lonp, double latp, double rp,
                      GLQ glq_lon, GLQ glq_lat, GLQ glq_r);
extern double tess_gxx(TESSEROID tess, double lonp, double latp, double rp,
                      GLQ glq_lon, GLQ glq_lat, GLQ glq_r);
extern double tess_gxy(TESSEROID tess, double lonp, double latp, double rp,
                      GLQ glq_lon, GLQ glq_lat, GLQ glq_r);
extern double tess_gxz(TESSEROID tess, double lonp, double latp, double rp,
                      GLQ glq_lon, GLQ glq_lat, GLQ glq_r);
extern double tess_gyy(TESSEROID tess, double lonp, double latp, double rp,
                      GLQ glq_lon, GLQ glq_lat, GLQ glq_r);
extern double tess_gyz(TESSEROID tess, double lonp, double latp, double rp,
                      GLQ glq_lon, GLQ glq_lat, GLQ glq_r);
extern double tess_gzz(TESSEROID tess, double lonp, double latp, double rp,
                      GLQ glq_lon, GLQ glq_lat, GLQ glq_r);
#endif
