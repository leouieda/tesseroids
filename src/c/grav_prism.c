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
derivatives for the rectangular prism

Using the formulas in Nagy et al. (2000).

The coordinate system used is that of the article, ie:

x -> North  y -> East  z -> Down

@author Leonardo Uieda
@date 01 March 2010
*/

#include <math.h>
#include "grav_prism.h"
#include "constants.h"


/* Calculates the x component of gravitational attraction cause by a prism. */
double prism_gx(double dens, double x1, double x2, double y1, double y2,
                double z1, double z2, double xp, double yp, double zp)
{
    /*
    double r, res, deltax1, deltax2, deltay1, deltay2, deltaz1, deltaz2;
    */
    double x[2], y[2], z[2], kernel, res, r;
    register int i, j, k;

    /* First thing to do is make P the origin of the coordinate system */
    /*deltax1 = x1 - xp;
    deltax2 = x2 - xp;
    deltay1 = y1 - yp;
    deltay2 = y2 - yp;
    deltaz1 = z1 - zp;
    deltaz2 = z2 - zp;*/
    x[0] = x1 - xp;
    x[1] = x2 - xp;
    y[0] = y1 - yp;
    y[1] = y2 - yp;
    z[0] = z1 - zp;
    z[1] = z2 - zp;

    res = 0;

    /* Evaluate the integration limits */
    for(k=0; k<=1; k++)
    {
        for(j=0; j<=1; j++)
        {
            for(i=0; i<=1; i++)
            {
                r = sqrt(x[i]*x[i] + y[j]*y[j] + z[k]*z[k]);

                kernel = y[i]*log(z[j] + r) + z[j]*log(y[i] + r)
                        - x[k]*atan2(z[i]*y[j], x[k]*r);

                res += pow(-1, i + j + k)*kernel;
            }
        }
    }

    /* Now all that is left is to multiply res by the gravitational constant and
       density and convert it to mGal units */
    res *= G*SI2MGAL*dens;

    return res;
}


/* Calculates the y component of gravitational attraction cause by a prism. */
double prism_gy(double dens, double x1, double x2, double y1, double y2,
                double z1, double z2, double xp, double yp, double zp)
{
    /*
    double r, res, deltax1, deltax2, deltay1, deltay2, deltaz1, deltaz2;
    */
    double x[2], y[2], z[2], kernel, res, r;
    register int i, j, k;

    /* First thing to do is make P the origin of the coordinate system */
    /*deltax1 = x1 - xp;
    deltax2 = x2 - xp;
    deltay1 = y1 - yp;
    deltay2 = y2 - yp;
    deltaz1 = z1 - zp;
    deltaz2 = z2 - zp;*/
    x[0] = x1 - xp;
    x[1] = x2 - xp;
    y[0] = y1 - yp;
    y[1] = y2 - yp;
    z[0] = z1 - zp;
    z[1] = z2 - zp;

    res = 0;

    /* Evaluate the integration limits */
    for(k=0; k<=1; k++)
    {
        for(j=0; j<=1; j++)
        {
            for(i=0; i<=1; i++)
            {
                r = sqrt(x[i]*x[i] + y[j]*y[j] + z[k]*z[k]);

                kernel = z[i]*log(x[j] + r) + x[j]*log(z[i] + r)
                        - y[k]*atan2(z[i]*x[j], y[k]*r);

                res += pow(-1, i + j + k)*kernel;
            }
        }
    }

    /* Now all that is left is to multiply res by the gravitational constant and
       density and convert it to mGal units */
    res *= G*SI2MGAL*dens;

    return res;
}


/* Calculates the z component of gravitational attraction cause by a prism. */
double prism_gz(double dens, double x1, double x2, double y1, double y2,
                double z1, double z2, double xp, double yp, double zp)
{
    double r, res, deltax1, deltax2, deltay1, deltay2, deltaz1, deltaz2;

    /* First thing to do is make P the origin of the coordinate system */
    deltax1 = x1 - xp;
    deltax2 = x2 - xp;
    deltay1 = y1 - yp;
    deltay2 = y2 - yp;
    deltaz1 = z1 - zp;
    deltaz2 = z2 - zp;

    res = 0;

    /* Evaluate the integration limits */
    r = sqrt(deltax1*deltax1 + deltay1*deltay1 + deltaz1*deltaz1);

    res += 1*(deltax1*log(deltay1 + r) + deltay1*log(deltax1 + r) -
    		deltaz1*atan2(deltax1*deltay1, deltaz1*r));

    r = sqrt(deltax2*deltax2 + deltay1*deltay1 + deltaz1*deltaz1);

    res += -1*(deltax2*log(deltay1 + r) + deltay1*log(deltax2 + r) -
    		deltaz1*atan2(deltax2*deltay1, deltaz1*r));

    r = sqrt(deltax1*deltax1 + deltay2*deltay2 + deltaz1*deltaz1);

    res += -1*(deltax1*log(deltay2 + r) + deltay2*log(deltax1 + r) -
    		deltaz1*atan2(deltax1*deltay2, deltaz1*r));

    r = sqrt(deltax2*deltax2 + deltay2*deltay2 + deltaz1*deltaz1);

    res += 1*(deltax2*log(deltay2 + r) + deltay2*log(deltax2 + r) -
    		deltaz1*atan2(deltax2*deltay2, deltaz1*r));

    r = sqrt(deltax1*deltax1 + deltay1*deltay1 + deltaz2*deltaz2);

    res += -1*(deltax1*log(deltay1 + r) + deltay1*log(deltax1 + r) -
    		deltaz2*atan2(deltax1*deltay1, deltaz2*r));

    r = sqrt(deltax2*deltax2 + deltay1*deltay1 + deltaz2*deltaz2);

    res += 1*(deltax2*log(deltay1 + r) + deltay1*log(deltax2 + r) -
    		deltaz2*atan2(deltax2*deltay1, deltaz2*r));

    r = sqrt(deltax1*deltax1 + deltay2*deltay2 + deltaz2*deltaz2);

    res += 1*(deltax1*log(deltay2 + r) + deltay2*log(deltax1 + r) -
    		deltaz2*atan2(deltax1*deltay2, deltaz2*r));

    r = sqrt(deltax2*deltax2 + deltay2*deltay2 + deltaz2*deltaz2);

    res += -1*(deltax2*log(deltay2 + r) + deltay2*log(deltax2 + r) -
    		deltaz2*atan2(deltax2*deltay2, deltaz2*r));

    /* Now all that is left is to multiply res by the gravitational constant and
       density and convert it to mGal units */
    res *= G*SI2MGAL*dens;

    return res;
}


/* Calculates the gxx gravity gradient tensor component cause by a prism. */
double prism_gxx(double dens, double x1, double x2, double y1, double y2,
                double z1, double z2, double xp, double yp, double zp)
{
    double r, res, deltax1, deltax2, deltay1, deltay2, deltaz1, deltaz2;

    /* First thing to do is make P the origin of the coordinate system */
    deltax1 = x1 - xp;
    deltax2 = x2 - xp;
    deltay1 = y1 - yp;
    deltay2 = y2 - yp;
    deltaz1 = z1 - zp;
    deltaz2 = z2 - zp;

    res = 0;

    /* Evaluate the integration limits */
    r = sqrt(deltax1*deltax1 + deltay1*deltay1 + deltaz1*deltaz1);

    res += 1*atan2(deltay1*deltaz1, deltax1*r);

    r = sqrt(deltax2*deltax2 + deltay1*deltay1 + deltaz1*deltaz1);

    res += -1*atan2(deltay1*deltaz1, deltax2*r);

    r = sqrt(deltax1*deltax1 + deltay2*deltay2 + deltaz1*deltaz1);

    res += -1*atan2(deltay2*deltaz1, deltax1*r);

    r = sqrt(deltax2*deltax2 + deltay2*deltay2 + deltaz1*deltaz1);

    res += 1*atan2(deltay2*deltaz1, deltax2*r);

    r = sqrt(deltax1*deltax1 + deltay1*deltay1 + deltaz2*deltaz2);

    res += -1*atan2(deltay1*deltaz2, deltax1*r);

    r = sqrt(deltax2*deltax2 + deltay1*deltay1 + deltaz2*deltaz2);

    res += 1*atan2(deltay1*deltaz2, deltax2*r);

    r = sqrt(deltax1*deltax1 + deltay2*deltay2 + deltaz2*deltaz2);

    res += 1*atan2(deltay2*deltaz2, deltax1*r);

    r = sqrt(deltax2*deltax2 + deltay2*deltay2 + deltaz2*deltaz2);

    res += -1*atan2(deltay2*deltaz2, deltax2*r);

    /* Now all that is left is to multiply res by the gravitational constant and
        density and convert it to Eotvos units */
    res *= G*SI2EOTVOS*dens;

    return res;
}


/* Calculates the gxy gravity gradient tensor component cause by a prism. */
double prism_gxy(double dens, double x1, double x2, double y1, double y2,
                 double z1, double z2, double xp, double yp, double zp)
{
    double r, res, deltax1, deltax2, deltay1, deltay2, deltaz1, deltaz2;

    /* First thing to do is make P the origin of the coordinate system */
    deltax1 = x1 - xp;
    deltax2 = x2 - xp;
    deltay1 = y1 - yp;
    deltay2 = y2 - yp;
    deltaz1 = z1 - zp;
    deltaz2 = z2 - zp;

    res = 0;

    /* Evaluate the integration limits */
    r = sqrt(deltax1*deltax1 + deltay1*deltay1 + deltaz1*deltaz1);

    res += 1*(-1*log(deltaz1 + r));

    r = sqrt(deltax2*deltax2 + deltay1*deltay1 + deltaz1*deltaz1);

    res += -1*(-1*log(deltaz1 + r));

    r = sqrt(deltax1*deltax1 + deltay2*deltay2 + deltaz1*deltaz1);

    res += -1*(-1*log(deltaz1 + r));

    r = sqrt(deltax2*deltax2 + deltay2*deltay2 + deltaz1*deltaz1);

    res += 1*(-1*log(deltaz1 + r));

    r = sqrt(deltax1*deltax1 + deltay1*deltay1 + deltaz2*deltaz2);

    res += -1*(-1*log(deltaz2 + r));

    r = sqrt(deltax2*deltax2 + deltay1*deltay1 + deltaz2*deltaz2);

    res += 1*(-1*log(deltaz2 + r));

    r = sqrt(deltax1*deltax1 + deltay2*deltay2 + deltaz2*deltaz2);

    res += 1*(-1*log(deltaz2 + r));

    r = sqrt(deltax2*deltax2 + deltay2*deltay2 + deltaz2*deltaz2);

    res += -1*(-1*log(deltaz2 + r));

    /* Now all that is left is to multiply res by the gravitational constant and
        density and convert it to Eotvos units */
    res *= G*SI2EOTVOS*dens;

    return res;
}


/* Calculates the gxz gravity gradient tensor component cause by a prism. */
double prism_gxz(double dens, double x1, double x2, double y1, double y2,
                 double z1, double z2, double xp, double yp, double zp)
{
    double r, res, deltax1, deltax2, deltay1, deltay2, deltaz1, deltaz2;

    /* First thing to do is make P the origin of the coordinate system */
    deltax1 = x1 - xp;
    deltax2 = x2 - xp;
    deltay1 = y1 - yp;
    deltay2 = y2 - yp;
    deltaz1 = z1 - zp;
    deltaz2 = z2 - zp;

    res = 0;

    /* Evaluate the integration limits */
    r = sqrt(deltax1*deltax1 + deltay1*deltay1 + deltaz1*deltaz1);

    res += 1*(-1*log(deltay1 + r));

    r = sqrt(deltax2*deltax2 + deltay1*deltay1 + deltaz1*deltaz1);

    res += -1*(-1*log(deltay1 + r));

    r = sqrt(deltax1*deltax1 + deltay2*deltay2 + deltaz1*deltaz1);

    res += -1*(-1*log(deltay2 + r));

    r = sqrt(deltax2*deltax2 + deltay2*deltay2 + deltaz1*deltaz1);

    res += 1*(-1*log(deltay2 + r));

    r = sqrt(deltax1*deltax1 + deltay1*deltay1 + deltaz2*deltaz2);

    res += -1*(-1*log(deltay1 + r));

    r = sqrt(deltax2*deltax2 + deltay1*deltay1 + deltaz2*deltaz2);

    res += 1*(-1*log(deltay1 + r));

    r = sqrt(deltax1*deltax1 + deltay2*deltay2 + deltaz2*deltaz2);

    res += 1*(-1*log(deltay2 + r));

    r = sqrt(deltax2*deltax2 + deltay2*deltay2 + deltaz2*deltaz2);

    res += -1*(-1*log(deltay2 + r));

    /* Now all that is left is to multiply res by the gravitational constant and
        density and convert it to Eotvos units */
    res *= G*SI2EOTVOS*dens;

    return res;
}


/* Calculates the gyy gravity gradient tensor component cause by a prism. */
double prism_gyy(double dens, double x1, double x2, double y1, double y2,
                 double z1, double z2, double xp, double yp, double zp)
{
    double r, res, deltax1, deltax2, deltay1, deltay2, deltaz1, deltaz2;

    /* First thing to do is make P the origin of the coordinate system */
    deltax1 = x1 - xp;
    deltax2 = x2 - xp;
    deltay1 = y1 - yp;
    deltay2 = y2 - yp;
    deltaz1 = z1 - zp;
    deltaz2 = z2 - zp;

    res = 0;

    /* Evaluate the integration limits */
    r = sqrt(deltax1*deltax1 + deltay1*deltay1 + deltaz1*deltaz1);

    res += 1*(atan2(deltaz1*deltax1, deltay1*r));

    r = sqrt(deltax2*deltax2 + deltay1*deltay1 + deltaz1*deltaz1);

    res += -1*(atan2(deltaz1*deltax2, deltay1*r));

    r = sqrt(deltax1*deltax1 + deltay2*deltay2 + deltaz1*deltaz1);

    res += -1*(atan2(deltaz1*deltax1, deltay2*r));

    r = sqrt(deltax2*deltax2 + deltay2*deltay2 + deltaz1*deltaz1);

    res += 1*(atan2(deltaz1*deltax2, deltay2*r));

    r = sqrt(deltax1*deltax1 + deltay1*deltay1 + deltaz2*deltaz2);

    res += -1*(atan2(deltaz2*deltax1, deltay1*r));

    r = sqrt(deltax2*deltax2 + deltay1*deltay1 + deltaz2*deltaz2);

    res += 1*(atan2(deltaz2*deltax2, deltay1*r));

    r = sqrt(deltax1*deltax1 + deltay2*deltay2 + deltaz2*deltaz2);

    res += 1*(atan2(deltaz2*deltax1, deltay2*r));

    r = sqrt(deltax2*deltax2 + deltay2*deltay2 + deltaz2*deltaz2);

    res += -1*(atan2(deltaz2*deltax2, deltay2*r));

    /* Now all that is left is to multiply res by the gravitational constant and
        density and convert it to Eotvos units */
    res *= G*SI2EOTVOS*dens;

    return res;
}


/* Calculates the gyz gravity gradient tensor component cause by a prism. */
double prism_gyz(double dens, double x1, double x2, double y1, double y2,
                 double z1, double z2, double xp, double yp, double zp)
{
    double r, res, deltax1, deltax2, deltay1, deltay2, deltaz1, deltaz2;

    /* First thing to do is make P the origin of the coordinate system */
    deltax1 = x1 - xp;
    deltax2 = x2 - xp;
    deltay1 = y1 - yp;
    deltay2 = y2 - yp;
    deltaz1 = z1 - zp;
    deltaz2 = z2 - zp;

    res = 0;

    /* Evaluate the integration limits */
    r = sqrt(deltax1*deltax1 + deltay1*deltay1 + deltaz1*deltaz1);

    res += 1*(-1*log(deltax1 + r));

    r = sqrt(deltax2*deltax2 + deltay1*deltay1 + deltaz1*deltaz1);

    res += -1*(-1*log(deltax2 + r));

    r = sqrt(deltax1*deltax1 + deltay2*deltay2 + deltaz1*deltaz1);

    res += -1*(-1*log(deltax1 + r));

    r = sqrt(deltax2*deltax2 + deltay2*deltay2 + deltaz1*deltaz1);

    res += 1*(-1*log(deltax2 + r));

    r = sqrt(deltax1*deltax1 + deltay1*deltay1 + deltaz2*deltaz2);

    res += -1*(-1*log(deltax1 + r));

    r = sqrt(deltax2*deltax2 + deltay1*deltay1 + deltaz2*deltaz2);

    res += 1*(-1*log(deltax2 + r));

    r = sqrt(deltax1*deltax1 + deltay2*deltay2 + deltaz2*deltaz2);

    res += 1*(-1*log(deltax1 + r));

    r = sqrt(deltax2*deltax2 + deltay2*deltay2 + deltaz2*deltaz2);

    res += -1*(-1*log(deltax2 + r));

    /* Now all that is left is to multiply res by the gravitational constant and
        density and convert it to Eotvos units */
    res *= G*SI2EOTVOS*dens;

    return res;
}



/* Calculates the gzz gravity gradient tensor component cause by a prism. */
double prism_gzz(double dens, double x1, double x2, double y1, double y2,
                 double z1, double z2, double xp, double yp, double zp)
{
    double r, res, deltax1, deltax2, deltay1, deltay2, deltaz1, deltaz2;

    /* First thing to do is make P the origin of the coordinate system */
    deltax1 = x1 - xp;
    deltax2 = x2 - xp;
    deltay1 = y1 - yp;
    deltay2 = y2 - yp;
    deltaz1 = z1 - zp;
    deltaz2 = z2 - zp;

    res = 0;

    /* Evaluate the integration limits */
    r = sqrt(deltax1*deltax1 + deltay1*deltay1 + deltaz1*deltaz1);

    res += 1*(atan2(deltax1*deltay1, deltaz1*r));

    r = sqrt(deltax2*deltax2 + deltay1*deltay1 + deltaz1*deltaz1);

    res += -1*(atan2(deltax2*deltay1, deltaz1*r));

    r = sqrt(deltax1*deltax1 + deltay2*deltay2 + deltaz1*deltaz1);

    res += -1*(atan2(deltax1*deltay2, deltaz1*r));

    r = sqrt(deltax2*deltax2 + deltay2*deltay2 + deltaz1*deltaz1);

    res += 1*(atan2(deltax2*deltay2, deltaz1*r));

    r = sqrt(deltax1*deltax1 + deltay1*deltay1 + deltaz2*deltaz2);

    res += -1*(atan2(deltax1*deltay1, deltaz2*r));

    r = sqrt(deltax2*deltax2 + deltay1*deltay1 + deltaz2*deltaz2);

    res += 1*(atan2(deltax2*deltay1, deltaz2*r));

    r = sqrt(deltax1*deltax1 + deltay2*deltay2 + deltaz2*deltaz2);

    res += 1*(atan2(deltax1*deltay2, deltaz2*r));

    r = sqrt(deltax2*deltax2 + deltay2*deltay2 + deltaz2*deltaz2);

    res += -1*(atan2(deltax2*deltay2, deltaz2*r));

    /* Now all that is left is to multiply res by the gravitational constant and
        density and convert it to Eotvos units */
    res *= G*SI2EOTVOS*dens;

    return res;
}
