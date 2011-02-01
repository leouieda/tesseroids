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
Program to generate a regular grid of points in spherical coordinates.

@author Leonardo Uieda
@date 01 Feb 2011
*/


#include <stdio.h>
#include <string.h>
#include "logger.h"
#include "version.h"


struct tessmkgrd_args
{
    double w;
    double e;
    double s;
    double n;
    int nlon;
    int nlat;
    double height;
    int verbose;
    int logtofile;
    char *logfile;
};


/** Print out the help instructions */
void print_help();


/** Parse command line arguments and execute */
int main(int argc, char **argv)
{
    log_init(LOG_INFO);

    int bad_args = 0, parsed_args = 0, total_args = 3;
    struct tessmkgrd_args args;

    /* Default values for options */
    args.verbose = 0;
    args.logtofile = 0;

    /* Parse arguments */
    int i;
    for(i = 1; i < argc; i++)
    {
        if(argv[i][0] == '-')
        {
            switch(argv[i][1])
            {
                case 'h':
                    print_help();
                    return 0;

                case 'v':
                    args.verbose = 1;
                    break;

                case 'l':
                {
                    char *params = &argv[i][2];
                    if(strlen(params) == 0)
                    {
                        log_error("bad input argument -l. Missing filename.\n");
                        bad_args++;
                    }
                    else
                    {
                        args.logtofile = 1;
                        args.logfile = params;
                    }
                    break;
                }
                case '-':
                {
                    char *params = &argv[i][2];
                    if(strcmp(params, "version"))
                    {
                        log_error("invalid argument '%s'\n", argv[i]);
                        bad_args++;
                    }
                    else
                    {
                        print_version("tessmkgrd");
                        return 0;
                    }
                    break;
                }
                case 'r':
                {
                    char *params = &argv[i][2];
                    int nchar = 0;
                    int nread = sscanf(params, "%lf/%lf/%lf/%lf%n", &(args.w),
                                       &(args.e), &(args.s), &(args.n), &nchar);
                    if(nread != 4 || *(params + nchar) != '\0')
                    {
                        log_error("bad input argument '%s'\n", argv[i]);
                        bad_args++;
                    }
                    parsed_args++;
                    break;
                }
                case 'b':
                {
                    char *params = &argv[i][2];
                    int nchar = 0;
                    int nread = sscanf(params, "%d/%d%n", &(args.nlon),
                                       &(args.nlat), &nchar);
                    if(nread != 2 || *(params + nchar) != '\0')
                    {
                        log_error("bad input argument '%s'\n", argv[i]);
                        bad_args++;
                    }
                    parsed_args++;
                    break;
                }
                case 'z':
                {
                    char *params = &argv[i][2];
                    int nchar = 0;
                    int nread = sscanf(params, "%lf%n", &(args.height), &nchar);
                    
                    if(nread != 1 || *(params + nchar) != '\0')
                    {
                        log_error("bad input argument '%s'\n", argv[i]);
                        bad_args++;
                    }
                    parsed_args++;
                    break;
                }
                default:
                    log_error("invalid argument '%s'\n", argv[i]);
                    bad_args++;
                    break;
            }
        }
        else
        {
            log_error("invalid argument '%s'\n", argv[i]);
            bad_args++;
        }
    }

    /* Check if parsing went well */
    if(bad_args > 0 || parsed_args < total_args)
    {
        if(parsed_args < total_args)
        {
            log_info("tessmkgrd: missing arguments.");
            log_info(" given %d out of %d.\n\n", parsed_args, total_args);
        }
        if(bad_args > 0)
        {
            log_info("%d bad input argument(s).\n", bad_args);
        }
        log_info("Terminating due to bad input.\n");
        log_info("Try 'tessmkgrd -h' for instructions.\n");
        
        return 1;
    }



    return 0;
}


void print_help()
{
    printf("Usage: tessmkgrd [PARAMS] [OPTIONS]\n\n");
    printf("Make a regular grid of points in spherical coordinates.\n\n");
    printf("Output is printed to standard output (stdout).\n");
    printf("All units either SI or degrees!\n\n");
    printf("Parameters:\n");
    printf("  -r           W/E/S/N: Bounding region of the grid.\n");
    printf("  -b           NLON/NLAT: Number of grid points in the\n");
    printf("               longitudinal and latitudinal directions.\n");
    printf("  -z           HEIGHT: Height of the grid with respect to the\n");
    printf("               mean Earth radius.\n");
    printf("  -h           Print instructions.\n");
    printf("  --version    Print version and license information.\n");
    printf("\nOptions:\n");
    printf("  -v    Enable verbose printing to stderr.\n");
    printf("  -l    FILENAME: Print log messages to file FILENAME.\n");
    printf("\n");
    printf("Part of the Tesseroids package.\n");
    printf("Project site: <http://code.google.com/p/tesseroids>\n");
    printf("Report bugs at: ");
    printf("<http://code.google.com/p/tesseroids/issues/list>\n");
}