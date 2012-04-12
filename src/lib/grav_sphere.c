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
This module contains a set of functions that calculate the gravitational
potential and its first and second derivatives for the sphere in spherical
coordinates.

The position of the sphere and computation point are in spherical coordinates.

The derivatives of the potential are made with respect to the local coordinate
system x->North, y->East, z->out. So it would be normal for a sphere of positive
density to have negative gz

<b>References</b>

- Grombein, T.; Seitz, K.; Heck, B. (2010): Untersuchungen zur effizienten
Berechnung topographischer Effekte auf den Gradiententensor am Fallbeispiel der
Satellitengradiometriemission GOCE.
KIT Scientific Reports 7547, ISBN 978-3-86644-510-9, KIT Scientific Publishing,
Karlsruhe, Germany.
(<a href="http://digbib.ubka.uni-karlsruhe.de/volltexte/documents/1336300">
http://digbib.ubka.uni-karlsruhe.de/volltexte/documents/1336300</a>).

@author Leonardo Uieda
@date 25 Jan 2011
*/


#include <math.h>
#include "utils.h"
#include "constants.h"
#include "grav_sphere.h"


/* Calculates the potential caused by a sphere */
double sphere_pot(SPHERE sphere, double lonp, double latp, double rp)
{
    double mass, l_sqr, d2r = PI/180., coslatp, coslatc, sinlatp, sinlatc,
           coslon;

    mass = (double)(sphere.density*4.*PI*sphere.r*sphere.r*sphere.r)/3.;

    coslatp = cos(d2r*latp);
    coslatc = cos(d2r*sphere.latc);
    sinlatp = sin(d2r*latp);
    sinlatc = sin(d2r*sphere.latc);
    coslon = cos(d2r*(lonp - sphere.lonc));

    l_sqr = rp*rp + sphere.rc*sphere.rc - 2*rp*sphere.rc*(
                                      sinlatp*sinlatc + coslatp*coslatc*coslon);

    return G*mass/sqrt(l_sqr);
}


/* Calculates the gx component of gravitational attraction caused by a sphere */
double sphere_gx(SPHERE sphere, double lonp, double latp, double rp)
{
    double mass, l_sqr, d2r = PI/180., kphi, coslatp, coslatc, sinlatp, sinlatc,
           coslon;

    mass = (double)(sphere.density*4.*PI*sphere.r*sphere.r*sphere.r)/3.;

    coslatp = cos(d2r*latp);
    coslatc = cos(d2r*sphere.latc);
    sinlatp = sin(d2r*latp);
    sinlatc = sin(d2r*sphere.latc);
    coslon = cos(d2r*(lonp - sphere.lonc));

    l_sqr = rp*rp + sphere.rc*sphere.rc - 2*rp*sphere.rc*(
                                      sinlatp*sinlatc + coslatp*coslatc*coslon);

    kphi = coslatp*sinlatc - sinlatp*coslatc*coslon;

    return G*SI2MGAL*mass*(sphere.rc*kphi)/pow(l_sqr, 1.5);
}


/* Calculates the gy component of gravitational attraction caused by a sphere */
double sphere_gy(SPHERE sphere, double lonp, double latp, double rp)
{
    double mass, l_sqr, d2r = PI/180., cospsi, coslatc, kern;

    mass = (double)(sphere.density*4.*PI*sphere.r*sphere.r*sphere.r)/3.;

    coslatc = cos(d2r*sphere.latc);

    cospsi = sin(d2r*latp)*sin(d2r*sphere.latc) +  cos(d2r*latp)*coslatc*
                                            cos(d2r*(lonp - sphere.lonc));

    l_sqr = rp*rp + sphere.rc*sphere.rc - 2*rp*sphere.rc*cospsi;

    kern = (sphere.rc*coslatc*sin(d2r*(sphere.lonc - lonp)))/pow(l_sqr, 1.5);

    return G*SI2MGAL*mass*kern;
}


/* Calculates the gz component of gravitational attraction caused by a sphere */
double sphere_gz(SPHERE sphere, double lonp, double latp, double rp)
{
    double mass, l_sqr, d2r = PI/180., cospsi;

    mass = (double)(sphere.density*4.*PI*sphere.r*sphere.r*sphere.r)/3.;

    cospsi = sin(d2r*latp)*sin(d2r*sphere.latc) +  cos(d2r*latp)*
                            cos(d2r*sphere.latc)*cos(d2r*(lonp - sphere.lonc));

    l_sqr = rp*rp + sphere.rc*sphere.rc - 2*rp*sphere.rc*cospsi;

    return G*SI2MGAL*mass*(sphere.rc*cospsi - rp)/pow(l_sqr, 1.5);
}


/* Calculate the xx component of gravity gradient tensor cause by a sphere */
double sphere_gxx(SPHERE sphere, double lonp, double latp, double rp)
{
    double mass, l_sqr, d2r = PI/180., kphi, coslatp, coslatc, sinlatp, sinlatc,
           coslon, kern;

    mass = (double)(sphere.density*4.*PI*sphere.r*sphere.r*sphere.r)/3.;

    coslatp = cos(d2r*latp);
    coslatc = cos(d2r*sphere.latc);
    sinlatp = sin(d2r*latp);
    sinlatc = sin(d2r*sphere.latc);
    coslon = cos(d2r*(lonp - sphere.lonc));

    l_sqr = rp*rp + sphere.rc*sphere.rc - 2*rp*sphere.rc*(sinlatp*sinlatc +
                                                        coslatp*coslatc*coslon);

    kphi = coslatp*sinlatc - sinlatp*coslatc*coslon;

    kern = (3*sphere.rc*kphi*sphere.rc*kphi - l_sqr)/pow(l_sqr, 2.5);

    return G*SI2EOTVOS*mass*kern;
}


/* Calculate the xy component of gravity gradient tensor cause by a sphere */
double sphere_gxy(SPHERE sphere, double lonp, double latp, double rp)
{
    double mass, l_sqr, d2r = PI/180., kphi, coslatp, coslatc, sinlatp, sinlatc,
           coslon, kern;

    mass = (double)(sphere.density*4.*PI*sphere.r*sphere.r*sphere.r)/3.;

    coslatp = cos(d2r*latp);
    coslatc = cos(d2r*sphere.latc);
    sinlatp = sin(d2r*latp);
    sinlatc = sin(d2r*sphere.latc);
    coslon = cos(d2r*(lonp - sphere.lonc));

    l_sqr = rp*rp + sphere.rc*sphere.rc - 2*rp*sphere.rc*(sinlatp*sinlatc +
                                                        coslatp*coslatc*coslon);

    kphi = coslatp*sinlatc - sinlatp*coslatc*coslon;

    kern = (3*sphere.rc*sphere.rc*kphi*coslatp*sin(d2r*(sphere.lonc - lonp)))/
                                                                pow(l_sqr, 2.5);

    return G*SI2EOTVOS*mass*kern;
}


/* Calculate the xz component of gravity gradient tensor cause by a sphere */
double sphere_gxz(SPHERE sphere, double lonp, double latp, double rp)
{
    double mass, l_sqr, d2r = PI/180., kphi, coslatp, coslatc, sinlatp, sinlatc,
           coslon, kern, cospsi;

    mass = (double)(sphere.density*4.*PI*sphere.r*sphere.r*sphere.r)/3.;

    coslatp = cos(d2r*latp);
    coslatc = cos(d2r*sphere.latc);
    sinlatp = sin(d2r*latp);
    sinlatc = sin(d2r*sphere.latc);
    coslon = cos(d2r*(lonp - sphere.lonc));

    cospsi = sinlatp*sinlatc + coslatp*coslatc*coslon;

    l_sqr = rp*rp + sphere.rc*sphere.rc - 2*rp*sphere.rc*cospsi;

    kphi = coslatp*sinlatc - sinlatp*coslatc*coslon;

    kern = 3*sphere.rc*kphi*(sphere.rc*cospsi - rp)/pow(l_sqr, 2.5);

    return G*SI2EOTVOS*mass*kern;
}


/* Calculate the yy component of gravity gradient tensor cause by a sphere */
double sphere_gyy(SPHERE sphere, double lonp, double latp, double rp)
{
    double mass, l_sqr, d2r = PI/180., coslatp, coslatc, sinlatp, sinlatc,
           coslon, sinlon, kern, cospsi;

    mass = (double)(sphere.density*4.*PI*sphere.r*sphere.r*sphere.r)/3.;

    coslatp = cos(d2r*latp);
    coslatc = cos(d2r*sphere.latc);
    sinlatp = sin(d2r*latp);
    sinlatc = sin(d2r*sphere.latc);
    coslon = cos(d2r*(lonp - sphere.lonc));
    sinlon = sin(d2r*(sphere.lonc - lonp));

    cospsi = sinlatp*sinlatc + coslatp*coslatc*coslon;

    l_sqr = rp*rp + sphere.rc*sphere.rc - 2*rp*sphere.rc*cospsi;

    kern = (3*sphere.rc*sphere.rc*coslatc*coslatc*sinlon*sinlon - l_sqr)/
                                                                pow(l_sqr, 2.5);

    return G*SI2EOTVOS*mass*kern;
}


/* Calculate the yz component of gravity gradient tensor cause by a sphere */
double sphere_gyz(SPHERE sphere, double lonp, double latp, double rp)
{
    double mass, l_sqr, d2r = PI/180., coslatp, coslatc, sinlatp, sinlatc,
           coslon, sinlon, kern, cospsi;

    mass = (double)(sphere.density*4.*PI*sphere.r*sphere.r*sphere.r)/3.;

    coslatp = cos(d2r*latp);
    coslatc = cos(d2r*sphere.latc);
    sinlatp = sin(d2r*latp);
    sinlatc = sin(d2r*sphere.latc);
    coslon = cos(d2r*(lonp - sphere.lonc));
    sinlon = sin(d2r*(sphere.lonc - lonp));

    cospsi = sinlatp*sinlatc + coslatp*coslatc*coslon;

    l_sqr = rp*rp + sphere.rc*sphere.rc - 2*rp*sphere.rc*cospsi;

    kern = 3*sphere.rc*coslatc*sinlon*(sphere.rc*cospsi - rp)/pow(l_sqr, 2.5);

    return G*SI2EOTVOS*mass*kern;
}


/* Calculate the zz component of gravity gradient tensor cause by a sphere */
double sphere_gzz(SPHERE sphere, double lonp, double latp, double rp)
{
    double mass, l_sqr, d2r = PI/180., deltaz, cospsi;

    mass = (double)(sphere.density*4.*PI*sphere.r*sphere.r*sphere.r)/3.;

    cospsi = sin(d2r*latp)*sin(d2r*sphere.latc) + cos(d2r*latp)*
                            cos(d2r*sphere.latc)*cos(d2r*(lonp - sphere.lonc));

    l_sqr = rp*rp + sphere.rc*sphere.rc - 2*rp*sphere.rc*cospsi;

    deltaz = sphere.rc*cospsi - rp;

    return G*SI2EOTVOS*mass*(3*deltaz*deltaz - l_sqr)/pow(l_sqr, 2.5);
}
