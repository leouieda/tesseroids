/*
Data structures for geometric elements and functions that operate on them.
Defines the TESSEROID, SPHERE, and PRISM structures.
*/

#ifndef _TESSEROIDS_GEOMETRY_H_
#define _TESSEROIDS_GEOMETRY_H_


/* Store information on a tesseroid */
typedef struct tess_struct {
    /* s, n, w, e in degrees. r1 and r2 are the smaller and larger radius */
    double density; /* in SI units */
    double w; /* western longitude border in degrees */
    double e; /* eastern longitude border in degrees */
    double s; /* southern latitude border in degrees */
    double n; /* northern latitude border in degrees */
    double r1; /* smallest radius border in SI units */
    double r2; /* largest radius border in SI units */
} TESSEROID;


/* Store information on a rectangular prism */
typedef struct prism_struct {
    double density; /* in SI units */
    double x1; /* in SI units */
    double x2; /* in SI units */
    double y1; /* in SI units */
    double y2; /* in SI units */
    double z1; /* in SI units */
    double z2; /* in SI units */
    /* Geodetic coordinates of the center of the top face of the prism */
    double lon, lat, r;
} PRISM;


/* Store information on a sphere */
typedef struct sphere_struct {
    double density; /* in SI units */
    double r; /* radius of the sphere in SI units */
    double lonc; /* longitude of the center of the sphere in degrees */
    double latc; /* latitude of the center of the sphere in degrees */
    double rc; /* radial coordinate of the center of the sphere in SI units */
} SPHERE;


/* Split a tesseroid into 8.

@param tess tesseroid that will be split
@param split array of 8 tesseroids with memory allocated. Used to return.
*/
extern void split_tess(TESSEROID tess, TESSEROID *split);


/* Calculate the total mass of a tesseroid model.

Give all in SI units and degrees!

@param model array of tesseroids
@param size size of the model

@return The calculated mass
*/
extern double tess_total_mass(TESSEROID *model, int size);


/* Calculate the mass of a tesseroid model within a density range.

Give all in SI units and degrees!

@param model array of tesseroids
@param size size of the model
@param low_dens lower bound of the density range
@param high_dens upper bound of the density range

@return The calculated mass
*/
extern double tess_range_mass(TESSEROID *model, int size, double low_dens,
                              double high_dens);


/* Convert a tesseroid into a rectangular prism of equal volume (Wild-Pfeiffer, 2008).

\f[
\Delta x = \frac{r_1 + r_2}{2} \Delta \phi,
\f]
\f[
\Delta y = \frac{r_1 + r_2}{2} \cos\left(\frac{\phi_1 + \phi_2}{2}\right) \Delta\lambda,
\f]
\f[
\Delta z = \Delta r,
\f]

<b>References</b>

- Wild-Pfeiffer, F. (2008). A comparison of different mass elements for use in
gravity gradiometry. Journal of Geodesy, 82(10), 637-653.

@param tess tesseroid to convert
@param prism prism with equal volume of the tesseroid (used to return)
*/
extern void tess2prism(TESSEROID tess, PRISM *prism);


/* Convert a tesseroid into a rectangular prism of equal volume by
approximating 1 degree by 111.11 km.

@param tess tesseroid to convert
@param prism prism with equal volume of the tesseroid (used to return)
*/
extern void tess2prism_flatten(TESSEROID tess, PRISM *prism);


/* Convert a tesseroid into a sphere of equal volume.

Parameters:
@param tess tesseroid to convert
@param sphere sphere with equal volume of the tesseroid (used to return)
*/
extern void tess2sphere(TESSEROID tess, SPHERE *sphere);


/* Convert a rectangular prism into a sphere of equal volume.

Parameters:
@param prism prism to convert
@param lonc longitude of the desired center of the sphere, in degrees
@param latc latitude of the desired center of the sphere, in degrees
@param rc radial coordinate of the desired center of the sphere, in SI units
@param sphere sphere with equal volume of the prism (used to return)
*/
extern void prism2sphere(PRISM prism, double lonc, double latc, double rc,
                         SPHERE *sphere);


/* Calculate the volume of a tesseroid.

@param tess the tesseroid whose volume will be calculated

@return the volume in the respective units
*/
extern double tess_volume(TESSEROID tess);


/* Calculate the volume of a sphere.

@param sphere the sphere whose volume will be calculated

@return the volume in the respective units
*/
extern double sphere_volume(SPHERE sphere);


/* Calculate the volume of a prism

@param prism the prism whose volume will be calculated

@return the volume in the respective units
*/
extern double prism_volume(PRISM prism);


#endif
