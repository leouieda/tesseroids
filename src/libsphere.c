/*
This module contains a set of functions that calculate the gravitational
potential and its first and second derivatives for the sphere in spherical
coordinates.
*/

#include <math.h>
#include "geometry.h"
#include "constants.h"
#include "libsphere.h"

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
    l_sqr = (rp*rp + sphere.rc*sphere.rc -
             2*rp*sphere.rc*(sinlatp*sinlatc + coslatp*coslatc*coslon));
    return G*mass/sqrt(l_sqr);
}

double sphere_gx(SPHERE sphere, double lonp, double latp, double rp)
{
    double mass, l_sqr, d2r = PI/180., kphi, coslatp, coslatc, sinlatp,
           sinlatc, coslon;
    mass = (double)(sphere.density*4.*PI*sphere.r*sphere.r*sphere.r)/3.;
    coslatp = cos(d2r*latp);
    coslatc = cos(d2r*sphere.latc);
    sinlatp = sin(d2r*latp);
    sinlatc = sin(d2r*sphere.latc);
    coslon = cos(d2r*(lonp - sphere.lonc));
    l_sqr = (rp*rp + sphere.rc*sphere.rc -
             2*rp*sphere.rc*(sinlatp*sinlatc + coslatp*coslatc*coslon));
    kphi = coslatp*sinlatc - sinlatp*coslatc*coslon;
    return G*SI2MGAL*mass*(sphere.rc*kphi)/pow(l_sqr, 1.5);
}

double sphere_gy(SPHERE sphere, double lonp, double latp, double rp)
{
    double mass, l_sqr, d2r = PI/180., cospsi, coslatc, kern;
    mass = (double)(sphere.density*4.*PI*sphere.r*sphere.r*sphere.r)/3.;
    coslatc = cos(d2r*sphere.latc);
    cospsi = (sin(d2r*latp)*sin(d2r*sphere.latc) +
              cos(d2r*latp)*coslatc*cos(d2r*(lonp - sphere.lonc)));
    l_sqr = rp*rp + sphere.rc*sphere.rc - 2*rp*sphere.rc*cospsi;
    kern = (sphere.rc*coslatc*sin(d2r*(sphere.lonc - lonp)))/pow(l_sqr, 1.5);
    return G*SI2MGAL*mass*kern;
}

double sphere_gz(SPHERE sphere, double lonp, double latp, double rp)
{
    double mass, l_sqr, d2r = PI/180., cospsi;
    mass = (double)(sphere.density*4.*PI*sphere.r*sphere.r*sphere.r)/3.;
    cospsi = sin(d2r*latp)*sin(d2r*sphere.latc) +
             cos(d2r*latp)*cos(d2r*sphere.latc)*cos(d2r*(lonp - sphere.lonc));
    l_sqr = rp*rp + sphere.rc*sphere.rc - 2*rp*sphere.rc*cospsi;
    return G*SI2MGAL*mass*(sphere.rc*cospsi - rp)/pow(l_sqr, 1.5);
}

double sphere_gxx(SPHERE sphere, double lonp, double latp, double rp)
{
    double mass, l_sqr, d2r = PI/180., kphi, coslatp, coslatc, sinlatp,
           sinlatc, coslon, kern;
    mass = (double)(sphere.density*4.*PI*sphere.r*sphere.r*sphere.r)/3.;
    coslatp = cos(d2r*latp);
    coslatc = cos(d2r*sphere.latc);
    sinlatp = sin(d2r*latp);
    sinlatc = sin(d2r*sphere.latc);
    coslon = cos(d2r*(lonp - sphere.lonc));
    l_sqr = (rp*rp + sphere.rc*sphere.rc -
             2*rp*sphere.rc*(sinlatp*sinlatc + coslatp*coslatc*coslon));
    kphi = coslatp*sinlatc - sinlatp*coslatc*coslon;
    kern = (3*sphere.rc*kphi*sphere.rc*kphi - l_sqr)/pow(l_sqr, 2.5);
    return G*SI2EOTVOS*mass*kern;
}

double sphere_gxy(SPHERE sphere, double lonp, double latp, double rp)
{
    double mass, l_sqr, d2r = PI/180., kphi, coslatp, coslatc, sinlatp,
           sinlatc, coslon, kern;
    mass = (double)(sphere.density*4.*PI*sphere.r*sphere.r*sphere.r)/3.;
    coslatp = cos(d2r*latp);
    coslatc = cos(d2r*sphere.latc);
    sinlatp = sin(d2r*latp);
    sinlatc = sin(d2r*sphere.latc);
    coslon = cos(d2r*(lonp - sphere.lonc));
    l_sqr = (rp*rp + sphere.rc*sphere.rc -
             2*rp*sphere.rc*(sinlatp*sinlatc + coslatp*coslatc*coslon));
    kphi = coslatp*sinlatc - sinlatp*coslatc*coslon;
    kern = (3*sphere.rc*sphere.rc*kphi*coslatp*sin(d2r*(sphere.lonc - lonp)))
            /pow(l_sqr, 2.5);
    return G*SI2EOTVOS*mass*kern;
}

double sphere_gxz(SPHERE sphere, double lonp, double latp, double rp)
{
    double mass, l_sqr, d2r = PI/180., kphi, coslatp, coslatc, sinlatp,
           sinlatc, coslon, kern, cospsi;
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
    kern = ((3*sphere.rc*sphere.rc*coslatc*coslatc*sinlon*sinlon - l_sqr)
            /pow(l_sqr, 2.5));
    return G*SI2EOTVOS*mass*kern;
}

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

double sphere_gzz(SPHERE sphere, double lonp, double latp, double rp)
{
    double mass, l_sqr, d2r = PI/180., deltaz, cospsi;
    mass = (double)(sphere.density*4.*PI*sphere.r*sphere.r*sphere.r)/3.;
    cospsi = sin(d2r*latp)*sin(d2r*sphere.latc) +
             cos(d2r*latp)*cos(d2r*sphere.latc)*cos(d2r*(lonp - sphere.lonc));
    l_sqr = rp*rp + sphere.rc*sphere.rc - 2*rp*sphere.rc*cospsi;
    deltaz = sphere.rc*cospsi - rp;
    return G*SI2EOTVOS*mass*(3*deltaz*deltaz - l_sqr)/pow(l_sqr, 2.5);
}
