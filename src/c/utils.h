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
\brief Set of misc utilities and data structures.

Defines the TESSEROID, SPHERE and PRISM structures.

@author Leonardo Uieda
@date 25 Jan 2011
*/

#ifndef _TESSEROIDS_UTILS_H_
#define _TESSEROIDS_UTILS_H_


/* Need for the definition of FILE */
#include <stdio.h>


/** Store information on a tesseroid */
typedef struct tess_struct {
    /* s, n, w, e in degrees. r1 and r2 are the smaller and larger radius */
    double density; /**< in SI units */
    double s; /**< southern latitude border in degrees */
    double n; /**< northern latitude border in degrees */
    double w; /**< western longitude border in degrees */
    double e; /**< eastern longitude border in degrees */
    double r1; /**< smallest radius border in SI units */
    double r2; /**< largest radius border in SI units */
} TESSEROID;


/** Store information on a rectangular prism */
typedef struct prism_struct {
    double density; /**< in SI units */
    double x1; /**< in SI units */
    double x2; /**< in SI units */
    double y1; /**< in SI units */
    double y2; /**< in SI units */
    double z1; /**< in SI units */
    double z2; /**< in SI units */
    /* Geodetic coordinates of the center of the top face of the prism */
    /* double lonc, latc, rc;*/
} PRISM;


/** Store information on a sphere */
typedef struct sphere_struct {
    double density; /**< in SI units */
    double r; /**< radius of the sphere in SI units */
    double lonc; /**< longitude of the center of the sphere in degrees */
    double latc; /**< latitude of the center of the sphere in degrees */
    double rc; /**< radial coordinate of the center of the sphere in SI units */
} SPHERE;


/** Change the coordinates of a point P from a global coordinate system to the
    local system of a point Q

Coordinates given in SI and degrees!

Uses constant MEAN_EARTH_RADIUS.

\image latex coordinate_sys.pdf "Points P and Q in the global coordinate system and their local systems"
\image html coordinate_sys.png "Points P and Q in the global coordinate system and their local systems"

@param lonp longitude of point P
@param latp latitude of point P
@param rp radial coordinate of point P
@param lonq longitude of point Q (origin of the local system)
@param latq latitude of point Q (origin of the local system)
@param rq radial coordinate of point Q (origin of the local system)
@param xp x coordinate of P in the local system of Q
@param yp y coordinate of P in the local system of Q
@param zp z coordinate of P in the local system of Q
*/
extern void chcoord_g2l(double lonp, double latp, double rp, double lonq,
                    double latq, double rq, double *xp, double *yp, double *zp);


/** Convert a tesseroid into a rectangular prism of equal volume.

@param tess tesseroid to convert
@param prism prism with equal volume of the tesseroid (used to return)
*/
extern void tess2prism(TESSEROID tess, PRISM *prism);


/** Convert a tesseroid into a sphere of equal volume.

Parameters:
@param tess tesseroid to convert
@param sphere sphere with equal volume of the tesseroid (used to return)
*/
extern void tess2sphere(TESSEROID tess, SPHERE *sphere);


/** Convert a rectangular prism into a sphere of equal volume.

Parameters:
@param prism prism to convert
@param lonc longitude of the desired center of the sphere, in degrees
@param latc latitude of the desired center of the sphere, in degrees
@param rc radial coordinate of the desired center of the sphere, in SI units
@param sphere sphere with equal volume of the prism (used to return)
*/
extern void prism2sphere(PRISM prism, double lonc, double latc, double rc,
                         SPHERE *sphere);


/** Calculate the volume of a tesseroid.

@param tess the tesseroid whose volume will be calculated

@return the volume in the respective units
*/
extern double tess_volume(TESSEROID tess);


/** Calculate the volume of a sphere.

@param sphere the sphere whose volume will be calculated

@return the volume in the respective units
*/
extern double sphere_volume(SPHERE sphere);


/** Calculate the volume of a prism

@param prism the prism whose volume will be calculated

@return the volume in the respective units
*/
extern double prism_volume(PRISM prism);


/** Strip trailing spaces and newlines from the end of a string

Done IN PLACE!

@param str string to strip
*/
extern void strstrip(char *str);


/** Read a single tesseroid from a string

@param str string with the tesseroid parameters
@param tess used to return the read tesseroid

@return 0 if all went well, 1 if failed to read.
*/
extern int gets_tess(const char *str, TESSEROID *tess);


/** Read tesseroids from an open file and store them in an array.

Allocates memory. Don't forget to free 'model'!

@param modelfile open FILE for reading with the tesseroid model
@param size used to return the size of the model read

@return pointer to array with the model. NULL if there was an error
*/
extern TESSEROID * read_tess_model(FILE *modelfile, int *size);

#endif