/* *****************************************************************************
 Copyright 2011 Leonardo Uieda

 Tesseroids is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 Tesseroids is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with Tesseroids.  If not, see <http://www.gnu.org/licenses/>.
 **************************************************************************** */

/** \file
Functions that calculate the gravitational potential and its first and second
derivatives for the sphere in spherical coordinates.

The position of the sphere and computation point are in spherical coordinates.

The derivatives of the potential are made with respect to the local coordinate
system <b>x->North, y->East, z->out</b>. So it would be normal for a sphere of
positive density to have negative gz.

Used the generic formula for gravity gradient computation:

\f[
g_{ij}(r_p,\phi_p,\lambda_p) = G M \left(\frac{3\Delta x_i \Delta x_j}{\ell^5} -
    \frac{\delta_{ij}}{\ell^3} \right)\ \ i,j \in \{1,2,3\}
\f]

where M is the mass of the sphere, the subscripts 1, 2, and 3 should be
interpreted as the x, y, and z axis and

\f{eqnarray*}{
\Delta x_1 &=& r_c K_{\phi} \\
\Delta x_2 &=& r_c \cos \phi_c \sin(\lambda_c - \lambda_p) \\
\Delta x_3 &=& r_c \cos \psi - r_p\\
\ell &=& \sqrt{r_c^2 + r_p^2 - 2 r_c r_p \cos \psi} \\
\cos\psi &=& \sin\phi_p\sin\phi_c + \cos\phi_p\cos\phi_c
             \cos(\lambda_c - \lambda_p) \\
K_{\phi} &=& \cos\phi_p\sin\phi_c - \sin\phi_p\cos\phi_c
             \cos(\lambda_c - \lambda_p)
\f}

\f$ \phi \f$ is latitude, \f$ \lambda \f$ is longitude, \f$ r \f$ is radius. The
subscript \f$ c \f$ is for the center of the sphere and \f$ p \f$ for the
computation point.

\todo Possible speedup: Replace sphere.rc with a local copy
\todo Put reference for formulas

@author Leonardo Uieda
@date 25 Jan 2011
*/

#ifndef _TESSEROIDS_GRAV_SPHERE_H_
#define _TESSEROIDS_GRAV_SPHERE_H_


/* Needed for definition of SPHERE */
#include "utils.h"


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


/** Calculates gx caused by a sphere.

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


/** Calculates gy caused by a sphere.

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


/** Calculates gz caused by a sphere.

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


/** Calculates gxx caused by a sphere.

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


/** Calculates gxy caused by a sphere.

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


/** Calculates gxz caused by a sphere.

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


/** Calculates gyy caused by a sphere.

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


/** Calculates gyz caused by a sphere.

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


/** Calculates gzz caused by a sphere.

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
