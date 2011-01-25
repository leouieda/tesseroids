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

/* *****************************************************************************
This module contains a set of functions that calculate the gravitational
potential and its first and second derivatives for the sphere.

Author: Leonardo Uieda
Date: 25 Jan 2011
***************************************************************************** */

#ifndef _GRAV_SPHERE_H_
#define _GRAV_SPHERE_H_


/* Calculates the components of the gravity attraction caused by a sphere.

The coordinate system of the input parameters is assumed to be
    x->north, y->east; z->down.

Input values in SI units and returns values in mGal!

Parameters:
    * double dens: density of the sphere;
    * double radius: of the sphere;
    * double xc, yc, zc: coordinates of the center of the sphere;
    * double xp, yp, zp: coordinates of the point P where the effect will be
                         calculated;
*/
extern double sphere_gz(double dens, double radius, double xc, double yc,
                        double zc, double xp, double yp, double zp);

                        
/* Calculates the components of the gravity gradient tensor caused by a sphere.

The coordinate system of the input parameters is assumed to be
    x->north, y->east; z->down.

Input values in SI units and returns values in Eotvos!

Parameters:
    * double dens: density of the sphere;
    * double radius: of the sphere;
    * double xc, yc, zc: coordinates of the center of the sphere;
    * double xp, yp, zp: coordinates of the point P where the effect will be
                         calculated;
*/
extern double sphere_gxx(double dens, double radius, double xc, double yc,
                         double zc, double xp, double yp, double zp);

extern double sphere_gyy(double dens, double radius, double xc, double yc,
                         double zc, double xp, double yp, double zp);

extern double sphere_gzz(double dens, double radius, double xc, double yc,
                         double zc, double xp, double yp, double zp);
/* ************************************************************************** */

#endif
