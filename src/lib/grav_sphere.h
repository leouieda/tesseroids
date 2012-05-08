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

#ifndef _TESSEROIDS_GRAV_SPHERE_H_
#define _TESSEROIDS_GRAV_SPHERE_H_


/* Needed for definition of SPHERE */
#include "geometry.h"


/** Calculates potential caused by a sphere.

\f[
V(r_p,\phi_p,\lambda_p) = \frac{G M}{\ell}
\f]

The position of the sphere and computation point should be in spherical
coordinates.

<b>Input and output values in SI units and degrees</b>

@param sphere data structure describing the sphere
@param lonp longitude of the computation point P
@param latp latitude of the computation point P
@param rp radial coordinate of the computation point P

@return field calculated at P
*/
extern double sphere_pot(SPHERE sphere, double lonp, double latp, double rp);


/** Calculates gx caused by a sphere (Grombein et al., 2010).

\f[
g_x(r_p,\phi_p,\lambda_p) = G M \frac{r_c K_{\phi}}{\ell^3}
\f]

The position of the sphere and computation point should be in spherical
coordinates.

The derivatives of the potential are made with respect to the local coordinate
system <b>x->North, y->East, z->out</b>

<b>Input values in SI units and <b>degrees</b> and returns values in mGal!</b>

@param sphere data structure describing the sphere
@param lonp longitude of the computation point P
@param latp latitude of the computation point P
@param rp radial coordinate of the computation point P

@return field calculated at P
*/
extern double sphere_gx(SPHERE sphere, double lonp, double latp, double rp);


/** Calculates gy caused by a sphere (Grombein et al., 2010).

\f[
g_y(r_p,\phi_p,\lambda_p) = G M \frac{r_c\cos\phi_c\sin(\phi_c-\phi_p)}{\ell^3}
\f]

The position of the sphere and computation point should be in spherical
coordinates.

The derivatives of the potential are made with respect to the local coordinate
system <b>x->North, y->East, z->out</b>

<b>Input values in SI units and <b>degrees</b> and returns values in mGal!</b>

@param sphere data structure describing the sphere
@param lonp longitude of the computation point P
@param latp latitude of the computation point P
@param rp radial coordinate of the computation point P

@return field calculated at P
*/
extern double sphere_gy(SPHERE sphere, double lonp, double latp, double rp);


/** Calculates gz caused by a sphere (Grombein et al., 2010).

\f[
g_z(r_p,\phi_p,\lambda_p) = G M \frac{r_c\cos\psi - r_p}{\ell^3}
\f]

The position of the sphere and computation point should be in spherical
coordinates.

The derivatives of the potential are made with respect to the local coordinate
system <b>x->North, y->East, z->out</b>

<b>Input values in SI units and <b>degrees</b> and returns values in mGal!</b>

@param sphere data structure describing the sphere
@param lonp longitude of the computation point P
@param latp latitude of the computation point P
@param rp radial coordinate of the computation point P

@return field calculated at P
*/
extern double sphere_gz(SPHERE sphere, double lonp, double latp, double rp);


/** Calculates gxx caused by a sphere (Grombein et al., 2010).

\f[
g_{xx}(r_p,\phi_p,\lambda_p) = G M \frac{3(r_c K_{\phi})^2 - \ell^2}{\ell^5}
\f]

The position of the sphere and computation point are in spherical coordinates.

The derivatives of the potential are made with respect to the local coordinate
system <b>x->North, y->East, z->out</b>

<b>Input values in SI units and <b>degrees</b> and returns values in Eotvos!</b>

@param sphere data structure describing the sphere
@param lonp longitude of the computation point P
@param latp latitude of the computation point P
@param rp radial coordinate of the computation point P

@return  field calculated at P
*/
extern double sphere_gxx(SPHERE sphere, double lonp, double latp, double rp);


/** Calculates gxy caused by a sphere (Grombein et al., 2010).

\f[
g_{xy}(r_p,\phi_p,\lambda_p) = G M \frac{3r_c^2 K_{\phi}\cos\phi_c
    \sin(\lambda_c - \lambda_p)}{\ell^5}
\f]

The position of the sphere and computation point are in spherical coordinates.

The derivatives of the potential are made with respect to the local coordinate
system <b>x->North, y->East, z->out</b>

<b>Input values in SI units and <b>degrees</b> and returns values in Eotvos!</b>

@param sphere data structure describing the sphere
@param lonp longitude of the computation point P
@param latp latitude of the computation point P
@param rp radial coordinate of the computation point P

@return  field calculated at P
*/
extern double sphere_gxy(SPHERE sphere, double lonp, double latp, double rp);


/** Calculates gxz caused by a sphere (Grombein et al., 2010).

\f[
g_{xz}(r_p,\phi_p,\lambda_p) = G M \frac{3 r_c K_{\phi}(r_c \cos\psi - r_p)}
    {\ell^5}
\f]

The position of the sphere and computation point are in spherical coordinates.

The derivatives of the potential are made with respect to the local coordinate
system <b>x->North, y->East, z->out</b>

<b>Input values in SI units and <b>degrees</b> and returns values in Eotvos!</b>

@param sphere data structure describing the sphere
@param lonp longitude of the computation point P
@param latp latitude of the computation point P
@param rp radial coordinate of the computation point P

@return  field calculated at P
*/
extern double sphere_gxz(SPHERE sphere, double lonp, double latp, double rp);


/** Calculates gyy caused by a sphere (Grombein et al., 2010).

\f[
g_{yy}(r_p,\phi_p,\lambda_p) = G M \frac{3(r_c\cos\phi_c
    \sin(\lambda_c - \lambda_p))^2 - \ell^2}{\ell^5}
\f]

The position of the sphere and computation point are in spherical coordinates.

The derivatives of the potential are made with respect to the local coordinate
system <b>x->North, y->East, z->out</b>

<b>Input values in SI units and <b>degrees</b> and returns values in Eotvos!</b>

@param sphere data structure describing the sphere
@param lonp longitude of the computation point P
@param latp latitude of the computation point P
@param rp radial coordinate of the computation point P

@return  field calculated at P
*/
extern double sphere_gyy(SPHERE sphere, double lonp, double latp, double rp);


/** Calculates gyz caused by a sphere (Grombein et al., 2010).

\f[
g_{yz}(r_p,\phi_p,\lambda_p) = G M \frac{3 r_c \cos\phi_c \sin(\lambda_c -
    \lambda_p)(r_c\cos\psi - r_p)}{\ell^5}
\f]

The position of the sphere and computation point are in spherical coordinates.

The derivatives of the potential are made with respect to the local coordinate
system <b>x->North, y->East, z->out</b>

<b>Input values in SI units and <b>degrees</b> and returns values in Eotvos!</b>

@param sphere data structure describing the sphere
@param lonp longitude of the computation point P
@param latp latitude of the computation point P
@param rp radial coordinate of the computation point P

@return  field calculated at P
*/
extern double sphere_gyz(SPHERE sphere, double lonp, double latp, double rp);


/** Calculates gzz caused by a sphere (Grombein et al., 2010).

\f[
g_{zz}(r_p,\phi_p,\lambda_p) = G M \frac{3(r_c\cos\psi-r_p)^2 - \ell^2}{\ell^5}
\f]

The position of the sphere and computation point are in spherical coordinates.

The derivatives of the potential are made with respect to the local coordinate
system <b>x->North, y->East, z->out</b>

<b>Input values in SI units and <b>degrees</b> and returns values in Eotvos!</b>

@param sphere data structure describing the sphere
@param lonp longitude of the computation point P
@param latp latitude of the computation point P
@param rp radial coordinate of the computation point P

@return  field calculated at P
*/
extern double sphere_gzz(SPHERE sphere, double lonp, double latp, double rp);


#endif
