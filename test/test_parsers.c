/* 
Unit tests for I/O parser functions.
*/

#include <stdio.h>
#include <math.h>
#include "minunit.h"
#include "../src/lib/parsers.h"
#include "../src/lib/constants.h"

/* To store fail messages */
char msg[1000];


/* UNIT TESTS */

static char * test_gets_tess()
{
    int i;
    char str[1000];
    TESSEROID res;
    TESSEROID tesses[4] = {
        {1,0,1,0,1,6000000,6001000},
        {1,180,190,80,85,6300000,6301000},
        {1,160,200,-90,-70,5500000,6000000},
        {1,-10,5,-7,15,6500000,6505000}};

    for(i = 0; i < 4; i++)
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
    PRISM prisms[4] = {
        {1,0,1000,0,2000,100,2000,0,0,0},
        {1,-500,200,300,500,-1000,4000,0,0,0},
        {1,-10000000,5000000,5000000,8000000,0,3000000,0,0,0},
        {1,-1000000,50000,500000,800000,0,300000,0,0,0}};

    for(i = 0; i < 4; i++)
    {
        sprintf(str, "%g %g %g %g %g %g %g", prisms[i].x1, prisms[i].x2,
                prisms[i].y1, prisms[i].y2, prisms[i].z1, prisms[i].z2,
                prisms[i].density);

        if(gets_prism(str, &res))
        {
            sprintf(msg, "(prism %d) gets_prism returned 1", i);
            mu_assert(0, msg);
        }

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


static char * test_gets_prism_sph()
{
    int i;
    char str[1000];
    PRISM res;
    PRISM prisms[4] = {
        {1,-1000,1000,-2000,2000,0,2000,2,3,1},
        {1,-500,500,-500,500,0,4000,-3,1.2344,18.048},
        {1,-10000000,10000000,-8000000,8000000,0,3000000,2123.2,2,45.33},
        {1,-50000,50000,-800000,800000,0,300000,783.245,3.57,345}};

    for(i = 0; i < 4; i++)
    {
        
        sprintf(str, "%g %g %g %g %g %g %g",
                prisms[i].x2 - prisms[i].x1,
                prisms[i].y2 - prisms[i].y1, 
                prisms[i].z2 - prisms[i].z1,
                prisms[i].density,
                prisms[i].lon, prisms[i].lat, prisms[i].r);

        if(gets_prism_sph(str, &res))
        {
            sprintf(msg, "(prism %d) gets_prism_sph returned 1", i);
            mu_assert(0, msg);
        }

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
        sprintf(msg, "(prism %d) failed to read lon. read=%g true=%g", i,
                res.lon, prisms[i].lon);
        mu_assert(res.lon == prisms[i].lon, msg);
        sprintf(msg, "(prism %d) failed to read lat. read=%g true=%g", i,
                res.lat, prisms[i].lat);
        mu_assert(res.lat == prisms[i].lat, msg);
        sprintf(msg, "(prism %d) failed to read r. read=%g true=%g", i,
                res.r, prisms[i].r);
        mu_assert(res.r == prisms[i].r, msg);
    }
    return 0;
}


static char * test_gets_prism_fail()
{
    int i = 0, j;
    char str[1000];
    PRISM res;
    PRISM prisms[4] = {
        {1,0,1000,0,2000,100,2000,0,0,0},
        {1,-500,200,300,500,-1000,4000,0,0,0},
        {1,-10000000,5000000,5000000,8000000,0,3000000,0,0,0},
        {1,-1000000,50000,500000,800000,0,300000,0,0,0}};

    j = 1;
    sprintf(str, "%g %g %g %g %g %g %g 1", prisms[i].x1, prisms[i].x2,
            prisms[i].y1, prisms[i].y2, prisms[i].z1, prisms[i].z2,
            prisms[i].density);
    sprintf(msg, "(test %d) gets_prism did not fail for bad input", j);
    mu_assert(gets_prism(str, &res), msg);
    
    j = 2;
    sprintf(str, "%g %g %g %g %g %g %g 1.3", prisms[i].x1, prisms[i].x2,
            prisms[i].y1, prisms[i].y2, prisms[i].z1, prisms[i].z2,
            prisms[i].density);
    sprintf(msg, "(test %d) gets_prism did not fail for bad input", j);
    mu_assert(gets_prism(str, &res), msg);
    
    j = 3;
    sprintf(str, "%g %g %g %g %g %g %g meh", prisms[i].x1, prisms[i].x2,
            prisms[i].y1, prisms[i].y2, prisms[i].z1, prisms[i].z2,
            prisms[i].density);
    sprintf(msg, "(test %d) gets_prism did not fail for bad input", j);
    mu_assert(gets_prism(str, &res), msg);
    
    j = 4;
    sprintf(str, "%g %g %g %g %g %g %g 1 4.5 234556 blablabla",
            prisms[i].x1, prisms[i].x2,
            prisms[i].y1, prisms[i].y2, prisms[i].z1, prisms[i].z2,
            prisms[i].density);
    sprintf(msg, "(test %d) gets_prism did not fail for bad input", j);
    mu_assert(gets_prism(str, &res), msg);
    
    return 0;
}


void parsers_run_all()
{
    mu_run_test(test_gets_tess, "gets_tess reads correctly from string");
    mu_run_test(test_gets_prism, "gets_prism reads correctly from string");
    mu_run_test(test_gets_prism_sph,
                "gets_prism_sph reads correctly from string");
    mu_run_test(test_gets_prism_fail, "gets_prism fails for bad input");
}
