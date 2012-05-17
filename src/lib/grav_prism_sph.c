/*
Functions that calculate the gravitational potential and its first and second
derivatives for the rectangular prism in spherical coordinates.
Uses the formulas in Nagy et al. (2000).

References
----------

* Nagy, D., Papp, G., Benedek, J. (2000): The gravitational potential and its
  derivatives for the prism. Journal of Geodesy, 74, 552–560.
*/


#include <math.h>
#include "geometry.h"
#include "constants.h"
#include "grav_prism_sph.h"
#include "grav_prism.h"


/* Transform spherical coordinates to local Cartesian coordinates of the prism*/
int global2local(double lon, double lat, double r, PRISM prism, double *x,
                 double *y, double *z)
{
    double cosa, cosb, sina, sinb, d2r, X, Y, Z;

    /* degrees to radians */
    d2r = PI/180.;

    X = r*cos(d2r*lat)*cos(d2r*lon) -
        prism.r*cos(d2r*prism.lat)*cos(d2r*prism.lon);
    Y = r*cos(d2r*lat)*sin(d2r*lon) -
        prism.r*cos(d2r*prism.lat)*sin(d2r*prism.lon);
    Z = r*sin(d2r*lat) - prism.r*sin(d2r*prism.lat);

    cosa = cos(d2r*(90 - prism.lat));
    sina = sin(d2r*(90 - prism.lat));
    cosb = cos(d2r*(180 - prism.lon));
    sinb = sin(d2r*(180 - prism.lon));

    *x = X*cosa*cosb - Y*cosa*sinb + Z*sina;
    *y = -X*sinb - Y*cosb;
    /* -1 because Nagy et al. (2000) use z->down */
    *z = -1*(-X*sina*cosb + Y*sina*sinb + Z*cosa);
    
    return 0;
}


/* Rotate the gravity vector from the prisms coordinate system to the local
system of the computation point. */
int g_prism2point(double *atprism, PRISM prism, double lon, double lat,
                  double r, double *atpoint)
{    
    #define POS(x, y, cols) (((x)*(cols))+(y))

    register int i, k;
    double R[9], d2r, cosbeta, sinbeta, cosphi, sinphi, cosphil, sinphil;

    /* degrees to radians */
    d2r = PI/180.;
    
    cosbeta = cos(d2r*(prism.lon - lon));
    sinbeta = sin(d2r*(prism.lon - lon));
    cosphi = cos(d2r*lat);
    sinphi = sin(d2r*lat);
    cosphil = cos(d2r*prism.lat);
    sinphil = sin(d2r*prism.lat);

    /* The transformation matrix */
    R[0] = cosbeta*sinphi*sinphil + cosphi*cosphil;
    R[1] = sinbeta*sinphi;
    R[2] = -cosbeta*sinphi*cosphil + cosphi*sinphil;
    R[3] = -sinbeta*sinphil;
    R[4] = cosbeta;
    R[5] = sinbeta*cosphil;
    R[6] = -cosbeta*cosphi*sinphil + sinphi*cosphil;
    R[7] = -sinbeta*cosphi;
    R[8] = cosbeta*cosphi*cosphil + sinphi*sinphil;

    /* Matrix-vector multiplication */
    for(i = 0; i < 3; i++)
    {
       atpoint[i] = 0;
       for(k = 0; k < 3; k++)
       {
           atpoint[i] += R[POS(i, k, 3)]*atprism[k];
       }
    }
    #undef POS
    return 0;
}


/* Rotate the gravity tensor from the prisms coordinate system to the local
system of the computation point. */
int ggt_prism2point(double *atprism, PRISM prism, double lon, double lat,
                    double r, double *atpoint)
{
    #define POS(x, y, cols) (((x)*(cols))+(y))

    register int i, j, k;
    double R[9], tmp[9], d2r, cosbeta, sinbeta, cosphi, sinphi, cosphil, sinphil;

    /* degrees to radians */
    d2r = PI/180.;
    
    cosbeta = cos(d2r*(prism.lon - lon));
    sinbeta = sin(d2r*(prism.lon - lon));
    cosphi = cos(d2r*lat);
    sinphi = sin(d2r*lat);
    cosphil = cos(d2r*prism.lat);
    sinphil = sin(d2r*prism.lat);

    /* The transformation matrix */
    R[0] = cosbeta*sinphi*sinphil + cosphi*cosphil;
    R[1] = sinbeta*sinphi;
    R[2] = -cosbeta*sinphi*cosphil + cosphi*sinphil;
    R[3] = -sinbeta*sinphil;
    R[4] = cosbeta;
    R[5] = sinbeta*cosphil;
    R[6] = -cosbeta*cosphi*sinphil + sinphi*cosphil;
    R[7] = -sinbeta*cosphi;
    R[8] = cosbeta*cosphi*cosphil + sinphi*sinphil;

    /* Multiply tmp = R*Tensor */
    for(i = 0; i < 3; i++)
    {
        for(j = 0; j < 3; j++)
        {
            tmp[POS(i, j, 3)] = 0;
            for(k = 0; k < 3; k++)
            {
                tmp[POS(i, j, 3)] += R[POS(i, k, 3)]*atprism[POS(k, j, 3)];
            }
        }
    }

    /* Multiply tmp*R^T */
    for(i = 0; i < 3; i++)
    {
        for(j = 0; j < 3; j++)
        {
            atpoint[POS(i, j, 3)] = 0;
            for(k = 0; k < 3; k++)
            {
                atpoint[POS(i, j, 3)] += tmp[POS(i, k, 3)]*R[POS(j, k, 3)];
            }
        }
    }
    
    #undef POS
    return 0;
}


/* Calculates the gravity gradient tensor caused by a prism. */
int prism_ggt_sph(PRISM prism, double lonp, double latp, double rp, double *ggt)
{
    double x = 0, y = 0, z = 0, ggtprism[9], ggtpoint[9];

    global2local(lonp, latp, rp, prism, &x, &y, &z);
    ggtprism[0] = prism_gxx(prism, x, y, z);
    ggtprism[1] = prism_gxy(prism, x, y, z);
    /* -1 because the prisms z is Down, but transformation assumes z is Up */
    /* z -> Up is the system of the tesseroid */
    ggtprism[2] = -1*prism_gxz(prism, x, y, z);
    ggtprism[3] = ggtprism[1];
    ggtprism[4] = prism_gyy(prism, x, y, z);
    /* Same as xz */
    ggtprism[5] = -1*prism_gyz(prism, x, y, z);
    ggtprism[6] = ggtprism[2];
    ggtprism[7] = ggtprism[5];
    ggtprism[8] = -(ggtprism[0] + ggtprism[4]);
    ggt_prism2point(ggtprism, prism, lonp, latp, rp, ggtpoint);
    ggt[0] = ggtpoint[0];
    ggt[1] = ggtpoint[1];
    ggt[2] = ggtpoint[2];
    ggt[3] = ggtpoint[4];
    ggt[4] = ggtpoint[5];
    ggt[5] = ggtpoint[8];

    return 0;
}


/* Calculates the gravitational attraction caused by a prism. */
int prism_g_sph(PRISM prism, double lonp, double latp, double rp, double *gx,
                double *gy, double *gz)
{
    double x = 0, y = 0, z = 0, gprism[3], gpoint[3];

    global2local(lonp, latp, rp, prism, &x, &y, &z);
    gprism[0] = prism_gx(prism, x, y, z);
    gprism[1] = prism_gy(prism, x, y, z);
    /* Nagy wants z down, but the transformation assumes z up */
    gprism[2] = -prism_gz(prism, x, y, z);
    g_prism2point(gprism, prism, lonp, latp, rp, gpoint);
    *gx = gpoint[0];
    *gy = gpoint[1];
    /* Put z back down again to maintain the normal convention for gz */
    *gz = -gpoint[2];

    return 0;
}

/* Calculates the potential caused by a prism. */
double prism_pot_sph(PRISM prism, double lonp, double latp, double rp)
{
    double x = 0, y = 0, z = 0, res;

    global2local(lonp, latp, rp, prism, &x, &y, &z);
    res = prism_pot(prism, x, y, z);

    return res;
}



