/*
Unit tests for libtesseroid.c functions.
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "libsphere.h"
#include "libtesseroid.h"
#include "geometry.h"
#include "constants.h"


char msg[1000];
double d2r = 180./PI;

/* Test data taken from:
    http://mathworld.wolfram.com/Legendre-GaussQuadrature.html */
double o2roots[2] = {-0.577350269, 0.577350269},
       o3roots[3] = {-0.774596669, 0., 0.774596669},
       o4roots[4] = {-0.861136312, -0.339981044, 0.339981044, 0.861136312},
       o5roots[5] = {-0.906179846, -0.53846931, 0., 0.53846931, 0.906179846},
       o19roots[19] = {-0.992406843843584350,
                       -0.960208152134830020,
                       -0.903155903614817900,
                       -0.822714656537142820,
                       -0.720966177335229390,
                       -0.600545304661680990,
                       -0.464570741375960940,
                       -0.316564099963629830,
                       -0.160358645640225370,
                        0.000000000000000000,
                        0.160358645640225370,
                        0.316564099963629830,
                        0.464570741375960940,
                        0.600545304661680990,
                        0.720966177335229390,
                        0.822714656537142820,
                        0.903155903614817900,
                        0.960208152134830020,
                        0.992406843843584350};

double o2weights[2] = {1., 1.},
       o3weights[3] = {0.555555556, 0.888888889, 0.555555556},
       o4weights[4] = {0.347854845, 0.652145155, 0.652145155, 0.347854845},
       o5weights[5] = {0.236926885, 0.47862867, 0.568888889, 0.47862867,
            0.236926885};

static char * test_glq_next_root_fail()
{
    double roots[10];
    int i, order, rc;

    /* Test order fail */
    i = 1;
    order = -1;
    rc = glq_next_root(0.5, i, order, roots);
    sprintf(msg, "(order %d) return code %d, expected 1", order, rc);
    mu_assert(rc == 1, msg);

    order = 0;
    rc = glq_next_root(-0.1, i, order, roots);
    sprintf(msg, "(order %d) return code %d, expected 1", order, rc);
    mu_assert(rc == 1, msg);

    order = 1;
    rc = glq_next_root(1.1, i, order, roots);
    sprintf(msg, "(order %d) return code %d, expected 1", order, rc);
    mu_assert(rc == 1, msg);

    /* Test index fail */
    order = 5;
    i = -1;
    rc = glq_next_root(0.5, i, order, roots);
    sprintf(msg, "(index %d, order %d) return code %d, expected 2", order, i,
            rc);
    mu_assert(rc == 2, msg);

    i = 5;
    rc = glq_next_root(0.5, i, order, roots);
    sprintf(msg, "(index %d, order %d) return code %d, expected 2", order, i,
            rc);
    mu_assert(rc == 2, msg);

    i = 10;
    rc = glq_next_root(0.5, i, order, roots);
    sprintf(msg, "(index %d, order %d) return code %d, expected 2", order, i,
            rc);
    mu_assert(rc == 2, msg);

    return 0;
}


static char * test_glq_next_root()
{
    double prec = pow(10, -9), root[19], initial;
    int rc, i, order;

    /* Test order 2 */
    order = 2;
    for(i = 0; i < order; i++)
    {
        initial = cos(PI*((order - i) - 0.25)/(order + 0.5));

        rc = glq_next_root(initial, i, order, root);

        sprintf(msg, "(order %d, root %d) return code %d, expected 0", order, i,
                rc);
        mu_assert(rc == 0, msg);

        sprintf(msg, "(order %d, root %d) expected %.15f got %.15f", order, i,
                o2roots[i], root[i]);
        mu_assert_almost_equals(root[i], o2roots[i], prec, msg);
    }

    /* Test order 3 */
    order = 3;
    for(i = 0; i < order; i++)
    {
        initial = cos(PI*((order - i) - 0.25)/(order + 0.5));

        rc = glq_next_root(initial, i, order, root);

        sprintf(msg, "(order %d, root %d) return code %d, expected 0", order, i,
                rc);
        mu_assert(rc == 0, msg);

        sprintf(msg, "(order %d, root %d) expected %.15f got %.15f", order, i,
                o3roots[i], root[i]);
        mu_assert_almost_equals(root[i], o3roots[i], prec, msg);
    }

    /* Test order 4 */
    order = 4;
    for(i = 0; i < order; i++)
    {
        initial = cos(PI*((order - i) - 0.25)/(order + 0.5));

        rc = glq_next_root(initial, i, order, root);

        sprintf(msg, "(order %d, root %d) return code %d, expected 0", order, i,
                rc);
        mu_assert(rc == 0, msg);

        sprintf(msg, "(order %d, root %d) expected %.15f got %.15f", order, i,
                o4roots[i], root[i]);
        mu_assert_almost_equals(root[i], o4roots[i], prec, msg);
    }

    /* Test order 5 */
    order = 5;
    for(i = 0; i < order; i++)
    {
        initial = cos(PI*((order - i) - 0.25)/(order + 0.5));

        rc = glq_next_root(initial, i, order, root);

        sprintf(msg, "(order %d, root %d) return code %d, expected 0", order, i,
                rc);
        mu_assert(rc == 0, msg);

        sprintf(msg, "(order %d, root %d) expected %.15f got %.15f", order, i,
                o5roots[i], root[i]);
        mu_assert_almost_equals(root[i], o5roots[i], prec, msg);
    }

    /* Test order 19 */
    order = 19;
    for(i = 0; i < order; i++)
    {
        initial = cos(PI*((order - i) - 0.25)/(order + 0.5));

        rc = glq_next_root(initial, i, order, root);

        sprintf(msg, "(order %d, root %d) return code %d, expected 0", order, i,
                rc);
        mu_assert(rc == 0, msg);

        sprintf(msg, "(order %d, root %d) expected %.15f got %.15f", order, i,
                o19roots[i], root[i]);
        mu_assert_almost_equals(root[i], o19roots[i], prec, msg);
    }

    return 0;
}


static char * test_glq_weights()
{
    double prec = pow(10, -9), weights[5];
    int rc, i, order;

    /* Test order 2 */
    order = 2;

    rc = glq_weights(order, o2roots, weights);

    sprintf(msg, "(order %d) return code %d, expected 0", order, rc);
    mu_assert(rc == 0, msg);

    for(i = 0; i < order; i++)
    {
        sprintf(msg, "(order %d, weight %d) expected %.15f got %.15f", order,
                i, o2weights[i], weights[i]);
        mu_assert_almost_equals(weights[i], o2weights[i], prec, msg);
    }

    /* Test order 3 */
    order = 3;

    rc = glq_weights(order, o3roots, weights);

    sprintf(msg, "(order %d) return code %d, expected 0", order, rc);
    mu_assert(rc == 0, msg);

    for(i = 0; i < order; i++)
    {
        sprintf(msg, "(order %d, weight %d) expected %.15f got %.15f", order,
                i, o3weights[i], weights[i]);
        mu_assert_almost_equals(weights[i], o3weights[i], prec, msg);
    }

    /* Test order 4 */
    order = 4;

    rc = glq_weights(order, o4roots, weights);

    sprintf(msg, "(order %d) return code %d, expected 0", order, rc);
    mu_assert(rc == 0, msg);

    for(i = 0; i < order; i++)
    {
        sprintf(msg, "(order %d, weight %d) expected %.15f got %.15f", order,
                i, o4weights[i], weights[i]);
        mu_assert_almost_equals(weights[i], o4weights[i], prec, msg);
    }

    /* Test order 5 */
    order = 5;

    rc = glq_weights(order, o5roots, weights);

    sprintf(msg, "(order %d) return code %d, expected 0", order, rc);
    mu_assert(rc == 0, msg);

    for(i = 0; i < order; i++)
    {
        sprintf(msg, "(order %d, weight %d) expected %.15f got %.15f", order,
                i, o5weights[i], weights[i]);
        mu_assert_almost_equals(weights[i], o5weights[i], prec, msg);
    }

    return 0;
}


static char * test_glq_nodes()
{
    double prec = pow(10, -9), nodes[19];
    int rc, i, order;

    /* Test order 2 */
    order = 2;

    rc = glq_nodes(order, nodes);

    sprintf(msg, "(order %d) return code %d, expected 0", order, rc);
    mu_assert(rc == 0, msg);

    for(i = 0; i < order; i++)
    {
        sprintf(msg, "(order %d, node %d) expected %.15f got %.15f", order,
                i, o2roots[i], nodes[i]);
        mu_assert_almost_equals(nodes[i], o2roots[i], prec, msg);
    }

    /* Test order 3 */
    order = 3;

    rc = glq_nodes(order, nodes);

    sprintf(msg, "(order %d) return code %d, expected 0", order, rc);
    mu_assert(rc == 0, msg);

    for(i = 0; i < order; i++)
    {
        sprintf(msg, "(order %d, node %d) expected %.15f got %.15f", order,
                i, o3roots[i], nodes[i]);
        mu_assert_almost_equals(nodes[i], o3roots[i], prec, msg);
    }

    /* Test order 4 */
    order = 4;

    rc = glq_nodes(order, nodes);

    sprintf(msg, "(order %d) return code %d, expected 0", order, rc);
    mu_assert(rc == 0, msg);

    for(i = 0; i < order; i++)
    {
        sprintf(msg, "(order %d, node %d) expected %.15f got %.15f", order,
                i, o4roots[i], nodes[i]);
        mu_assert_almost_equals(nodes[i], o4roots[i], prec, msg);
    }

    /* Test order 5 */
    order = 5;

    rc = glq_nodes(order, nodes);

    sprintf(msg, "(order %d) return code %d, expected 0", order, rc);
    mu_assert(rc == 0, msg);

    for(i = 0; i < order; i++)
    {
        sprintf(msg, "(order %d, node %d) expected %.15f got %.15f", order,
                i, o5roots[i], nodes[i]);
        mu_assert_almost_equals(nodes[i], o5roots[i], prec, msg);
    }

    /* Test order 19 */
    order = 19;

    rc = glq_nodes(order, nodes);

    sprintf(msg, "(order %d) return code %d, expected 0", order, rc);
    mu_assert(rc == 0, msg);

    for(i = 0; i < order; i++)
    {
        sprintf(msg, "(order %d, node %d) expected %.15f got %.15f", order,
                i, o19roots[i], nodes[i]);
        mu_assert_almost_equals(nodes[i], o19roots[i], prec, msg);
    }

    return 0;
}


static char * test_glq_set_limits()
{
    double prec = pow(10, -9), unscaled[5], scaled[5], a, b, correct;
    int rc, i;
    GLQ glq;

    glq.nodes_unscaled = unscaled;
    glq.nodes = scaled;

    glq.order = 2;
    a = -2.54;
    b = 14.9;
    mu_arraycp(o2roots, glq.nodes_unscaled, glq.order);

    rc = glq_set_limits(a, b, &glq);
    sprintf(msg, "(order %d, a %g, b %g) return code %d, expected 0", glq.order,
            a, b, rc);
    mu_assert(rc == 0, msg);

    for(i = 0; i < glq.order; i++)
    {
        correct = 8.72*o2roots[i] + 6.18;
        sprintf(msg,
                "(order %d, index %d, a %g, b %g) expected %.15f, got %.15f",
                glq.order, i, a, b, correct, glq.nodes[i]);
        mu_assert_almost_equals(glq.nodes[i], correct, prec, msg);
    }

    glq.order = 3;
    a = 125.6;
    b = 234.84;
    mu_arraycp(o3roots, glq.nodes_unscaled, glq.order);

    rc = glq_set_limits(a, b, &glq);
    sprintf(msg, "(order %d, a %g, b %g) return code %d, expected 0", glq.order,
            a, b, rc);
    mu_assert(rc == 0, msg);

    for(i = 0; i < glq.order; i++)
    {
        correct = 54.62*o3roots[i] + 180.22;
        sprintf(msg,
                "(order %d, index %d, a %g, b %g) expected %.15f, got %.15f",
                glq.order, i, a, b, correct, glq.nodes[i]);
        mu_assert_almost_equals(glq.nodes[i], correct, prec, msg);
    }

    glq.order = 4;
    a = 3.5;
    b = -12.4;
    mu_arraycp(o4roots, glq.nodes_unscaled, glq.order);

    rc = glq_set_limits(a, b, &glq);
    sprintf(msg, "(order %d, a %g, b %g) return code %d, expected 0", glq.order,
            a, b, rc);
    mu_assert(rc == 0, msg);

    for(i = 0; i < glq.order; i++)
    {
        correct = -7.95*o4roots[i] - 4.45;
        sprintf(msg,
                "(order %d, index %d, a %g, b %g) expected %.15f, got %.15f",
                glq.order, i, a, b, correct, glq.nodes[i]);
        mu_assert_almost_equals(glq.nodes[i], correct, prec, msg);
    }

    glq.order = 5;
    a = 0.0;
    b = 0.0;
    mu_arraycp(o5roots, glq.nodes_unscaled, glq.order);

    rc = glq_set_limits(a, b, &glq);
    sprintf(msg, "(order %d, a %g, b %g) return code %d, expected 0", glq.order,
            a, b, rc);
    mu_assert(rc == 0, msg);

    for(i = 0; i < glq.order; i++)
    {
        correct = 0.0;
        sprintf(msg,
                "(order %d, index %d, a %g, b %g) expected %.15f, got %.15f",
                glq.order, i, a, b, correct, glq.nodes[i]);
        mu_assert_almost_equals(glq.nodes[i], correct, prec, msg);
    }

    return 0;
}


static char * test_glq_intcos()
{
    double result, expected;
    double angles[6];
    int i, t, orders[6] = {2, 3, 5, 8, 15, 25};
    GLQ *glq;

    angles[0] = PI*0.1;
    angles[1] = PI;
    angles[2] = PI*1.2;
    angles[3] = PI*1.9;
    angles[4] = PI*4.3;
    angles[5] = PI*6.9;

    for(t = 0; t < 6; t++)
    {
        glq = glq_new(orders[t], 0., angles[t]);

        if(glq == NULL)
        {
            sprintf(msg,
                "(order %d, angle %g) failed to create new GLQ struct",
                orders[t], angles[t]);
            mu_assert(0, msg);
        }

        for(i = 0, result = 0; i < orders[t]; i++)
        {
            result += glq->weights[i]*cos(glq->nodes[i]);
        }
        result *= 0.5*angles[t];

        expected = sin(angles[t]);

        glq_free(glq);

        sprintf(msg, "(order %d, angle %g) expected %f, got %f", orders[t],
                angles[t], expected, result);
        mu_assert_almost_equals(result, expected, pow(10, -5), msg);
    }

    return 0;
}


static char * test_tess2sphere_pot()
{
    SPHERE sphere;
    TESSEROID tess;
    double radius, dist, restess, ressphere;
    GLQ *glqlon, *glqlat, *glqr;

    tess.density = 1000.;
    tess.w = 44;
    tess.e = 46;
    tess.s = -1;
    tess.n = 1;
    tess.r1 = MEAN_EARTH_RADIUS - 100000;
    tess.r2 = MEAN_EARTH_RADIUS;

    glqlon = glq_new(8, tess.w, tess.e);
    if(glqlon == NULL)
        mu_assert(0, "GLQ allocation error");

    glqlat = glq_new(8, tess.s, tess.n);
    if(glqlat == NULL)
        mu_assert(0, "GLQ allocation error");

    glqr = glq_new(8, tess.r1, tess.r2);
    if(glqr == NULL)
        mu_assert(0, "GLQ allocation error");

    radius = tess.r2;

    /* Make a sphere with the same mass as the tesseroid */
    tess2sphere(tess, &sphere);
    for(dist=1000000; dist <= 2000000; dist += 1000)
    {
        restess = tess_pot(tess, 0, sin(d2r*40), cos(d2r*40), radius + dist,
                           *glqlon, *glqlat, *glqr);
        ressphere = sphere_pot(sphere, 0, 40, radius + dist);
        sprintf(msg, "(distance %g m) tess = %.5f  sphere = %.5f", dist,
                restess, ressphere);
        mu_assert_almost_equals(restess, ressphere, 0.01, msg);
    }
    return 0;
}


static char * test_tess2sphere_gx()
{
    SPHERE sphere;
    TESSEROID tess;
    double radius, dist, restess, ressphere;
    GLQ *glqlon, *glqlat, *glqr;

    tess.density = 1000.;
    tess.w = 44;
    tess.e = 46;
    tess.s = -1;
    tess.n = 1;
    tess.r1 = MEAN_EARTH_RADIUS - 100000;
    tess.r2 = MEAN_EARTH_RADIUS;

    glqlon = glq_new(8, tess.w, tess.e);
    if(glqlon == NULL)
        mu_assert(0, "GLQ allocation error");

    glqlat = glq_new(8, tess.s, tess.n);
    if(glqlat == NULL)
        mu_assert(0, "GLQ allocation error");

    glqr = glq_new(8, tess.r1, tess.r2);
    if(glqr == NULL)
        mu_assert(0, "GLQ allocation error");

    radius = tess.r2;

    /* Make a sphere with the same mass as the tesseroid */
    tess2sphere(tess, &sphere);

    for(dist=1000000; dist <= 2000000; dist += 1000)
    {
        restess = tess_gx(tess, 0, sin(d2r*40), cos(d2r*40), radius + dist,
                          *glqlon, *glqlat, *glqr);
        ressphere = sphere_gx(sphere, 0, 40, radius + dist);
        sprintf(msg, "(distance %g m) tess = %.5f  sphere = %.5f", dist,
                restess, ressphere);
        mu_assert_almost_equals(restess, ressphere, 0.1, msg);
    }

    return 0;
}


static char * test_tess2sphere_gy()
{
    SPHERE sphere;
    TESSEROID tess;
    double radius, dist, restess, ressphere;
    GLQ *glqlon, *glqlat, *glqr;

    tess.density = 1000.;
    tess.w = 44;
    tess.e = 46;
    tess.s = -1;
    tess.n = 1;
    tess.r1 = MEAN_EARTH_RADIUS - 100000;
    tess.r2 = MEAN_EARTH_RADIUS;

    glqlon = glq_new(8, tess.w, tess.e);
    if(glqlon == NULL)
        mu_assert(0, "GLQ allocation error");

    glqlat = glq_new(8, tess.s, tess.n);
    if(glqlat == NULL)
        mu_assert(0, "GLQ allocation error");

    glqr = glq_new(8, tess.r1, tess.r2);
    if(glqr == NULL)
        mu_assert(0, "GLQ allocation error");

    radius = tess.r2;

    /* Make a sphere with the same mass as the tesseroid */
    tess2sphere(tess, &sphere);

    for(dist=1000000; dist <= 2000000; dist += 1000)
    {
        restess = tess_gy(tess, 5, sin(d2r*45), cos(d2r*45), radius + dist,
                          *glqlon, *glqlat, *glqr);
        ressphere = sphere_gy(sphere, 5, 45, radius + dist);

        sprintf(msg, "(distance %g m) tess = %.5f  sphere = %.5f", dist,
                restess, ressphere);
        mu_assert_almost_equals(restess, ressphere, 0.1, msg);
    }

    return 0;
}


static char * test_tess2sphere_gz()
{
    SPHERE sphere;
    TESSEROID tess;
    double radius, dist, restess, ressphere;
    GLQ *glqlon, *glqlat, *glqr;

    tess.density = 1000.;
    tess.w = 44;
    tess.e = 46;
    tess.s = -1;
    tess.n = 1;
    tess.r1 = MEAN_EARTH_RADIUS - 100000;
    tess.r2 = MEAN_EARTH_RADIUS;

    glqlon = glq_new(8, tess.w, tess.e);
    if(glqlon == NULL)
        mu_assert(0, "GLQ allocation error");

    glqlat = glq_new(8, tess.s, tess.n);
    if(glqlat == NULL)
        mu_assert(0, "GLQ allocation error");

    glqr = glq_new(8, tess.r1, tess.r2);
    if(glqr == NULL)
        mu_assert(0, "GLQ allocation error");

    radius = tess.r2;

    /* Make a sphere with the same mass as the tesseroid */
    tess2sphere(tess, &sphere);

    for(dist=1500000; dist <= 2000000; dist += 1000)
    {
        restess = -tess_gz(tess, 0, sin(d2r*45,radius+dist,*glqlon,*glqlat,*glqr);
        ressphere = sphere_gz(sphere,0,45,radius+dist);

        sprintf(msg, "(distance %g m) tess = %.5f  sphere = %.5f", dist,
                restess, ressphere);
        mu_assert_almost_equals(restess, ressphere, 0.1, msg);
    }

    return 0;
}


static char * test_tess2sphere_gxx()
{
    SPHERE sphere;
    TESSEROID tess;
    double radius, dist, restess, ressphere;
    GLQ *glqlon, *glqlat, *glqr;

    tess.density = 1000.;
    tess.w = 44;
    tess.e = 46;
    tess.s = -1;
    tess.n = 1;
    tess.r1 = MEAN_EARTH_RADIUS - 100000;
    tess.r2 = MEAN_EARTH_RADIUS;

    glqlon = glq_new(8, tess.w, tess.e);
    if(glqlon == NULL)
        mu_assert(0, "GLQ allocation error");

    glqlat = glq_new(8, tess.s, tess.n);
    if(glqlat == NULL)
        mu_assert(0, "GLQ allocation error");

    glqr = glq_new(8, tess.r1, tess.r2);
    if(glqr == NULL)
        mu_assert(0, "GLQ allocation error");

    radius = tess.r2;

    /* Make a sphere with the same mass as the tesseroid */
    tess2sphere(tess, &sphere);

    for(dist=1300000; dist <= 2000000; dist += 1000)
    {
        restess = tess_gxx(tess,0,45,radius+dist,*glqlon,*glqlat,*glqr);
        ressphere = sphere_gxx(sphere,0,45,radius+dist);

        sprintf(msg, "(distance %g m) tess = %.5f  sphere = %.5f", dist,
                restess, ressphere);
        mu_assert_almost_equals(restess, ressphere, 0.001, msg);
    }

    return 0;
}


static char * test_tess2sphere_gxy()
{
    SPHERE sphere;
    TESSEROID tess;
    double radius, dist, restess, ressphere;
    GLQ *glqlon, *glqlat, *glqr;

    tess.density = 1000.;
    tess.w = 44;
    tess.e = 46;
    tess.s = -1;
    tess.n = 1;
    tess.r1 = MEAN_EARTH_RADIUS - 100000;
    tess.r2 = MEAN_EARTH_RADIUS;

    glqlon = glq_new(8, tess.w, tess.e);
    if(glqlon == NULL)
        mu_assert(0, "GLQ allocation error");

    glqlat = glq_new(8, tess.s, tess.n);
    if(glqlat == NULL)
        mu_assert(0, "GLQ allocation error");

    glqr = glq_new(8, tess.r1, tess.r2);
    if(glqr == NULL)
        mu_assert(0, "GLQ allocation error");

    radius = tess.r2;

    /* Make a sphere with the same mass as the tesseroid */
    tess2sphere(tess, &sphere);

    for(dist=1500000; dist <= 2000000; dist += 1000)
    {
        restess = tess_gxy(tess,5,50,radius+dist,*glqlon,*glqlat,*glqr);
        ressphere = sphere_gxy(sphere,5,50,radius+dist);

        sprintf(msg, "(distance %g m) tess = %.5f  sphere = %.5f", dist,
                restess, ressphere);
        mu_assert_almost_equals(restess, ressphere, 0.001, msg);
    }

    return 0;
}


static char * test_tess2sphere_gxz()
{
    SPHERE sphere;
    TESSEROID tess;
    double radius, dist, restess, ressphere;
    GLQ *glqlon, *glqlat, *glqr;

    tess.density = 1000.;
    tess.w = 44;
    tess.e = 46;
    tess.s = -1;
    tess.n = 1;
    tess.r1 = MEAN_EARTH_RADIUS - 100000;
    tess.r2 = MEAN_EARTH_RADIUS;

    glqlon = glq_new(8, tess.w, tess.e);
    if(glqlon == NULL)
        mu_assert(0, "GLQ allocation error");

    glqlat = glq_new(8, tess.s, tess.n);
    if(glqlat == NULL)
        mu_assert(0, "GLQ allocation error");

    glqr = glq_new(8, tess.r1, tess.r2);
    if(glqr == NULL)
        mu_assert(0, "GLQ allocation error");

    radius = tess.r2;

    /* Make a sphere with the same mass as the tesseroid */
    tess2sphere(tess, &sphere);

    for(dist=1500000; dist <= 2000000; dist += 1000)
    {
        restess = tess_gxz(tess,0,50,radius+dist,*glqlon,*glqlat,*glqr);
        ressphere = sphere_gxz(sphere,0,50,radius+dist);

        sprintf(msg, "(distance %g m) tess = %.5f  sphere = %.5f", dist,
                restess, ressphere);
        mu_assert_almost_equals(restess, ressphere, 0.001, msg);
    }

    return 0;
}


static char * test_tess2sphere_gyy()
{
    SPHERE sphere;
    TESSEROID tess;
    double radius, dist, restess, ressphere;
    GLQ *glqlon, *glqlat, *glqr;

    tess.density = 1000.;
    tess.w = 44;
    tess.e = 46;
    tess.s = -1;
    tess.n = 1;
    tess.r1 = MEAN_EARTH_RADIUS - 100000;
    tess.r2 = MEAN_EARTH_RADIUS;

    glqlon = glq_new(8, tess.w, tess.e);
    if(glqlon == NULL)
        mu_assert(0, "GLQ allocation error");

    glqlat = glq_new(8, tess.s, tess.n);
    if(glqlat == NULL)
        mu_assert(0, "GLQ allocation error");

    glqr = glq_new(8, tess.r1, tess.r2);
    if(glqr == NULL)
        mu_assert(0, "GLQ allocation error");

    radius = tess.r2;

    /* Make a sphere with the same mass as the tesseroid */
    tess2sphere(tess, &sphere);

    for(dist=1500000; dist <= 2000000; dist += 1000)
    {
        restess = tess_gyy(tess,0,45,radius+dist,*glqlon,*glqlat,*glqr);
        ressphere = sphere_gyy(sphere,0,45,radius+dist);

        sprintf(msg, "(distance %g m) tess = %.5f  sphere = %.5f", dist,
                restess, ressphere);
        mu_assert_almost_equals(restess, ressphere, 0.001, msg);
    }

    return 0;
}


static char * test_tess2sphere_gyz()
{
    SPHERE sphere;
    TESSEROID tess;
    double radius, dist, restess, ressphere;
    GLQ *glqlon, *glqlat, *glqr;

    tess.density = 1000.;
    tess.w = 44;
    tess.e = 46;
    tess.s = -1;
    tess.n = 1;
    tess.r1 = MEAN_EARTH_RADIUS - 100000;
    tess.r2 = MEAN_EARTH_RADIUS;

    glqlon = glq_new(8, tess.w, tess.e);
    if(glqlon == NULL)
        mu_assert(0, "GLQ allocation error");

    glqlat = glq_new(8, tess.s, tess.n);
    if(glqlat == NULL)
        mu_assert(0, "GLQ allocation error");

    glqr = glq_new(8, tess.r1, tess.r2);
    if(glqr == NULL)
        mu_assert(0, "GLQ allocation error");

    radius = tess.r2;

    /* Make a sphere with the same mass as the tesseroid */
    tess2sphere(tess, &sphere);

    for(dist=1500000; dist <= 2000000; dist += 1000)
    {
        restess = tess_gyz(tess,5,45,radius+dist,*glqlon,*glqlat,*glqr);
        ressphere = sphere_gyz(sphere,5,45,radius+dist);

        sprintf(msg, "(distance %g m) tess = %.5f  sphere = %.5f", dist,
                restess, ressphere);
        mu_assert_almost_equals(restess, ressphere, 0.001, msg);
    }

    return 0;
}


static char * test_tess2sphere_gzz()
{
    SPHERE sphere;
    TESSEROID tess;
    double radius, dist, restess, ressphere;
    GLQ *glqlon, *glqlat, *glqr;

    tess.density = 1000.;
    tess.w = 44;
    tess.e = 46;
    tess.s = -1;
    tess.n = 1;
    tess.r1 = MEAN_EARTH_RADIUS - 100000;
    tess.r2 = MEAN_EARTH_RADIUS;

    glqlon = glq_new(8, tess.w, tess.e);
    if(glqlon == NULL)
        mu_assert(0, "GLQ allocation error");

    glqlat = glq_new(8, tess.s, tess.n);
    if(glqlat == NULL)
        mu_assert(0, "GLQ allocation error");

    glqr = glq_new(8, tess.r1, tess.r2);
    if(glqr == NULL)
        mu_assert(0, "GLQ allocation error");

    radius = tess.r2;

    /* Make a sphere with the same mass as the tesseroid */
    tess2sphere(tess, &sphere);

    for(dist=1500000; dist <= 2000000; dist += 1000)
    {
        restess = tess_gzz(tess,0,45,radius+dist,*glqlon,*glqlat,*glqr);
        ressphere = sphere_gzz(sphere,0,45,radius+dist);

        sprintf(msg, "(distance %g m) tess = %.5f  sphere = %.5f", dist,
                restess, ressphere);
        mu_assert_almost_equals(restess, ressphere, 0.001, msg);
    }

    return 0;
}


static char * test_tess_tensor_trace()
{
    #define N 4
    TESSEROID tesses[N] = {
        {1,0,1,0,1,6000000,6001000},
        {1,180,183,80,81.5,6300000,6302000},
        {1,200,203,-90,-88,5500000,5500100},
        {1,-10,-7,7,7.5,6500000,6505000}};
    GLQ *glqlon, *glqlat, *glqr;
    int i;
    double lon, lat, r, trace, dist;

    glqlon = glq_new(8, tesses[0].w, tesses[0].e);
    if(glqlon == NULL)
        mu_assert(0, "GLQ allocation error");

    glqlat = glq_new(8, tesses[0].s, tesses[0].n);
    if(glqlat == NULL)
        mu_assert(0, "GLQ allocation error");

    glqr = glq_new(8, tesses[0].r1, tesses[0].r2);
    if(glqr == NULL)
        mu_assert(0, "GLQ allocation error");

    for(i = 0; i < N; i++)
    {
        lon = 0.5*(tesses[i].w + tesses[i].e);
        lat = 0.5*(tesses[i].n + tesses[i].s);
        r = tesses[i].r2;

        for(dist=100000; dist <= 5000000; dist += 5000)
        {
            trace = calc_tess_model_adapt(&tesses[i], 1, lon, lat, r + dist,
                        glqlon, glqlat, glqr, tess_gxx,
                        TESSEROID_GXX_SIZE_RATIO) +
                    calc_tess_model_adapt(&tesses[i], 1, lon, lat, r + dist,
                        glqlon, glqlat, glqr, tess_gyy,
                        TESSEROID_GYY_SIZE_RATIO) +
                    calc_tess_model_adapt(&tesses[i], 1, lon, lat, r + dist,
                        glqlon, glqlat, glqr, tess_gzz,
                        TESSEROID_GZZ_SIZE_RATIO);

            sprintf(msg, "(tess %d dist %g) trace %.10f", i, dist, trace);
            mu_assert_almost_equals(trace, 0, 0.0000000001, msg);
        }
    }

    glq_free(glqlon);
    glq_free(glqlat);
    glq_free(glqr);
    #undef N
    return 0;
}


static char * test_adaptative()
{
    /* Check if the adaptative is dividing properly and returning the same thing
       as the non-adaptative (do spliting by hand) */
    TESSEROID tess,
              split[8];
    GLQ *glqlon, *glqlat, *glqr;
    double mindist, resadapt, resnormal;

    tess.density = 1000.;
    tess.w = -0.5;
    tess.e = 0.5;
    tess.s = -0.5;
    tess.n = 0.5;
    tess.r1 = MEAN_EARTH_RADIUS - 10000;
    tess.r2 = MEAN_EARTH_RADIUS;

    glqlon = glq_new(2, tess.w, tess.e);
    if(glqlon == NULL)
        mu_assert(0, "GLQ allocation error");

    glqlat = glq_new(2, tess.s, tess.n);
    if(glqlat == NULL)
        mu_assert(0, "GLQ allocation error");

    glqr = glq_new(2, tess.r1, tess.r2);
    if(glqr == NULL)
        mu_assert(0, "GLQ allocation error");

    mindist = TESSEROID_GZZ_SIZE_RATIO*111110.*(tess.e - tess.w);

    /* If at half mindist should only divide once */
    resadapt = calc_tess_model_adapt(&tess, 1, 0, 0,
                                     0.5*mindist + MEAN_EARTH_RADIUS, glqlon,
                                     glqlat, glqr, tess_gzz,
                                     TESSEROID_GZZ_SIZE_RATIO);

    split_tess(tess, split);
    resnormal = calc_tess_model(split, 8, 0, 0,
                                0.5*mindist + MEAN_EARTH_RADIUS, glqlon,
                                glqlat, glqr, tess_gzz);

    sprintf(msg, "adapt = %.10f  normal = %.10f", resadapt, resnormal);
    mu_assert_almost_equals(resadapt, resnormal, pow(10, -10), msg);

    return 0;
}


int grav_tess_run_all()
{
    int failed = 0;
    failed += mu_run_test(test_glq_next_root_fail,
                "glq_next_root returns correct fail code");
    failed += mu_run_test(test_glq_next_root,
                "glq_next_root produces correct results");
    failed += mu_run_test(test_glq_nodes, "glq_nodes produces correct results");
    failed += mu_run_test(test_glq_set_limits,
                "glq_set_limits produces correct results");
    failed += mu_run_test(test_glq_weights,
                "glq_weights produces correct results");
    failed += mu_run_test(test_glq_intcos,
                "glq cossine integration produces correct results");
    failed += mu_run_test(test_tess2sphere_pot,
                "tess_pot results equal to sphere of same mass at distance");
    failed += mu_run_test(test_tess2sphere_gx,
                "tess_gx results equal to sphere of same mass at distance");
    failed += mu_run_test(test_tess2sphere_gy,
                "tess_gy results equal to sphere of same mass at distance");
    failed += mu_run_test(test_tess2sphere_gz,
                "tess_gz results equal to sphere of same mass at distance");
    failed += mu_run_test(test_tess2sphere_gxx,
                "tess_gxx results equal to sphere of same mass at distance");
    failed += mu_run_test(test_tess2sphere_gxy,
                "tess_gxy results equal to sphere of same mass at distance");
    failed += mu_run_test(test_tess2sphere_gxz,
                "tess_gxz results equal to sphere of same mass at distance");
    failed += mu_run_test(test_tess2sphere_gyy,
                "tess_gyy results equal to sphere of same mass at distance");
    failed += mu_run_test(test_tess2sphere_gyz,
                "tess_gyz results equal to sphere of same mass at distance");
    failed += mu_run_test(test_tess2sphere_gzz,
                "tess_gzz results equal to sphere of same mass at distance");
    failed += mu_run_test(test_tess_tensor_trace,
                "trace of GGT for tesseroid is zero");
    failed += mu_run_test(test_adaptative,
            "calc_tess_model_adapt results as non-adapt with split by hand");
    return failed;
}
