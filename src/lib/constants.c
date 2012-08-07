/*
Define constants used, like the gravitational constant and unit conversions.

All values are in SI units!
*/

#include "constants.h"

/* Mean Earth radius [\f$ m \f$] */
const double MEAN_EARTH_RADIUS = 6378137.0;

/* The gravitational constant [\f$ m^3*kg^{-1}*s^{-1} \f$] */
const double G = 0.00000000006673;

/* Conversion factor from SI units to Eotvos
[\f$ \frac{1}{s^2} = 10^9\ Eotvos \f$] */
const double SI2EOTVOS = 1000000000.0;

/* Conversion factor from SI units to mGal
[\f$ 1 \frac{m}{s^2} = 10^5\ mGal \f$] */
const double SI2MGAL = 100000.0;

/* Pi */
const double PI = 3.1415926535897932384626433832795;

/* minimum distance-to-size ratio for potential computations to be accurate */
const double TESSEROID_POT_SIZE_RATIO = 1.5;
/* Minimum distance-to-size ratio for gravity computations to be accurate */
const double TESSEROID_GX_SIZE_RATIO = 3;
const double TESSEROID_GY_SIZE_RATIO = 3;
const double TESSEROID_GZ_SIZE_RATIO = 2;
/* Minimum distance-to-size ratio for gravity gradient computations to be
accurate */
const double TESSEROID_GXX_SIZE_RATIO = 3;
const double TESSEROID_GXY_SIZE_RATIO = 4.5;
const double TESSEROID_GXZ_SIZE_RATIO = 4;
const double TESSEROID_GYY_SIZE_RATIO = 3;
const double TESSEROID_GYZ_SIZE_RATIO = 4;
const double TESSEROID_GZZ_SIZE_RATIO = 3;
