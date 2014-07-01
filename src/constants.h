/*
Define constants used, like the gravitational constant and unit conversions.

Values are assigned in file constants.c

All values are in SI units!
*/

#ifndef _TESSEROIDS_CONSTANTS_H_
#define _TESSEROIDS_CONSTANTS_H_

/* Mean Earth radius meters */
extern const double MEAN_EARTH_RADIUS;

/* The gravitational constant m^3*kg^-1*s^-1 */
extern const double G;

/* Conversion factor from SI units to Eotvos s^-2 = 10^9\ Eotvos */
extern const double SI2EOTVOS;

/* Conversion factor from SI units to mGal 1 m*s^-2 = 10^5 mGal */
extern const double SI2MGAL;

/* Pi */
extern const double PI;

/* Minimum distance-to-size ratio for potential computations to be accurate */
extern const double TESSEROID_POT_SIZE_RATIO;
/* Minimum distance-to-size ratio for gravity computations to be accurate */
extern const double TESSEROID_GX_SIZE_RATIO;
extern const double TESSEROID_GY_SIZE_RATIO;
extern const double TESSEROID_GZ_SIZE_RATIO;
/* Minimum distance-to-size ratio for gravity gradient computations to be
accurate */
extern const double TESSEROID_GXX_SIZE_RATIO;
extern const double TESSEROID_GXY_SIZE_RATIO;
extern const double TESSEROID_GXZ_SIZE_RATIO;
extern const double TESSEROID_GYY_SIZE_RATIO;
extern const double TESSEROID_GYZ_SIZE_RATIO;
extern const double TESSEROID_GZZ_SIZE_RATIO;

/* Max iterations of the root-finder algorithm */
extern const int GLQ_MAXIT;
/*i Max error allowed for the root-finder algorithm */
extern const double GLQ_MAXERROR;

#endif
