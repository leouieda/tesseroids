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
Program to calculate gz of a tesseroid model on a set of points.

@author Leonardo Uieda
@date 02 Feb 2011
*/


#include "lib/constants.h"
#include "lib/grav_tess.h"
#include "lib/tessg_main.h"


/** Main */
int main(int argc, char **argv)
{
    return run_tessg_main(argc, argv, "tessgz", &tess_gz,
                          TESSEROID_G_SIZE_RATIO);
}
