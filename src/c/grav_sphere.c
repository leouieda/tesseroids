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


#include <math.h>
#include "grav_sphere.h"
#include "constants.h"


/* Calculates the gz component of gravitational attraction caused by a sphere */
double sphere_gz(double dens, double radius, double xc, double yc, double zc,
                 double xp, double yp, double zp)
{
    double mass, dx, dy, dz, r_sqr, res;

    mass = (double)(dens*4.*3.1415926535897931*radius*radius*radius)/3.;

    dx = xc - xp;
    dy = yc - yp;
    dz = zc - zp;

    r_sqr = dx*dx + dy*dy + dz*dz;

    res = (double)(G*SI2MGAL*mass*dz/pow(r_sqr, 1.5));

    return res;
}


/* Calculate the xx component of gravity gradient tensor cause by a sphere */
double sphere_gxx(double dens, double radius, double xc, double yc, double zc,
                  double xp, double yp, double zp)
{
    double mass, dx, dy, dz, r_sqr, res;

    mass = (double)(dens*4.*3.1415926535897931*radius*radius*radius)/3.;

    dx = xc - xp;
    dy = yc - yp;
    dz = zc - zp;

    r_sqr = dx*dx + dy*dy + dz*dz;

    res = (double)(G*SI2EOTVOS*mass*(3*dx*dx - r_sqr)/pow(r_sqr, 2.5));

    return res;
}


/* Calculate the yy component of gravity gradient tensor cause by a sphere */
double sphere_gyy(double dens, double radius, double xc, double yc, double zc,
                  double xp, double yp, double zp)
{
    double mass, dx, dy, dz, r_sqr, res;

    mass = (double)(dens*4.*3.1415926535897931*radius*radius*radius)/3.;

    dx = xc - xp;
    dy = yc - yp;
    dz = zc - zp;

    r_sqr = dx*dx + dy*dy + dz*dz;

    res = (double)(G*SI2EOTVOS*mass*(3*dy*dy - r_sqr)/pow(r_sqr, 2.5));

    return res;
}


/* Calculate the zz component of gravity gradient tensor cause by a sphere */
double sphere_gzz(double dens, double radius, double xc, double yc, double zc,
                  double xp, double yp, double zp)
{
    double mass, dx, dy, dz, r_sqr, res;

    mass = (double)(dens*4.*3.1415926535897931*radius*radius*radius)/3.;

    dx = xc - xp;
    dy = yc - yp;
    dz = zc - zp;

    r_sqr = dx*dx + dy*dy + dz*dz;

    res = (double)(G*SI2EOTVOS*mass*(3*dz*dz - r_sqr)/pow(r_sqr, 2.5));

    return res;
}