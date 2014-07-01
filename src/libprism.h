/*
Functions that calculate the gravitational potential and its first and second
derivatives for the rectangular prism using the formulas in Nagy et al. (2000).
Supports Cartesian coordinates and spherical coordinates.

The coordinate system used is that of the article, ie:

x -> North  y -> East  z -> Down

References
----------

* Nagy, D., Papp, G., Benedek, J. (2000): The gravitational potential and its
  derivatives for the prism. Journal of Geodesy, 74, 552–560.
*/


#ifndef _TESSEROIDS_LIBPRISM_H_
#define _TESSEROIDS_LIBPRISM_H_


/* Needed for definition of PRISM */
#include "geometry.h"

extern double prism_pot(PRISM prism, double xp, double yp, double zp);
extern double prism_gx(PRISM prism, double xp, double yp, double zp);
extern double prism_gy(PRISM prism, double xp, double yp, double zp);
extern double prism_gz(PRISM prism, double xp, double yp, double zp);
extern double prism_gxx(PRISM prism, double xp, double yp, double zp);
extern double prism_gxy(PRISM prism, double xp, double yp, double zp);
extern double prism_gxz(PRISM prism, double xp, double yp, double zp);
extern double prism_gyy(PRISM prism, double xp, double yp, double zp);
extern double prism_gyz(PRISM prism, double xp, double yp, double zp);
extern double prism_gzz(PRISM prism, double xp, double yp, double zp);

/* Transform spherical coordinates to local Cartesian coords of the prism */
extern int global2local(double lon, double lat, double r, PRISM prism,
                        double *x, double *y, double *z);
/* Rotate the g vector from prism coordinates to the local system of the
computation point.
* atprism: the 3 component gravity vector in the coordinates of the prism.
* prism: the prism used to calculate atprism.
* lon, lat, r: coordinates of the computation point.
* atpoint: used to return the 3 component gravity vector in the coordinates of
           the computation point.
*/
extern int rotate_gravity(double *atprism, PRISM prism, double lon, double lat,
                          double r, double *atpoint);
/* Rotate the gravity gradient tensor from prism coordinates to the local
system of the computation point.
* atprism: the 9 component gravity tensor in the coordinates of the prism.
           The order is: gxx, gxy, gxz, gyx, gyy, gyz, gzx, gzy, gzz
* prism: the prism used to calculate atprism.
* lon, lat, r: coordinates of the computation point.
* atpoint: used to return the 9 component gravity tensor in the coordinates of
           the computation point.
*/
extern int rotate_tensor(double *atprism, PRISM prism, double lon, double lat,
                         double r, double *atpoint);
/* The following calculate the potential, gravity vector and gravity gradient
tensor for the prism in spherical coordinates.
The components of the tensor and vector have to be calculated simultaneously
for the rotation. */
extern double prism_pot_sph(PRISM prism, double lonp, double latp, double rp);
extern int prism_g_sph(PRISM prism, double lonp, double latp, double rp,
                       double *gx, double *gy, double *gz);
extern int prism_ggt_sph(PRISM prism, double lonp, double latp, double rp,
                         double *ggt);

#endif
