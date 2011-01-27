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
***************************************************************************** */

/** \file
Functions that calculate the gravitational potential and its first and second
derivatives for the rectangular prism

Using the formulas in Nagy et al. (2000).

The coordinate system used is that of the article, ie:

x -> North  y -> East  z -> Down

@author Leonardo Uieda
@date 01 March 2010
*/

#ifndef _GRAV_PRISM_H_
#define _GRAV_PRISM_H_


/** Calculates gx component caused by a right rectangular prism.

<b> Input values in SI units and returns values in mGal! </b>

@param dens density of the prism
@param x1 southern x-axis border of the prism
@param x2 northern x-axis border of the prism
@param y1 western y-axis border of the prism
@param y2 eastern y-axis border of the prism
@param z1 up-most z-axis border of the prism
@param z2 down-most z-axis border of the prism
@param xp x coordinate of the computation point
@param yp y coordinate of the computation point
@param zp z coordinate of the computation point

@return field calculated at P */
extern double prism_gx(double dens, double x1, double x2, double y1, double y2,
                        double z1, double z2, double xp, double yp, double zp);

/** Calculates gy component caused by a right rectangular prism.

<b> Input values in SI units and returns values in mGal! </b>

@param dens density of the prism
@param x1 southern x-axis border of the prism
@param x2 northern x-axis border of the prism
@param y1 western y-axis border of the prism
@param y2 eastern y-axis border of the prism
@param z1 up-most z-axis border of the prism
@param z2 down-most z-axis border of the prism
@param xp x coordinate of the computation point
@param yp y coordinate of the computation point
@param zp z coordinate of the computation point

@return field calculated at P */
extern double prism_gy(double dens, double x1, double x2, double y1, double y2,
                        double z1, double z2, double xp, double yp, double zp);

/** Calculates gz component caused by a right rectangular prism.

<b> Input values in SI units and returns values in mGal! </b>

@param dens density of the prism
@param x1 southern x-axis border of the prism
@param x2 northern x-axis border of the prism
@param y1 western y-axis border of the prism
@param y2 eastern y-axis border of the prism
@param z1 up-most z-axis border of the prism
@param z2 down-most z-axis border of the prism
@param xp x coordinate of the computation point
@param yp y coordinate of the computation point
@param zp z coordinate of the computation point

@return field calculated at P */
extern double prism_gz(double dens, double x1, double x2, double y1, double y2,
                        double z1, double z2, double xp, double yp, double zp);



/** Calculates gxx component caused by a right rectangular prism.

<b> Input values in SI units and returns values in Eotvos! </b>

@param dens density of the prism
@param x1 southern x-axis border of the prism
@param x2 northern x-axis border of the prism
@param y1 western y-axis border of the prism
@param y2 eastern y-axis border of the prism
@param z1 up-most z-axis border of the prism
@param z2 down-most z-axis border of the prism
@param xp x coordinate of the computation point
@param yp y coordinate of the computation point
@param zp z coordinate of the computation point

@return field calculated at P */
extern double prism_gxx(double dens, double x1, double x2, double y1, double y2,
                        double z1, double z2, double xp, double yp, double zp);

/** Calculates gxy component caused by a right rectangular prism.

<b> Input values in SI units and returns values in Eotvos! </b>

@param dens density of the prism
@param x1 southern x-axis border of the prism
@param x2 northern x-axis border of the prism
@param y1 western y-axis border of the prism
@param y2 eastern y-axis border of the prism
@param z1 up-most z-axis border of the prism
@param z2 down-most z-axis border of the prism
@param xp x coordinate of the computation point
@param yp y coordinate of the computation point
@param zp z coordinate of the computation point

@return field calculated at P */
extern double prism_gxy(double dens, double x1, double x2, double y1, double y2,
                        double z1, double z2, double xp, double yp, double zp);

/** Calculates gxz component caused by a right rectangular prism.

<b> Input values in SI units and returns values in Eotvos! </b>

@param dens density of the prism
@param x1 southern x-axis border of the prism
@param x2 northern x-axis border of the prism
@param y1 western y-axis border of the prism
@param y2 eastern y-axis border of the prism
@param z1 up-most z-axis border of the prism
@param z2 down-most z-axis border of the prism
@param xp x coordinate of the computation point
@param yp y coordinate of the computation point
@param zp z coordinate of the computation point

@return field calculated at P */
extern double prism_gxz(double dens, double x1, double x2, double y1, double y2,
                        double z1, double z2, double xp, double yp, double zp);

/** Calculates gyy component caused by a right rectangular prism.

<b> Input values in SI units and returns values in Eotvos! </b>

@param dens density of the prism
@param x1 southern x-axis border of the prism
@param x2 northern x-axis border of the prism
@param y1 western y-axis border of the prism
@param y2 eastern y-axis border of the prism
@param z1 up-most z-axis border of the prism
@param z2 down-most z-axis border of the prism
@param xp x coordinate of the computation point
@param yp y coordinate of the computation point
@param zp z coordinate of the computation point

@return field calculated at P */
extern double prism_gyy(double dens, double x1, double x2, double y1, double y2,
                        double z1, double z2, double xp, double yp, double zp);

/** Calculates gyz component caused by a right rectangular prism.

<b> Input values in SI units and returns values in Eotvos! </b>

@param dens density of the prism
@param x1 southern x-axis border of the prism
@param x2 northern x-axis border of the prism
@param y1 western y-axis border of the prism
@param y2 eastern y-axis border of the prism
@param z1 up-most z-axis border of the prism
@param z2 down-most z-axis border of the prism
@param xp x coordinate of the computation point
@param yp y coordinate of the computation point
@param zp z coordinate of the computation point

@return field calculated at P */
extern double prism_gyz(double dens, double x1, double x2, double y1, double y2,
                        double z1, double z2, double xp, double yp, double zp);

/** Calculates gzz component caused by a right rectangular prism.

<b> Input values in SI units and returns values in Eotvos! </b>

@param dens density of the prism
@param x1 southern x-axis border of the prism
@param x2 northern x-axis border of the prism
@param y1 western y-axis border of the prism
@param y2 eastern y-axis border of the prism
@param z1 up-most z-axis border of the prism
@param z2 down-most z-axis border of the prism
@param xp x coordinate of the computation point
@param yp y coordinate of the computation point
@param zp z coordinate of the computation point

@return field calculated at P */
extern double prism_gzz(double dens, double x1, double x2, double y1, double y2,
                        double z1, double z2, double xp, double yp, double zp);
                        
#endif