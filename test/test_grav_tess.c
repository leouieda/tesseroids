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
Unit tests for grav_tess.c functions.

Author: Leonardo Uieda
Date: 24 Jan 2011
***************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../src/c/grav_sphere.h"
#include "../src/c/grav_tess.h"
#include "../src/c/glq.h"
#include "../src/c/utils.h"
#include "../src/c/constants.h"


char msg[1000];


static char * test_tess2sphere_pot()
{
    SPHERE sphere;
    TESSEROID tess = {1000,44,46,-1,1,MEAN_EARTH_RADIUS-100000,MEAN_EARTH_RADIUS};
    double radius, dist, restess, ressphere;
    GLQ *glqlon, *glqlat, *glqr;

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

        sprintf(msg, "(distance %g m) tess = %.5lf  sphere = %.5lf", dist,
                restess, ressphere);
        mu_assert_almost_equals(restess, ressphere, 0.01, msg);
    }

    return 0;
}


static char * test_tess2sphere_gx()
{
    SPHERE sphere;
    TESSEROID tess = {1000,44,46,-1,1,MEAN_EARTH_RADIUS-100000,MEAN_EARTH_RADIUS};
    double radius, dist, restess, ressphere;
    GLQ *glqlon, *glqlat, *glqr;

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

        sprintf(msg, "(distance %g m) tess = %.5lf  sphere = %.5lf", dist,
                restess, ressphere);
        mu_assert_almost_equals(restess, ressphere, 0.1, msg);
    }
    
    return 0;
}


static char * test_tess2sphere_gy()
{
    SPHERE sphere;
    TESSEROID tess = {1000,44,46,-1,1,MEAN_EARTH_RADIUS-100000,MEAN_EARTH_RADIUS};
    double radius, dist, restess, ressphere;
    GLQ *glqlon, *glqlat, *glqr;

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

        sprintf(msg, "(distance %g m) tess = %.5lf  sphere = %.5lf", dist,
                restess, ressphere);
        mu_assert_almost_equals(restess, ressphere, 0.1, msg);
    }

    return 0;
}


static char * test_tess2sphere_gz()
{
    SPHERE sphere;
    TESSEROID tess = {1000,44,46,-1,1,MEAN_EARTH_RADIUS-100000,MEAN_EARTH_RADIUS};
    double radius, dist, restess, ressphere;
    GLQ *glqlon, *glqlat, *glqr;

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

        sprintf(msg, "(distance %g m) tess = %.5lf  sphere = %.5lf", dist,
                restess, ressphere);
        mu_assert_almost_equals(restess, ressphere, 0.1, msg);
    }

    return 0;
}


static char * test_tess2sphere_gxx()
{
    SPHERE sphere;
    TESSEROID tess = {1000,44,46,-1,1,MEAN_EARTH_RADIUS-100000,MEAN_EARTH_RADIUS};
    double radius, dist, restess, ressphere;
    GLQ *glqlon, *glqlat, *glqr;

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

        sprintf(msg, "(distance %g m) tess = %.5lf  sphere = %.5lf", dist,
                restess, ressphere);
        mu_assert_almost_equals(restess, ressphere, 0.001, msg);
    }

    return 0;
}


static char * test_tess2sphere_gxy()
{
    SPHERE sphere;
    TESSEROID tess = {1000,44,46,-1,1,MEAN_EARTH_RADIUS-100000,MEAN_EARTH_RADIUS};
    double radius, dist, restess, ressphere;
    GLQ *glqlon, *glqlat, *glqr;

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

        sprintf(msg, "(distance %g m) tess = %.5lf  sphere = %.5lf", dist,
                restess, ressphere);
        mu_assert_almost_equals(restess, ressphere, 0.001, msg);
    }

    return 0;
}


static char * test_tess2sphere_gxz()
{
    SPHERE sphere;
    TESSEROID tess = {1000,44,46,-1,1,MEAN_EARTH_RADIUS-100000,MEAN_EARTH_RADIUS};
    double radius, dist, restess, ressphere;
    GLQ *glqlon, *glqlat, *glqr;

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

        sprintf(msg, "(distance %g m) tess = %.5lf  sphere = %.5lf", dist,
                restess, ressphere);
        mu_assert_almost_equals(restess, ressphere, 0.001, msg);
    }

    return 0;
}


static char * test_tess2sphere_gyy()
{
    SPHERE sphere;
    TESSEROID tess = {1000,44,46,-1,1,MEAN_EARTH_RADIUS-100000,MEAN_EARTH_RADIUS};
    double radius, dist, restess, ressphere;
    GLQ *glqlon, *glqlat, *glqr;

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

        sprintf(msg, "(distance %g m) tess = %.5lf  sphere = %.5lf", dist,
                restess, ressphere);
        mu_assert_almost_equals(restess, ressphere, 0.001, msg);
    }

    return 0;
}


static char * test_tess2sphere_gyz()
{
    SPHERE sphere;
    TESSEROID tess = {1000,44,46,-1,1,MEAN_EARTH_RADIUS-100000,MEAN_EARTH_RADIUS};
    double radius, dist, restess, ressphere;
    GLQ *glqlon, *glqlat, *glqr;

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

        sprintf(msg, "(distance %g m) tess = %.5lf  sphere = %.5lf", dist,
                restess, ressphere);
        mu_assert_almost_equals(restess, ressphere, 0.001, msg);
    }

    return 0;
}


static char * test_tess2sphere_gzz()
{
    SPHERE sphere;
    TESSEROID tess = {1000,44,46,-1,1,MEAN_EARTH_RADIUS-100000,MEAN_EARTH_RADIUS};
    double radius, dist, restess, ressphere;
    GLQ *glqlon, *glqlat, *glqr;

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

        sprintf(msg, "(distance %g m) tess = %.5lf  sphere = %.5lf", dist,
                restess, ressphere);
        mu_assert_almost_equals(restess, ressphere, 0.001, msg);
    }

    return 0;
}


static char * test_adaptative()
{
    /* Check if the adaptative is dividing properly and returning the same thing
       as the non-adaptative (do spliting by hand) */
    TESSEROID tess = {1000.,-0.5,0.5,-0.5,0.5,MEAN_EARTH_RADIUS-10000,MEAN_EARTH_RADIUS},
              split[8];
    GLQ *glqlon, *glqlat, *glqr;
    double mindist, resadapt, resnormal;

    glqlon = glq_new(2, tess.w, tess.e);
    if(glqlon == NULL)
        mu_assert(0, "GLQ allocation error");

    glqlat = glq_new(2, tess.s, tess.n);
    if(glqlat == NULL)
        mu_assert(0, "GLQ allocation error");

    glqr = glq_new(2, tess.r1, tess.r2);
    if(glqr == NULL)
        mu_assert(0, "GLQ allocation error");

    mindist = TESSEROID_SIZE_RATIO*111110.*(tess.e - tess.w);

    /* If at half mindist should only divide once */
    resadapt = calc_tess_model_adapt(&tess, 1, 0, 0,
                                     0.5*mindist + MEAN_EARTH_RADIUS, glqlon,
                                     glqlat, glqr, tess_gzz);

    split_tess(tess, split);
    resnormal = calc_tess_model(split, 8, 0, 0,
                                0.5*mindist + MEAN_EARTH_RADIUS, glqlon,
                                glqlat, glqr, tess_gzz);

    sprintf(msg, "adapt = %.10lf  normal = %.10lf", resadapt, resnormal);
    mu_assert_almost_equals(resadapt, resnormal, pow(10, -10), msg);

    return 0;
}


void grav_tess_run_all()
{
    mu_run_test(test_tess2sphere_pot,
                "tess_pot results equal to sphere of same mass at distance");
    mu_run_test(test_tess2sphere_gx,
                "tess_gx results equal to sphere of same mass at distance");
    mu_run_test(test_tess2sphere_gy,
                "tess_gy results equal to sphere of same mass at distance");
    mu_run_test(test_tess2sphere_gz,
                "tess_gz results equal to sphere of same mass at distance");
    mu_run_test(test_tess2sphere_gxx,
                "tess_gxx results equal to sphere of same mass at distance");
    mu_run_test(test_tess2sphere_gxy,
                "tess_gxy results equal to sphere of same mass at distance");
    mu_run_test(test_tess2sphere_gxz,
                "tess_gxz results equal to sphere of same mass at distance");
    mu_run_test(test_tess2sphere_gyy,
                "tess_gyy results equal to sphere of same mass at distance");
    mu_run_test(test_tess2sphere_gyz,
                "tess_gyz results equal to sphere of same mass at distance");
    mu_run_test(test_tess2sphere_gzz,
                "tess_gzz results equal to sphere of same mass at distance");
    mu_run_test(test_adaptative,
            "calc_tess_model_adapt results as non-adapt with split by hand");
}