/*
Data structures for geometric elements and functions that operate on them.
Defines the TESSEROID, SPHERE, and PRISM structures.
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "constants.h"
#include "logger.h"
#include "geometry.h"


void split_tess(TESSEROID tess, TESSEROID *split)
{
    double dlon = 0.5*(tess.e - tess.w),
           dlat = 0.5*(tess.n - tess.s),
           dr = 0.5*(tess.r2 - tess.r1),
           ws[2], ss[2], r1s[2];
    int i, j, k, t = 0;

    ws[0] = tess.w;
    ws[1] = tess.w + dlon;
    ss[0] = tess.s;
    ss[1] = tess.s + dlat;
    r1s[0] = tess.r1;
    r1s[1] = tess.r1 + dr;
    for(k = 0; k < 2; k++)
    {
        for(j = 0; j < 2; j++)
        {
            for(i = 0; i < 2; i++)
            {
                split[t].w = ws[i];
                split[t].e = ws[i] + dlon;
                split[t].s = ss[j];
                split[t].n = ss[j] + dlat;
                split[t].r1 = r1s[k];
                split[t].r2 = r1s[k] + dr;
                split[t].density = tess.density;
                t++;
            }
        }
    }
}

double tess_total_mass(TESSEROID *model, int size)
{
    double mass;
    int i;

    for(mass = 0, i = 0; i < size; i++)
    {
        mass += model[i].density*tess_volume(model[i]);
    }

    return mass;
}

double tess_range_mass(TESSEROID *model, int size, double low_dens,
                       double high_dens)
{
    double mass;
    int i;

    for(mass = 0, i = 0; i < size; i++)
    {
        if(model[i].density >= low_dens && model[i].density <= high_dens)
        {
            mass += model[i].density*tess_volume(model[i]);
        }
    }

    return mass;
}

void tess2prism(TESSEROID tess, PRISM *prism)
{
    double deg2rad = PI/180., r0, dx, dy;

    r0 = 0.5*(tess.r1 + tess.r2);
    dx = r0*deg2rad*(tess.n - tess.s);
    dy = r0*cos(deg2rad*0.5*(tess.n + tess.s))*deg2rad*(tess.e - tess.w);
    prism->x1 = -0.5*dx;
    prism->x2 = 0.5*dx;
    prism->y1 = -0.5*dy;
    prism->y2 = 0.5*dy;
    /* z1 = 0 because the center of the top face of the prism is the origin of
    the coordiante system */
    prism->z1 = 0.;
    prism->z2 = tess.r2 - tess.r1;
    /* Calculate the density of the prism so that they will have exactly
       the same mass */
    prism->density = (double)tess.density*
                             tess_volume(tess)/prism_volume(*prism);
    /* Set the coordinates of the center of the prisms top face */
    prism->lon = 0.5*(tess.e + tess.w);
    prism->lat = 0.5*(tess.n + tess.s);
    prism->r = tess.r2; /* The top face */
}

void tess2prism_flatten(TESSEROID tess, PRISM *prism)
{
    prism->x1 = tess.s*111110.;
    prism->x2 = tess.n*111110.;
    prism->y1 = tess.w*111110.;
    prism->y2 = tess.e*111110.;
    /* r1 is not z1 because r1 is the bottom face (because Nagy et al., 2000,
    use z->Down) */
    prism->z1 = MEAN_EARTH_RADIUS - tess.r2;
    prism->z2 = MEAN_EARTH_RADIUS - tess.r1;
    /* Calculate the density of the prism so that they will have exactly
       the same mass */
    prism->density = (double)tess.density*
                             tess_volume(tess)/prism_volume(*prism);
}

void tess2sphere(TESSEROID tess, SPHERE *sphere)
{
    sphere->density = tess.density;
    sphere->lonc = 0.5*(tess.e + tess.w);
    sphere->latc = 0.5*(tess.n + tess.s);
    sphere->rc = 0.5*(tess.r1 + tess.r2);
    sphere->r = pow(3*tess_volume(tess)/(4.*PI), (double)1./3.);
}

void prism2sphere(PRISM prism, double lonc, double latc, double rc,
                  SPHERE *sphere)
{
    sphere->density = prism.density;
    sphere->lonc = lonc;
    sphere->latc = latc;
    sphere->rc = rc;
    sphere->r = pow(3*prism_volume(prism)/(4.*PI), (double)1./3.);
}

double tess_volume(TESSEROID tess)
{
    double d2r = PI/180., vol;

    vol = d2r*(tess.e - tess.w)*(pow(tess.r2, 3) - pow(tess.r1, 3))*
          (sin(d2r*tess.n) - sin(d2r*tess.s))/3.;

    return vol;
}

double sphere_volume(SPHERE sphere)
{
    return 4.*PI*pow(sphere.r, 3)/3.;
}

double prism_volume(PRISM prism)
{
    return (prism.x2 - prism.x1)*(prism.y2 - prism.y1)*(prism.z2 - prism.z1);
}
