/*
Unit tests for geometry module.
*/

#include <stdio.h>
#include <math.h>
#include "minunit.h"
#include "../src/geometry.h"
#include "../src/constants.h"

/* To store fail messages */
char msg[1000];


static char * test_split_tess()
{
    TESSEROID tess = {1, 2, 4, -1, 1, 5, 7},
              expect[] = {{1, 2, 3, -1, 0, 5, 6}, {1, 3, 4, -1, 0, 5, 6},
                        {1, 2, 3, 0, 1, 5, 6}, {1, 3, 4, 0, 1, 5, 6},
                        {1, 2, 3, -1, 0, 6, 7}, {1, 3, 4, -1, 0, 6, 7},
                        {1, 2, 3, 0, 1, 6, 7}, {1, 3, 4, 0, 1, 6, 7}},
              res[8];
    int i;

    split_tess(tess, res);
    for(i = 0; i < 8; i++)
    {
        sprintf(msg, "failed for split %d: %g %g %g %g %g %g %g", i, res[i].w,
                res[i].e, res[i].s, res[i].n, res[i].r1, res[i].r2,
                res[i].density);
        mu_assert(res[i].w == expect[i].w && res[i].e == expect[i].e &&
                  res[i].s == expect[i].s && res[i].n == expect[i].n &&
                  res[i].r1 == expect[i].r1 && res[i].r2 == expect[i].r2 &&
                  res[i].density == expect[i].density, msg);
    }
    return 0;
}


static char * test_prism_volume()
{
    PRISM prisms[4] = {
        {0,0,1,0,1,0,1,0,0,0},
        {0,0,2,0,1,0,1,0,0,0},
        {0,0,2,0,2,0,2,0,0,0},
        {0,1,2,-1,1,2,3,0,0,0}};
    double pvolumes[4] = {1, 2, 8, 2};
    double res;
    int i;

    for(i = 0; i < 4; i++)
    {
        res = prism_volume(prisms[i]);
        sprintf(msg, "(prism %d) expected %g, got %g", i, pvolumes[i], res);
        mu_assert(res == pvolumes[i], msg);
    }

    return 0;
}


static char * test_tess_volume()
{
    TESSEROID tesses[4] = {{0,0,360,-90,90,0,1}, {0,0,360,0,90,0,1},
                                 {0,180,360,0,90,0,1}, {0,0,90,-90,0,0,1}};
    double tvolumes[4] = {4.188790205, 2.094395102, 1.047197551, 0.523598776};
    double res;
    int i;

    for(i = 0; i < 4; i++)
    {
        res = tess_volume(tesses[i]);
        sprintf(msg, "(tess %d) expected %g, got %g", i, tvolumes[i], res);
        mu_assert_almost_equals(res, tvolumes[i], pow(10, -8), msg);
    }

    return 0;
}


static char * test_tess_total_mass()
{
    TESSEROID tesses[4] = {{1,0,360,-90,90,0,1}, {1,0,360,0,90,0,1},
                                 {1,180,360,0,90,0,1}, {1,0,90,-90,0,0,1}};
    double tvolumes[4] = {4.188790205, 2.094395102, 1.047197551, 0.523598776};
    double res, expect;
    int i;

    res = tess_total_mass(tesses, 4);

    for(expect = 0, i = 0; i < 4; i++)
    {
        expect += tvolumes[i];
    }

    sprintf(msg, "(tess %d) expected %g, got %g", i, expect, res);
    mu_assert_almost_equals(res, expect, pow(10, -6), msg);

    return 0;
}


static char * test_tess_range_mass()
{
    TESSEROID tesses[4] = {{1,0,360,-90,90,0,1}, {-1,0,360,0,90,0,1},
                                 {-1,180,360,0,90,0,1}, {1,0,90,-90,0,0,1}};
    double tvolumes[4] = {4.188790205, 2.094395102, 1.047197551, 0.523598776};
    double res, expect;

    res = tess_range_mass(tesses, 4, 0, 1);
    expect = tvolumes[0] + tvolumes[3];
    sprintf(msg, "Expected %g, got %g", expect, res);
    mu_assert_almost_equals(res, expect, pow(10, -6), msg);

    return 0;
}


static char * test_tess2prism()
{
    int i;
    double expect, res;
    double lons[4] = {0.5, 185, 180, -2.5},
           lats[4] = {0.5, 82.5, -80, 4},
           rs[4] = {6001000, 6301000, 6000000, 6505000},
           zs[4] = {1000, 1000, 500000, 5000};
    PRISM prism;
    TESSEROID tesses[4] = {
        {1,0,1,0,1,6000000,6001000},
        {1,180,190,80,85,6300000,6301000},
        {1,160,200,-90,-70,5500000,6000000},
        {1,-10,5,-7,15,6500000,6505000}};

    for(i = 0; i <  4; i++)
    {
        tess2prism(tesses[i], &prism);
        /* check the volume */
        res = prism_volume(prism);
        expect = tess_volume(tesses[i]);
        sprintf(msg, "(tess %d) expected volume %g, got %g", i, expect, res);
        mu_assert_almost_equals((double)(res - expect)/expect, 0., 0.01, msg);
        /* check the mass */
        res *= prism.density;
        expect *= tesses[i].density;
        sprintf(msg, "(tess %d) expected mass %g, got %g", i, expect, res);
        mu_assert_almost_equals((double)(res - expect)/expect, 0., 0.01, msg);
        /* check the coordinates */
        res = prism.lon;
        expect = lons[i];
        sprintf(msg, "(tess %d) expected lon %g, got %g", i, expect, res);
        mu_assert_almost_equals((double)(res - expect)/expect, 0., 0.0001, msg);
        res = prism.lat;
        expect = lats[i];
        sprintf(msg, "(tess %d) expected lat %g, got %g", i, expect, res);
        mu_assert_almost_equals((double)(res - expect)/expect, 0., 0.0001, msg);
        res = prism.r;
        expect = rs[i];
        sprintf(msg, "(tess %d) expected r %g, got %g", i, expect, res);
        mu_assert_almost_equals((double)(res - expect)/expect, 0., 0.0001, msg);
        res = prism.z1;
        expect = 0.;
        sprintf(msg, "(tess %d) expected z1 %g, got %g", i, expect, res);
        mu_assert_almost_equals(res, 0., 0.000000000000001, msg);
        res = prism.z2;
        expect = zs[i];
        sprintf(msg, "(tess %d) expected z2 %g, got %g", i, expect, res);
        mu_assert_almost_equals((double)(res - expect)/expect, 0., 0.0001, msg);
    }

    return 0;
}


static char * test_tess2prism_flatten()
{
    double expect, res;
    PRISM prism;
    int i;
    TESSEROID tesses[4] = {
        {1,0,1,0,1,6000000,6001000},
        {1,180,190,80,85,6300000,6301000},
        {1,160,200,-90,-70,5500000,6000000},
        {1,-10,5,-7,15,6500000,6505000}};

    for(i = 0; i <  4; i++)
    {
        tess2prism_flatten(tesses[i], &prism);
        res = prism_volume(prism)*prism.density;
        expect = tess_volume(tesses[i])*tesses[i].density;
        sprintf(msg, "(tess %d) expected mass %g, got %g", i, expect, res);
        mu_assert_almost_equals((double)(res - expect)/expect, 0., 0.01, msg);
    }

    return 0;
}


static char * test_tess2sphere()
{
    double expect, res;
    SPHERE sphere;
    int i;
    TESSEROID tesses[4] = {
        {1,0,1,0,1,6000000,6001000},
        {1,180,190,80,85,6300000,6301000},
        {1,160,200,-90,-70,5500000,6000000},
        {1,-10,5,-7,15,6500000,6505000}};

    for(i = 0; i <  4; i++)
    {
        tess2sphere(tesses[i], &sphere);
        res = sphere_volume(sphere);
        expect = tess_volume(tesses[i]);
        sprintf(msg, "(tess %d) expected volume %g, got %g", i, expect, res);
        mu_assert_almost_equals(res/expect, 1., 0.01, msg);
    }

    return 0;
}


static char * test_prism2sphere()
{
    double expect, res;
    SPHERE sphere;
    int i;
    PRISM prisms[4] = {
        {1,0,1000,0,2000,100,2000,0,0,0},
        {1,-500,200,300,500,-1000,4000,0,0,0},
        {1,-10000000,5000000,5000000,8000000,0,3000000,0,0,0},
        {1,-1000000,50000,500000,800000,0,300000,0,0,0}};

    for(i = 0; i <  4; i++)
    {
        prism2sphere(prisms[i], 0., 0., 0., &sphere);
        res = sphere_volume(sphere);
        expect = prism_volume(prisms[i]);
        sprintf(msg, "(prism %d) expected volume %g, got %g", i, expect, res);
        mu_assert_almost_equals(res/expect, 1., 0.001, msg);
    }

    return 0;
}


int geometry_run_all()
{
    int failed = 0;
    failed += mu_run_test(test_prism_volume, "prism_volume return correct results");
    failed += mu_run_test(test_tess_volume, "tess_volume return correct results");
    failed += mu_run_test(test_tess_total_mass, "tess_total_mass returns correct result");
    failed += mu_run_test(test_tess_range_mass, "tess_range_mass returns correct result");
    failed += mu_run_test(test_tess2prism, "tess2prism produces prism with right volume");
    failed += mu_run_test(test_tess2prism_flatten,
                "tess2prism_flatten produces prism with right mass");
    failed += mu_run_test(test_tess2sphere,
                "tess2sphere produces sphere with right volume");
    failed += mu_run_test(test_prism2sphere,
                "prism2sphere produces sphere with right volume");
    failed += mu_run_test(test_split_tess, "split_tess returns correct results");
    return failed;
}
