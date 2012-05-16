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


#include <math.h>
#include "geometry.h"
#include "constants.h"
#include "grav_prism.h"


/* Calculates the potential cause by a prism. */
double prism_pot(PRISM prism, double xp, double yp, double zp)
{
    double x[2], y[2], z[2], kernel, res, r;
    register int i, j, k; 

    /* This field has a problem with the log(z+r) when zp > z1 */
    /* Changing the sign is not a problem since the potential should be equal
    on top or bellow */
    if(zp > prism.z1)
    {
        zp = -zp;
    }
    
    /* First thing to do is make P the origin of the coordinate system */
    x[0] = prism.x1 - xp;
    x[1] = prism.x2 - xp;
    y[0] = prism.y1 - yp;
    y[1] = prism.y2 - yp;
    z[0] = prism.z1 - zp;
    z[1] = prism.z2 - zp;

    res = 0;

    /* Evaluate the integration limits */
    for(k=0; k<=1; k++)
    {
        for(j=0; j<=1; j++)
        {
            for(i=0; i<=1; i++)
            {
                r = sqrt(x[i]*x[i] + y[j]*y[j] + z[k]*z[k]);

                kernel = -x[i]*y[j]*log(z[k] + r)
                         - y[j]*z[k]*log(x[i] + r)
                         - x[i]*z[k]*log(y[j] + r)
                         + 0.5*x[i]*x[i]*atan2(z[k]*y[j], x[i]*r)
                         + 0.5*y[j]*y[j]*atan2(z[k]*x[i], y[j]*r)
                         + 0.5*z[k]*z[k]*atan2(x[i]*y[j], z[k]*r);

                res += pow(-1, i + j + k)*kernel;
            }
        }
    }

    /* Now all that is left is to multiply res by the gravitational constant and
       density */
    res *= G*prism.density;

    return res;
}


/* Calculates the x component of gravitational attraction cause by a prism. */
double prism_gx(PRISM prism, double xp, double yp, double zp)
{
    double x[2], y[2], z[2], kernel, res, r;
    register int i, j, k;

    /* This field has a problem with the log(z+r) when zp > z1 */
    /* Changing the sign is not a problem since the gx should be equal
    on top or bellow */
    if(zp > prism.z1)
    {
        zp = -zp;
    }
    
    /* First thing to do is make P the origin of the coordinate system */
    x[0] = prism.x1 - xp;
    x[1] = prism.x2 - xp;
    y[0] = prism.y1 - yp;
    y[1] = prism.y2 - yp;
    z[0] = prism.z1 - zp;
    z[1] = prism.z2 - zp;

    res = 0;

    /* Evaluate the integration limits */
    for(k=0; k<=1; k++)
    {
        for(j=0; j<=1; j++)
        {
            for(i=0; i<=1; i++)
            {
                r = sqrt(x[i]*x[i] + y[j]*y[j] + z[k]*z[k]);

                kernel = y[j]*log(z[k] + r) + z[k]*log(y[j] + r)
                        - x[i]*atan2(z[k]*y[j], x[i]*r);

                res += pow(-1, i + j + k)*kernel;
            }
        }
    }

    /* Now all that is left is to multiply res by the gravitational constant and
       density and convert it to mGal units */
    res *= G*SI2MGAL*prism.density;

    return res;
}


/* Calculates the y component of gravitational attraction cause by a prism. */
double prism_gy(PRISM prism, double xp, double yp, double zp)
{
    double x[2], y[2], z[2], kernel, res, r;
    register int i, j, k;

    /* This field has a problem with the log(z+r) when zp > z1 */
    /* Changing the sign is not a problem since the gy should be equal
    on top or bellow */
    if(zp > prism.z1)
    {
        zp = -zp;
    }
    
    /* First thing to do is make P the origin of the coordinate system */
    x[0] = prism.x1 - xp;
    x[1] = prism.x2 - xp;
    y[0] = prism.y1 - yp;
    y[1] = prism.y2 - yp;
    z[0] = prism.z1 - zp;
    z[1] = prism.z2 - zp;

    res = 0;

    /* Evaluate the integration limits */
    for(k=0; k<=1; k++)
    {
        for(j=0; j<=1; j++)
        {
            for(i=0; i<=1; i++)
            {
                r = sqrt(x[i]*x[i] + y[j]*y[j] + z[k]*z[k]);

                kernel = z[k]*log(x[i] + r) + x[i]*log(z[k] + r)
                        - y[j]*atan2(z[k]*x[i], y[j]*r);

                res += pow(-1, i + j + k)*kernel;
            }
        }
    }

    /* Now all that is left is to multiply res by the gravitational constant and
       density and convert it to mGal units */
    res *= G*SI2MGAL*prism.density;

    return res;
}


/* Calculates the z component of gravitational attraction cause by a prism. */
double prism_gz(PRISM prism, double xp, double yp, double zp)
{
    double x[2], y[2], z[2], kernel, res, r;
    register int i, j, k;

    /* This field has a problem with the log(z+r) when zp > z1 */
    /* Will correct for the change in sign before returning */
    if(zp > prism.z1)
    {
        zp = -zp;
    }
    
    /* First thing to do is make P the origin of the coordinate system */
    x[0] = prism.x1 - xp;
    x[1] = prism.x2 - xp;
    y[0] = prism.y1 - yp;
    y[1] = prism.y2 - yp;
    z[0] = prism.z1 - zp;
    z[1] = prism.z2 - zp;

    res = 0;

    /* Evaluate the integration limits */
    for(k=0; k<=1; k++)
    {
        for(j=0; j<=1; j++)
        {
            for(i=0; i<=1; i++)
            {
                r = sqrt(x[i]*x[i] + y[j]*y[j] + z[k]*z[k]);

                kernel = x[i]*log(y[j] + r) + y[j]*log(x[i] + r)
                        - z[k]*atan2(x[i]*y[j], z[k]*r);

                res += pow(-1, i + j + k)*kernel;
            }
        }
    }

    /* Now all that is left is to multiply res by the gravitational constant and
       density and convert it to mGal units */
    res *= G*SI2MGAL*prism.density;

    /* Need to correct for the fact that I changed the sign of zp */
    if(zp > prism.z1)
    {
        res = -res;
    }

    return res;
}


/* Calculates the gxx gravity gradient tensor component cause by a prism. */
double prism_gxx(PRISM prism, double xp, double yp, double zp)
{
    double r, res, deltax1, deltax2, deltay1, deltay2, deltaz1, deltaz2;

    /* First thing to do is make P the origin of the coordinate system */
    deltax1 = prism.x1 - xp;
    deltax2 = prism.x2 - xp;
    deltay1 = prism.y1 - yp;
    deltay2 = prism.y2 - yp;
    deltaz1 = prism.z1 - zp;
    deltaz2 = prism.z2 - zp;

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
    res *= G*SI2EOTVOS*prism.density;

    return res;
}


/* Calculates the gxy gravity gradient tensor component cause by a prism. */
double prism_gxy(PRISM prism, double xp, double yp, double zp)
{
    double r, res, deltax1, deltax2, deltay1, deltay2, deltaz1, deltaz2;

    /* This field has a problem with the log(z+r) when zp > z1 */
    /* Changing the sign is not a problem since the gxy should be equal
    on top or bellow */
    if(zp > prism.z1)
    {
        zp = -zp;
    }
    
    /* First thing to do is make P the origin of the coordinate system */
    deltax1 = prism.x1 - xp;
    deltax2 = prism.x2 - xp;
    deltay1 = prism.y1 - yp;
    deltay2 = prism.y2 - yp;
    deltaz1 = prism.z1 - zp;
    deltaz2 = prism.z2 - zp;

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
    res *= G*SI2EOTVOS*prism.density;

    return res;
}


/* Calculates the gxz gravity gradient tensor component cause by a prism. */
double prism_gxz(PRISM prism, double xp, double yp, double zp)
{
    double r, res, deltax1, deltax2, deltay1, deltay2, deltaz1, deltaz2;

    /* First thing to do is make P the origin of the coordinate system */
    deltax1 = prism.x1 - xp;
    deltax2 = prism.x2 - xp;
    deltay1 = prism.y1 - yp;
    deltay2 = prism.y2 - yp;
    deltaz1 = prism.z1 - zp;
    deltaz2 = prism.z2 - zp;

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
    res *= G*SI2EOTVOS*prism.density;

    return res;
}


/* Calculates the gyy gravity gradient tensor component cause by a prism. */
double prism_gyy(PRISM prism, double xp, double yp, double zp)
{
    double r, res, deltax1, deltax2, deltay1, deltay2, deltaz1, deltaz2;

    /* First thing to do is make P the origin of the coordinate system */
    deltax1 = prism.x1 - xp;
    deltax2 = prism.x2 - xp;
    deltay1 = prism.y1 - yp;
    deltay2 = prism.y2 - yp;
    deltaz1 = prism.z1 - zp;
    deltaz2 = prism.z2 - zp;

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
    res *= G*SI2EOTVOS*prism.density;

    return res;
}


/* Calculates the gyz gravity gradient tensor component cause by a prism. */
double prism_gyz(PRISM prism, double xp, double yp, double zp)
{
    double r, res, deltax1, deltax2, deltay1, deltay2, deltaz1, deltaz2;

    /* First thing to do is make P the origin of the coordinate system */
    deltax1 = prism.x1 - xp;
    deltax2 = prism.x2 - xp;
    deltay1 = prism.y1 - yp;
    deltay2 = prism.y2 - yp;
    deltaz1 = prism.z1 - zp;
    deltaz2 = prism.z2 - zp;

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
    res *= G*SI2EOTVOS*prism.density;

    return res;
}



/* Calculates the gzz gravity gradient tensor component cause by a prism. */
double prism_gzz(PRISM prism, double xp, double yp, double zp)
{
    double r, res, deltax1, deltax2, deltay1, deltay2, deltaz1, deltaz2;

    /* First thing to do is make P the origin of the coordinate system */
    deltax1 = prism.x1 - xp;
    deltax2 = prism.x2 - xp;
    deltay1 = prism.y1 - yp;
    deltay2 = prism.y2 - yp;
    deltaz1 = prism.z1 - zp;
    deltaz2 = prism.z2 - zp;

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
    res *= G*SI2EOTVOS*prism.density;

    return res;
}
