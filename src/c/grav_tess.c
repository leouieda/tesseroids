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

/*
Functions that calculate the gravitational potential and its first and second
derivatives for the tesseroid.

The derivatives of the potential are made with respect to the local coordinate
system <b>x->North, y->East, z->out</b>. So it would be normal for a tesseroid of
positive density to have negative gz.

@author Leonardo Uieda
@date 27 Jan 2011
*/

#include <math.h>
#include "constants.h"
#include "grav_tess.h"

/* Calculates gx caused by a tesseroid. */
double tess_gx(double dens, double s, double n, double w, double e, double r1,
    double r2, double lonp, double latp, double rp, double *nodes, int order)
{
    return 0;
}


/* Calculates gy caused by a tesseroid. */
double tess_gy(double dens, double s, double n, double w, double e, double r1,
    double r2, double lonp, double latp, double rp, double *nodes, int order)
{
    return 0;
}


/* Calculates gz caused by a tesseroid. */
double tess_gz(double dens, double s, double n, double w, double e, double r1,
    double r2, double lonp, double latp, double rp, double *nodes, int order)
{
    return 0;
}

/* Calculates gxx caused by a tesseroid. */
double tess_gxx(double dens, double s, double n, double w, double e, double r1,
    double r2, double lonp, double latp, double rp, double *nodes, int order)
{
    return 0;
}

/* Calculates gxy caused by a tesseroid. */
double tess_gxy(double dens, double s, double n, double w, double e, double r1,
    double r2, double lonp, double latp, double rp, double *nodes, int order)
{
    return 0;
}

/* Calculates gxz caused by a tesseroid. */
double tess_gxz(double dens, double s, double n, double w, double e, double r1,
                double r2, double lonp, double latp, double rp)
{
    return 0;
}

/* Calculates gyy caused by a tesseroid. */
double tess_gyy(double dens, double s, double n, double w, double e, double r1,
                double r2, double lonp, double latp, double rp)
{
    return 0;
}

/* Calculates gyz caused by a tesseroid. */
double tess_gyz(double dens, double s, double n, double w, double e, double r1,
                double r2, double lonp, double latp, double rp)
{
    return 0;
}

/* Calculates gzz caused by a tesseroid. */
double tess_gzz(double dens, double s, double n, double w, double e, double r1,
                double r2, double lonp, double latp, double rp)
{
    return 0;
}