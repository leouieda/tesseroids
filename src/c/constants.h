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
Define constants used, like the gravitational constant and unit conversions.
All values are in SI units!

Author: Leonardo Uieda
Date: 24 Jan 2011
***************************************************************************** */

#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

/* Mean Earth radius (m) */
extern const double MEAN_EARTH_RADIUS;

/* The gravitational constant (m^3*kg^-1*s^-1) */
extern const double G;

/* Conversion factor from SI units to Eotvos: 1 /s**2 = 10**9 Eotvos */
extern const double SI2EOTVOS;

/* Conversion factor from SI units to mGal: 1 m/s**2 = 10**5 mGal */
extern const double SI2MGAL;

/* Pi ... */
extern const double PI;

#endif