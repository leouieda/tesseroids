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
***************************************************************************** */

/* *****************************************************************************
Unit tests for GLQ functions.

Author: Leonardo Uieda
Date: 24 Jan 2011
***************************************************************************** */

#include <stdio.h>
#include <math.h>
#include "minunit.h"
#include "../src/lib/utils.h"
#include "../src/lib/constants.h"

/* To store fail messages */
char msg[1000];

/* FIXTURES */
#define NTESSES 4
TESSEROID tesses[NTESSES] = {
    {0,0,1,0,1,6000000,6001000},
    {0,180,190,80,85,6300000,6301000},
    {0,160,200,-90,-70,5500000,6000000},
    {0,-10,5,-7,15,6500000,6505000}};

#define NPRISMS 4
PRISM prisms[NPRISMS] = {
    {0,0,1000,0,2000,100,2000},
    {0,-500,200,300,500,-1000,4000},
    {0,-10000000,5000000,5000000,8000000,0,3000000},
    {0,-1000000,50000,500000,800000,0,300000}};

/* UNIT TESTS */

static char * test_prism_volume()
{
    PRISM prisms[4] = {{0,0,1,0,1,0,1}, {0,0,2,0,1,0,1}, {0,0,2,0,2,0,2},
                       {0,1,2,-1,1,2,3}};
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
    TESSEROID local_tesses[4] = {{0,0,360,-90,90,0,1}, {0,0,360,0,90,0,1},
                                 {0,180,360,0,90,0,1}, {0,0,90,-90,0,0,1}};
    double tvolumes[4] = {4.188790205, 2.094395102, 1.047197551, 0.523598776};
    double res;
    int i;

    for(i = 0; i < 4; i++)
    {
        res = tess_volume(local_tesses[i]);
        sprintf(msg, "(tess %d) expected %g, got %g", i, tvolumes[i], res);
        mu_assert_almost_equals(res, tvolumes[i], pow(10, -8), msg);
    }

    return 0;
}


static char * test_tess_total_mass()
{
    TESSEROID local_tesses[4] = {{1,0,360,-90,90,0,1}, {1,0,360,0,90,0,1},
                                 {1,180,360,0,90,0,1}, {1,0,90,-90,0,0,1}};
    double tvolumes[4] = {4.188790205, 2.094395102, 1.047197551, 0.523598776};
    double res, expect;
    int i;

    res = tess_total_mass(local_tesses, 4);

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
    TESSEROID local_tesses[4] = {{1,0,360,-90,90,0,1}, {-1,0,360,0,90,0,1},
                                 {-1,180,360,0,90,0,1}, {1,0,90,-90,0,0,1}};
    double tvolumes[4] = {4.188790205, 2.094395102, 1.047197551, 0.523598776};
    double res, expect;

    res = tess_range_mass(local_tesses, 4, 0, 1);
    expect = tvolumes[0] + tvolumes[3];
    sprintf(msg, "Expected %g, got %g", expect, res);
    mu_assert_almost_equals(res, expect, pow(10, -6), msg);

    return 0;
}


static char * test_tess2prism()
{
    double expect, res;
    PRISM prism;
    int i;

    for(i = 0; i <  NTESSES; i++)
    {
        tess2prism(tesses[i], &prism);
        res = prism_volume(prism);
        expect = tess_volume(tesses[i]);
        sprintf(msg, "(tess %d) expected volume %g, got %g", i, expect, res);
        mu_assert_almost_equals(res/expect, 1., 0.01, msg);
    }
    
    return 0;
}


static char * test_tess2sphere()
{
    double expect, res;
    SPHERE sphere;
    int i;

    for(i = 0; i <  NTESSES; i++)
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

    for(i = 0; i <  NPRISMS; i++)
    {
        prism2sphere(prisms[i], 0., 0., 0., &sphere);
        res = sphere_volume(sphere);
        expect = prism_volume(prisms[i]);
        sprintf(msg, "(prism %d) expected volume %g, got %g", i, expect, res);
        mu_assert_almost_equals(res/expect, 1., 0.001, msg);
    }

    return 0;
}


static char * test_gets_tess()
{
    int i;
    char str[1000];
    TESSEROID res;

    for(i = 0; i < NTESSES; i++)
    {
        sprintf(str, "%g %g %g %g %g %g %g", tesses[i].w, tesses[i].e,
                tesses[i].s, tesses[i].n, tesses[i].r2 - MEAN_EARTH_RADIUS,
                tesses[i].r1 - MEAN_EARTH_RADIUS, tesses[i].density);

        gets_tess(str, &res);

        sprintf(msg, "(tess %d) failed to read w. read=%g true=%g", i, res.w,
                tesses[i].w);
        mu_assert(res.w == tesses[i].w, msg);
        sprintf(msg, "(tess %d) failed to read e. read=%g true=%g", i, res.e,
                tesses[i].e);
        mu_assert(res.e == tesses[i].e, msg);
        sprintf(msg, "(tess %d) failed to read s. read=%g true=%g", i, res.s,
                tesses[i].s);
        mu_assert(res.s == tesses[i].s, msg);
        sprintf(msg, "(tess %d) failed to read n. read=%g true=%g", i, res.n,
                tesses[i].n);
        mu_assert(res.n == tesses[i].n, msg);
        sprintf(msg, "(tess %d) failed to read r2. read=%g true=%g", i, res.r2,
                tesses[i].r2);
        mu_assert(res.r2 == tesses[i].r2, msg);
        sprintf(msg, "(tess %d) failed to read r1. read=%g true=%g", i, res.r1,
                tesses[i].r1);
        mu_assert(res.r1 == tesses[i].r1, msg);
        sprintf(msg, "(tess %d) failed to read density. read=%g true=%g", i,
                res.density, tesses[i].density);
        mu_assert(res.density == tesses[i].density, msg);
    }
    return 0;
}


static char * test_gets_prism()
{
    int i;
    char str[1000];
    PRISM res;

    for(i = 0; i < NPRISMS; i++)
    {
        sprintf(str, "%g %g %g %g %g %g %g", prisms[i].x1, prisms[i].x2,
                prisms[i].y1, prisms[i].y2, prisms[i].z1, prisms[i].z2,
                prisms[i].density);

        gets_prism(str, &res);

        sprintf(msg, "(prism %d) failed to read x1. read=%g true=%g", i, res.x1,
                prisms[i].x1);
        mu_assert(res.x1 == prisms[i].x1, msg);
        sprintf(msg, "(prism %d) failed to read x2. read=%g true=%g", i, res.x2,
                prisms[i].x2);
        mu_assert(res.x2 == prisms[i].x2, msg);
        sprintf(msg, "(prism %d) failed to read y1. read=%g true=%g", i, res.y1,
                prisms[i].y1);
        mu_assert(res.y1 == prisms[i].y1, msg);
        sprintf(msg, "(prism %d) failed to read y2. read=%g true=%g", i, res.y2,
                prisms[i].y2);
        mu_assert(res.y2 == prisms[i].y2, msg);
        sprintf(msg, "(prism %d) failed to read z1. read=%g true=%g", i, res.z1,
                prisms[i].z1);
        mu_assert(res.z1 == prisms[i].z1, msg);
        sprintf(msg, "(prism %d) failed to read z2. read=%g true=%g", i, res.z2,
                prisms[i].z2);
        mu_assert(res.z2 == prisms[i].z2, msg);
        sprintf(msg, "(prism %d) failed to read density. read=%g true=%g", i,
                res.density, prisms[i].density);
        mu_assert(res.density == prisms[i].density, msg);
    }
    return 0;
}


static char * test_split_tess()
{
    TESSEROID tess = {1, 2, 4, -1, 1, 5, 7},
              expect[] = {{1, 2, 3, -1, 0, 5, 6}, {1, 3, 4, -1, 0, 5, 6},
                        {1, 2, 3, 0, 1, 5, 6}, {1, 3, 4, 0, 1, 5, 6},
                        {1, 2, 3, -1, 0, 6, 7}, {1, 3, 4, -1, 0, 6, 7},
                        {1, 2, 3, 0, 1, 6, 7}, {1, 3, 4, 0, 1, 6, 7}},
              res[8];

    split_tess(tess, res);
    
    int i;
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


void utils_run_all()
{
    mu_run_test(test_prism_volume, "prism_volume return correct results");
    mu_run_test(test_tess_volume, "tess_volume return correct results");
    mu_run_test(test_tess_total_mass, "tess_total_mass returns correct result");
    mu_run_test(test_tess_range_mass, "tess_range_mass returns correct result");
    mu_run_test(test_tess2prism, "tess2prism produces prism with right volume");
    mu_run_test(test_tess2sphere,
                "tess2sphere produces sphere with right volume");
    mu_run_test(test_prism2sphere,
                "prism2sphere produces sphere with right volume");
    mu_run_test(test_gets_tess, "gets_tess reads correctly from string");
    mu_run_test(test_gets_prism, "gets_prism reads correctly from string");
    mu_run_test(test_split_tess, "split_tess returns correct results");
}
