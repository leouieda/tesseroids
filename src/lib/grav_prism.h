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


/** Calculates potential caused by a right rectangular prism.

<b> Input and output values in SI units! </b>

@param prism data structure describing the prism
@param xp x coordinate of the computation point
@param yp y coordinate of the computation point
@param zp z coordinate of the computation point

@return field calculated at P */
extern double prism_pot(PRISM prism, double xp, double yp, double zp);


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
