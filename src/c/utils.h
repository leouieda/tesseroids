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
This module contains a set of misc utilities. Also defines the TESSEROID, SPHERE
and PRISM structures.

Author: Leonardo Uieda
Date: 25 Jan 2011
***************************************************************************** */

#ifndef _UTILS_H_
#define _UTILS_H_


typedef struct tess_struct {
    /* s, n, w, e in degrees. r1 and r2 are the smaller and larger radius */
    double density, s, n, w, e, r1, r2;
} TESSEROID;


typedef struct prism_struct {
    double density, x1, x2, y1, y2, z1, z2;
    /* Geodetic coordinates of the center of the top face of the prism */
    /* double lonc, latc, rc;*/
} PRISM;


typedef struct sphere_struct {
    /* lonc, latc, rc are spherical coordinates of the center of the sphere */
    double density, r, lonc, latc, rc;
} SPHERE;


/* Convert a tesseroid into a rectangular prism of equal volume.

Parameters:
    * TESSEROID tess: data structure with the information on the tesseroid
    * PRISM *prism: pointer to the prism structure to return the converted
        tesseroid
*/
extern void tess2prism(TESSEROID tess, PRISM *prism);


/* Convert a tesseroid into a sphere of equal volume.

Parameters:
    * TESSEROID tess: data structure with the information on the tesseroid
    * PRISM *prism: pointer to the prism structure to return the converted
        tesseroid
*/
extern void tess2sphere(TESSEROID tess, SPHERE *sphere);


/* Calculate the volume of a tesseroid.

Parameters:
    * TESSEROID tess: the tesseroid whose volume will be calculated

Returns:
    the volume in the respective units
*/
extern double tess_volume(TESSEROID tess);


/* Calculate the volume of a sphere.

Parameters:
    * TESSEROID tess: the tesseroid whose volume will be calculated

Returns:
    the volume in the respective units
*/
extern double sphere_volume(SPHERE sphere);


/* Calculate the volume of a prism

Parameters:
    * PRISM prism: the prism whose volume will be calculated

Returns:
    the volume in the respective units
*/
extern double prism_volume(PRISM prism);

#endif