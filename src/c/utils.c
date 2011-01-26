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
This module contains a set of misc utilities. Also defines the TESSEROID and
PRISM structures.

Author: Leonardo Uieda
Date: 25 Jan 2011
***************************************************************************** */

#include <math.h>
#include "constants.h"
#include "utils.h"


/* Convert a tesseroid to a rectangular prism of equal volume. */
void tess2prism(TESSEROID tess, PRISM *prism)
{
    double deg2rad = PI/180., r0, dx, dy;

    r0 = 0.5*(tess.r1 + tess.r2);

    dx = r0*deg2rad*(tess.n - tess.s);
    dy = r0*cos(deg2rad*0.5*(tess.n + tess.s))*deg2rad*(tess.e - tess.w);

    prism->x1 = -0.5*dx;
    prism->x2 = 0.5*dx;
    prism->y1 = -0.5*dy;
    prism->y2 = 0.5*dy;
    /* r1 is not z1 because r1 is the bottom face */
    prism->z1 = MEAN_EARTH_RADIUS - tess.r2;
    prism->z2 = MEAN_EARTH_RADIUS - tess.r1;
    prism->density = tess.density;
}


/* Convert a tesseroid to a sphere of equal volume. */
void tess2sphere(TESSEROID tess, SPHERE *sphere)
{
    sphere->density = tess.density;
    sphere->lonc = 0.5*(tess.e + tess.w);
    sphere->latc = 0.5*(tess.n + tess.s);
    sphere->rc = 0.5*(tess.r1 + tess.r2);
    sphere->r = cbrt(3*tess_volume(tess)/(4.*PI));
}


/* Calculate the volume of a tesseroid */
double tess_volume(TESSEROID tess)
{
    double d2r = PI/180., vol;

    vol = d2r*(tess.e - tess.w)*(pow(tess.r2, 3) - pow(tess.r1, 3))*
          (sin(d2r*tess.n) - sin(d2r*tess.s))/3.;

    return vol;
}


/* Calculate the volume of a sphere */
double sphere_volume(SPHERE sphere)
{
    return 4.*PI*pow(sphere.r, 3)/3.;
}


/* Calculate the volume of a prism */
double prism_volume(PRISM prism)
{
    return (prism.x2 - prism.x1)*(prism.y2 - prism.y1)*(prism.z2 - prism.z1);
}