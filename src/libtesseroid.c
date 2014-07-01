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


#include <stdlib.h>
#include <math.h>
#include "logger.h"
#include "geometry.h"
#include "constants.h"
#include "libtesseroid.h"

GLQ * glq_new(int order, double lower, double upper)
{
    GLQ *glq;
    int rc;

    glq = (GLQ *)malloc(sizeof(GLQ));
    if(glq == NULL)
    {
        return NULL;
    }
    glq->order = order;
    glq->nodes = (double *)malloc(sizeof(double)*order);
    if(glq->nodes == NULL)
    {
        free(glq);
        return NULL;
    }
    glq->nodes_unscaled = (double *)malloc(sizeof(double)*order);
    if(glq->nodes_unscaled == NULL)
    {
        free(glq);
        free(glq->nodes);
        return NULL;
    }
    glq->weights = (double *)malloc(sizeof(double)*order);
    if(glq->weights == NULL)
    {
        free(glq);
        free(glq->nodes);
        free(glq->nodes_unscaled);
        return NULL;
    }
    rc = glq_nodes(order, glq->nodes_unscaled);
    if(rc != 0 && rc != 3)
    {
        switch(rc)
        {
            case 1:
                log_error("glq_nodes invalid GLQ order %d. Should be >= 2.",
                          order);
                break;
            case 2:
                log_error("glq_nodes NULL pointer for nodes");
                break;
            default:
                log_error("glq_nodes unknown error code %g", rc);
                break;
        }
        glq_free(glq);
        return NULL;
    }
    else if(rc == 3)
    {
        log_warning("glq_nodes max iterations reached in root finder");
        log_warning("nodes might not have desired accuracy %g", GLQ_MAXERROR);
    }
    rc = glq_weights(order, glq->nodes_unscaled, glq->weights);
    if(rc != 0)
    {
        switch(rc)
        {
            case 1:
                log_error("glq_weights invalid GLQ order %d. Should be >= 2.",
                          order);
                break;
            case 2:
                log_error("glq_weights NULL pointer for nodes");
                break;
            case 3:
                log_error("glq_weights NULL pointer for weights");
                break;
            default:
                log_error("glq_weights unknown error code %d\n", rc);
                break;
        }
        glq_free(glq);
        return NULL;
    }
    if(glq_set_limits(lower, upper, glq) != 0)
    {
        glq_free(glq);
        return NULL;
    }
    return glq;
}

void glq_free(GLQ *glq)
{
    free(glq->nodes);
    free(glq->nodes_unscaled);
    free(glq->weights);
    free(glq);
}

int glq_nodes(int order, double *nodes)
{
    register int i;
    int rc = 0;
    double initial;

    if(order < 2)
    {
        return 1;
    }
    if(nodes == NULL)
    {
        return 2;
    }
    for(i = 0; i < order; i++)
    {
        initial = cos(PI*(order - i - 0.25)/(order + 0.5));
        if(glq_next_root(initial, i, order, nodes) == 3)
        {
            rc = 3;
        }
    }
    return rc;
}

int glq_set_limits(double lower, double upper, GLQ *glq)
{
    /* Only calculate once to optimize the code */
    double tmpplus = 0.5*(upper + lower), tmpminus = 0.5*(upper - lower);
    register int i;

    if(glq->order < 2)
    {
        return 1;
    }
    if(glq->nodes == NULL)
    {
        return 2;
    }
    if(glq->nodes_unscaled == NULL)
    {
        return 2;
    }
    for(i = 0; i < glq->order; i++)
    {
        glq->nodes[i] = tmpminus*glq->nodes_unscaled[i] + tmpplus;
    }
    return 0;
}

int glq_next_root(double initial, int root_index, int order, double *roots)
{
    double x1, x0, pn, pn_2, pn_1, pn_line, sum;
    int it = 0;
    register int n;

    if(order < 2)
    {
        return 1;
    }
    if(root_index < 0 || root_index >= order)
    {
        return 2;
    }
    x1 = initial;
    do
    {
        x0 = x1;

        /* Calculate Pn(x0) */
        /* Starting from P0(x) and P1(x), */
        /* find the others using the recursive relation: */
        /*     Pn(x)=(2n-1)xPn_1(x)/n - (n-1)Pn_2(x)/n   */
        pn_1 = 1.;   /* This is Po(x) */
        pn = x0;    /* and this P1(x) */
        for(n = 2; n <= order; n++)
        {
            pn_2 = pn_1;
            pn_1 = pn;
            pn = ( ((2*n - 1)*x0*pn_1) - ((n - 1)*pn_2) )/n;
        }
        /* Now calculate Pn'(x0) using another recursive relation: */
        /*     Pn'(x)=n(xPn(x)-Pn_1(x))/(x*x-1)                    */
        pn_line = order*(x0*pn - pn_1)/(x0*x0 - 1);
        /* Sum the roots found so far */
        for(n = 0, sum = 0; n < root_index; n++)
        {
            sum += 1./(x0 - roots[n]);
        }
        /* Update the estimate for the root */
        x1 = x0 - (double)pn/(pn_line - pn*sum);

    /** Compute the absolute value of x */
    #define GLQ_ABS(x) ((x) < 0 ? -1*(x) : (x))
    } while(GLQ_ABS(x1 - x0) > GLQ_MAXERROR && ++it <= GLQ_MAXIT);
    #undef GLQ_ABS

    roots[root_index] = x1;

    /* Tell the user if stagnation occurred */
    if(it > GLQ_MAXIT)
    {
        return 3;
    }
    return 0;
}

int glq_weights(int order, double *nodes, double *weights)
{
    register int i, n;
    double xi, pn, pn_2, pn_1, pn_line;

    if(order < 2)
    {
        return 1;
    }
    if(nodes == NULL)
    {
        return 2;
    }
    if(weights == NULL)
    {
        return 3;
    }
    for(i = 0; i < order; i++){

        xi = nodes[i];

        /* Find Pn'(xi) with the recursive relation to find Pn and Pn-1: */
        /*   Pn(x)=(2n-1)xPn_1(x)/n - (n-1)Pn_2(x)/n   */
        /* Then use:   Pn'(x)=n(xPn(x)-Pn_1(x))/(x*x-1) */

        /* Find Pn and Pn-1 stating from P0 and P1 */
        pn_1 = 1;   /* This is Po(x) */
        pn = xi;    /* and this P1(x) */
        for(n = 2; n <= order; n++)
        {
            pn_2 = pn_1;
            pn_1 = pn;
            pn = ((2*n - 1)*xi*pn_1 - (n - 1)*pn_2)/n;
        }
        pn_line = order*(xi*pn - pn_1)/(xi*xi - 1.);
        /* ith weight is: wi = 2/(1 - xi^2)(Pn'(xi)^2) */
        weights[i] = 2./((1 - xi*xi)*pn_line*pn_line);
    }
    return 0;
}

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
        res += field(model[tess], lonp, latp, rp, *glq_lon, *glq_lat, *glq_r);
    }
    return res;
}

double calc_tess_model_adapt(TESSEROID *model, int size, double lonp,
          double latp, double rp, GLQ *glq_lon, GLQ *glq_lat, GLQ *glq_r,
          double (*field)(TESSEROID, double, double, double, GLQ, GLQ, GLQ),
          double ratio)
{
    double res, dist, lont, latt, rt, d2r = PI/180.;
    int tess;
    TESSEROID split[8];

    res = 0;
    for(tess = 0; tess < size; tess++)
    {
        rt = model[tess].r2;
        lont = 0.5*(model[tess].w + model[tess].e);
        latt = 0.5*(model[tess].s + model[tess].n);
        dist = sqrt(rp*rp + rt*rt - 2*rp*rt*(sin(d2r*latp)*sin(d2r*latt) +
                    cos(d2r*latp)*cos(d2r*latt)*cos(d2r*(lonp - lont))));

        /* Would get stuck in infinite loop if dist = 0 and get wrong results if
           inside de tesseroid. Still do the calculation but warn user that it's
           probably wrong. */
        if(lonp >= model[tess].w && lonp <= model[tess].e &&
           latp >= model[tess].s && latp <= model[tess].n &&
           rp >= model[tess].r1 && rp <= model[tess].r2)
        {
            log_warning("Point (%g %g %g) is on top of tesseroid %d: %g %g %g %g %g %g %g. Can't guarantee accuracy.",
                        lonp, latp, rp - MEAN_EARTH_RADIUS, tess,
                        model[tess].w, model[tess].e, model[tess].s,
                        model[tess].n, model[tess].r2 - MEAN_EARTH_RADIUS,
                        model[tess].r1 - MEAN_EARTH_RADIUS,
                        model[tess].density);
            glq_set_limits(model[tess].w, model[tess].e, glq_lon);
            glq_set_limits(model[tess].s, model[tess].n, glq_lat);
            glq_set_limits(model[tess].r1, model[tess].r2, glq_r);
            res += field(model[tess], lonp, latp, rp, *glq_lon, *glq_lat,
                         *glq_r);
        }
        /* Check if the computation point is at an acceptable distance. If not
           split the tesseroid using the given ratio */
        else if(
            dist < ratio*MEAN_EARTH_RADIUS*d2r*(model[tess].e - model[tess].w) ||
            dist < ratio*MEAN_EARTH_RADIUS*d2r*(model[tess].n - model[tess].s) ||
            dist < ratio*(model[tess].r2 - model[tess].r1))
        {
            log_debug("Splitting tesseroid %d (%g %g %g %g %g %g %g) at point (%g %g %g) using ratio %g",
                      tess, model[tess].w, model[tess].e, model[tess].s,
                      model[tess].n, model[tess].r2 - MEAN_EARTH_RADIUS,
                      model[tess].r1 - MEAN_EARTH_RADIUS, model[tess].density,
                      lonp, latp, rp - MEAN_EARTH_RADIUS, ratio);
            /* Do it recursively until ratio*size is smaller than distance */
            split_tess(model[tess], split);
            res += calc_tess_model_adapt(split, 8, lonp, latp, rp, glq_lon,
                                         glq_lat, glq_r, field, ratio);
        }
        else
        {
            glq_set_limits(model[tess].w, model[tess].e, glq_lon);
            glq_set_limits(model[tess].s, model[tess].n, glq_lat);
            glq_set_limits(model[tess].r1, model[tess].r2, glq_r);
            res += field(model[tess], lonp, latp, rp, *glq_lon, *glq_lat,
                         *glq_r);
        }
    }
    return res;
}

double tess_pot(TESSEROID tess, double lonp, double latp, double rp, GLQ glq_lon,
               GLQ glq_lat, GLQ glq_r)
{
    double d2r = PI/180., l_sqr, coslatp, coslatc, sinlatp, sinlatc,
           coslon, rc, kappa, res;
    register int i, j, k;
    coslatp = cos(d2r*latp);
    sinlatp = sin(d2r*latp);
    res = 0;
    for(k = 0; k < glq_lon.order; k++)
    {
        for(j = 0; j < glq_lat.order; j++)
        {
            for(i = 0; i < glq_r.order; i++)
            {
                rc = glq_r.nodes[i];
                sinlatc = sin(d2r*glq_lat.nodes[j]);
                coslatc = cos(d2r*glq_lat.nodes[j]);
                coslon = cos(d2r*(lonp - glq_lon.nodes[k]));
                l_sqr = rp*rp + rc*rc - 2*rp*rc*(sinlatp*sinlatc +
                                                 coslatp*coslatc*coslon);
                kappa = rc*rc*coslatc;
                res += glq_lon.weights[k]*glq_lat.weights[j]*glq_r.weights[i]*
                       kappa/sqrt(l_sqr);
            }
        }
    }
    res *= G*tess.density*d2r*(tess.e - tess.w)*d2r*(tess.n - tess.s)*
           (tess.r2 - tess.r1)*0.125;
    return res;
}

double tess_gx(TESSEROID tess, double lonp, double latp, double rp, GLQ glq_lon,
               GLQ glq_lat, GLQ glq_r)
{
    double d2r = PI/180., l_sqr, kphi, coslatp, coslatc, sinlatp, sinlatc,
           coslon, rc, kappa, res;
    register int i, j, k;
    coslatp = cos(d2r*latp);
    sinlatp = sin(d2r*latp);
    res = 0;
    for(k = 0; k < glq_lon.order; k++)
    {
        for(j = 0; j < glq_lat.order; j++)
        {
            for(i = 0; i < glq_r.order; i++)
            {
                rc = glq_r.nodes[i];
                sinlatc = sin(d2r*glq_lat.nodes[j]);
                coslatc = cos(d2r*glq_lat.nodes[j]);
                coslon = cos(d2r*(lonp - glq_lon.nodes[k]));
                l_sqr = rp*rp + rc*rc - 2*rp*rc*(sinlatp*sinlatc +
                                                 coslatp*coslatc*coslon);
                kphi = coslatp*sinlatc - sinlatp*coslatc*coslon;
                kappa = rc*rc*coslatc;
                res += glq_lon.weights[k]*glq_lat.weights[j]*glq_r.weights[i]*
                       kappa*(rc*kphi)/pow(l_sqr, 1.5);
            }
        }
    }
    res *= SI2MGAL*G*tess.density*d2r*(tess.e - tess.w)*d2r*(tess.n - tess.s)*
           (tess.r2 - tess.r1)*0.125;
    return res;
}

double tess_gy(TESSEROID tess, double lonp, double latp, double rp, GLQ glq_lon,
               GLQ glq_lat, GLQ glq_r)
{
    double d2r = PI/180., l_sqr, coslatp, coslatc, sinlatp, sinlatc,
           coslon, sinlon, rc, kappa, res;
    register int i, j, k;
    coslatp = cos(d2r*latp);
    sinlatp = sin(d2r*latp);
    res = 0;
    for(k = 0; k < glq_lon.order; k++)
    {
        for(j = 0; j < glq_lat.order; j++)
        {
            for(i = 0; i < glq_r.order; i++)
            {
                rc = glq_r.nodes[i];
                sinlatc = sin(d2r*glq_lat.nodes[j]);
                coslatc = cos(d2r*glq_lat.nodes[j]);
                coslon = cos(d2r*(lonp - glq_lon.nodes[k]));
                sinlon = sin(d2r*(glq_lon.nodes[k] - lonp));
                l_sqr = rp*rp + rc*rc - 2*rp*rc*(sinlatp*sinlatc +
                                                 coslatp*coslatc*coslon);
                kappa = rc*rc*coslatc;
                res += glq_lon.weights[k]*glq_lat.weights[j]*glq_r.weights[i]*
                       kappa*(rc*coslatc*sinlon)/pow(l_sqr, 1.5);
            }
        }
    }
    res *= SI2MGAL*G*tess.density*d2r*(tess.e - tess.w)*d2r*(tess.n - tess.s)*
           (tess.r2 - tess.r1)*0.125;
    return res;
}

double tess_gz(TESSEROID tess, double lonp, double latp, double rp, GLQ glq_lon,
               GLQ glq_lat, GLQ glq_r)
{
    double d2r = PI/180., l_sqr, coslatp, coslatc, sinlatp, sinlatc,
           coslon, cospsi, rc, kappa, res;
    register int i, j, k;
    coslatp = cos(d2r*latp);
    sinlatp = sin(d2r*latp);
    res = 0;
    for(k = 0; k < glq_lon.order; k++)
    {
        for(j = 0; j < glq_lat.order; j++)
        {
            for(i = 0; i < glq_r.order; i++)
            {
                rc = glq_r.nodes[i];
                sinlatc = sin(d2r*glq_lat.nodes[j]);
                coslatc = cos(d2r*glq_lat.nodes[j]);
                coslon = cos(d2r*(lonp - glq_lon.nodes[k]));
                cospsi = sinlatp*sinlatc + coslatp*coslatc*coslon;
                l_sqr = rp*rp + rc*rc - 2*rp*rc*cospsi;
                kappa = rc*rc*coslatc;
                res += glq_lon.weights[k]*glq_lat.weights[j]*glq_r.weights[i]*
                       kappa*(rc*cospsi - rp)/pow(l_sqr, 1.5);
            }
        }
    }
    res *= SI2MGAL*G*tess.density*d2r*(tess.e - tess.w)*d2r*(tess.n - tess.s)*
           (tess.r2 - tess.r1)*0.125;
    /* Used this to make z point down */
    return -1*res;
}

double tess_gxx(TESSEROID tess, double lonp, double latp, double rp, GLQ glq_lon,
                GLQ glq_lat, GLQ glq_r)
{
    double d2r = PI/180., l_sqr, kphi, coslatp, coslatc, sinlatp, sinlatc,
           coslon, rc, kappa, res;
    register int i, j, k;
    coslatp = cos(d2r*latp);
    sinlatp = sin(d2r*latp);
    res = 0;
    for(k = 0; k < glq_lon.order; k++)
    {
        for(j = 0; j < glq_lat.order; j++)
        {
            for(i = 0; i < glq_r.order; i++)
            {
                rc = glq_r.nodes[i];
                sinlatc = sin(d2r*glq_lat.nodes[j]);
                coslatc = cos(d2r*glq_lat.nodes[j]);
                coslon = cos(d2r*(lonp - glq_lon.nodes[k]));
                l_sqr = rp*rp + rc*rc - 2*rp*rc*(sinlatp*sinlatc +
                                                 coslatp*coslatc*coslon);
                kphi = coslatp*sinlatc - sinlatp*coslatc*coslon;
                kappa = rc*rc*coslatc;
                res += glq_lon.weights[k]*glq_lat.weights[j]*glq_r.weights[i]*
                       kappa*(3*rc*kphi*rc*kphi - l_sqr)/pow(l_sqr, 2.5);
            }
        }
    }
    res *= SI2EOTVOS*G*tess.density*d2r*(tess.e - tess.w)*d2r*(tess.n - tess.s)*
           (tess.r2 - tess.r1)*0.125;
    return res;
}

double tess_gxy(TESSEROID tess, double lonp, double latp, double rp, GLQ glq_lon,
                GLQ glq_lat, GLQ glq_r)
{
    double d2r = PI/180., l_sqr, kphi, coslatp, coslatc, sinlatp, sinlatc,
           coslon, sinlon, rc, kappa, deltax, deltay, res;
    register int i, j, k;
    coslatp = cos(d2r*latp);
    sinlatp = sin(d2r*latp);
    res = 0;
    for(k = 0; k < glq_lon.order; k++)
    {
        for(j = 0; j < glq_lat.order; j++)
        {
            for(i = 0; i < glq_r.order; i++)
            {
                rc = glq_r.nodes[i];
                sinlatc = sin(d2r*glq_lat.nodes[j]);
                coslatc = cos(d2r*glq_lat.nodes[j]);
                coslon = cos(d2r*(lonp - glq_lon.nodes[k]));
                sinlon = sin(d2r*(glq_lon.nodes[k] - lonp));
                l_sqr = rp*rp + rc*rc - 2*rp*rc*(sinlatp*sinlatc +
                                                 coslatp*coslatc*coslon);
                kphi = coslatp*sinlatc - sinlatp*coslatc*coslon;
                kappa = rc*rc*coslatc;
                deltax = rc*kphi;
                deltay = rc*coslatc*sinlon;
                res += glq_lon.weights[k]*glq_lat.weights[j]*glq_r.weights[i]*
                       kappa*(3*deltax*deltay)/pow(l_sqr, 2.5);
            }
        }
    }
    res *= SI2EOTVOS*G*tess.density*d2r*(tess.e - tess.w)*d2r*(tess.n - tess.s)*
           (tess.r2 - tess.r1)*0.125;

    return res;
}

double tess_gxz(TESSEROID tess, double lonp, double latp, double rp, GLQ glq_lon,
                GLQ glq_lat, GLQ glq_r)
{
    double d2r = PI/180., l_sqr, kphi, coslatp, coslatc, sinlatp, sinlatc,
           coslon, cospsi, rc, kappa, deltax, deltaz, res;
    register int i, j, k;
    coslatp = cos(d2r*latp);
    sinlatp = sin(d2r*latp);
    res = 0;
    for(k = 0; k < glq_lon.order; k++)
    {
        for(j = 0; j < glq_lat.order; j++)
        {
            for(i = 0; i < glq_r.order; i++)
            {
                rc = glq_r.nodes[i];
                sinlatc = sin(d2r*glq_lat.nodes[j]);
                coslatc = cos(d2r*glq_lat.nodes[j]);
                coslon = cos(d2r*(lonp - glq_lon.nodes[k]));
                cospsi = sinlatp*sinlatc + coslatp*coslatc*coslon;
                l_sqr = rp*rp + rc*rc - 2*rp*rc*cospsi;
                kphi = coslatp*sinlatc - sinlatp*coslatc*coslon;
                kappa = rc*rc*coslatc;
                deltax = rc*kphi;
                deltaz = rc*cospsi - rp;
                res += glq_lon.weights[k]*glq_lat.weights[j]*glq_r.weights[i]*
                       kappa*(3*deltax*deltaz)/pow(l_sqr, 2.5);
            }
        }
    }
    res *= SI2EOTVOS*G*tess.density*d2r*(tess.e - tess.w)*d2r*(tess.n - tess.s)*
           (tess.r2 - tess.r1)*0.125;
    return res;
}

double tess_gyy(TESSEROID tess, double lonp, double latp, double rp, GLQ glq_lon,
                GLQ glq_lat, GLQ glq_r)
{
    double d2r = PI/180., l_sqr, coslatp, coslatc, sinlatp, sinlatc,
           coslon, sinlon, rc, kappa, deltay, res;
    register int i, j, k;
    coslatp = cos(d2r*latp);
    sinlatp = sin(d2r*latp);
    res = 0;
    for(k = 0; k < glq_lon.order; k++)
    {
        for(j = 0; j < glq_lat.order; j++)
        {
            for(i = 0; i < glq_r.order; i++)
            {
                rc = glq_r.nodes[i];
                sinlatc = sin(d2r*glq_lat.nodes[j]);
                coslatc = cos(d2r*glq_lat.nodes[j]);
                coslon = cos(d2r*(lonp - glq_lon.nodes[k]));
                sinlon = sin(d2r*(glq_lon.nodes[k] - lonp));
                l_sqr = rp*rp + rc*rc - 2*rp*rc*(sinlatp*sinlatc +
                                                 coslatp*coslatc*coslon);
                kappa = rc*rc*coslatc;
                deltay = rc*coslatc*sinlon;
                res += glq_lon.weights[k]*glq_lat.weights[j]*glq_r.weights[i]*
                       kappa*(3*deltay*deltay - l_sqr)/pow(l_sqr, 2.5);
            }
        }
    }
    res *= SI2EOTVOS*G*tess.density*d2r*(tess.e - tess.w)*d2r*(tess.n - tess.s)*
           (tess.r2 - tess.r1)*0.125;
    return res;
}

double tess_gyz(TESSEROID tess, double lonp, double latp, double rp, GLQ glq_lon,
                GLQ glq_lat, GLQ glq_r)
{
    double d2r = PI/180., l_sqr, coslatp, coslatc, sinlatp, sinlatc,
           coslon, sinlon, cospsi, rc, kappa, deltay, deltaz, res;
    register int i, j, k;
    coslatp = cos(d2r*latp);
    sinlatp = sin(d2r*latp);
    res = 0;
    for(k = 0; k < glq_lon.order; k++)
    {
        for(j = 0; j < glq_lat.order; j++)
        {
            for(i = 0; i < glq_r.order; i++)
            {
                rc = glq_r.nodes[i];
                sinlatc = sin(d2r*glq_lat.nodes[j]);
                coslatc = cos(d2r*glq_lat.nodes[j]);
                coslon = cos(d2r*(lonp - glq_lon.nodes[k]));
                sinlon = sin(d2r*(glq_lon.nodes[k] - lonp));
                cospsi = sinlatp*sinlatc + coslatp*coslatc*coslon;
                l_sqr = rp*rp + rc*rc - 2*rp*rc*cospsi;
                kappa = rc*rc*coslatc;
                deltay = rc*coslatc*sinlon;
                deltaz = rc*cospsi - rp;
                res += glq_lon.weights[k]*glq_lat.weights[j]*glq_r.weights[i]*
                       kappa*(3*deltay*deltaz)/pow(l_sqr, 2.5);
            }
        }
    }
    res *= SI2EOTVOS*G*tess.density*d2r*(tess.e - tess.w)*d2r*(tess.n - tess.s)*
           (tess.r2 - tess.r1)*0.125;
    return res;
}

double tess_gzz(TESSEROID tess, double lonp, double latp, double rp, GLQ glq_lon,
                GLQ glq_lat, GLQ glq_r)
{
    double d2r = PI/180., l_sqr, coslatp, coslatc, sinlatp, sinlatc,
           coslon, cospsi, rc, kappa, deltaz, res;
    register int i, j, k;
    coslatp = cos(d2r*latp);
    sinlatp = sin(d2r*latp);
    res = 0;
    for(k = 0; k < glq_lon.order; k++)
    {
        for(j = 0; j < glq_lat.order; j++)
        {
            for(i = 0; i < glq_r.order; i++)
            {
                rc = glq_r.nodes[i];
                sinlatc = sin(d2r*glq_lat.nodes[j]);
                coslatc = cos(d2r*glq_lat.nodes[j]);
                coslon = cos(d2r*(lonp - glq_lon.nodes[k]));
                cospsi = sinlatp*sinlatc + coslatp*coslatc*coslon;
                l_sqr = rp*rp + rc*rc - 2*rp*rc*cospsi;
                kappa = rc*rc*coslatc;
                deltaz = rc*cospsi - rp;
                res += glq_lon.weights[k]*glq_lat.weights[j]*glq_r.weights[i]*
                       kappa*(3*deltaz*deltaz - l_sqr)/pow(l_sqr, 2.5);
            }
        }
    }
    res *= SI2EOTVOS*G*tess.density*d2r*(tess.e - tess.w)*d2r*(tess.n - tess.s)*
           (tess.r2 - tess.r1)*0.125;
    return res;
}
