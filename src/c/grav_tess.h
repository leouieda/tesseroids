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
derivatives for the tesseroid.

The gravity gradients can be calculated using the general formula:

\f[
g_{\alpha\beta}(r_p,\phi_p,\lambda_p) = G \rho \displaystyle\int_{\lambda_1}^{\lambda_2}
    \displaystyle\int_{\phi_1}^{\phi_2} \displaystyle\int_{r_1}^{r_2}
    I_{\alpha\beta}\ d r' d \phi' d \lambda'
    \ \ \alpha,\beta \in \{1,2,3\}
\f]

\f[
I_{\alpha\beta} = \left(\frac{3\Delta x_i \Delta x_j}{\ell^5} -
    \frac{\delta_{ij}}{\ell^3} \right)\kappa\
\f]

and solved using the Gauss-Legendre Quadrature rule:

\f[
g_{\alpha\beta}(r_p,\phi_p,\lambda_p) \approx G \rho \frac{(\lambda_2 - \lambda_1)
    (\phi_2 - \phi_1)(r_2 - r_1)}{8} \displaystyle\sum_{k=0}^{N_k - 1}
    \displaystyle\sum_{j=0}^{N_j - 1} \displaystyle\sum_{i=1}^{N_i - 1}
    I_{\alpha\beta}({r'}_i, {\phi'}_j, {\lambda'}_k )\kappa\ \ i,j \in \{1,2,3\}
\f]

where \f$ \rho \f$ is density, the subscripts 1, 2, and 3 should be
interpreted as the x, y, and z axis and

\f{eqnarray*}{
\Delta x_1 &=& r' K_{\phi} \\
\Delta x_2 &=& r' \cos \phi' \sin(\lambda' - \lambda_p) \\
\Delta x_3 &=& r' \cos \psi - r_p\\
\ell &=& \sqrt{r'^2 + r_p^2 - 2 r' r_p \cos \psi} \\
\cos\psi &=& \sin\phi_p\sin\phi' + \cos\phi_p\cos\phi'
             \cos(\lambda' - \lambda_p) \\
K_{\phi} &=& \cos\phi_p\sin\phi' - \sin\phi_p\cos\phi'
             \cos(\lambda' - \lambda_p)\\
\kappa &=& {r'}^2 \cos \phi'
\f}

\f$ \phi \f$ is latitude, \f$ \lambda \f$ is longitude, \f$ r \f$ is radius. The
subscript \f$ p \f$ is for the computation point.

The derivatives of the potential are made with respect to the local coordinate
system <b>x->North, y->East, z->out</b>. So it would be normal for a tesseroid of
positive density to have negative gz.

@author Leonardo Uieda
@date 27 Jan 2011
*/

#ifndef _GRAV_TESS_H_
#define _GRAV_TESS_H_


/** Calculates gz caused by a tesseroid.

\f[
g_z(r_p,\phi_p,\lambda_p) = G \rho \displaystyle\int_{\lambda_1}^{\lambda_2}
    \displaystyle\int_{\phi_1}^{\phi_2} \displaystyle\int_{r_1}^{r_2}
    \frac{r'\cos\psi - r_p}{\ell^3}\ d r' d \phi' d \lambda'
\f]

The derivatives of the potential are made with respect to the local coordinate
system <b>x->North, y->East, z->out</b>

<b>Input values in SI units and <b>degrees</b> and returns values in mGal!</b>

@param dens density of the tesseroid
@param s latitude of the southern border of the tesseroid
@param n latitude of the northern border of the tesseroid
@param w longitude of the western border of the tesseroid
@param e longitude of the eastern border of the tesseroid
@param r1 radial distance from the center of the Earth to the lower-most border
          of the tesseroid
@param r2 radial distance from the center of the Earth to the upper-most border
          of the tesseroid
@param lonp longitude of the computation point P
@param latp latitude of the computation point P
@param rp radial coordinate of the computation point P

@return field calculated at P
*/
extern double tess_gz(double dens, double s, double n, double w, double e,
                    double r1, double r2, double lonp, double latp, double rp);

/** Calculates gxx caused by a tesseroid.

\f[
g_{xx}(r_p,\phi_p,\lambda_p) = G \rho \displaystyle\int_{\lambda_1}^{\lambda_2}
    \displaystyle\int_{\phi_1}^{\phi_2} \displaystyle\int_{r_1}^{r_2}
    \frac{3(r' K_{\phi})^2 - \ell^2}{\ell^5}\ d r' d \phi' d \lambda'
\f]

The derivatives of the potential are made with respect to the local coordinate
system <b>x->North, y->East, z->out</b>

<b>Input values in SI units and <b>degrees</b> and returns values in Eotvos!</b>

@param dens density of the tesseroid
@param s latitude of the southern border of the tesseroid
@param n latitude of the northern border of the tesseroid
@param w longitude of the western border of the tesseroid
@param e longitude of the eastern border of the tesseroid
@param r1 radial distance from the center of the Earth to the lower-most border
          of the tesseroid
@param r2 radial distance from the center of the Earth to the upper-most border
          of the tesseroid
@param lonp longitude of the computation point P
@param latp latitude of the computation point P
@param rp radial coordinate of the computation point P

@return  field calculated at P
*/
extern double tess_gxx(double dens, double s, double n, double w, double e,
                    double r1, double r2, double lonp, double latp, double rp);

/** Calculates gxy caused by a tesseroid.

\f[
g_{xy}(r_p,\phi_p,\lambda_p) = G \rho \displaystyle\int_{\lambda_1}^{\lambda_2}
    \displaystyle\int_{\phi_1}^{\phi_2} \displaystyle\int_{r_1}^{r_2}
    \frac{3{r'}^2 K_{\phi}\cos\phi'\sin(\lambda' - \lambda_p)}{\ell^5}
    \ d r' d \phi' d \lambda'
\f]

The derivatives of the potential are made with respect to the local coordinate
system <b>x->North, y->East, z->out</b>

<b>Input values in SI units and <b>degrees</b> and returns values in Eotvos!</b>

@param dens density of the tesseroid
@param s latitude of the southern border of the tesseroid
@param n latitude of the northern border of the tesseroid
@param w longitude of the western border of the tesseroid
@param e longitude of the eastern border of the tesseroid
@param r1 radial distance from the center of the Earth to the lower-most border
          of the tesseroid
@param r2 radial distance from the center of the Earth to the upper-most border
          of the tesseroid
@param lonp longitude of the computation point P
@param latp latitude of the computation point P
@param rp radial coordinate of the computation point P

@return  field calculated at P
*/
extern double tess_gxy(double dens, double s, double n, double w, double e,
                    double r1, double r2, double lonp, double latp, double rp);

/** Calculates gxz caused by a tesseroid.

\f[
g_{xz}(r_p,\phi_p,\lambda_p) = G \rho \displaystyle\int_{\lambda_1}^{\lambda_2}
    \displaystyle\int_{\phi_1}^{\phi_2} \displaystyle\int_{r_1}^{r_2}
    \frac{3 r' K_{\phi}(r' \cos\psi - r_p)}{\ell^5}\ d r' d \phi' d \lambda'
\f]

The derivatives of the potential are made with respect to the local coordinate
system <b>x->North, y->East, z->out</b>

<b>Input values in SI units and <b>degrees</b> and returns values in Eotvos!</b>

@param dens density of the tesseroid
@param s latitude of the southern border of the tesseroid
@param n latitude of the northern border of the tesseroid
@param w longitude of the western border of the tesseroid
@param e longitude of the eastern border of the tesseroid
@param r1 radial distance from the center of the Earth to the lower-most border
          of the tesseroid
@param r2 radial distance from the center of the Earth to the upper-most border
          of the tesseroid
@param lonp longitude of the computation point P
@param latp latitude of the computation point P
@param rp radial coordinate of the computation point P

@return  field calculated at P
*/
extern double tess_gxz(double dens, double s, double n, double w, double e,
                    double r1, double r2, double lonp, double latp, double rp);

/** Calculates gyy caused by a tesseroid.

\f[
g_{yy}(r_p,\phi_p,\lambda_p) = G \rho \displaystyle\int_{\lambda_1}^{\lambda_2}
    \displaystyle\int_{\phi_1}^{\phi_2} \displaystyle\int_{r_1}^{r_2}
    \frac{3(r'\cos\phi'\sin(\lambda' - \lambda_p))^2 - \ell^2}{\ell^5}
    \ d r' d \phi' d \lambda'
\f]

The derivatives of the potential are made with respect to the local coordinate
system <b>x->North, y->East, z->out</b>

<b>Input values in SI units and <b>degrees</b> and returns values in Eotvos!</b>

@param dens density of the tesseroid
@param s latitude of the southern border of the tesseroid
@param n latitude of the northern border of the tesseroid
@param w longitude of the western border of the tesseroid
@param e longitude of the eastern border of the tesseroid
@param r1 radial distance from the center of the Earth to the lower-most border
          of the tesseroid
@param r2 radial distance from the center of the Earth to the upper-most border
          of the tesseroid
@param lonp longitude of the computation point P
@param latp latitude of the computation point P
@param rp radial coordinate of the computation point P

@return  field calculated at P
*/
extern double tess_gyy(double dens, double s, double n, double w, double e,
                    double r1, double r2, double lonp, double latp, double rp);

/** Calculates gyz caused by a tesseroid.

\f[
g_{yz}(r_p,\phi_p,\lambda_p) = G \rho \displaystyle\int_{\lambda_1}^{\lambda_2}
    \displaystyle\int_{\phi_1}^{\phi_2} \displaystyle\int_{r_1}^{r_2}
    \frac{3 r' \cos\phi' \sin(\lambda' - \lambda_p)(r'\cos\psi - r_p)}{\ell^5}
    \ d r' d \phi' d \lambda'
\f]

The derivatives of the potential are made with respect to the local coordinate
system <b>x->North, y->East, z->out</b>

<b>Input values in SI units and <b>degrees</b> and returns values in Eotvos!</b>

@param dens density of the tesseroid
@param s latitude of the southern border of the tesseroid
@param n latitude of the northern border of the tesseroid
@param w longitude of the western border of the tesseroid
@param e longitude of the eastern border of the tesseroid
@param r1 radial distance from the center of the Earth to the lower-most border
          of the tesseroid
@param r2 radial distance from the center of the Earth to the upper-most border
          of the tesseroid
@param lonp longitude of the computation point P
@param latp latitude of the computation point P
@param rp radial coordinate of the computation point P

@return  field calculated at P
*/
extern double tess_gyz(double dens, double s, double n, double w, double e,
                    double r1, double r2, double lonp, double latp, double rp);
   
/** Calculates gzz caused by a tesseroid.

\f[
g_{zz}(r_p,\phi_p,\lambda_p) = G \rho \displaystyle\int_{\lambda_1}^{\lambda_2}
    \displaystyle\int_{\phi_1}^{\phi_2} \displaystyle\int_{r_1}^{r_2}
    \frac{3(r'\cos\psi-r_p)^2 - \ell^2}{\ell^5}\ d r' d \phi' d \lambda'
\f]

The derivatives of the potential are made with respect to the local coordinate
system <b>x->North, y->East, z->out</b>

<b>Input values in SI units and <b>degrees</b> and returns values in Eotvos!</b>

@param dens density of the tesseroid
@param s latitude of the southern border of the tesseroid
@param n latitude of the northern border of the tesseroid
@param w longitude of the western border of the tesseroid
@param e longitude of the eastern border of the tesseroid
@param r1 radial distance from the center of the Earth to the lower-most border
          of the tesseroid
@param r2 radial distance from the center of the Earth to the upper-most border
          of the tesseroid
@param lonp longitude of the computation point P
@param latp latitude of the computation point P
@param rp radial coordinate of the computation point P

@return  field calculated at P
*/
extern double tess_gzz(double dens, double s, double n, double w, double e,
                    double r1, double r2, double lonp, double latp, double rp);
                    
#endif
