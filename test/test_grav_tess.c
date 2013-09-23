/*
Unit tests for grav_tess.c functions.
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../src/lib/grav_sphere.h"
#include "../src/lib/grav_tess.h"
#include "../src/lib/glq.h"
#include "../src/lib/geometry.h"
#include "../src/lib/constants.h"


char msg[1000];


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
        restess = tess_pot(tess,0,40,radius+dist,*glqlon,*glqlat,*glqr);
        ressphere = sphere_pot(sphere,0,40,radius+dist);
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
        restess = tess_gx(tess,0,40,radius+dist,*glqlon,*glqlat,*glqr);
        ressphere = sphere_gx(sphere,0,40,radius+dist);
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
        restess = tess_gy(tess,5,45,radius+dist,*glqlon,*glqlat,*glqr);
        ressphere = sphere_gy(sphere,5,45,radius+dist);

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
        restess = -tess_gz(tess,0,45,radius+dist,*glqlon,*glqlat,*glqr);
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
    failed += mu_run_test(test_tess_tensor_trace, "trace of GGT for tesseroid is zero");
    failed += mu_run_test(test_adaptative,
            "calc_tess_model_adapt results as non-adapt with split by hand");
    return failed;
}
