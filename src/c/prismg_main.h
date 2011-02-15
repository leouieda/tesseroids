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
Generic main function for the prismg* programs.

\todo Option for calculating on spherical coordinates
\todo Catch errors in input points that occur at the end of the line

@author Leonardo Uieda
@date 08 Feb 2011
*/


#ifndef _TESSEROIDS_PRISMG_MAIN_H_
#define _TESSEROIDS_PRISMG_MAIN_H_


/* For the definitions of PRISM */
#include "utils.h"


/** Print the help message
*/
extern void print_help();


/** Run the main for a generic prismg* program

@param argc number of command line arguments
@param argv command line arguments
@param progname name of the specific program
@param field pointer to function that calculates the field of a single prism

@return 0 is all went well. 1 if failed.
*/
extern int run_prismg_main(int argc, char **argv, const char *progname,
                           double (*field)(PRISM, double, double, double));

#endif