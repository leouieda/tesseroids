/*
Functions that calculate the gravitational potential and its first and second
derivatives for the tesseroid.

The gravity gradients can be calculated using the general formula of
Grombein et al. (2010).
The integrals are solved using the Gauss-Legendre Quadrature rule
(Asgharzadeh et al., 2007).

The derivatives of the potential are made with respect to the local coordinate
system x->North, y->East, z->Up (away from center of the Earth).

To maintain the standard convention, only for component gz the z axis is
inverted, so a positive density results in positive gz.

Example
-------

To calculate the gzz component due to a tesseroid on a regular grid:

    #include <stdio.h>
    #include "glq.h"r
    #include "constants.h"
    #include "grav_tess.h"

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

#ifndef _TESSEROIDS_GRAV_TESS_H_
#define _TESSEROIDS_GRAV_TESS_H_


/* Needed for definition of TESSEROID */
#include "geometry.h"
/* Needed for definition of GLQ */
#include "glq.h"


/** Calculates the field of a tesseroid model at a given point.

Uses a function pointer to call one of the apropriate field calculating
functions:
    - tess_gx()
    - tess_gy()
    - tess_gz()
    - tess_gxx()
    - tess_gxy()
    - tess_gxz()
    - tess_gyy()
    - tess_gyz()
    - tess_gzz()

To pass a function pointer to a function use something like:

\verbatim
calc_tess_model(my_model, 10, 0, 10, 1, glqlon, glqlat, glqr, &tess_gx);
\endverbatim

This would calculate the gx effect of the model my_model with 10 tesseroids
at lon=0 lat=10 r=1.

Will re-use the same GLQ structures, and therefore the <b>same order, for all
the tesseroids</b>.

@param model TESSEROID array defining the model
@param size number of tesseroids in the model
@param lonp longitude of the computation point P
@param latp latitude of the computation point P
@param rp radial coordinate of the computation point P
@param glq_lon pointer to GLQ structure used for the longitudinal integration
@param glq_lat pointer to GLQ structure used for the latitudinal integration
@param glq_r pointer to GLQ structure used for the radial integration
@param field pointer to one of the field calculating functions

@return the sum of the fields of all the tesseroids in the model
*/
extern double calc_tess_model(TESSEROID *model, int size, double lonp,
    double latp, double rp, GLQ *glq_lon, GLQ *glq_lat, GLQ *glq_r,
    double (*field)(TESSEROID, double, double, double, GLQ, GLQ, GLQ));


/** Adaptatively calculate the field of a tesseroid model at a given point by
splitting the tesseroids if necessary to maintain GLQ stability.

See calc_tess_model() for more details.

Will re-use the same GLQ structures, and therefore the <b>same order, for all
the tesseroids</b>.

@param model TESSEROID array defining the model
@param size number of tesseroids in the model
@param lonp longitude of the computation point P
@param latp latitude of the computation point P
@param rp radial coordinate of the computation point P
@param glq_lon pointer to GLQ structure used for the longitudinal integration
@param glq_lat pointer to GLQ structure used for the latitudinal integration
@param glq_r pointer to GLQ structure used for the radial integration
@param field pointer to one of the field calculating functions
@param ratio distance-to-size ratio for doing adaptative resizing

@return the sum of the fields of all the tesseroids in the model
*/
extern double calc_tess_model_adapt(TESSEROID *model, int size, double lonp,
    double latp, double rp, GLQ *glq_lon, GLQ *glq_lat, GLQ *glq_r,
    double (*field)(TESSEROID, double, double, double, GLQ, GLQ, GLQ),
    int ratio);


/** Calculates potential caused by a tesseroid.

\f[
V(r_p,\phi_p,\lambda_p) = G \rho \displaystyle\int_{\lambda_1}^{\lambda_2}
    \displaystyle\int_{\phi_1}^{\phi_2} \displaystyle\int_{r_1}^{r_2}
    \frac{1}{\ell}\kappa \ d r' d \phi' d \lambda'
\f]

<b>Input and output values in SI units and degrees</b>!

Use function glq_new() to create the GLQ parameters required. The integration
limits should be set to:
    - glq_lon: lower = tess.w and upper = tess.e  (in degrees)
    - glq_lat: lower = tess.s and upper = tess.n  (in degrees)
    - glq_r: lower = tess.r1 and upper = tess.r2

@param tess data structure describing the tesseroid
@param lonp longitude of the computation point P
@param latp latitude of the computation point P
@param rp radial coordinate of the computation point P
@param glq_lon GLQ structure with the nodes, weights and integration limits set
    for the longitudinal integration
@param glq_lat GLQ structure with the nodes, weights and integration limits set
    for the latitudinal integration
@param glq_r GLQ structure with the nodes, weights and integration limits set
    for the radial integration

@return field calculated at P
*/
extern double tess_pot(TESSEROID tess, double lonp, double latp, double rp,
                       GLQ glq_lon, GLQ glq_lat, GLQ glq_r);


/** Calculates gx caused by a tesseroid (Grombein et al., 2010).

\f[
g_x(r_p,\phi_p,\lambda_p) = G \rho \displaystyle\int_{\lambda_1}^{\lambda_2}
    \displaystyle\int_{\phi_1}^{\phi_2} \displaystyle\int_{r_1}^{r_2}
    \frac{r'K_{\phi}}{\ell^3}\kappa \ d r' d \phi' d \lambda'
\f]

The derivatives of the potential are made with respect to the local coordinate
system <b>x->North, y->East, z->out</b>

<b>Input values in SI units and <b>degrees</b> and returns values in mGal!</b>

Use function glq_new() to create the GLQ parameters required. The integration
limits should be set to:
    - glq_lon: lower = tess.w and upper = tess.e  (in degrees)
    - glq_lat: lower = tess.s and upper = tess.n  (in degrees)
    - glq_r: lower = tess.r1 and upper = tess.r2

@param tess data structure describing the tesseroid
@param lonp longitude of the computation point P
@param latp latitude of the computation point P
@param rp radial coordinate of the computation point P
@param glq_lon GLQ structure with the nodes, weights and integration limits set
    for the longitudinal integration
@param glq_lat GLQ structure with the nodes, weights and integration limits set
    for the latitudinal integration
@param glq_r GLQ structure with the nodes, weights and integration limits set
    for the radial integration

@return field calculated at P
*/
extern double tess_gx(TESSEROID tess, double lonp, double latp, double rp,
                      GLQ glq_lon, GLQ glq_lat, GLQ glq_r);

/** Calculates gy caused by a tesseroid (Grombein et al., 2010).

\f[
g_y(r_p,\phi_p,\lambda_p) = G \rho \displaystyle\int_{\lambda_1}^{\lambda_2}
    \displaystyle\int_{\phi_1}^{\phi_2} \displaystyle\int_{r_1}^{r_2}
    \frac{r'\cos\phi'\sin(\lambda'-\lambda)}{\ell^3}\kappa
    \ d r' d \phi' d \lambda'
\f]

The derivatives of the potential are made with respect to the local coordinate
system <b>x->North, y->East, z->out</b>

<b>Input values in SI units and <b>degrees</b> and returns values in mGal!</b>

Use function glq_new() to create the GLQ parameters required. The integration
limits should be set to:
    - glq_lon: lower = tess.w and upper = tess.e  (in degrees)
    - glq_lat: lower = tess.s and upper = tess.n  (in degrees)
    - glq_r: lower = tess.r1 and upper = tess.r2

@param tess data structure describing the tesseroid
@param lonp longitude of the computation point P
@param latp latitude of the computation point P
@param rp radial coordinate of the computation point P
@param glq_lon GLQ structure with the nodes, weights and integration limits set
    for the longitudinal integration
@param glq_lat GLQ structure with the nodes, weights and integration limits set
    for the latitudinal integration
@param glq_r GLQ structure with the nodes, weights and integration limits set
    for the radial integration

@return field calculated at P
*/
extern double tess_gy(TESSEROID tess, double lonp, double latp, double rp,
                      GLQ glq_lon, GLQ glq_lat, GLQ glq_r);

/** Calculates gz caused by a tesseroid (Grombein et al., 2010).

\f[
g_z(r_p,\phi_p,\lambda_p) = G \rho \displaystyle\int_{\lambda_1}^{\lambda_2}
    \displaystyle\int_{\phi_1}^{\phi_2} \displaystyle\int_{r_1}^{r_2}
    \frac{r'\cos\psi - r_p}{\ell^3}\kappa \ d r' d \phi' d \lambda'
\f]

The derivatives of the potential are made with respect to the local coordinate
system <b>x->North, y->East, z->out</b>

<b>Input values in SI units and <b>degrees</b> and returns values in mGal!</b>

Use function glq_new() to create the GLQ parameters required. The integration
limits should be set to:
    - glq_lon: lower = tess.w and upper = tess.e  (in degrees)
    - glq_lat: lower = tess.s and upper = tess.n  (in degrees)
    - glq_r: lower = tess.r1 and upper = tess.r2

@param tess data structure describing the tesseroid
@param lonp longitude of the computation point P
@param latp latitude of the computation point P
@param rp radial coordinate of the computation point P
@param glq_lon GLQ structure with the nodes, weights and integration limits set
    for the longitudinal integration
@param glq_lat GLQ structure with the nodes, weights and integration limits set
    for the latitudinal integration
@param glq_r GLQ structure with the nodes, weights and integration limits set
    for the radial integration

@return field calculated at P
*/
extern double tess_gz(TESSEROID tess, double lonp, double latp, double rp,
                      GLQ glq_lon, GLQ glq_lat, GLQ glq_r);

/** Calculates gxx caused by a tesseroid (Grombein et al., 2010).

\f[
g_{xx}(r_p,\phi_p,\lambda_p) = G \rho \displaystyle\int_{\lambda_1}^{\lambda_2}
    \displaystyle\int_{\phi_1}^{\phi_2} \displaystyle\int_{r_1}^{r_2}
    \frac{3(r' K_{\phi})^2 - \ell^2}{\ell^5}\kappa \ d r' d \phi' d \lambda'
\f]

The derivatives of the potential are made with respect to the local coordinate
system <b>x->North, y->East, z->out</b>

<b>Input values in SI units and <b>degrees</b> and returns values in Eotvos!</b>

Use function glq_new() to create the GLQ parameters required. The integration
limits should be set to:
    - glq_lon: lower = tess.w and upper = tess.e  (in degrees)
    - glq_lat: lower = tess.s and upper = tess.n  (in degrees)
    - glq_r: lower = tess.r1 and upper = tess.r2

@param tess data structure describing the tesseroid
@param lonp longitude of the computation point P
@param latp latitude of the computation point P
@param rp radial coordinate of the computation point P
@param glq_lon GLQ structure with the nodes, weights and integration limits set
    for the longitudinal integration
@param glq_lat GLQ structure with the nodes, weights and integration limits set
    for the latitudinal integration
@param glq_r GLQ structure with the nodes, weights and integration limits set
    for the radial integration

@return field calculated at P
*/
extern double tess_gxx(TESSEROID tess, double lonp, double latp, double rp,
                      GLQ glq_lon, GLQ glq_lat, GLQ glq_r);

/** Calculates gxy caused by a tesseroid (Grombein et al., 2010).

\f[
g_{xy}(r_p,\phi_p,\lambda_p) = G \rho \displaystyle\int_{\lambda_1}^{\lambda_2}
    \displaystyle\int_{\phi_1}^{\phi_2} \displaystyle\int_{r_1}^{r_2}
    \frac{3{r'}^2 K_{\phi}\cos\phi'\sin(\lambda' - \lambda_p)}{\ell^5}
    \kappa \ d r' d \phi' d \lambda'
\f]

The derivatives of the potential are made with respect to the local coordinate
system <b>x->North, y->East, z->out</b>

<b>Input values in SI units and <b>degrees</b> and returns values in Eotvos!</b>

Use function glq_new() to create the GLQ parameters required. The integration
limits should be set to:
    - glq_lon: lower = tess.w and upper = tess.e  (in degrees)
    - glq_lat: lower = tess.s and upper = tess.n  (in degrees)
    - glq_r: lower = tess.r1 and upper = tess.r2

@param tess data structure describing the tesseroid
@param lonp longitude of the computation point P
@param latp latitude of the computation point P
@param rp radial coordinate of the computation point P
@param glq_lon GLQ structure with the nodes, weights and integration limits set
    for the longitudinal integration
@param glq_lat GLQ structure with the nodes, weights and integration limits set
    for the latitudinal integration
@param glq_r GLQ structure with the nodes, weights and integration limits set
    for the radial integration

@return field calculated at P
*/
extern double tess_gxy(TESSEROID tess, double lonp, double latp, double rp,
                      GLQ glq_lon, GLQ glq_lat, GLQ glq_r);

/** Calculates gxz caused by a tesseroid (Grombein et al., 2010).

\f[
g_{xz}(r_p,\phi_p,\lambda_p) = G \rho \displaystyle\int_{\lambda_1}^{\lambda_2}
    \displaystyle\int_{\phi_1}^{\phi_2} \displaystyle\int_{r_1}^{r_2}
    \frac{3 r' K_{\phi}(r' \cos\psi - r_p)}{\ell^5}\kappa
    \ d r' d \phi' d \lambda'
\f]

The derivatives of the potential are made with respect to the local coordinate
system <b>x->North, y->East, z->out</b>

<b>Input values in SI units and <b>degrees</b> and returns values in Eotvos!</b>

Use function glq_new() to create the GLQ parameters required. The integration
limits should be set to:
    - glq_lon: lower = tess.w and upper = tess.e  (in degrees)
    - glq_lat: lower = tess.s and upper = tess.n  (in degrees)
    - glq_r: lower = tess.r1 and upper = tess.r2

@param tess data structure describing the tesseroid
@param lonp longitude of the computation point P
@param latp latitude of the computation point P
@param rp radial coordinate of the computation point P
@param glq_lon GLQ structure with the nodes, weights and integration limits set
    for the longitudinal integration
@param glq_lat GLQ structure with the nodes, weights and integration limits set
    for the latitudinal integration
@param glq_r GLQ structure with the nodes, weights and integration limits set
    for the radial integration

@return field calculated at P
*/
extern double tess_gxz(TESSEROID tess, double lonp, double latp, double rp,
                      GLQ glq_lon, GLQ glq_lat, GLQ glq_r);

/** Calculates gyy caused by a tesseroid (Grombein et al., 2010).

\f[
g_{yy}(r_p,\phi_p,\lambda_p) = G \rho \displaystyle\int_{\lambda_1}^{\lambda_2}
    \displaystyle\int_{\phi_1}^{\phi_2} \displaystyle\int_{r_1}^{r_2}
    \frac{3(r'\cos\phi'\sin(\lambda' - \lambda_p))^2 - \ell^2}{\ell^5}
    \kappa \ d r' d \phi' d \lambda'
\f]

The derivatives of the potential are made with respect to the local coordinate
system <b>x->North, y->East, z->out</b>

<b>Input values in SI units and <b>degrees</b> and returns values in Eotvos!</b>

Use function glq_new() to create the GLQ parameters required. The integration
limits should be set to:
    - glq_lon: lower = tess.w and upper = tess.e  (in degrees)
    - glq_lat: lower = tess.s and upper = tess.n  (in degrees)
    - glq_r: lower = tess.r1 and upper = tess.r2

@param tess data structure describing the tesseroid
@param lonp longitude of the computation point P
@param latp latitude of the computation point P
@param rp radial coordinate of the computation point P
@param glq_lon GLQ structure with the nodes, weights and integration limits set
    for the longitudinal integration
@param glq_lat GLQ structure with the nodes, weights and integration limits set
    for the latitudinal integration
@param glq_r GLQ structure with the nodes, weights and integration limits set
    for the radial integration

@return field calculated at P
*/
extern double tess_gyy(TESSEROID tess, double lonp, double latp, double rp,
                      GLQ glq_lon, GLQ glq_lat, GLQ glq_r);

/** Calculates gyz caused by a tesseroid (Grombein et al., 2010).

\f[
g_{yz}(r_p,\phi_p,\lambda_p) = G \rho \displaystyle\int_{\lambda_1}^{\lambda_2}
    \displaystyle\int_{\phi_1}^{\phi_2} \displaystyle\int_{r_1}^{r_2}
    \frac{3 r' \cos\phi' \sin(\lambda' - \lambda_p)(r'\cos\psi - r_p)}{\ell^5}
    \kappa \ d r' d \phi' d \lambda'
\f]

The derivatives of the potential are made with respect to the local coordinate
system <b>x->North, y->East, z->out</b>

<b>Input values in SI units and <b>degrees</b> and returns values in Eotvos!</b>

Use function glq_new() to create the GLQ parameters required. The integration
limits should be set to:
    - glq_lon: lower = tess.w and upper = tess.e  (in degrees)
    - glq_lat: lower = tess.s and upper = tess.n  (in degrees)
    - glq_r: lower = tess.r1 and upper = tess.r2

@param tess data structure describing the tesseroid
@param lonp longitude of the computation point P
@param latp latitude of the computation point P
@param rp radial coordinate of the computation point P
@param glq_lon GLQ structure with the nodes, weights and integration limits set
    for the longitudinal integration
@param glq_lat GLQ structure with the nodes, weights and integration limits set
    for the latitudinal integration
@param glq_r GLQ structure with the nodes, weights and integration limits set
    for the radial integration

@return field calculated at P
*/
extern double tess_gyz(TESSEROID tess, double lonp, double latp, double rp,
                      GLQ glq_lon, GLQ glq_lat, GLQ glq_r);

/** Calculates gzz caused by a tesseroid (Grombein et al., 2010).

\f[
g_{zz}(r_p,\phi_p,\lambda_p) = G \rho \displaystyle\int_{\lambda_1}^{\lambda_2}
    \displaystyle\int_{\phi_1}^{\phi_2} \displaystyle\int_{r_1}^{r_2}
    \frac{3(r'\cos\psi-r_p)^2 - \ell^2}{\ell^5}\kappa \ d r' d \phi' d \lambda'
\f]

The derivatives of the potential are made with respect to the local coordinate
system <b>x->North, y->East, z->out</b>

<b>Input values in SI units and <b>degrees</b> and returns values in Eotvos!</b>

Use function glq_new() to create the GLQ parameters required. The integration
limits should be set to:
    - glq_lon: lower = tess.w and upper = tess.e  (in degrees)
    - glq_lat: lower = tess.s and upper = tess.n  (in degrees)
    - glq_r: lower = tess.r1 and upper = tess.r2

@param tess data structure describing the tesseroid
@param lonp longitude of the computation point P
@param latp latitude of the computation point P
@param rp radial coordinate of the computation point P
@param glq_lon GLQ structure with the nodes, weights and integration limits set
    for the longitudinal integration
@param glq_lat GLQ structure with the nodes, weights and integration limits set
    for the latitudinal integration
@param glq_r GLQ structure with the nodes, weights and integration limits set
    for the radial integration

@return field calculated at P
*/
extern double tess_gzz(TESSEROID tess, double lonp, double latp, double rp,
                      GLQ glq_lon, GLQ glq_lat, GLQ glq_r);

#endif
