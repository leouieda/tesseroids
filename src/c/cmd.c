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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "logger.h"
#include "version.h"
#include "cmd.h"


/* Parse basic command line arguments for programs */
int parse_basic_args(int argc, char **argv, const char *progname,
                     BASIC_ARGS *args, void (*print_help)(void))
{
    int bad_args = 0, parsed_args = 0, total_args = 1;

    /* Default values for options */
    args->verbose = 0;
    args->logtofile = 0;

    /* Parse arguments */
    int i;
    for(i = 1; i < argc; i++)
    {
        if(argv[i][0] == '-')
        {
            switch(argv[i][1])
            {
                case 'h':
                    if(argv[i][2] != '\0')
                    {
                        log_error("invalid argument '%s'", argv[i]);
                        bad_args++;
                        break;
                    }
                    print_help();
                    return 2;

                case 'v':
                    if(argv[i][2] != '\0')
                    {
                        log_error("invalid argument '%s'", argv[i]);
                        bad_args++;
                        break;
                    }
                    if(args->verbose)
                    {
                        log_error("repeated option -v");
                        bad_args++;
                        break;
                    }
                    args->verbose = 1;
                    break;

                case 'l':
                {
                    if(args->logtofile)
                    {
                        log_error("repeated option -l");
                        bad_args++;
                        break;
                    }
                    char *params = &argv[i][2];
                    if(strlen(params) == 0)
                    {
                        log_error("bad input argument -l. Missing filename.");
                        bad_args++;
                    }
                    else
                    {
                        args->logtofile = 1;
                        args->logfname = params;
                    }
                    break;
                }
                case '-':
                {
                    char *params = &argv[i][2];
                    if(strcmp(params, "version"))
                    {
                        log_error("invalid argument '%s'", argv[i]);
                        bad_args++;
                    }
                    else
                    {
                        print_version(progname);
                        return 2;
                    }
                    break;
                }
                default:
                    log_error("invalid argument '%s'", argv[i]);
                    bad_args++;
                    break;
            }
        }
        else
        {
            if(parsed_args == 0)
            {
                args->inputfname = argv[i];
                parsed_args++;
            }
            else
            {
                log_error("invalid argument '%s'. Already given model file %s",
                          argv[i], args->inputfname);
                bad_args++;
            }
        }
    }

    /* Check if parsing went well */
    if(parsed_args > total_args)
    {
        log_error("%s: too many input arguments. given %d, max %d.",
                    progname, parsed_args, total_args);
    }
    if(bad_args > 0)
    {
        log_error("%d bad input argument(s)", bad_args);
        return 1;
    }
    if(parsed_args < total_args)
    {
        return 3;
    }

    return 0;
}


/* Parse command line arguments for tessmass program */
int parse_tessmass_args(int argc, char **argv, const char *progname,
                        TESSMASS_ARGS *args, void (*print_help)(void))
{
    int bad_args = 0, parsed_args = 0, total_args = 1, parsed_r = 0;
    

    /* Default values for options */
    args->verbose = 0;
    args->logtofile = 0;
    args->use_range = 0;

    /* Parse arguments */
    int i;
    for(i = 1; i < argc; i++)
    {
        if(argv[i][0] == '-')
        {
            switch(argv[i][1])
            {
                case 'h':
                    if(argv[i][2] != '\0')
                    {
                        log_error("invalid argument '%s'", argv[i]);
                        bad_args++;
                        break;
                    }
                    print_help();
                    return 2;

                case 'v':
                    if(argv[i][2] != '\0')
                    {
                        log_error("invalid argument '%s'", argv[i]);
                        bad_args++;
                        break;
                    }
                    if(args->verbose)
                    {
                        log_error("repeated option -v");
                        bad_args++;
                        break;
                    }
                    args->verbose = 1;
                    break;

                case 'l':
                {
                    if(args->logtofile)
                    {
                        log_error("repeated option -l");
                        bad_args++;
                        break;
                    }
                    char *params = &argv[i][2];
                    if(strlen(params) == 0)
                    {
                        log_error("bad input argument -l. Missing filename.");
                        bad_args++;
                    }
                    else
                    {
                        args->logtofile = 1;
                        args->logfname = params;
                    }
                    break;
                }
                case '-':
                {
                    char *params = &argv[i][2];
                    if(strcmp(params, "version"))
                    {
                        log_error("invalid argument '%s'", argv[i]);
                        bad_args++;
                    }
                    else
                    {
                        print_version(progname);
                        return 2;
                    }
                    break;
                }
                case 'r':
                {
                    if(parsed_r)
                    {
                        log_error("repeated argument -r");
                        bad_args++;
                        break;
                    }
                    char *params = &argv[i][2];
                    int nchar = 0;
                    int nread = sscanf(params, "%lf/%lf%n", &(args->low_dens),
                                       &(args->high_dens), &nchar);
                    if(nread != 2 || *(params + nchar) != '\0')
                    {
                        log_error("bad input argument '%s'", argv[i]);
                        bad_args++;
                    }
                    parsed_r = 1;
                    args->use_range = 1;
                    break;
                }
                default:
                    log_error("invalid argument '%s'", argv[i]);
                    bad_args++;
                    break;
            }
        }
        else
        {
            if(parsed_args == 0)
            {
                args->inputfname = argv[i];
                parsed_args++;
            }
            else
            {
                log_error("invalid argument '%s'. Already given model file %s",
                          argv[i], args->inputfname);
                bad_args++;
            }
        }
    }

    /* Check if parsing went well */
    if(parsed_args > total_args)
    {
        log_error("%s: too many input arguments. given %d, max %d.",
                    progname, parsed_args, total_args);
    }
    if(bad_args > 0)
    {
        log_error("%d bad input argument(s)", bad_args);
        return 1;
    }
    if(parsed_args < total_args)
    {
        return 3;
    }
    
    return 0;
}


/** Parse command line arguments for tessmodgen program

@param argc number of command line arguments
@param argv command line arguments
@param progname name of the program
@param args to return the parsed arguments
@param print_help pointer to a function that prints the help message for the
                  program

@return Return code:
    - 0: if all went well
    - 1: if there were bad arguments and program should exit
    - 2: if printed help or version info and program should exit
*/
int parse_tessmodgen_args(int argc, char **argv, const char *progname,
                          TESSMODGEN_ARGS *args, void (*print_help)(void))
{
    int bad_args = 0, parsed_args = 0, total_args = 3,
        parsed_s = 0, parsed_z = 0, parsed_d = 0;


    /* Default values for options */
    args->verbose = 0;
    args->logtofile = 0;

    /* Parse arguments */
    int i;
    for(i = 1; i < argc; i++)
    {
        if(argv[i][0] == '-')
        {
            switch(argv[i][1])
            {
                case 'h':
                    if(argv[i][2] != '\0')
                    {
                        log_error("invalid argument '%s'", argv[i]);
                        bad_args++;
                        break;
                    }
                    print_help();
                    return 2;

                case 'v':
                    if(argv[i][2] != '\0')
                    {
                        log_error("invalid argument '%s'", argv[i]);
                        bad_args++;
                        break;
                    }
                    if(args->verbose)
                    {
                        log_error("repeated option -v");
                        bad_args++;
                        break;
                    }
                    args->verbose = 1;
                    break;

                case 'l':
                {
                    if(args->logtofile)
                    {
                        log_error("repeated option -l");
                        bad_args++;
                        break;
                    }
                    char *params = &argv[i][2];
                    if(strlen(params) == 0)
                    {
                        log_error("bad input argument -l. Missing filename.");
                        bad_args++;
                    }
                    else
                    {
                        args->logtofile = 1;
                        args->logfname = params;
                    }
                    break;
                }
                case '-':
                {
                    char *params = &argv[i][2];
                    if(strcmp(params, "version"))
                    {
                        log_error("invalid argument '%s'", argv[i]);
                        bad_args++;
                    }
                    else
                    {
                        print_version(progname);
                        return 2;
                    }
                    break;
                }
                case 's':
                {
                    if(parsed_s)
                    {
                        log_error("repeated argument -s");
                        bad_args++;
                        break;
                    }
                    char *params = &argv[i][2];
                    int nchar = 0;
                    int nread = sscanf(params, "%lf/%lf%n", &(args->dlon),
                                       &(args->dlat), &nchar);
                    if(nread != 2 || *(params + nchar) != '\0')
                    {
                        log_error("bad input argument '%s'", argv[i]);
                        bad_args++;
                    }
                    parsed_s = 1;
                    parsed_args++;
                    break;
                }
                case 'z':
                {
                    if(parsed_z)
                    {
                        log_error("repeated argument -z");
                        bad_args++;
                        break;
                    }
                    char *params = &argv[i][2];
                    int nchar = 0;
                    int nread = sscanf(params, "%lf%n", &(args->ref), &nchar);
                    if(nread != 1 || *(params + nchar) != '\0')
                    {
                        log_error("bad input argument '%s'", argv[i]);
                        bad_args++;
                    }
                    parsed_z = 1;
                    parsed_args++;
                    break;
                }
                case 'd':
                {
                    if(parsed_d)
                    {
                        log_error("repeated argument -d");
                        bad_args++;
                        break;
                    }
                    char *params = &argv[i][2];
                    int nchar = 0;
                    int nread = sscanf(params, "%lf%n", &(args->dens), &nchar);
                    if(nread != 1 || *(params + nchar) != '\0')
                    {
                        log_error("bad input argument '%s'", argv[i]);
                        bad_args++;
                    }
                    parsed_d = 1;
                    parsed_args++;
                    break;
                }
                default:
                    log_error("invalid argument '%s'", argv[i]);
                    bad_args++;
                    break;
            }
        }
        else
        {
            log_error("invalid argument '%s'", argv[i]);
            bad_args++;
        }
    }

    /* Check if parsing went well */
    if(bad_args > 0 || parsed_args != total_args)
    {
        if(parsed_args < total_args)
        {
            log_error("%s: missing input arguments. given %d out of %d.",
                      progname, parsed_args, total_args);
        }
        if(parsed_args > total_args)
        {
            log_error("%s: too many input arguments. given %d, max %d.",
                      progname, parsed_args, total_args);
        }
        if(bad_args > 0)
        {
            log_error("%d bad input argument(s)", bad_args);
        }
        return 1;
    }

    return 0;
}


/* Parse command line arguments for tessg* programs */
int parse_tessg_args(int argc, char **argv, const char *progname,
                      TESSG_ARGS *args)
{
    int bad_args = 0, parsed_args = 0, total_args = 1;
    int parsed_order = 0;

    /* Default values for options */
    args->verbose = 0;
    args->logtofile = 0;
    args->lon_order = 2;
    args->lat_order = 2;
    args->r_order = 2;

    /* Parse arguments */
    int i;
    for(i = 1; i < argc; i++)
    {
        if(argv[i][0] == '-')
        {
            switch(argv[i][1])
            {
                case 'h':
                    if(argv[i][2] != '\0')
                    {
                        log_error("invalid argument '%s'", argv[i]);
                        bad_args++;
                        break;
                    }
                    print_tessg_help(progname);
                    return 2;

                case 'v':
                    if(argv[i][2] != '\0')
                    {
                        log_error("invalid argument '%s'", argv[i]);
                        bad_args++;
                        break;
                    }
                    if(args->verbose)
                    {
                        log_error("repeated option -v");
                        bad_args++;
                        break;
                    }
                    args->verbose = 1;
                    break;

                case 'l':
                {
                    if(args->logtofile)
                    {
                        log_error("repeated option -l");
                        bad_args++;
                        break;
                    }
                    char *params = &argv[i][2];
                    if(strlen(params) == 0)
                    {
                        log_error("bad input argument -l. Missing filename.");
                        bad_args++;
                    }
                    else
                    {
                        args->logtofile = 1;
                        args->logfname = params;
                    }
                    break;
                }
                case '-':
                {
                    char *params = &argv[i][2];
                    if(strcmp(params, "version"))
                    {
                        log_error("invalid argument '%s'", argv[i]);
                        bad_args++;
                    }
                    else
                    {
                        print_version(progname);
                        return 2;
                    }
                    break;
                }
                case 'o':
                {
                    if(parsed_order)
                    {
                        log_error("repeated option -o");
                        bad_args++;
                        break;
                    }
                    char *params = &argv[i][2];
                    int nchar = 0;
                    int nread = sscanf(params, "%d/%d/%d%n", &(args->lon_order),
                                &(args->lat_order), &(args->r_order), &nchar);
                    if(nread != 3 || *(params + nchar) != '\0')
                    {
                        log_error("bad input argument '%s'", argv[i]);
                        bad_args++;
                    }
                    parsed_order = 1;
                    break;
                }
                default:
                    log_error("invalid argument '%s'", argv[i]);
                    bad_args++;
                    break;
            }
        }
        else
        {
            if(parsed_args == 0)
            {
                args->modelfname = argv[i];
                parsed_args++;
            }
            else
            {
                log_error("invalid argument '%s'. Already given model file %s",
                          argv[i], args->modelfname);
                bad_args++;
            }
        }
    }

    /* Check if parsing went well */
    if(bad_args > 0 || parsed_args > total_args)
    {
        if(parsed_args < total_args)
        {
            log_error("%s: missing input file.",
                      progname, parsed_args, total_args);
        }
        if(parsed_args > total_args)
        {
            log_error("%s: too many input arguments. given %d, max %d.",
                      progname, parsed_args, total_args);
        }
        if(bad_args > 0)
        {
            log_error("%d bad input argument(s)", bad_args);
        }
        return 1;
    }

    return 0;
}


/* Parse command line arguments for tessgrd program */
int parse_tessgrd_args(int argc, char **argv, TESSGRD_ARGS *args)
{
    int bad_args = 0, parsed_args = 0, total_args = 3,
        parsed_r = 0, parsed_b = 0, parsed_z = 0;
    
    char progname[] = "tessgrd";

    /* Default values for options */
    args->verbose = 0;
    args->logtofile = 0;

    /* Parse arguments */
    int i;
    for(i = 1; i < argc; i++)
    {
        if(argv[i][0] == '-')
        {
            switch(argv[i][1])
            {
                case 'h':
                    if(argv[i][2] != '\0')
                    {
                        log_error("invalid argument '%s'", argv[i]);
                        bad_args++;
                        break;
                    }
                    print_tessgrd_help();
                    return 2;

                case 'v':
                    if(argv[i][2] != '\0')
                    {
                        log_error("invalid argument '%s'", argv[i]);
                        bad_args++;
                        break;
                    }
                    if(args->verbose)
                    {
                        log_error("repeated option -v");
                        bad_args++;
                        break;
                    }
                    args->verbose = 1;
                    break;

                case 'l':
                {
                    if(args->logtofile)
                    {
                        log_error("repeated option -l");
                        bad_args++;
                        break;
                    }
                    char *params = &argv[i][2];
                    if(strlen(params) == 0)
                    {
                        log_error("bad input argument -l. Missing filename.");
                        bad_args++;
                    }
                    else
                    {
                        args->logtofile = 1;
                        args->logfname = params;
                    }
                    break;
                }
                case '-':
                {
                    char *params = &argv[i][2];
                    if(strcmp(params, "version"))
                    {
                        log_error("invalid argument '%s'", argv[i]);
                        bad_args++;
                    }
                    else
                    {
                        print_version(progname);
                        return 2;
                    }
                    break;
                }
                case 'r':
                {
                    if(parsed_r)
                    {
                        log_error("repeated argument -r");
                        bad_args++;
                        break;
                    }
                    char *params = &argv[i][2];
                    int nchar = 0;
                    int nread = sscanf(params, "%lf/%lf/%lf/%lf%n", &(args->w),
                                       &(args->e), &(args->s), &(args->n), &nchar);
                    if(nread != 4 || *(params + nchar) != '\0')
                    {
                        log_error("bad input argument '%s'", argv[i]);
                        bad_args++;
                    }
                    parsed_args++;
                    parsed_r = 1;
                    break;
                }
                case 'b':
                {
                    if(parsed_b)
                    {
                        log_error("repeated argument -b");
                        bad_args++;
                        break;
                    }
                    char *params = &argv[i][2];
                    int nchar = 0;
                    int nread = sscanf(params, "%d/%d%n", &(args->nlon),
                                       &(args->nlat), &nchar);
                    if(nread != 2 || *(params + nchar) != '\0')
                    {
                        log_error("bad input argument '%s'", argv[i]);
                        bad_args++;
                    }
                    parsed_args++;
                    parsed_b = 1;
                    break;
                }
                case 'z':
                {
                    if(parsed_z)
                    {
                        log_error("repeated argument -z");
                        bad_args++;
                        break;
                    }
                    char *params = &argv[i][2];
                    int nchar = 0;
                    int nread = sscanf(params, "%lf%n", &(args->height), &nchar);

                    if(nread != 1 || *(params + nchar) != '\0')
                    {
                        log_error("bad input argument '%s'", argv[i]);
                        bad_args++;
                    }
                    parsed_args++;
                    parsed_z = 1;
                    break;
                }
                default:
                    log_error("invalid argument '%s'", argv[i]);
                    bad_args++;
                    break;
            }
        }
        else
        {
            log_error("invalid argument '%s'", argv[i]);
            bad_args++;
        }
    }

    /* Check if parsing went well */
    if(bad_args > 0 || parsed_args != total_args)
    {
        if(parsed_args < total_args)
        {
            log_error("%s: missing input arguments. given %d out of %d.",
                      progname, parsed_args, total_args);
        }
        if(parsed_args > total_args)
        {
            log_error("%s: too many input arguments. given %d, max %d.",
                      progname, parsed_args, total_args);
        }
        if(bad_args > 0)
        {
            log_error("%d bad input argument(s)", bad_args);
        }
        return 1;
    }

    return 0;
}


/* Print the help message for tessg* programs */
void print_tessg_help(const char *progname)
{
    printf("Usage: %s MODELFILE [OPTIONS]\n\n", progname);
    printf("Calculate the %s component due to a tesseroid model on\n",
           progname + 4);
    printf("specified observation points.\n\n");
    printf("All units either SI or degrees!\n\n");
    printf("Input:\n");
    printf("  Computation points passed through standard input (stdin).\n");
    printf("  Reads 3 or more values per line and inteprets the first 3 as\n");
    printf("  longitude, latitude and height of a computation points. Other\n");
    printf("  values in the line are ignored. Lines that start with # are\n");
    printf("  ignored as comments. Lines should be no longer than 10000\n");
    printf("  (ten thousand) characters.\n\n");
    printf("Output:\n");
    printf("  Printed to standard output (stdout) in the form:\n");
    printf("    lon lat height ... result\n");
    printf("  ... represents any values that were read from input and\n");
    printf("  ignored. In other words, the result is appended to the last\n");
    printf("  column of the input. Use this to pipe tessg* programs\n");
    printf("  together.\n\n");
    printf("  * Comments about the provenance of the data are inserted into\n");
    printf("    the top of the output\n\n");
    printf("MODELFILE: File containing the tesseroid model\n");
    printf("  * Each tesseroid is specified by the values of its borders\n");
    printf("    and density\n");
    printf("  * The file should contain one tesseroid per line\n");
    printf("  * Each line should have the following column format:\n");
    printf("      West East South North Top Bottom Density\n");
    printf("  * Top and Bottom should be read as 'depth to top' and \n");
    printf("    'depth to bottom' from the mean Earth radius. Use negative\n");
    printf("    values if above the surface, for example when modeling\n");
    printf("    topography\n");
    printf("  * If a line starts with # it will be considered a comment and\n");
    printf("    will be ignored.\n\n");
    printf("Options:\n");
    printf("  -o           LONORDER/LATORDER/RORDER: the GLQ order to use\n");
    printf("               in the longitudinal, latitudinal and radial\n");
    printf("               integrations, respectively. Defaults to 2/2/2.\n");
    printf("  -h           Print instructions.\n");
    printf("  --version    Print version and license information.\n");
    printf("  -v           Enable verbose printing to stderr.\n");
    printf("  -l           FILENAME: Print log messages to file FILENAME.\n");
    printf("\nPart of the Tesseroids package.\n");
    printf("Project site: <http://code.google.com/p/tesseroids>\n");
    printf("Report bugs at: ");
    printf("<http://code.google.com/p/tesseroids/issues/list>\n");
}


/* Print the help message for tessgrd program */
void print_tessgrd_help()
{
    printf("Usage: tessgrd [PARAMS] [OPTIONS]\n\n");
    printf("Make a regular grid of points.\n\n");
    printf("All units either SI or degrees!\n\n");
    printf("Output:\n");
    printf("  Printed to standard output (stdout) in the format:\n");
    printf("    lon1    lat1    height\n");
    printf("    lon2    lat1    height\n");
    printf("    ...     ...     ...\n");
    printf("    lonNLON lat1    height\n");
    printf("    lon1    lat2    height\n");
    printf("    ...     ...     ...\n");
    printf("    ...     ...     ...\n");
    printf("    lonNLON latNLAT height\n\n");
    printf("  * Comments about the provenance of the data are inserted into\n");
    printf("    the top of the output\n\n");
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
    printf("\nPart of the Tesseroids package.\n");
    printf("Project site: <http://code.google.com/p/tesseroids>\n");
    printf("Report bugs at: ");
    printf("<http://code.google.com/p/tesseroids/issues/list>\n");
}