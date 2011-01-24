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

#include "constants.h"

/* The gravitational constant (m^3*kg^-1*s^-1) */
const double G = 0.00000000006673;

/* Conversion factor from SI units to Eotvos: 1 /s**2 = 10**9 Eotvos */
const double SI2EOTVOS = 1000000000.0;

/* Conversion factor from SI units to mGal: 1 m/s**2 = 10**5 mGal */
const double SI2MGAL = 100000.0;