/*
Generic main function for the tessg* programs.
*/


#ifndef _TESSEROIDS_TESSG_MAIN_H_
#define _TESSEROIDS_TESSG_MAIN_H_


/* For the definitions of GLQ and TESSEROID */
#include "glq.h"
#include "geometry.h"


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
