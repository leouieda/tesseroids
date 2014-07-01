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


/* Split a tesseroid into 8. */
extern void split_tess(TESSEROID tess, TESSEROID *split);
/* Calculate the total mass of a tesseroid model. Returns he calculated mass.
Give all in SI units and degrees */
extern double tess_total_mass(TESSEROID *model, int size);
/* Calculate the mass of a tesseroid model within a density range. */
extern double tess_range_mass(TESSEROID *model, int size, double low_dens,
                              double high_dens);
/* Convert a tesseroid into a rectangular prism of equal volume as in:
   Wild-Pfeiffer, F. (2008). A comparison of different mass elements for use in
   gravity gradiometry. Journal of Geodesy, 82(10), 637-653. */
extern void tess2prism(TESSEROID tess, PRISM *prism);
/* Convert a tesseroid into a rectangular prism of equal volume by
approximating 1 degree by 111.11 km. */
extern void tess2prism_flatten(TESSEROID tess, PRISM *prism);
/* Convert a tesseroid into a sphere of equal volume. */
extern void tess2sphere(TESSEROID tess, SPHERE *sphere);
/* Convert a rectangular prism into a sphere of equal volume.
lonc, latc, rc are coordinates of the desired center of the sphere, in degrees
and meters. */
extern void prism2sphere(PRISM prism, double lonc, double latc, double rc,
                         SPHERE *sphere);
/* Calculate the volume of a tesseroid. */
extern double tess_volume(TESSEROID tess);
/* Calculate the volume of a sphere. */
extern double sphere_volume(SPHERE sphere);
/* Calculate the volume of a prism */
extern double prism_volume(PRISM prism);

#endif
