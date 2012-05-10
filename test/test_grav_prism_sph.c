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
}
