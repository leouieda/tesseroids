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
Generic main function for the tessg* programs.

@author Leonardo Uieda
@date 03 Feb 2011
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "logger.h"
#include "version.h"


#ifndef _TESSEROIDS_TESSG_MAIN_H_
#define _TESSEROIDS_TESSG_MAIN_H_


/* For the definitions of GLQ and TESSEROID */
#include "glq.h"
#include "utils.h"


/** Run the main for a generic tessg* program

@param argc number of command line arguments
@param argv command line arguments
@param progname name of the specific program
@param tess_calculator pointer to function that calculates the field of a single
                       tesseroidi

@return 0 is all went well. 1 if failed.
*/
extern int run_tessg_main(int argc, char **argv, const char *progname,
   double (*tess_calculator)(TESSEROID, double, double, double, GLQ, GLQ, GLQ));

#endif