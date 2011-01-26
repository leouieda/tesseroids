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

/* *****************************************************************************
This module contains a set of functions that calculate the gravitational
potential and its first and second derivatives for the rectangular prism
using the formulas in Nagy et al. (2000).

Author: Leonardo Uieda
Date: 01 March 2010
***************************************************************************** */

#ifndef _GRAV_PRISM_H_
#define _GRAV_PRISM_H_


/* Calculates the components of the gravity attraction caused by a right
rectangular prism using the formulas given in Nagy (2000).

The coordinate system of the input parameters is assumed to be
    x->north, y->east; z->down.

Input values in SI units and returns values in mGal!

Parameters:
    * double dens: density of the prism;
    * double x1, x2, y1, ... z2: the borders of the prism;
    * double xp, yp, zp: coordinates of the point P where the effect will be
                         calculated;
*/
extern double prism_gx(double dens, double x1, double x2, double y1, double y2,
                        double z1, double z2, double xp, double yp, double zp);

extern double prism_gy(double dens, double x1, double x2, double y1, double y2,
                        double z1, double z2, double xp, double yp, double zp);

extern double prism_gz(double dens, double x1, double x2, double y1, double y2,
                        double z1, double z2, double xp, double yp, double zp);

                        
/* Calculates the components gravity gradient tensor components caused by a
right rectangular prism using the formulas given in Nagy et al. (2000).

The coordinate system of the input parameters is assumed to be
    x->north, y->east; z->down.

Input values in SI units and returns values in Eotvos!

Parameters:
    * double dens: density of the prism;
    * double x1, x2, y1, ... z2: the borders of the prism;
    * double xp, yp, zp: coordinates of the point P where the effect will be
                         calculated;
*/
extern double prism_gxx(double dens, double x1, double x2, double y1, double y2,
                        double z1, double z2, double xp, double yp, double zp);

extern double prism_gxy(double dens, double x1, double x2, double y1, double y2,
                        double z1, double z2, double xp, double yp, double zp);

extern double prism_gxz(double dens, double x1, double x2, double y1, double y2,
                        double z1, double z2, double xp, double yp, double zp);

extern double prism_gyy(double dens, double x1, double x2, double y1, double y2,
                        double z1, double z2, double xp, double yp, double zp);

extern double prism_gyz(double dens, double x1, double x2, double y1, double y2,
                        double z1, double z2, double xp, double yp, double zp);

extern double prism_gzz(double dens, double x1, double x2, double y1, double y2,
                        double z1, double z2, double xp, double yp, double zp);
                        
#endif