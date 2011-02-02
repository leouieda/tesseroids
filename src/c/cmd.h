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
Command line parsing tools.

@author Leonardo Uieda
@date 02 Feb 2011
*/


#ifndef _TESSEROIDS_CMD_H_
#define _TESSEROIDS_CMD_H_


/** Store input arguments and option flags for tessg* programs */
typedef struct tessg_args
{
    int lon_order; /**< glq order in longitude integration */
    int lat_order; /**< glq order in latitude integration */
    int r_order; /**< glq order in radial integration */
    char *modelfname; /**< name of the file with the tesseroid model */
    int verbose; /**< flag to indicate if verbose printing is enabled */
    int logtofile; /**< flag to indicate if logging to a file is enabled */
    char *logfname;
} TESSG_ARGS;


/** Store input arguments and option flags for tessmkgrd program */
typedef struct tessmkgrd_args
{
    double w;
    double e;
    double s;
    double n;
    int nlon;
    int nlat;
    double height;
    int verbose; /**< flag to indicate if verbose printing is enabled */
    int logtofile; /**< flag to indicate if logging to a file is enabled */
    char *logfname;
} TESSMKGRD_ARGS;


/** Parse command line arguments for tessg* programs

logs the bad argument warnings using logger.h

@param argc number of command line arguments
@param argv command line arguments
@param progname name of the specific program
@param args to return the parsed arguments

@return Return code:
    - 0: if all went well
    - 1: if there were bad arguments and program should exit
    - 2: if printed help or version info and program should exit
*/
extern int parse_tessg_args(int argc, char **argv, const char *progname,
                            TESSG_ARGS *args);


/** Parse command line arguments for tessgmkgrd program

logs the bad argument warnings using logger.h

@param argc number of command line arguments
@param argv command line arguments
@param args to return the parsed arguments

@return Return code:
    - 0: if all went well
    - 1: if there were bad arguments and program should exit
    - 2: if printed help or version info and program should exit
*/
extern int parse_tessmkgrd_args(int argc, char **argv, TESSMKGRD_ARGS *args);


/** Print the help message for tessg* programs

@param progname name of the specific tessg* program
*/
extern void print_tessg_help(const char *progname);


/** Print the help message for tessmkgrd program

Prints to stdout.

*/
extern void print_tessmkgrd_help();

#endif