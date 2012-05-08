/*
Define constants used, like the gravitational constant and unit conversions.

All values are in SI units!
*/

#include "constants.h"

/** \var MEAN_EARTH_RADIUS
Mean Earth radius [\f$ m \f$] */
const double MEAN_EARTH_RADIUS = 6378137.0;

/** \var G
The gravitational constant [\f$ m^3*kg^{-1}*s^{-1} \f$] */
const double G = 0.00000000006673;

/** \var SI2EOTVOS
Conversion factor from SI units to Eotvos
[\f$ \frac{1}{s^2} = 10^9\ Eotvos \f$] */
const double SI2EOTVOS = 1000000000.0;

/** \var SI2MGAL
Conversion factor from SI units to mGal
[\f$ 1 \frac{m}{s^2} = 10^5\ mGal \f$] */
const double SI2MGAL = 100000.0;

/** \var PI
Pi */
const double PI = 3.1415926535897932384626433832795;

/** \var TESSEROID_POT_SIZE_RATIO
Minimum distance-to-size ratio for potential computations to be accurate */
const int TESSEROID_POT_SIZE_RATIO = 1;
/** \var TESSEROID_G_SIZE_RATIO
Minimum distance-to-size ratio for gravity computations to be accurate */
const int TESSEROID_G_SIZE_RATIO = 2;
/** \var TESSEROID_GG_SIZE_RATIO
Minimum distance-to-size ratio for gravity gradient computations to be
accurate */
const int TESSEROID_GG_SIZE_RATIO = 4;
