/*
Functions that calculate the gravitational potential and its first and second
derivatives for the rectangular prism using the formulas in Nagy et al. (2000).
Supports Cartesian and spherical coordinates.

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
#include "libprism.h"


double prism_pot(PRISM prism, double xp, double yp, double zp)
{
    double x[2], y[2], z[2], kernel, res, r;
    register int i, j, k;

    /* This field has a problem with the log(z+r) when bellow the prism */
    /* Will calculate on top and correct the sign later */
    if(zp > prism.z2)
    {
        zp = prism.z1 - (zp - prism.z2);
    }

    /* First thing to do is make P the origin of the coordinate system */
    x[0] = prism.x2 - xp;
    x[1] = prism.x1 - xp;
    y[0] = prism.y2 - yp;
    y[1] = prism.y1 - yp;
    z[0] = prism.z2 - zp;
    z[1] = prism.z1 - zp;

    res = 0;

    /* Evaluate the integration limits */
    for(k=0; k<=1; k++)
    {
        for(j=0; j<=1; j++)
        {
            for(i=0; i<=1; i++)
            {
                r = sqrt(x[i]*x[i] + y[j]*y[j] + z[k]*z[k]);

                kernel = x[i]*y[j]*log(z[k] + r)
                         + y[j]*z[k]*log(x[i] + r)
                         + x[i]*z[k]*log(y[j] + r)
                         - 0.5*x[i]*x[i]*atan2(z[k]*y[j], x[i]*r)
                         - 0.5*y[j]*y[j]*atan2(z[k]*x[i], y[j]*r)
                         - 0.5*z[k]*z[k]*atan2(x[i]*y[j], z[k]*r);

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

    /* First thing to do is make P the origin of the coordinate system */
    x[0] = prism.x2 - xp;
    x[1] = prism.x1 - xp;
    y[0] = prism.y2 - yp;
    y[1] = prism.y1 - yp;
    z[0] = prism.z2 - zp;
    z[1] = prism.z1 - zp;

    res = 0;

    /* Evaluate the integration limits */
    for(k=0; k<=1; k++)
    {
        for(j=0; j<=1; j++)
        {
            for(i=0; i<=1; i++)
            {
                r = sqrt(x[i]*x[i] + y[j]*y[j] + z[k]*z[k]);

                kernel = -(y[j]*log(z[k] + r) + z[k]*log(y[j] + r)
                           - x[i]*atan2(z[k]*y[j], x[i]*r));

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

    /* First thing to do is make P the origin of the coordinate system */
    x[0] = prism.x2 - xp;
    x[1] = prism.x1 - xp;
    y[0] = prism.y2 - yp;
    y[1] = prism.y1 - yp;
    z[0] = prism.z2 - zp;
    z[1] = prism.z1 - zp;

    res = 0;

    /* Evaluate the integration limits */
    for(k=0; k<=1; k++)
    {
        for(j=0; j<=1; j++)
        {
            for(i=0; i<=1; i++)
            {
                r = sqrt(x[i]*x[i] + y[j]*y[j] + z[k]*z[k]);

                kernel = -(z[k]*log(x[i] + r) + x[i]*log(z[k] + r)
                           - y[j]*atan2(z[k]*x[i], y[j]*r));

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
    int changed = 0;

    /* This field has a problem with the log(z+r) when bellow the prism */
    /* Will calculate on top and correct the sign later */
    if(zp > prism.z2)
    {
        zp = prism.z1 - (zp - prism.z2);
        changed = 1;
    }

    /* First thing to do is make P the origin of the coordinate system */
    x[0] = prism.x2 - xp;
    x[1] = prism.x1 - xp;
    y[0] = prism.y2 - yp;
    y[1] = prism.y1 - yp;
    z[0] = prism.z2 - zp;
    z[1] = prism.z1 - zp;

    res = 0;

    /* Evaluate the integration limits */
    for(k=0; k<=1; k++)
    {
        for(j=0; j<=1; j++)
        {
            for(i=0; i<=1; i++)
            {
                r = sqrt(x[i]*x[i] + y[j]*y[j] + z[k]*z[k]);

                kernel = -(x[i]*log(y[j] + r) + y[j]*log(x[i] + r)
                           - z[k]*atan2(x[i]*y[j], z[k]*r));

                res += pow(-1, i + j + k)*kernel;
            }
        }
    }

    /* Now all that is left is to multiply res by the gravitational constant and
       density and convert it to mGal units */
    res *= G*SI2MGAL*prism.density;

    /* Need to correct for the fact that I changed the sign of zp */
    if(changed)
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

    /* This field has a problem with the log(z+r) when bellow the prism */
    /* Will calculate on top and correct the sign later */
    if(zp > prism.z2)
    {
        zp = prism.z1 - (zp - prism.z2);
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

int global2local(double lon, double lat, double r, PRISM prism, double *x,
                 double *y, double *z)
{
    double cosa, cosb, sina, sinb, d2r = PI/180., X, Y, Z;

    X = r*cos(d2r*lat)*cos(d2r*lon) -
        prism.r*cos(d2r*prism.lat)*cos(d2r*prism.lon);
    Y = r*cos(d2r*lat)*sin(d2r*lon) -
        prism.r*cos(d2r*prism.lat)*sin(d2r*prism.lon);
    Z = r*sin(d2r*lat) - prism.r*sin(d2r*prism.lat);

    cosa = cos(d2r*(90 - prism.lat));
    sina = sin(d2r*(90 - prism.lat));
    cosb = cos(d2r*(180 - prism.lon));
    sinb = sin(d2r*(180 - prism.lon));

    *x = X*cosa*cosb - Y*cosa*sinb + Z*sina;
    *y = -X*sinb - Y*cosb;
    /* -1 because Nagy et al. (2000) use z->down */
    *z = -1*(-X*sina*cosb + Y*sina*sinb + Z*cosa);

    return 0;
}

int rotate_gravity(double *atprism, PRISM prism, double lon, double lat,
                  double r, double *atpoint)
{
    #define POS(x, y, cols) (((x)*(cols))+(y))

    register int i, k;
    double R[9], d2r, cosbeta, sinbeta, cosphi, sinphi, cosphil, sinphil;

    /* degrees to radians */
    d2r = PI/180.;

    cosbeta = cos(d2r*(prism.lon - lon));
    sinbeta = sin(d2r*(prism.lon - lon));
    cosphi = cos(d2r*lat);
    sinphi = sin(d2r*lat);
    cosphil = cos(d2r*prism.lat);
    sinphil = sin(d2r*prism.lat);

    /* The transformation matrix */
    R[0] = cosbeta*sinphi*sinphil + cosphi*cosphil;
    R[1] = sinbeta*sinphi;
    R[2] = -cosbeta*sinphi*cosphil + cosphi*sinphil;
    R[3] = -sinbeta*sinphil;
    R[4] = cosbeta;
    R[5] = sinbeta*cosphil;
    R[6] = -cosbeta*cosphi*sinphil + sinphi*cosphil;
    R[7] = -sinbeta*cosphi;
    R[8] = cosbeta*cosphi*cosphil + sinphi*sinphil;

    /* Matrix-vector multiplication */
    for(i = 0; i < 3; i++)
    {
       atpoint[i] = 0;
       for(k = 0; k < 3; k++)
       {
           atpoint[i] += R[POS(i, k, 3)]*atprism[k];
       }
    }
    #undef POS
    return 0;
}

int rotate_tensor(double *atprism, PRISM prism, double lon, double lat,
                    double r, double *atpoint)
{
    #define POS(x, y, cols) (((x)*(cols))+(y))

    register int i, j, k;
    double R[9], tmp[9], d2r, cosbeta, sinbeta, cosphi, sinphi, cosphil, sinphil;

    /* degrees to radians */
    d2r = PI/180.;

    cosbeta = cos(d2r*(prism.lon - lon));
    sinbeta = sin(d2r*(prism.lon - lon));
    cosphi = cos(d2r*lat);
    sinphi = sin(d2r*lat);
    cosphil = cos(d2r*prism.lat);
    sinphil = sin(d2r*prism.lat);

    /* The transformation matrix */
    R[0] = cosbeta*sinphi*sinphil + cosphi*cosphil;
    R[1] = sinbeta*sinphi;
    R[2] = -cosbeta*sinphi*cosphil + cosphi*sinphil;
    R[3] = -sinbeta*sinphil;
    R[4] = cosbeta;
    R[5] = sinbeta*cosphil;
    R[6] = -cosbeta*cosphi*sinphil + sinphi*cosphil;
    R[7] = -sinbeta*cosphi;
    R[8] = cosbeta*cosphi*cosphil + sinphi*sinphil;

    /* Multiply tmp = R*Tensor */
    for(i = 0; i < 3; i++)
    {
        for(j = 0; j < 3; j++)
        {
            tmp[POS(i, j, 3)] = 0;
            for(k = 0; k < 3; k++)
            {
                tmp[POS(i, j, 3)] += R[POS(i, k, 3)]*atprism[POS(k, j, 3)];
            }
        }
    }

    /* Multiply tmp*R^T */
    for(i = 0; i < 3; i++)
    {
        for(j = 0; j < 3; j++)
        {
            atpoint[POS(i, j, 3)] = 0;
            for(k = 0; k < 3; k++)
            {
                atpoint[POS(i, j, 3)] += tmp[POS(i, k, 3)]*R[POS(j, k, 3)];
            }
        }
    }

    #undef POS
    return 0;
}

int prism_ggt_sph(PRISM prism, double lonp, double latp, double rp, double *ggt)
{
    double x = 0, y = 0, z = 0, ggtprism[9], ggtpoint[9];

    global2local(lonp, latp, rp, prism, &x, &y, &z);
    ggtprism[0] = prism_gxx(prism, x, y, z);
    ggtprism[1] = prism_gxy(prism, x, y, z);
    /* -1 because the prisms z is Down, but transformation assumes z is Up */
    /* z -> Up is the system of the tesseroid */
    ggtprism[2] = -1*prism_gxz(prism, x, y, z);
    ggtprism[3] = ggtprism[1];
    ggtprism[4] = prism_gyy(prism, x, y, z);
    /* Same as xz */
    ggtprism[5] = -1*prism_gyz(prism, x, y, z);
    ggtprism[6] = ggtprism[2];
    ggtprism[7] = ggtprism[5];
    ggtprism[8] = -(ggtprism[0] + ggtprism[4]);
    rotate_tensor(ggtprism, prism, lonp, latp, rp, ggtpoint);
    ggt[0] = ggtpoint[0];
    ggt[1] = ggtpoint[1];
    ggt[2] = ggtpoint[2];
    ggt[3] = ggtpoint[4];
    ggt[4] = ggtpoint[5];
    ggt[5] = ggtpoint[8];

    return 0;
}

int prism_g_sph(PRISM prism, double lonp, double latp, double rp, double *gx,
                double *gy, double *gz)
{
    double x = 0, y = 0, z = 0, gprism[3], gpoint[3];

    global2local(lonp, latp, rp, prism, &x, &y, &z);
    gprism[0] = prism_gx(prism, x, y, z);
    gprism[1] = prism_gy(prism, x, y, z);
    /* Nagy wants z down, but the transformation assumes z up */
    gprism[2] = -prism_gz(prism, x, y, z);
    rotate_gravity(gprism, prism, lonp, latp, rp, gpoint);
    *gx = gpoint[0];
    *gy = gpoint[1];
    /* Put z back down again to maintain the normal convention for gz */
    *gz = -gpoint[2];

    return 0;
}

double prism_pot_sph(PRISM prism, double lonp, double latp, double rp)
{
    double x = 0, y = 0, z = 0, res;

    global2local(lonp, latp, rp, prism, &x, &y, &z);
    res = prism_pot(prism, x, y, z);

    return res;
}
