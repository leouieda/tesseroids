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
    return 0;
}


/* Rotate the g vector or the ggt from the prisms coordinate system to the local
system of the computation point. */
int prism2point(double *atprism, int nc, PRISM prism, double lon, double lat,
                double r, double *atpoint)
{
    return 0;
}


/* Calculates the gravity gradient tensor caused by a prism. */
int prism_ggt_sph(PRISM prism, double lonp, double latp, double rp, double *ggt)
{
    double x = 0, y = 0, z = 0, ggtprism[9], ggtpoint[9];

    global2local(lonp, latp, rp, prism, &x, &y, &z);
    ggtprism[0] = prism_gxx(prism, x, y, z);
    ggtprism[1] = prism_gxy(prism, x, y, z);
    ggtprism[2] = prism_gxz(prism, x, y, z);    
    ggtprism[3] = ggtprism[1];
    ggtprism[4] = prism_gyy(prism, x, y, z);
    ggtprism[5] = prism_gyz(prism, x, y, z);    
    ggtprism[6] = ggtprism[2];
    ggtprism[7] = ggtprism[5];
    ggtprism[8] = -(ggtprism[0] + ggtprism[3]);
    prism2point(ggtprism, 3, prism, lonp, latp, rp, ggtpoint);
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
    gprism[2] = prism_gz(prism, x, y, z);
    prism2point(gprism, 1, prism, lonp, latp, rp, gpoint);
    *gx = gpoint[0];
    *gy = gpoint[1];
    *gz = gpoint[2];

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



