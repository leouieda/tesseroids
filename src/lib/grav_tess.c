/*
Functions that calculate the gravitational potential and its first and second
derivatives for the tesseroid.

References
----------

* Grombein, T.; Seitz, K.; Heck, B. (2010): Untersuchungen zur effizienten
Berechnung topographischer Effekte auf den Gradiententensor am Fallbeispiel der
Satellitengradiometriemission GOCE.
KIT Scientific Reports 7547, ISBN 978-3-86644-510-9, KIT Scientific Publishing,
Karlsruhe, Germany.
*/


#include <math.h>
#include "logger.h"
#include "geometry.h"
#include "glq.h"
#include "constants.h"
#include "grav_tess.h"

#define STKSIZE 10000


/* Calculates the field of a tesseroid model at a given point. */
double calc_tess_model(TESSEROID *model, int size, double lonp, double latp,
    double rp, GLQ *glq_lon, GLQ *glq_lat, GLQ *glq_r,
    double (*field)(TESSEROID, double, double, double, GLQ, GLQ, GLQ))
{
    double res;
    int tess;

    res = 0;
    for(tess = 0; tess < size; tess++)
    {
        glq_set_limits(model[tess].w, model[tess].e, glq_lon);
        glq_set_limits(model[tess].s, model[tess].n, glq_lat);
        glq_set_limits(model[tess].r1, model[tess].r2, glq_r);
        glq_precompute_sincos(glq_lat);
        res += field(model[tess], lonp, latp, rp, *glq_lon, *glq_lat, *glq_r);
    }
    return res;
}


/* Adaptatively calculate the field of a tesseroid model at a given point */
double calc_tess_model_adapt(TESSEROID *model, int size, double lonp,
          double latp, double rp, GLQ *glq_lon, GLQ *glq_lat, GLQ *glq_r,
          double (*field)(TESSEROID, double, double, double, GLQ, GLQ, GLQ),
          double ratio, double power)
{
    double res, distance, lont, latt, rt, d2r = PI/180.,
           coslatp, sinlatp, rp_sqr, rlonp,
           Llon, Llat, Lr,
           sinlatt, coslatt;
    int t, n, nlon, nlat, nr, stktop = 0;
    TESSEROID stack[STKSIZE], tess;

    #define SQ(x) (x)*(x)
    /* Pre-compute these things out of the loop */
    rlonp = d2r*lonp;
    rp_sqr = SQ(rp);
    coslatp = cos(d2r*latp);
    sinlatp = sin(d2r*latp);
    res = 0;
    for(t = 0; t < size; t++)
    {
        /* Initialize the tesseroid division stack (a LIFO structure) */
        stack[0] = model[t];
        stktop = 0;
        while(stktop >= 0)
        {
            /* Pop the stack */
            tess = stack[stktop];
            stktop--;
            /* Compute the distance from the computation point to the
             * geometric center of the tesseroid. */
            rt = 0.5*(tess.r2 + tess.r1);
            lont = d2r*0.5*(tess.w + tess.e);
            latt = d2r*0.5*(tess.s + tess.n);
            sinlatt = sin(latt);
            coslatt = cos(latt);
            distance = sqrt(rp_sqr + SQ(rt) - 2*rp*rt*(
                sinlatp*sinlatt + coslatp*coslatt*cos(rlonp - lont)));
            /* Get the size of each dimension of the tesseroid in meters */
            Llon = tess.r2*acos(
                SQ(sinlatt) + SQ(coslatt)*cos(d2r*(tess.e - tess.w)));
            Llat = tess.r2*acos(
                sin(d2r*tess.n)*sin(d2r*tess.s) +
                cos(d2r*tess.n)*cos(d2r*tess.s));
            Lr = tess.r2 - tess.r1;
            /* Number of times to split the tesseroid in each dimension */
            nlon = 1;
            nlat = 1;
            nr = 1;
            /* Check if the tesseroid is at a suitable distance (defined
             * the value of "ratio"). If not, mark that dimension for
             * division. */
            distance = pow(distance, power);
            if(distance < ratio*Llon)
            {
                nlon = 2;
            }
            if(distance < ratio*Llat)
            {
                nlat = 2;
            }
            if(distance < ratio*Lr)
            {
                nr = 2;
            }
            /* In case none of the dimensions need dividing,
             * put the GLQ roots in the proper scale and compute the
             * gravitational field of the tesseroid. */
            /* Also compute the effect if the tesseroid stack if full
             * (but warn the user that the computation might not be very
             * precise). */
            if((nlon == 1 && nlat == 1 && nr == 1)
               || (nlon*nlat*nr + stktop >= STKSIZE))
            {
                if(nlon*nlat*nr + stktop >= STKSIZE)
                {
                    log_error(
                        "Stack overflow: "
                        "tesseroid %d in the model file on "
                        "lon=%lf lat=%lf height=%lf."
                        "\n  Calculated without fully dividing the tesseroid. "
                        "Accuracy of the solution cannot be guaranteed."
                        "\n  This is probably caused by a computation point "
                        "too close to the tesseroid."
                        "\n  Try increasing the computation height."
                        "\n  *Expert users* can try modifying the "
                        "distance-size ratio."
                        "\n  *Beware* that this might affect "
                        "the accuracy of the solution.",
                        t + 1, lonp, latp, rp);
                }
                glq_set_limits(tess.w, tess.e, glq_lon);
                glq_set_limits(tess.s, tess.n, glq_lat);
                glq_set_limits(tess.r1, tess.r2, glq_r);
                glq_precompute_sincos(glq_lat);
                res += field(tess, lonp, latp, rp, *glq_lon, *glq_lat, *glq_r);
            }
            else
            {
                /* Divide the tesseroid in each dimension that needs dividing
                 * Put each of the smaller tesseroids on the stack for
                 * computing in the next iteration. */
                n = split_tess(tess, nlon, nlat, nr, &stack[stktop + 1]);
                stktop += n;
                /* Sanity check */
                if(n != nlon*nlat*nr)
                {
                    log_error("Splitting into %d instead of %d", n,
                                nlon*nlat*nr);
                }
            }
        }
    }
    #undef SQ
    return res;
}


/* Calculates potential caused by a tesseroid. */
double tess_pot(TESSEROID tess, double lonp, double latp, double rp, GLQ glq_lon,
               GLQ glq_lat, GLQ glq_r)
{
    double d2r = PI/180., l_sqr, coslatp, coslatc, sinlatp, sinlatc,
           coslon, rc, kappa, res,
           cospsi, wlon, wlat, wr, scale;
    register int i, j, k;

    coslatp = cos(d2r*latp);
    sinlatp = sin(d2r*latp);

    res = 0;
    for(k = 0; k < glq_lon.order; k++)
    {
        coslon = cos(d2r*(lonp - glq_lon.nodes[k]));
        wlon = glq_lon.weights[k];
        for(j = 0; j < glq_lat.order; j++)
        {
            sinlatc = glq_lat.nodes_sin[j];
            coslatc = glq_lat.nodes_cos[j];
            cospsi = sinlatp*sinlatc + coslatp*coslatc*coslon;
            wlat = glq_lat.weights[j];
            for(i = 0; i < glq_r.order; i++)
            {
                wr = glq_r.weights[i];
                rc = glq_r.nodes[i];
                l_sqr = rp*rp + rc*rc - 2*rp*rc*cospsi;
                kappa = rc*rc*coslatc;
                res += wlon*wlat*wr*kappa/sqrt(l_sqr);
            }
        }
    }
    scale = d2r*(tess.e - tess.w)*d2r*(tess.n - tess.s)*(tess.r2 - tess.r1)/8.;
    res *= G*tess.density*scale;
    return res;
}


/* Calculates gx caused by a tesseroid. */
double tess_gx(TESSEROID tess, double lonp, double latp, double rp, GLQ glq_lon,
               GLQ glq_lat, GLQ glq_r)
{
    double d2r = PI/180., l_sqr, kphi, coslatp, coslatc, sinlatp, sinlatc,
           coslon, rc, kappa, res,
           cospsi, wlon, wlat, wr, scale;
    register int i, j, k;

    coslatp = cos(d2r*latp);
    sinlatp = sin(d2r*latp);

    res = 0;
    for(k = 0; k < glq_lon.order; k++)
    {
        coslon = cos(d2r*(lonp - glq_lon.nodes[k]));
        wlon = glq_lon.weights[k];
        for(j = 0; j < glq_lat.order; j++)
        {
            sinlatc = glq_lat.nodes_sin[j];
            coslatc = glq_lat.nodes_cos[j];
            kphi = coslatp*sinlatc - sinlatp*coslatc*coslon;
            cospsi = sinlatp*sinlatc + coslatp*coslatc*coslon;
            wlat = glq_lat.weights[j];
            for(i = 0; i < glq_r.order; i++)
            {
                wr = glq_r.weights[i];
                rc = glq_r.nodes[i];
                l_sqr = rp*rp + rc*rc - 2*rp*rc*cospsi;
                kappa = rc*rc*coslatc;
                res += wlon*wlat*wr*kappa*(rc*kphi)/pow(l_sqr, 1.5);
            }
        }
    }
    scale = d2r*(tess.e - tess.w)*d2r*(tess.n - tess.s)*(tess.r2 - tess.r1)/8.;
    res *= SI2MGAL*G*tess.density*scale;
    return res;
}


/* Calculates gy caused by a tesseroid. */
double tess_gy(TESSEROID tess, double lonp, double latp, double rp, GLQ glq_lon,
               GLQ glq_lat, GLQ glq_r)
{
    double d2r = PI/180., l_sqr, coslatp, coslatc, sinlatp, sinlatc,
           coslon, sinlon, rc, kappa, res,
           cospsi, wlon, wlat, wr, scale;
    register int i, j, k;

    coslatp = cos(d2r*latp);
    sinlatp = sin(d2r*latp);

    res = 0;

    for(k = 0; k < glq_lon.order; k++)
    {
        coslon = cos(d2r*(lonp - glq_lon.nodes[k]));
        sinlon = sin(d2r*(glq_lon.nodes[k] - lonp));
        wlon = glq_lon.weights[k];
        for(j = 0; j < glq_lat.order; j++)
        {
            sinlatc = glq_lat.nodes_sin[j];
            coslatc = glq_lat.nodes_cos[j];
            cospsi = sinlatp*sinlatc + coslatp*coslatc*coslon;
            wlat = glq_lat.weights[j];
            for(i = 0; i < glq_r.order; i++)
            {
                wr = glq_r.weights[i];
                rc = glq_r.nodes[i];
                l_sqr = rp*rp + rc*rc - 2*rp*rc*cospsi;
                kappa = rc*rc*coslatc;
                res += wlon*wlat*wr*kappa*(rc*coslatc*sinlon)/pow(l_sqr, 1.5);
            }
        }
    }
    scale = d2r*(tess.e - tess.w)*d2r*(tess.n - tess.s)*(tess.r2 - tess.r1)/8.;
    res *= SI2MGAL*G*tess.density*scale;
    return res;
}


/* Calculates gz caused by a tesseroid. */
double tess_gz(TESSEROID tess, double lonp, double latp, double rp, GLQ glq_lon,
               GLQ glq_lat, GLQ glq_r)
{
    double d2r = PI/180., l_sqr, coslatp, coslatc, sinlatp, sinlatc,
           coslon, cospsi, rc, kappa, res,
           wlon, wlat, wr, scale;
    register int i, j, k;

    coslatp = cos(d2r*latp);
    sinlatp = sin(d2r*latp);

    res = 0;

    for(k = 0; k < glq_lon.order; k++)
    {
        coslon = cos(d2r*(lonp - glq_lon.nodes[k]));
        wlon = glq_lon.weights[k];
        for(j = 0; j < glq_lat.order; j++)
        {
            sinlatc = glq_lat.nodes_sin[j];
            coslatc = glq_lat.nodes_cos[j];
            cospsi = sinlatp*sinlatc + coslatp*coslatc*coslon;
            wlat = glq_lat.weights[j];
            for(i = 0; i < glq_r.order; i++)
            {
                wr = glq_r.weights[i];
                rc = glq_r.nodes[i];
                l_sqr = rp*rp + rc*rc - 2*rp*rc*cospsi;
                kappa = rc*rc*coslatc;
                res += wlon*wlat*wr*kappa*(rc*cospsi - rp)/pow(l_sqr, 1.5);
            }
        }
    }
    scale = d2r*(tess.e - tess.w)*d2r*(tess.n - tess.s)*(tess.r2 - tess.r1)/8.;
    res *= SI2MGAL*G*tess.density*scale;
    /* Used this to make z point down */
    return -1*res;
}


/* Calculates gxx caused by a tesseroid. */
double tess_gxx(TESSEROID tess, double lonp, double latp, double rp, GLQ glq_lon,
                GLQ glq_lat, GLQ glq_r)
{
    double d2r = PI/180., l_sqr, kphi, coslatp, coslatc, sinlatp, sinlatc,
           coslon, rc, kappa, res, l5,
           cospsi, wlon, wlat, wr, scale;
    register int i, j, k;

    coslatp = cos(d2r*latp);
    sinlatp = sin(d2r*latp);

    res = 0;

    for(k = 0; k < glq_lon.order; k++)
    {
        coslon = cos(d2r*(lonp - glq_lon.nodes[k]));
        wlon = glq_lon.weights[k];
        for(j = 0; j < glq_lat.order; j++)
        {
            sinlatc = glq_lat.nodes_sin[j];
            coslatc = glq_lat.nodes_cos[j];
            kphi = coslatp*sinlatc - sinlatp*coslatc*coslon;
            cospsi = sinlatp*sinlatc + coslatp*coslatc*coslon;
            wlat = glq_lat.weights[j];
            for(i = 0; i < glq_r.order; i++)
            {
                wr = glq_r.weights[i];
                rc = glq_r.nodes[i];
                l_sqr = rp*rp + rc*rc - 2*rp*rc*cospsi;
                l5 = pow(l_sqr, 2.5);
                kappa = rc*rc*coslatc;
                res += wlon*wlat*wr*kappa*(3*rc*kphi*rc*kphi - l_sqr)/l5;
            }
        }
    }
    scale = d2r*(tess.e - tess.w)*d2r*(tess.n - tess.s)*(tess.r2 - tess.r1)/8.;
    res *= SI2EOTVOS*G*tess.density*scale;
    return res;
}


/* Calculates gxy caused by a tesseroid. */
double tess_gxy(TESSEROID tess, double lonp, double latp, double rp, GLQ glq_lon,
                GLQ glq_lat, GLQ glq_r)
{
    double d2r = PI/180., l_sqr, kphi, coslatp, coslatc, sinlatp, sinlatc,
           coslon, sinlon, rc, kappa, deltax, deltay, res,
           cospsi, wlon, wlat, wr, scale;
    register int i, j, k;

    coslatp = cos(d2r*latp);
    sinlatp = sin(d2r*latp);

    res = 0;

    for(k = 0; k < glq_lon.order; k++)
    {
        coslon = cos(d2r*(lonp - glq_lon.nodes[k]));
        sinlon = sin(d2r*(glq_lon.nodes[k] - lonp));
        wlon = glq_lon.weights[k];
        for(j = 0; j < glq_lat.order; j++)
        {
            sinlatc = glq_lat.nodes_sin[j];
            coslatc = glq_lat.nodes_cos[j];
            kphi = coslatp*sinlatc - sinlatp*coslatc*coslon;
            cospsi = sinlatp*sinlatc + coslatp*coslatc*coslon;
            wlat = glq_lat.weights[j];
            for(i = 0; i < glq_r.order; i++)
            {
                wr = glq_r.weights[i];
                rc = glq_r.nodes[i];
                l_sqr = rp*rp + rc*rc - 2*rp*rc*cospsi;
                kappa = rc*rc*coslatc;
                deltax = rc*kphi;
                deltay = rc*coslatc*sinlon;
                res += wlon*wlat*wr*kappa*(3*deltax*deltay)/pow(l_sqr, 2.5);
            }
        }
    }
    scale = d2r*(tess.e - tess.w)*d2r*(tess.n - tess.s)*(tess.r2 - tess.r1)/8.;
    res *= SI2EOTVOS*G*tess.density*scale;
    return res;
}


/* Calculates gxz caused by a tesseroid. */
double tess_gxz(TESSEROID tess, double lonp, double latp, double rp, GLQ glq_lon,
                GLQ glq_lat, GLQ glq_r)
{
    double d2r = PI/180., l_sqr, kphi, coslatp, coslatc, sinlatp, sinlatc,
           coslon, cospsi, rc, kappa, deltax, deltaz, res,
           wlon, wlat, wr, scale;
    register int i, j, k;

    coslatp = cos(d2r*latp);
    sinlatp = sin(d2r*latp);

    res = 0;

    for(k = 0; k < glq_lon.order; k++)
    {
        coslon = cos(d2r*(lonp - glq_lon.nodes[k]));
        wlon = glq_lon.weights[k];
        for(j = 0; j < glq_lat.order; j++)
        {
            sinlatc = glq_lat.nodes_sin[j];
            coslatc = glq_lat.nodes_cos[j];
            cospsi = sinlatp*sinlatc + coslatp*coslatc*coslon;
            kphi = coslatp*sinlatc - sinlatp*coslatc*coslon;
            wlat = glq_lat.weights[j];
            for(i = 0; i < glq_r.order; i++)
            {
                wr = glq_r.weights[i];
                rc = glq_r.nodes[i];
                l_sqr = rp*rp + rc*rc - 2*rp*rc*cospsi;
                kappa = rc*rc*coslatc;
                deltax = rc*kphi;
                deltaz = rc*cospsi - rp;
                res += wlon*wlat*wr*kappa*(3*deltax*deltaz)/pow(l_sqr, 2.5);
            }
        }
    }
    scale = d2r*(tess.e - tess.w)*d2r*(tess.n - tess.s)*(tess.r2 - tess.r1)/8.;
    res *= SI2EOTVOS*G*tess.density*scale;
    return res;
}


/* Calculates gyy caused by a tesseroid. */
double tess_gyy(TESSEROID tess, double lonp, double latp, double rp, GLQ glq_lon,
                GLQ glq_lat, GLQ glq_r)
{
    double d2r = PI/180., l_sqr, coslatp, coslatc, sinlatp, sinlatc,
           coslon, sinlon, rc, kappa, deltay, res, l5,
           cospsi, wlon, wlat, wr, scale;
    register int i, j, k;

    coslatp = cos(d2r*latp);
    sinlatp = sin(d2r*latp);

    res = 0;

    for(k = 0; k < glq_lon.order; k++)
    {
        coslon = cos(d2r*(lonp - glq_lon.nodes[k]));
        sinlon = sin(d2r*(glq_lon.nodes[k] - lonp));
        wlon = glq_lon.weights[k];
        for(j = 0; j < glq_lat.order; j++)
        {
            sinlatc = glq_lat.nodes_sin[j];
            coslatc = glq_lat.nodes_cos[j];
            cospsi = sinlatp*sinlatc + coslatp*coslatc*coslon;
            wlat = glq_lat.weights[j];
            for(i = 0; i < glq_r.order; i++)
            {
                wr = glq_r.weights[i];
                rc = glq_r.nodes[i];
                l_sqr = rp*rp + rc*rc - 2*rp*rc*cospsi;
                l5 = pow(l_sqr, 2.5);
                kappa = rc*rc*coslatc;
                deltay = rc*coslatc*sinlon;
                res += wlon*wlat*wr*kappa*(3*deltay*deltay - l_sqr)/l5;
            }
        }
    }
    scale = d2r*(tess.e - tess.w)*d2r*(tess.n - tess.s)*(tess.r2 - tess.r1)/8.;
    res *= SI2EOTVOS*G*tess.density*scale;
    return res;
}


/* Calculates gyz caused by a tesseroid. */
double tess_gyz(TESSEROID tess, double lonp, double latp, double rp, GLQ glq_lon,
                GLQ glq_lat, GLQ glq_r)
{
    double d2r = PI/180., l_sqr, coslatp, coslatc, sinlatp, sinlatc,
           coslon, sinlon, cospsi, rc, kappa, deltay, deltaz, res,
           wlon, wlat, wr, scale;
    register int i, j, k;

    coslatp = cos(d2r*latp);
    sinlatp = sin(d2r*latp);

    res = 0;

    for(k = 0; k < glq_lon.order; k++)
    {
        coslon = cos(d2r*(lonp - glq_lon.nodes[k]));
        sinlon = sin(d2r*(glq_lon.nodes[k] - lonp));
        wlon = glq_lon.weights[k];
        for(j = 0; j < glq_lat.order; j++)
        {
            sinlatc = glq_lat.nodes_sin[j];
            coslatc = glq_lat.nodes_cos[j];
            cospsi = sinlatp*sinlatc + coslatp*coslatc*coslon;
            wlat = glq_lat.weights[j];
            for(i = 0; i < glq_r.order; i++)
            {
                wr = glq_r.weights[i];
                rc = glq_r.nodes[i];
                l_sqr = rp*rp + rc*rc - 2*rp*rc*cospsi;
                kappa = rc*rc*coslatc;
                deltay = rc*coslatc*sinlon;
                deltaz = rc*cospsi - rp;
                res += wlon*wlat*wr*kappa*(3*deltay*deltaz)/pow(l_sqr, 2.5);
            }
        }
    }
    scale = d2r*(tess.e - tess.w)*d2r*(tess.n - tess.s)*(tess.r2 - tess.r1)/8.;
    res *= SI2EOTVOS*G*tess.density*scale;
    return res;
}


/* Calculates gzz caused by a tesseroid. */
double tess_gzz(TESSEROID tess, double lonp, double latp, double rp, GLQ glq_lon,
                GLQ glq_lat, GLQ glq_r)
{
    double d2r = PI/180., l_sqr, coslatp, coslatc, sinlatp, sinlatc,
           coslon, cospsi, rc, kappa, deltaz, res,
           wlon, wlat, wr, scale, l5;
    register int i, j, k;

    coslatp = cos(d2r*latp);
    sinlatp = sin(d2r*latp);

    res = 0;

    for(k = 0; k < glq_lon.order; k++)
    {
        coslon = cos(d2r*(lonp - glq_lon.nodes[k]));
        wlon = glq_lon.weights[k];
        for(j = 0; j < glq_lat.order; j++)
        {
            sinlatc = glq_lat.nodes_sin[j];
            coslatc = glq_lat.nodes_cos[j];
            cospsi = sinlatp*sinlatc + coslatp*coslatc*coslon;
            wlat = glq_lat.weights[j];
            for(i = 0; i < glq_r.order; i++)
            {
                wr = glq_r.weights[i];
                rc = glq_r.nodes[i];
                l_sqr = rp*rp + rc*rc - 2*rp*rc*cospsi;
                l5 = pow(l_sqr, 2.5);
                kappa = rc*rc*coslatc;
                deltaz = rc*cospsi - rp;
                res += wlon*wlat*wr*kappa*(3*deltaz*deltaz - l_sqr)/l5;
            }
        }
    }
    scale = d2r*(tess.e - tess.w)*d2r*(tess.n - tess.s)*(tess.r2 - tess.r1)/8.;
    res *= SI2EOTVOS*G*tess.density*scale;
    return res;
}
