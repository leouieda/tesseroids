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
Define constants used, like the gravitational constant and unit conversions.

Values are assigned in file constants.c

<b>All values are in SI units!</b>

@author Leonardo Uieda
@date 24 Jan 2011
*/

#ifndef _TESSEROIDS_CONSTANTS_H_
#define _TESSEROIDS_CONSTANTS_H_

/** \var MEAN_EARTH_RADIUS
Mean Earth radius [\f$ m \f$] */
extern const double MEAN_EARTH_RADIUS;

/** \var G
The gravitational constant [\f$ m^3*kg^{-1}*s^{-1} \f$] */
extern const double G;

/** \var SI2EOTVOS
Conversion factor from SI units to Eotvos
[\f$ \frac{1}{s^2} = 10^9\ Eotvos \f$] */
extern const double SI2EOTVOS;

/** \var SI2MGAL
Conversion factor from SI units to mGal
[\f$ 1 \frac{m}{s^2} = 10^5\ mGal \f$] */
extern const double SI2MGAL;

/** \var PI
Pi */
extern const double PI;

/** \var TESSEROID_POT_SIZE_RATIO
Minimum distance-to-size ratio for potential computations to be accurate */
extern const int TESSEROID_POT_SIZE_RATIO;
/** \var TESSEROID_G_SIZE_RATIO
Minimum distance-to-size ratio for gravity computations to be accurate */
extern const int TESSEROID_G_SIZE_RATIO;
/** \var TESSEROID_GG_SIZE_RATIO
Minimum distance-to-size ratio for gravity gradient computations to be
accurate */
extern const int TESSEROID_GG_SIZE_RATIO;

#endif
