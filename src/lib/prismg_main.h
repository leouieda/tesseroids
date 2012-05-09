/*
Generic main function for the prismg* programs.
*/


#ifndef _TESSEROIDS_PRISMG_MAIN_H_
#define _TESSEROIDS_PRISMG_MAIN_H_


/* For the definitions of PRISM */
#include "geometry.h"


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
                           double (*field)(PRISM, double, double, double),
                           double (*field_sph)(PRISM, double, double, double));

#endif
