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

#define SQ(x) (x)*(x)


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
        if(lonp >= model[tess].w && lonp <= model[tess].e &&
           latp >= model[tess].s && latp <= model[tess].n &&
           rp >= model[tess].r1 && rp <= model[tess].r2)
        {
            log_warning("Point (%g %g %g) is on tesseroid %d: %g %g %g %g %g %g %g. Can't guarantee accuracy.",
                        lonp, latp, rp - MEAN_EARTH_RADIUS, tess,
                        model[tess].w, model[tess].e, model[tess].s,
                        model[tess].n, model[tess].r2 - MEAN_EARTH_RADIUS,
                        model[tess].r1 - MEAN_EARTH_RADIUS,
                        model[tess].density);
        }
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
          double ratio)
{
    double res, distance, lont, latt, rt, d2r = PI/180.,
           coslatp, sinlatp, rp_sqr, rlonp,
           dlon, dlat, dr,
           sinlatt, coslatt,
           sinn, sins, cosn, coss, sindlon, cosdlon;
    int t, n, nlon, nlat, nr;
    TESSEROID split[8];

    /* Pre-compute these things out of the loop */
    rlonp = d2r*lonp;
    rp_sqr = SQ(rp);
    coslatp = cos(d2r*latp);
    sinlatp = sin(d2r*latp);

    res = 0;
    for(t = 0; t < size; t++)
    {
        /* Compute the distance squared (to avoid calling sqrt all the time) */
        rt = model[t].r2;
        lont = d2r*0.5*(model[t].w + model[t].e);
        latt = d2r*0.5*(model[t].s + model[t].n);
        sinlatt = sin(latt);
        coslatt = cos(latt);
        distance = sqrt(rp_sqr + SQ(rt) - 2*rp*rt*(
            sinlatp*sinlatt + coslatp*coslatt*cos(rlonp - lont)));

        /* Would get stuck in infinite loop if dist = 0 and get wrong results if
           inside de tesseroid. Still do the calculation but warn user that it's
           probably wrong. */
        if(lonp >= model[t].w && lonp <= model[t].e &&
           latp >= model[t].s && latp <= model[t].n &&
           rp >= model[t].r1 && rp <= model[t].r2)
        {
            log_warning("Point (%g %g %g) is on top of tesseroid %d: %g %g %g %g %g %g %g. Can't guarantee accuracy.",
                        lonp, latp, rp - MEAN_EARTH_RADIUS, t,
                        model[t].w, model[t].e, model[t].s,
                        model[t].n, model[t].r2 - MEAN_EARTH_RADIUS,
                        model[t].r1 - MEAN_EARTH_RADIUS,
                        model[t].density);
            glq_set_limits(model[t].w, model[t].e, glq_lon);
            glq_set_limits(model[t].s, model[t].n, glq_lat);
            glq_set_limits(model[t].r1, model[t].r2, glq_r);
            glq_precompute_sincos(glq_lat);
            res += field(model[t], lonp, latp, rp, *glq_lon, *glq_lat,
                         *glq_r);
        }
        /* Check if the computation point is at an acceptable distance. If not
           split the tesseroid using the given ratio */
        else
        {
            /* Number of times to split the tesseroid in each dimension */
            nlon = 1;
            nlat = 1;
            nr = 1;
            /* Get the size of each face of the tesseroid */
            /* Will use Vincenty's formula to calculate great-circle distance
             * for more accuracy (just in case) */
            sinn = sin(d2r*model[t].n);
            cosn = cos(d2r*model[t].n);
            sins = sin(d2r*model[t].s);
            coss = cos(d2r*model[t].s);
            sindlon = sin(d2r*(model[t].e - model[t].w));
            cosdlon = cos(d2r*(model[t].e - model[t].w));
            dlon = MEAN_EARTH_RADIUS*atan2(
                sqrt(SQ(coslatt*sindlon)
                     + SQ(coslatt*sinlatt - sinlatt*coslatt*cosdlon)),
                sinlatt*sinlatt + coslatt*coslatt*cosdlon);
            dlat = MEAN_EARTH_RADIUS*atan2(
                coss*sinn - sins*cosn, sins*sinn + coss*cosn);
            dr = model[t].r2 - model[t].r1;
            if(distance < ratio*dlon)
            {
                nlon = 2;
            }
            if(distance < ratio*dlat)
            {
                nlat = 2;
            }
            if(distance < ratio*dr)
            {
                nr = 2;
            }
            if(nlon == 1 && nlat == 1 && nr == 1)
            {
                glq_set_limits(model[t].w, model[t].e, glq_lon);
                glq_set_limits(model[t].s, model[t].n, glq_lat);
                glq_set_limits(model[t].r1, model[t].r2, glq_r);
                glq_precompute_sincos(glq_lat);
                res += field(model[t], lonp, latp, rp, *glq_lon, *glq_lat,
                             *glq_r);
            }
            else
            {
                log_debug(
                    "Splitting tesseroid %d (%g %g %g %g %g %g %g) into (%d %d %d) at point (%g %g %g) using ratio %g",
                    t, model[t].w, model[t].e, model[t].s,
                    model[t].n, model[t].r2 - MEAN_EARTH_RADIUS,
                    model[t].r1 - MEAN_EARTH_RADIUS, model[t].density,
                    nlon, nlat, nr,
                    lonp, latp, rp - MEAN_EARTH_RADIUS, ratio);
                n = split_tess(model[t], nlon, nlat, nr, split);
                if(n != nlon*nlat*nr)
                {
                    log_warning("Splitting into %d instead of %d", n, nlon*nlat*nr);
                }
                res += calc_tess_model_adapt(split, n, lonp, latp, rp, glq_lon,
                                             glq_lat, glq_r, field, ratio);
            }
        }
    }
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
