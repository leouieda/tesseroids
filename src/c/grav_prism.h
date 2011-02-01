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

#ifndef _TESSEROIDS_GRAV_PRISM_H_
#define _TESSEROIDS_GRAV_PRISM_H_


#include <math.h>
#include "utils.h"
#include "constants.h"


/** Calculates gx component caused by a right rectangular prism.

<b> Input values in SI units and returns values in mGal! </b>

@param prism data structure describing the prism
@param xp x coordinate of the computation point
@param yp y coordinate of the computation point
@param zp z coordinate of the computation point

@return field calculated at P */
extern double prism_gx(PRISM prism, double xp, double yp, double zp);


/** Calculates gy component caused by a right rectangular prism.

<b> Input values in SI units and returns values in mGal! </b>

@param prism data structure describing the prism
@param xp x coordinate of the computation point
@param yp y coordinate of the computation point
@param zp z coordinate of the computation point

@return field calculated at P */
extern double prism_gy(PRISM prism, double xp, double yp, double zp);


/** Calculates gz component caused by a right rectangular prism.

<b> Input values in SI units and returns values in mGal! </b>

@param prism data structure describing the prism
@param xp x coordinate of the computation point
@param yp y coordinate of the computation point
@param zp z coordinate of the computation point

@return field calculated at P */
extern double prism_gz(PRISM prism, double xp, double yp, double zp);


/** Calculates gxx component caused by a right rectangular prism.

<b> Input values in SI units and returns values in Eotvos! </b>

@param prism data structure describing the prism
@param xp x coordinate of the computation point
@param yp y coordinate of the computation point
@param zp z coordinate of the computation point

@return field calculated at P */
extern double prism_gxx(PRISM prism, double xp, double yp, double zp);


/** Calculates gxy component caused by a right rectangular prism.

<b> Input values in SI units and returns values in Eotvos! </b>

@param prism data structure describing the prism
@param xp x coordinate of the computation point
@param yp y coordinate of the computation point
@param zp z coordinate of the computation point

@return field calculated at P */
extern double prism_gxy(PRISM prism, double xp, double yp, double zp);


/** Calculates gxz component caused by a right rectangular prism.

<b> Input values in SI units and returns values in Eotvos! </b>

@param prism data structure describing the prism
@param xp x coordinate of the computation point
@param yp y coordinate of the computation point
@param zp z coordinate of the computation point

@return field calculated at P */
extern double prism_gxz(PRISM prism, double xp, double yp, double zp);


/** Calculates gyy component caused by a right rectangular prism.

<b> Input values in SI units and returns values in Eotvos! </b>

@param prism data structure describing the prism
@param xp x coordinate of the computation point
@param yp y coordinate of the computation point
@param zp z coordinate of the computation point

@return field calculated at P */
extern double prism_gyy(PRISM prism, double xp, double yp, double zp);


/** Calculates gyz component caused by a right rectangular prism.

<b> Input values in SI units and returns values in Eotvos! </b>

@param prism data structure describing the prism
@param xp x coordinate of the computation point
@param yp y coordinate of the computation point
@param zp z coordinate of the computation point

@return field calculated at P */
extern double prism_gyz(PRISM prism, double xp, double yp, double zp);


/** Calculates gzz component caused by a right rectangular prism.

<b> Input values in SI units and returns values in Eotvos! </b>

@param prism data structure describing the prism
@param xp x coordinate of the computation point
@param yp y coordinate of the computation point
@param zp z coordinate of the computation point

@return field calculated at P */
extern double prism_gzz(PRISM prism, double xp, double yp, double zp);

#endif