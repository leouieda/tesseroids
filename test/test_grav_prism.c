/*
Unit tests for grav_prism.c functions.
*/

#include <stdio.h>
#include <math.h>
#include "../src/libsphere.h"
#include "../src/libprism.h"
#include "../src/geometry.h"
#include "../src/constants.h"


char msg[1000];

static char * test_pot_bellow()
{
    PRISM prism = {3000,-5000,5000,-5000,5000,-5000,5000,0,0,0};
    double dist, restop, resbellow;

    for(dist=5010; dist <= 500000; dist += 100)
    {
        restop = prism_pot(prism, 0, 0, -dist);
        resbellow = prism_pot(prism, 0, 0, dist);

        sprintf(msg, "(distance %g m) top = %.5f  bellow = %.5f", dist,
                restop, resbellow);
        mu_assert_almost_equals((double)(restop - resbellow)/restop, 0.,
                                0.001, msg);
    }

    return 0;
}

static char * test_gx_bellow()
{
    PRISM prism = {3000,-5000,5000,-5000,5000,-5000,5000,0,0,0};
    double dist, restop, resbellow;

    for(dist=5010; dist <= 500000; dist += 100)
    {
        restop = prism_gx(prism, 5000, 0, -dist);
        resbellow = prism_gx(prism, 5000, 0, dist);

        sprintf(msg, "(distance %g m) top = %.5f  bellow = %.5f", dist,
                restop, resbellow);
        mu_assert_almost_equals((double)(restop - resbellow)/restop, 0.,
                                0.001, msg);
    }

    return 0;
}

static char * test_gy_bellow()
{
    PRISM prism = {3000,-5000,5000,-5000,5000,-5000,5000,0,0,0};
    double dist, restop, resbellow;

    for(dist=5010; dist <= 500000; dist += 100)
    {
        restop = prism_gy(prism, 0, 5000, -dist);
        resbellow = prism_gy(prism, 0, 5000, dist);

        sprintf(msg, "(distance %g m) top = %.5f  bellow = %.5f", dist,
                restop, resbellow);
        mu_assert_almost_equals((double)(restop - resbellow)/restop, 0.,
                                0.001, msg);
    }

    return 0;
}

static char * test_gz_bellow()
{
    PRISM prism = {3000,-5000,5000,-5000,5000,-5000,5000,0,0,0};
    double dist, restop, resbellow;

    for(dist=5010; dist <= 500000; dist += 100)
    {
        restop = prism_gz(prism, 0, 0,-dist);
        resbellow = prism_gz(prism, 0, 0, dist);

        sprintf(msg, "(distance %g m) top = %.5f  bellow = %.5f", dist,
                restop, resbellow);
        mu_assert_almost_equals((double)(restop - (-resbellow))/restop, 0.,
                                0.001, msg);
    }

    return 0;
}

static char * test_gxx_bellow()
{
    PRISM prism = {3000,-5000,5000,-5000,5000,-5000,5000,0,0,0};
    double dist, restop, resbellow;

    for(dist=5010; dist <= 500000; dist += 100)
    {
        restop = prism_gxx(prism, 0, 0,-dist);
        resbellow = prism_gxx(prism, 0, 0, dist);

        sprintf(msg, "(distance %g m) top = %.5f  bellow = %.5f", dist,
                restop, resbellow);
        mu_assert_almost_equals((double)(restop - resbellow)/restop, 0.,
                                0.001, msg);
    }

    return 0;
}

static char * test_gxy_bellow()
{
    PRISM prism = {3000,-5000,5000,-5000,5000,-5000,5000,0,0,0};
    double dist, restop, resbellow;

    for(dist=5010; dist <= 500000; dist += 100)
    {
        restop = prism_gxy(prism, 5000, 5000, -dist);
        resbellow = prism_gxy(prism, 5000, 5000, dist);

        sprintf(msg, "(distance %g m) top = %.5f  bellow = %.5f", dist,
                restop, resbellow);
        mu_assert_almost_equals((double)(restop - resbellow)/restop, 0.,
                                0.001, msg);
    }

    return 0;
}

static char * test_gxz_bellow()
{
    PRISM prism = {3000,-5000,5000,-5000,5000,-5000,5000,0,0,0};
    double dist, restop, resbellow;

    for(dist=5010; dist <= 500000; dist += 100)
    {
        restop = prism_gxz(prism, 5000, 0,-dist);
        resbellow = prism_gxz(prism, 5000, 0, dist);

        sprintf(msg, "(distance %g m) top = %.5f  bellow = %.5f", dist,
                restop, resbellow);
        mu_assert_almost_equals((double)(restop - -1*resbellow)/restop, 0.,
                                0.001, msg);
    }

    return 0;
}

static char * test_gyy_bellow()
{
    PRISM prism = {3000,-5000,5000,-5000,5000,-5000,5000,0,0,0};
    double dist, restop, resbellow;

    for(dist=5010; dist <= 500000; dist += 100)
    {
        restop = prism_gyy(prism, 0, 0,-dist);
        resbellow = prism_gyy(prism, 0, 0, dist);

        sprintf(msg, "(distance %g m) top = %.5f  bellow = %.5f", dist,
                restop, resbellow);
        mu_assert_almost_equals((double)(restop - resbellow)/restop, 0.,
                                0.001, msg);
    }

    return 0;
}

static char * test_gyz_bellow()
{
    PRISM prism = {3000,-5000,5000,-5000,5000,-5000,5000,0,0,0};
    double dist, restop, resbellow;

    for(dist=5010; dist <= 500000; dist += 100)
    {
        restop = prism_gyz(prism, 0, 5000, -dist);
        resbellow = prism_gyz(prism, 0, 5000, dist);

        sprintf(msg, "(distance %g m) top = %.5f  bellow = %.5f", dist,
                restop, resbellow);
        mu_assert_almost_equals((double)(restop - -1*resbellow)/restop, 0.,
                                0.001, msg);
    }

    return 0;
}

static char * test_gzz_bellow()
{
    PRISM prism = {3000,-5000,5000,-5000,5000,-5000,5000,0,0,0};
    double dist, restop, resbellow;

    for(dist=5010; dist <= 500000; dist += 100)
    {
        restop = prism_gzz(prism, 0, 0, -dist);
        resbellow = prism_gzz(prism, 0, 0, dist);

        sprintf(msg, "(distance %g m) top = %.5f  bellow = %.5f", dist,
                restop, resbellow);
        mu_assert_almost_equals((double)(restop - resbellow)/restop, 0.,
                                0.001, msg);
    }

    return 0;
}

static char * test_prism2sphere_pot()
{
    SPHERE sphere;
    PRISM prism = {3000,-5000,5000,-5000,5000,-5000,5000,0,0,0};
    double dist, resprism, ressphere;

    /* Make a sphere with the same mass as the prism and put it at the origin */
    prism2sphere(prism, 0, 0, 0, &sphere);

    for(dist=50000; dist <= 500000; dist += 500)
    {
        resprism = prism_pot(prism,0,0,-dist);
        ressphere = sphere_pot(sphere,0,90,dist);

        sprintf(msg, "(distance %g m) prism = %.5f  sphere = %.5f", dist,
                resprism, ressphere);
        mu_assert_almost_equals(resprism, ressphere, 0.001, msg);
    }

    return 0;
}

static char * test_prism2sphere_gx()
{
    SPHERE sphere;
    PRISM prism = {3000,-5000,5000,-5000,5000,-5000,5000,0,0,0};
    double dist, resprism, ressphere;

    /* Make a sphere with the same mass as the prism and put it at the origin */
    prism2sphere(prism, 0, 0, 0, &sphere);

    for(dist=10000; dist <= 500000; dist += 500)
    {
        resprism = prism_gx(prism,0,0,-dist);
        ressphere = sphere_gx(sphere,0,90,dist);

        sprintf(msg, "(distance %g m) prism = %.5f  sphere = %.5f", dist,
                resprism, ressphere);
        mu_assert_almost_equals(resprism, ressphere, 0.00000001, msg);
    }

    return 0;
}


static char * test_prism2sphere_gy()
{
    SPHERE sphere;
    PRISM prism = {3000,-5000,5000,-5000,5000,-5000,5000,0,0,0};
    double dist, resprism, ressphere;

    /* Make a sphere with the same mass as the prism and put it at the origin */
    prism2sphere(prism, 0, 0, 0, &sphere);

    for(dist=10000; dist <= 500000; dist += 500)
    {
        resprism = prism_gy(prism,0,0,-dist);
        ressphere = sphere_gy(sphere,0,90,dist);

        sprintf(msg, "(distance %g m) prism = %.5f  sphere = %.5f", dist,
                resprism, ressphere);
        mu_assert_almost_equals(resprism, ressphere, 0.00000001, msg);
    }

    return 0;
}


static char * test_prism2sphere_gz()
{
    SPHERE sphere;
    PRISM prism = {3000,-5000,5000,-5000,5000,-5000,5000,0,0,0};
    double dist, resprism, ressphere;

    /* Make a sphere with the same mass as the prism and put it at the origin */
    prism2sphere(prism, 0, 0, 0, &sphere);

    for(dist=50000; dist <= 500000; dist += 500)
    {
        resprism = prism_gz(prism,0,0,-dist);
        ressphere = sphere_gz(sphere,0,90,dist);

        sprintf(msg, "(distance %g m) prism = %.5f  sphere = %.5f", dist,
                resprism, ressphere);
        mu_assert_almost_equals(resprism, -1*ressphere, 0.01, msg);
    }

    return 0;
}


static char * test_prism2sphere_gxx()
{
    SPHERE sphere;
    PRISM prism = {3000,-5000,5000,-5000,5000,-5000,5000,0,0,0};
    double dist, resprism, ressphere;

    /* Make a sphere with the same mass as the prism and put it at the origin */
    prism2sphere(prism, 0, 0, 0, &sphere);

    for(dist=50000; dist <= 500000; dist += 500)
    {
        resprism = prism_gxx(prism,0,0,-dist);
        ressphere = sphere_gxx(sphere,0,90,dist);

        sprintf(msg, "(distance %g m) prism = %.5f  sphere = %.5f", dist,
                resprism, ressphere);
        mu_assert_almost_equals(resprism, ressphere, 0.001, msg);
    }

    return 0;
}


static char * test_prism2sphere_gxy()
{
    SPHERE sphere;
    PRISM prism = {3000,-5000,5000,-5000,5000,-5000,5000,0,0,0};
    double dist, resprism, ressphere;

    /* Make a sphere with the same mass as the prism and put it at the origin */
    prism2sphere(prism, 0, 0, 0, &sphere);

    for(dist=50000; dist <= 500000; dist += 500)
    {
        resprism = prism_gxy(prism,0,0,-dist);
        ressphere = sphere_gxy(sphere,0,90,dist);

        sprintf(msg, "(distance %g m) prism = %.5f  sphere = %.5f", dist,
                resprism, ressphere);
        mu_assert_almost_equals(resprism, ressphere, 0.001, msg);
    }

    return 0;
}


static char * test_prism2sphere_gxz()
{
    SPHERE sphere;
    PRISM prism = {3000,-5000,5000,-5000,5000,-5000,5000,0,0,0};
    double dist, resprism, ressphere;

    /* Make a sphere with the same mass as the prism and put it at the origin */
    prism2sphere(prism, 0, 0, 0, &sphere);

    for(dist=50000; dist <= 500000; dist += 500)
    {
        resprism = prism_gxz(prism,0,0,-dist);
        ressphere = sphere_gxz(sphere,0,90,dist);

        sprintf(msg, "(distance %g m) prism = %.5f  sphere = %.5f", dist,
                resprism, ressphere);
        mu_assert_almost_equals(resprism, -1*ressphere, 0.001, msg);
    }

    return 0;
}


static char * test_prism2sphere_gyy()
{
    SPHERE sphere;
    PRISM prism = {3000,-5000,5000,-5000,5000,-5000,5000,0,0,0};
    double dist, resprism, ressphere;

    /* Make a sphere with the same mass as the prism and put it at the origin */
    prism2sphere(prism, 0, 0, 0, &sphere);

    for(dist=50000; dist <= 500000; dist += 500)
    {
        resprism = prism_gyy(prism,0,0,-dist);
        ressphere = sphere_gyy(sphere,0,90,dist);

        sprintf(msg, "(distance %g m) prism = %.5f  sphere = %.5f", dist,
                resprism, ressphere);
        mu_assert_almost_equals(resprism, ressphere, 0.001, msg);
    }

    return 0;
}


static char * test_prism2sphere_gyz()
{
    SPHERE sphere;
    PRISM prism = {3000,-5000,5000,-5000,5000,-5000,5000,0,0,0};
    double dist, resprism, ressphere;

    /* Make a sphere with the same mass as the prism and put it at the origin */
    prism2sphere(prism, 0, 0, 0, &sphere);

    for(dist=50000; dist <= 500000; dist += 500)
    {
        resprism = prism_gyz(prism,0,0,-dist);
        ressphere = sphere_gyz(sphere,0,90,dist);

        sprintf(msg, "(distance %g m) prism = %.5f  sphere = %.5f", dist,
                resprism, ressphere);
        mu_assert_almost_equals(resprism, -1*ressphere, 0.001, msg);
    }

    return 0;
}


static char * test_prism2sphere_gzz()
{
    SPHERE sphere;
    PRISM prism = {3000,-5000,5000,-5000,5000,-5000,5000,0,0,0};
    double dist, resprism, ressphere;

    /* Make a sphere with the same mass as the prism and put it at the origin */
    prism2sphere(prism, 0, 0, 0, &sphere);

    for(dist=60000; dist <= 500000; dist += 500)
    {
        resprism = prism_gzz(prism,0,0,-dist);
        ressphere = sphere_gzz(sphere,0,90,dist);

        sprintf(msg, "(distance %g m) prism = %.5f  sphere = %.5f", dist,
                resprism, ressphere);
        mu_assert_almost_equals(resprism, ressphere, 0.001, msg);
    }

    return 0;
}

static char * test_prism_tensor_trace()
{
    #define N 4
    TESSEROID tesses[N] = {
        {1,0,1,0,1,6000000,6001000},
        {1,180,183,80,81.5,6300000,6302000},
        {1,200,203,-90,-88,5500000,5500100},
        {1,-10,-7,7,7.5,6500000,6505000}};
    PRISM prism;
    int i;
    double trace, dist, x, y;

    for(i = 0; i < N; i++)
    {
        tess2prism_flatten(tesses[i], &prism);
        x = 0.5*(prism.x1 + prism.x2);
        y = 0.5*(prism.y1 + prism.y2);

        for(dist=1000; dist <= 5000000; dist += 1000)
        {

            trace = prism_gxx(prism, x, y, prism.z1 - dist)
                    + prism_gyy(prism, x, y, prism.z1 - dist)
                    + prism_gzz(prism, x, y, prism.z1 - dist);

            sprintf(msg, "(prism %d dist %g) trace %.10f", i, dist, trace);
            mu_assert_almost_equals(trace, 0, 0.0000000001, msg);
        }
    }
    #undef N
    return 0;
}

/* Test coordinate transformation */
static char * test_global2local()
{
    #define R 6378137.0
    #define N 3
    PRISM prisms[N] = {
        {3000,-5000,5000,-5000,5000,0,5000, 2.45, -36.32, R},
        {2000,-3000,3000,-2000,2000,0,800, -45.45, -103.1, R},
        {1000,-2000,2000,-1000,1000,0,234, -80.45, 183.2, R}};
    double x, y, z, newz[N] = {-3000, 1234, -2.3456};
    int i;

    for(i = 0; i < N; i++)
    {
        global2local(prisms[i].lon, prisms[i].lat, R - newz[i], prisms[i],
                     &x, &y, &z);
        sprintf(msg, "(prism %d) x: expect %.10g got %.10g", i, 0., x);
        mu_assert_almost_equals(x, 0., 0.00000001, msg);
        sprintf(msg, "(prism %d) y: expect %.10g got %.10g", i, 0., y);
        mu_assert_almost_equals(y, 0., 0.00000001, msg);
        sprintf(msg, "(prism %d) z: expect %.10g got %.10g", i, newz[i], z);
        mu_assert_almost_equals(z, newz[i], 0.00000001, msg);
    }
    #undef R
    #undef N
    return 0;
}

/* Test agains grav_prism */
static char * test_prism_pot_sph()
{
    #define R 6378137.0
    PRISM prism = {3000,-5000,5000,-5000,5000,0,5000,187,38,R};
    double res, expect;
    int fix;

    fix = 1;
    res = prism_pot_sph(prism, 187, 38, R + 1000);
    expect = prism_pot(prism, 0, 0, -1000);
    sprintf(msg, "(fixture %d) expect %.10g got %.10g", fix, expect, res);
    mu_assert_almost_equals(res, expect, 0.0000000001, msg);

    #undef R
    return 0;
}


static char * test_prism_g_sph()
{
    #define R 6378137.0
    PRISM prism = {3000,-5000,5000,-5000,5000,0,5000,27,-78,R};
    double resx, resy, resz, expectx, expecty, expectz;
    int fix;

    fix = 1;
    prism_g_sph(prism, 27, -78, R + 1000, &resx, &resy, &resz);
    expectx = prism_gx(prism, 0, 0, -1000);
    expecty = prism_gy(prism, 0, 0, -1000);
    expectz = prism_gz(prism, 0, 0, -1000);
    sprintf(msg, "(fixture %d) gx: expect %.10g got %.10g", fix, expectx, resx);
    mu_assert_almost_equals(resx, expectx, 0.0000000001, msg);
    sprintf(msg, "(fixture %d) gy: expect %.10g got %.10g", fix, expecty, resy);
    mu_assert_almost_equals(resy, expecty, 0.0000000001, msg);
    sprintf(msg, "(fixture %d) gz: expect %.10g got %.10g", fix, expectz, resz);
    mu_assert_almost_equals(resz, expectz, 0.0000000001, msg);

    #undef R
    return 0;
}


static char * test_prism_ggt_sph()
{
    #define R 6378137.0
    PRISM prism = {3000,-5000,5000,-5000,5000,0,5000,-7,8,R};
    double res[6], expect[6];
    int fix, i;

    fix = 1;
    prism_ggt_sph(prism, -7, 8, R + 1000, res);
    expect[0] = prism_gxx(prism, 0, 0, -1000);
    expect[1] = prism_gxy(prism, 0, 0, -1000);
    expect[2] = prism_gxz(prism, 0, 0, -1000);
    expect[3] = prism_gyy(prism, 0, 0, -1000);
    expect[4] = prism_gyz(prism, 0, 0, -1000);
    expect[5] = prism_gzz(prism, 0, 0, -1000);
    for(i = 0; i < 6; i++)
    {
        sprintf(msg, "(fixture %d) cmp %d: expect %.10f got %.10f", fix, i,
            expect[i], res[i]);
        mu_assert_almost_equals(res[i], expect[i], 0.000000001, msg);
    }
    #undef R
    return 0;
}

static char * test_prism_tensor_sph_trace()
{
    #define N 4
    #define GXX 0
    #define GYY 3
    #define GZZ 5
    TESSEROID tesses[N] = {
        {1,0,1,0,1,6000000,6001000},
        {1,180,183,80,81.5,6300000,6302000},
        {1,200,203,-90,-88,5500000,5500100},
        {1,-10,-7,7,7.5,6500000,6505000}};
    PRISM prism;
    int i;
    double trace, dist, tensor[6];

    for(i = 0; i < N; i++)
    {
        tess2prism(tesses[i], &prism);
        for(dist=1000; dist <= 5000000; dist += 1000)
        {
            prism_ggt_sph(prism, prism.lon, prism.lat, prism.r + dist, tensor);
            trace = tensor[GXX] + tensor[GYY] + tensor[GZZ];

            sprintf(msg, "(prism %d dist %g) trace %.10f", i, dist, trace);
            mu_assert_almost_equals(trace, 0, 0.0000000001, msg);
        }
    }
    #undef N
    #undef GXX
    #undef GYY
    #undef GZZ
    return 0;
}

int grav_prism_run_all()
{
    int failed = 0;
    failed += mu_run_test(test_pot_bellow,
                "prism_pot results equal above and bellow the prism");
    failed += mu_run_test(test_gx_bellow,
                "prism_gx results equal above and bellow the prism");
    failed += mu_run_test(test_gy_bellow,
                "prism_gy results equal above and bellow the prism");
    failed += mu_run_test(test_gz_bellow,
                "prism_gz results equal above and bellow the prism");
    failed += mu_run_test(test_gxx_bellow,
                "prism_gxx results equal above and bellow the prism");
    failed += mu_run_test(test_gxy_bellow,
                "prism_gxy results equal above and bellow the prism");
    failed += mu_run_test(test_gxz_bellow,
                "prism_gxz results equal above and bellow the prism");
    failed += mu_run_test(test_gyy_bellow,
                "prism_gyy results equal above and bellow the prism");
    failed += mu_run_test(test_gyz_bellow,
                "prism_gyz results equal above and bellow the prism");
    failed += mu_run_test(test_gzz_bellow,
                "prism_gzz results equal above and bellow the prism");
    failed += mu_run_test(test_prism2sphere_pot,
                "prism_pot results equal to sphere of same mass at distance");
    failed += mu_run_test(test_prism2sphere_gx,
                "prism_gx results equal to sphere of same mass at distance");
    failed += mu_run_test(test_prism2sphere_gy,
                "prism_gy results equal to sphere of same mass at distance");
    failed += mu_run_test(test_prism2sphere_gz,
                "prism_gz results equal to sphere of same mass at distance");
    failed += mu_run_test(test_prism2sphere_gxx,
                "prism_gxx results equal to sphere of same mass at distance");
    failed += mu_run_test(test_prism2sphere_gxy,
                "prism_gxy results equal to sphere of same mass at distance");
    failed += mu_run_test(test_prism2sphere_gxz,
                "prism_gxz results equal to sphere of same mass at distance");
    failed += mu_run_test(test_prism2sphere_gyy,
                "prism_gyy results equal to sphere of same mass at distance");
    failed += mu_run_test(test_prism2sphere_gyz,
                "prism_gyz results equal to sphere of same mass at distance");
    failed += mu_run_test(test_prism2sphere_gzz,
                "prism_gzz results equal to sphere of same mass at distance");
    failed += mu_run_test(test_prism_tensor_trace,
        "trace of GGT for prism in Cartesian coordinates is zero");
    failed += mu_run_test(test_prism_pot_sph,
            "prism_pot_sph results equal to prism_pot when on top of prism");
    failed += mu_run_test(test_prism_g_sph,
            "prism_g_sph results equal to prism_gx, etc, when on top of prism");
    failed += mu_run_test(test_prism_ggt_sph,
        "prism_ggt_sph results equal to prism_gxx, etc, when on top of prism");
    failed += mu_run_test(test_prism_tensor_sph_trace,
        "trace of GGT for prism in spherical coordinates is zero");
    failed += mu_run_test(test_global2local,
        "global2local returns correct result");
    return failed;
}
