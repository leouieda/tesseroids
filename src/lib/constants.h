/*
Define constants used, like the gravitational constant and unit conversions.

Values are assigned in file constants.c

All values are in SI units!
*/

#ifndef _TESSEROIDS_CONSTANTS_H_
#define _TESSEROIDS_CONSTANTS_H_

/* Mean Earth radius [\f$ m \f$] */
extern const double MEAN_EARTH_RADIUS;

/* The gravitational constant [\f$ m^3*kg^{-1}*s^{-1} \f$] */
extern const double G;

/* Conversion factor from SI units to Eotvos
[\f$ \frac{1}{s^2} = 10^9\ Eotvos \f$] */
extern const double SI2EOTVOS;

/* Conversion factor from SI units to mGal
[\f$ 1 \frac{m}{s^2} = 10^5\ mGal \f$] */
extern const double SI2MGAL;

/* Pi */
extern const double PI;

/* Minimum distance-to-size ratio for potential computations to be accurate */
extern const int TESSEROID_POT_SIZE_RATIO;
/* Minimum distance-to-size ratio for gravity computations to be accurate */
extern const int TESSEROID_GX_SIZE_RATIO;
extern const int TESSEROID_GY_SIZE_RATIO;
extern const int TESSEROID_GZ_SIZE_RATIO;
/* Minimum distance-to-size ratio for gravity gradient computations to be
accurate */
extern const int TESSEROID_GXX_SIZE_RATIO;
extern const int TESSEROID_GXY_SIZE_RATIO;
extern const int TESSEROID_GXZ_SIZE_RATIO;
extern const int TESSEROID_GYY_SIZE_RATIO;
extern const int TESSEROID_GYZ_SIZE_RATIO;
extern const int TESSEROID_GZZ_SIZE_RATIO;

#endif
