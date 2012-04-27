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

\todo Catch errors in input points that occur at the end of the line

@author Leonardo Uieda
@date 03 Feb 2011
*/


#ifndef _TESSEROIDS_TESSG_MAIN_H_
#define _TESSEROIDS_TESSG_MAIN_H_


/* For the definitions of GLQ and TESSEROID */
#include "glq.h"
#include "utils.h"


/** Print the help message for tessg* programs

@param progname name of the specific tessg* program
*/
extern void print_tessg_help(const char *progname);


/** Run the main for a generic tessg* program

@param argc number of command line arguments
@param argv command line arguments
@param progname name of the specific program
@param field pointer to function that calculates the field of a single tesseroid
@param ratio distance-to-size ratio for doing adaptative resizing

@return 0 is all went well. 1 if failed.
*/
extern int run_tessg_main(int argc, char **argv, const char *progname,
   double (*field)(TESSEROID, double, double, double, GLQ, GLQ, GLQ),
   int ratio);

#endif
