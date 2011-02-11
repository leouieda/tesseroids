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

/** \file
Set of misc utilities and data structures.

Defines the TESSEROID, SPHERE and PRISM structures.

\todo Make functions that calculate the center of mass

@author Leonardo Uieda
@date 25 Jan 2011
*/


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "constants.h"
#include "logger.h"
#include "utils.h"


/* Change the coordinates of a point P from a global coordinate system to the
   local system of a point Q */
/*void chcoord_g2l(double lonp, double latp, double rp, double lonq, double latq,
                 double rq, double *xp, double *yp, double *zp)
{
    double d2r = PI/180.0, cartq[3], cartp[3];

    cartq[0] = rq*cos(d2r*latq)*cos(d2r*lonq);
    cartq[1] = rq*cos(d2r*latq)*sin(d2r*lonq);
    cartq[2] = rq*sin(d2r*latq);
    
    cartp[0] = rp*cos(d2r*latp)*cos(d2r*lonp);
    cartp[1] = rp*cos(d2r*latp)*sin(d2r*lonp);
    cartp[2] = rp*sin(d2r*latp);
    
    # Faz a matriz de tranformacao
    g2l = P2()*R2(latO - 90.)*R3(lonO - 180.)

    # Alinha os dois sistemas e desloca a origem para eO
    eL = g2l*(eG - eO)
}
*/


/* Split a tesseroid into 8. */
void split_tess(TESSEROID tess, TESSEROID *split)
{
    double dlon = 0.5*(tess.e - tess.w),
           dlat = 0.5*(tess.n - tess.s),
           dr = 0.5*(tess.r2 - tess.r1),
           ws[2], ss[2], r1s[2];

    ws[0] = tess.w;
    ws[1] = tess.w + dlon;
    ss[0] = tess.s;
    ss[1] = tess.s + dlat;
    r1s[0] = tess.r1;
    r1s[1] = tess.r1 + dr;

    int i, j, k, t = 0;

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


/* Calculate the total mass of a tesseroid model. */
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


/* Calculate the mass of a tesseroid model within a density range. */
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


/* Convert a tesseroid to a rectangular prism of equal volume. */
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
    /* r1 is not z1 because r1 is the bottom face */
    prism->z1 = MEAN_EARTH_RADIUS - tess.r2;
    prism->z2 = MEAN_EARTH_RADIUS - tess.r1;
    prism->density = tess.density;
}


/* Convert a tesseroid to a sphere of equal volume. */
void tess2sphere(TESSEROID tess, SPHERE *sphere)
{
    sphere->density = tess.density;
    /** \todo Put sphere in center of mass, not geometrical center */
    sphere->lonc = 0.5*(tess.e + tess.w);
    sphere->latc = 0.5*(tess.n + tess.s);
    sphere->rc = 0.5*(tess.r1 + tess.r2);
    sphere->r = cbrt(3*tess_volume(tess)/(4.*PI));
}


/* Convert a rectangular prism into a sphere of equal volume. */
void prism2sphere(PRISM prism, double lonc, double latc, double rc,
                  SPHERE *sphere)
{
    sphere->density = prism.density;
    sphere->lonc = lonc;
    sphere->latc = latc;
    sphere->rc = rc;
    sphere->r = cbrt(3*prism_volume(prism)/(4.*PI));
}


/* Calculate the volume of a tesseroid */
double tess_volume(TESSEROID tess)
{
    double d2r = PI/180., vol;

    vol = d2r*(tess.e - tess.w)*(pow(tess.r2, 3) - pow(tess.r1, 3))*
          (sin(d2r*tess.n) - sin(d2r*tess.s))/3.;

    return vol;
}


/* Calculate the volume of a sphere */
double sphere_volume(SPHERE sphere)
{
    return 4.*PI*pow(sphere.r, 3)/3.;
}


/* Calculate the volume of a prism */
double prism_volume(PRISM prism)
{
    return (prism.x2 - prism.x1)*(prism.y2 - prism.y1)*(prism.z2 - prism.z1);
}


/* Strip trailing spaces and newlines from the end of a string */
void strstrip(char *str)
{
    int i;
    for(i = strlen(str) - 1; i >= 0; i--)
    {
        if(str[i] != ' ' && str[i] != '\n' && str[i] != '\r' && str[i] != '\0')
            break;
    }
    str[i + 1] = '\0';
}


/* Read a single tesseroid from a string */
int gets_tess(const char *str, TESSEROID *tess)
{
    double w, e, s, n, top, bot, dens;
    int nread, nchars;
    
    nread = sscanf(str, "%lf %lf %lf %lf %lf %lf %lf%n", &w, &e, &s,
                    &n, &top, &bot, &dens, &nchars);

    if(nread != 7 || str[nchars] != '\0')
    {
        return 1;
    }

    tess->w = w;
    tess->e = e;
    tess->s = s;
    tess->n = n;
    tess->r1 = MEAN_EARTH_RADIUS + bot;
    tess->r2 = MEAN_EARTH_RADIUS + top;
    tess->density = dens;
    
    return 0;
}


/* Read tesseroids from an open file and store them in an array */
TESSEROID * read_tess_model(FILE *modelfile, int *size)
{
    TESSEROID *model;
    int buffsize = 100;

    /* Start with a single buffer allocation and expand later if necessary */
    model = (TESSEROID *)malloc(buffsize*sizeof(TESSEROID));
    if(model == NULL)
    {
        log_error("problem allocating initial memory to load tesseroid model");
        return NULL;
    }

    int nread, nchars, line, badinput = 0, error_exit = 0;
    char sbuff[10000];
    double w, e, s, n, top, bot, dens;
    TESSEROID *tmp;

    *size = 0;
    
    for(line = 1; !feof(modelfile); line++)
    {
        if(fgets(sbuff, 10000, modelfile) == NULL)
        {
            if(ferror(modelfile))
            {
                log_error("problem encountered reading line %d", line);
                error_exit = 1;
                break;
            }
        }
        else
        {
            /* Check for comments and blank lines */
            if(sbuff[0] == '#' || sbuff[0] == '\r' || sbuff[0] == '\n')
            {
                continue;
            }

            if(*size == buffsize)
            {
                buffsize += buffsize;
                tmp = (TESSEROID *)realloc(model, buffsize*sizeof(TESSEROID));
                if(tmp == NULL)
                {
                    /* Need to free because realloc leaves unchanged in case of
                       error */
                    free(model);
                    log_error("problem expanding memory for tesseroid model");
                    return NULL;
                }
                model = tmp;
            }

            /* Remove any trailing spaces or newlines */
            strstrip(sbuff);

            if(gets_tess(sbuff, &model[*size]))
            {
                log_warning("bad/invalid tesseroid at line %d", line);
                badinput = 1;
                continue;
            }
            
            (*size)++;
        }
    }

    if(badinput || error_exit)
    {
        free(model);
        return NULL;
    }

    /* Adjust the size of the model */
    tmp = (TESSEROID *)realloc(model, (*size)*sizeof(TESSEROID));
    if(tmp == NULL)
    {
        /* Need to free because realloc leaves unchanged in case of
            error */
        free(model);
        log_error("problem freeing excess memory for tesseroid model");
        return NULL;
    }
    model = tmp;
    
    return model;
}


/* Read a single rectangular prism from a string */
int gets_prism(const char *str, PRISM *prism)
{
    double x1, x2, y1, y2, z1, z2, dens;
    int nread, nchars;

    nread = sscanf(str, "%lf %lf %lf %lf %lf %lf %lf%n", &x1, &x2, &y1, &y2,
                   &z1, &z2, &dens, &nchars);
    /*
    if(nread != 7 || str[nchars] != '\0')
    {
        return 1;
    }*/
    /** \todo Read the position of the prism from the string 
    Not caring if there are more chars in the line so that can read output of tess2prism
    */
    if(nread != 7)
    {
        return 1;
    }

    prism->x1 = x1;
    prism->x2 = x2;
    prism->y1 = y1;
    prism->y2 = y2;
    prism->z1 = z1;
    prism->z2 = z2;
    prism->density = dens;

    return 0;
}


/* Read rectangular prisms from an open file and store them in an array. */
PRISM * read_prism_model(FILE *modelfile, int *size)
{
    PRISM *model;
    int buffsize = 100;

    /* Start with a single buffer allocation and expand later if necessary */
    model = (PRISM *)malloc(buffsize*sizeof(PRISM));
    if(model == NULL)
    {
        log_error("problem allocating initial memory to load prism model");
        return NULL;
    }

    int nread, nchars, line, badinput = 0, error_exit = 0;
    char sbuff[10000];
    double x1, x2, y1, y2, z1, z2, dens;
    PRISM *tmp;

    *size = 0;

    for(line = 1; !feof(modelfile); line++)
    {
        if(fgets(sbuff, 10000, modelfile) == NULL)
        {
            if(ferror(modelfile))
            {
                log_error("problem encountered reading line %d", line);
                error_exit = 1;
                break;
            }
        }
        else
        {
            /* Check for comments and blank lines */
            if(sbuff[0] == '#' || sbuff[0] == '\r' || sbuff[0] == '\n')
            {
                continue;
            }

            if(*size == buffsize)
            {
                buffsize += buffsize;
                tmp = (PRISM *)realloc(model, buffsize*sizeof(PRISM));
                if(tmp == NULL)
                {
                    /* Need to free because realloc leaves unchanged in case of
                       error */
                    free(model);
                    log_error("problem expanding memory for prism model");
                    return NULL;
                }
                model = tmp;
            }

            /* Remove any trailing spaces or newlines */
            strstrip(sbuff);

            if(gets_prism(sbuff, &model[*size]))
            {
                log_warning("bad/invalid prism at line %d", line);
                badinput = 1;
                continue;
            }

            (*size)++;
        }
    }

    if(badinput || error_exit)
    {
        free(model);
        return NULL;
    }

    /* Adjust the size of the model */
    tmp = (PRISM *)realloc(model, (*size)*sizeof(PRISM));
    if(tmp == NULL)
    {
        /* Need to free because realloc leaves unchanged in case of
            error */
        free(model);
        log_error("problem freeing excess memory for prism model");
        return NULL;
    }
    model = tmp;

    return model;
}