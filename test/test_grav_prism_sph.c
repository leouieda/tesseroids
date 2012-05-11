/* 
Unit tests for grav_prism.c functions.
*/

#include <stdio.h>
#include <math.h>
#include "../src/lib/grav_prism_sph.h"
#include "../src/lib/grav_prism.h"
#include "../src/lib/geometry.h"
#include "../src/lib/constants.h"


char msg[1000];


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
        mu_assert_almost_equals(x, 0., 0.000000001, msg);
        sprintf(msg, "(prism %d) y: expect %.10g got %.10g", i, 0., y);
        mu_assert_almost_equals(y, 0., 0.000000001, msg);
        sprintf(msg, "(prism %d) z: expect %.10g got %.10g", i, newz[i], z);
        mu_assert_almost_equals(z, newz[i], 0.000000001, msg);        
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
        sprintf(msg, "(fixture %d) cmp %d: expect %.10g got %.10g", fix, i,
            expect[i], res[i]);
        mu_assert_almost_equals(res[i], expect[i], 0.0000000001, msg);
    }
    #undef R
    return 0;
}



void grav_prism_sph_run_all()
{
    mu_run_test(test_prism_pot_sph,
            "prism_pot_sph results equal to prism_pot when on top of prism");
    mu_run_test(test_prism_g_sph,
            "prism_g_sph results equal to prism_gx, etc, when on top of prism");
    mu_run_test(test_prism_ggt_sph,
        "prism_ggt_sph results equal to prism_gxx, etc, when on top of prism");
    mu_run_test(test_global2local,
        "global2local returns correct result");
}
