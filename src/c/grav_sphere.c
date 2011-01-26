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
potential and its first and second derivatives for the sphere in spherical
coordinates.

Author: Leonardo Uieda
Date: 25 Jan 2011
***************************************************************************** */


#include <math.h>
#include "grav_sphere.h"
#include "constants.h"


/* Calculates the gz component of gravitational attraction caused by a sphere */
double sphere_gz(double dens, double radius, double lonc, double latc,
                 double rc, double lonp, double latp, double rp)
{
    double mass, l_sqr, d2r = PI/180., cospsi;

    mass = (double)(dens*4.*PI*radius*radius*radius)/3.;

    cospsi = sin(d2r*latp)*sin(d2r*latc) +  cos(d2r*latp)*cos(d2r*latc)*
                                            cos(d2r*(lonp - lonc));

    l_sqr = rp*rp + rc*rc - 2*rp*rc*cospsi;

    return G*SI2MGAL*mass*(rc*cospsi - rp)/pow(l_sqr, 1.5);
}


/* Calculate the xx component of gravity gradient tensor cause by a sphere */
double sphere_gxx(double dens, double radius, double lonc, double latc,
                  double rc, double lonp, double latp, double rp)
{
    double mass, l_sqr, d2r = PI/180., kphi, coslatp, coslatc, sinlatp, sinlatc,
           coslon;

    mass = (double)(dens*4.*PI*radius*radius*radius)/3.;

    coslatp = cos(d2r*latp);
    coslatc = cos(d2r*latc);
    sinlatp = sin(d2r*latp);
    sinlatc = sin(d2r*latc);
    coslon = cos(d2r*(lonp - lonc));

    l_sqr = rp*rp + rc*rc - 2*rp*rc*(sinlatp*sinlatc + coslatp*coslatc*coslon);

    kphi = coslatp*sinlatc - sinlatp*coslatc*coslon;
    
    return G*SI2EOTVOS*mass*(3*rc*kphi*rc*kphi - l_sqr)/pow(l_sqr, 2.5);
}


/* Calculate the xy component of gravity gradient tensor cause by a sphere */
double sphere_gxy(double dens, double radius, double lonc, double latc,
                  double rc, double lonp, double latp, double rp)
{
    double mass, l_sqr, d2r = PI/180., kphi, coslatp, coslatc, sinlatp, sinlatc,
           coslon, kern;

    mass = (double)(dens*4.*PI*radius*radius*radius)/3.;

    coslatp = cos(d2r*latp);
    coslatc = cos(d2r*latc);
    sinlatp = sin(d2r*latp);
    sinlatc = sin(d2r*latc);
    coslon = cos(d2r*(lonp - lonc));

    l_sqr = rp*rp + rc*rc - 2*rp*rc*(sinlatp*sinlatc + coslatp*coslatc*coslon);

    kphi = coslatp*sinlatc - sinlatp*coslatc*coslon;

    kern = (3*rc*rc*kphi*coslatp*sin(d2r*(lonc - lonp)))/pow(l_sqr, 2.5);

    return G*SI2EOTVOS*mass*kern;
}


/* Calculate the xz component of gravity gradient tensor cause by a sphere */
double sphere_gxz(double dens, double radius, double lonc, double latc,
                  double rc, double lonp, double latp, double rp)
{
    double mass, l_sqr, d2r = PI/180., kphi, coslatp, coslatc, sinlatp, sinlatc,
           coslon, kern, cospsi;

    mass = (double)(dens*4.*PI*radius*radius*radius)/3.;

    coslatp = cos(d2r*latp);
    coslatc = cos(d2r*latc);
    sinlatp = sin(d2r*latp);
    sinlatc = sin(d2r*latc);
    coslon = cos(d2r*(lonp - lonc));

    cospsi = sinlatp*sinlatc + coslatp*coslatc*coslon;

    l_sqr = rp*rp + rc*rc - 2*rp*rc*cospsi;

    kphi = coslatp*sinlatc - sinlatp*coslatc*coslon;

    kern = 3*rc*kphi*(rc*cospsi - rp)/pow(l_sqr, 2.5);

    return G*SI2EOTVOS*mass*kern;
}


/* Calculate the yy component of gravity gradient tensor cause by a sphere */
double sphere_gyy(double dens, double radius, double lonc, double latc,
                  double rc, double lonp, double latp, double rp)
{
    double mass, l_sqr, d2r = PI/180., coslatp, coslatc, sinlatp, sinlatc,
           coslon, sinlon, kern, cospsi;

    mass = (double)(dens*4.*PI*radius*radius*radius)/3.;

    coslatp = cos(d2r*latp);
    coslatc = cos(d2r*latc);
    sinlatp = sin(d2r*latp);
    sinlatc = sin(d2r*latc);
    coslon = cos(d2r*(lonp - lonc));
    sinlon = sin(d2r*(lonc - lonp));

    cospsi = sinlatp*sinlatc + coslatp*coslatc*coslon;

    l_sqr = rp*rp + rc*rc - 2*rp*rc*cospsi;

    kern = (3*rc*rc*coslatc*coslatc*sinlon*sinlon - l_sqr)/pow(l_sqr, 2.5);

    return G*SI2EOTVOS*mass*kern;
}


/* Calculate the yz component of gravity gradient tensor cause by a sphere */
double sphere_gyz(double dens, double radius, double lonc, double latc,
                  double rc, double lonp, double latp, double rp)
{
    double mass, l_sqr, d2r = PI/180., coslatp, coslatc, sinlatp, sinlatc,
           coslon, sinlon, kern, cospsi;

    mass = (double)(dens*4.*PI*radius*radius*radius)/3.;

    coslatp = cos(d2r*latp);
    coslatc = cos(d2r*latc);
    sinlatp = sin(d2r*latp);
    sinlatc = sin(d2r*latc);
    coslon = cos(d2r*(lonp - lonc));
    sinlon = sin(d2r*(lonc - lonp));

    cospsi = sinlatp*sinlatc + coslatp*coslatc*coslon;

    l_sqr = rp*rp + rc*rc - 2*rp*rc*cospsi;

    kern = 3*rc*coslatc*sinlon*(rc*cospsi - rp)/pow(l_sqr, 2.5);

    return G*SI2EOTVOS*mass*kern;
}


/* Calculate the zz component of gravity gradient tensor cause by a sphere */
double sphere_gzz(double dens, double radius, double lonc, double latc,
                  double rc, double lonp, double latp, double rp)
{
    double mass, l_sqr, d2r = PI/180., deltaz, cospsi;

    mass = (double)(dens*4.*PI*radius*radius*radius)/3.;

    cospsi = sin(d2r*latp)*sin(d2r*latc) + cos(d2r*latp)*cos(d2r*latc)*
                                           cos(d2r*(lonp - lonc));

    l_sqr = rp*rp + rc*rc - 2*rp*rc*cospsi;

    deltaz = rc*cospsi - rp;

    return G*SI2EOTVOS*mass*(3*deltaz*deltaz - l_sqr)/pow(l_sqr, 2.5);
}