/*
Functions that calculate the gravitational potential and its first and second
derivatives for the sphere in spherical coordinates.

The position of the sphere and computation point are in spherical coordinates.

The derivatives of the potential are made with respect to the local coordinate
system x->North, y->East, z->out. So it would be normal for a sphere of
positive density to have negative gz.

Used the generic formula for gravity gradient computation of tesseroids by
Grombein et al. (2010).

References
----------

* Grombein, T.; Seitz, K.; Heck, B. (2010): Untersuchungen zur effizienten
Berechnung topographischer Effekte auf den Gradiententensor am Fallbeispiel der
Satellitengradiometriemission GOCE.
KIT Scientific Reports 7547, ISBN 978-3-86644-510-9, KIT Scientific Publishing,
Karlsruhe, Germany.
*/

#ifndef _TESSEROIDS_LIBSPHERE_H_
#define _TESSEROIDS_LIBSPHERE_H_


/* Needed for definition of SPHERE */
#include "geometry.h"

extern double sphere_pot(SPHERE sphere, double lonp, double latp, double rp);
extern double sphere_gx(SPHERE sphere, double lonp, double latp, double rp);
extern double sphere_gy(SPHERE sphere, double lonp, double latp, double rp);
extern double sphere_gz(SPHERE sphere, double lonp, double latp, double rp);
extern double sphere_gxx(SPHERE sphere, double lonp, double latp, double rp);
extern double sphere_gxy(SPHERE sphere, double lonp, double latp, double rp);
extern double sphere_gxz(SPHERE sphere, double lonp, double latp, double rp);
extern double sphere_gyy(SPHERE sphere, double lonp, double latp, double rp);
extern double sphere_gyz(SPHERE sphere, double lonp, double latp, double rp);
extern double sphere_gzz(SPHERE sphere, double lonp, double latp, double rp);

#endif
