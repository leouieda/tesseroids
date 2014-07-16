/*
Functions that calculate the gravitational potential and its first and second
derivatives for the rectangular prism using the formulas in Nagy et al. (2000).

The coordinate system used is that of the article, ie:

x -> North  y -> East  z -> Down

References
----------

* Nagy, D., Papp, G., Benedek, J. (2000): The gravitational potential and its
  derivatives for the prism. Journal of Geodesy, 74, 552–560.
*/


#ifndef _TESSEROIDS_GRAV_PRISM_H_
#define _TESSEROIDS_GRAV_PRISM_H_


/* Needed for definition of PRISM */
#include "geometry.h"

extern double safe_atan2(double y, double x);
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

#endif
