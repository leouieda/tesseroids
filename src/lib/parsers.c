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
Input and output parsing tools.

@author Leonardo Uieda
@date 02 Feb 2011
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "logger.h"
#include "version.h"
#include "parsers.h"


/* Parse basic command line arguments for programs */
int parse_basic_args(int argc, char **argv, const char *progname,
                     BASIC_ARGS *args, void (*print_help)(void))
{
    int bad_args = 0, parsed_args = 0, total_args = 1, i;
    char *params;

    /* Default values for options */
    args->verbose = 0;
    args->logtofile = 0;
    /* Parse arguments */
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
                    params = &argv[i][2];
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
                    params = &argv[i][2];
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


/* Parse command line arguments for tess2prism program */
int parse_tess2prism_args(int argc, char **argv, const char *progname,
                        TESS2PRISM_ARGS *args, void (*print_help)(void))
{
    int bad_args = 0, parsed_args = 0, total_args = 1, i;
    char *params;
    
    /* Default values for options */
    args->verbose = 0;
    args->logtofile = 0;
    args->flatten = 0;
    /* Parse arguments */
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
                    params = &argv[i][2];
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
                    params = &argv[i][2];
                    if(!strcmp(params, "version"))
                    {
                        print_version(progname);
                        return 2;
                    }
                    else if(!strcmp(params, "flatten"))
                    {
                        args->flatten = 1;
                    }
                    else
                    {
                        log_error("invalid argument '%s'", argv[i]);
                        bad_args++;
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
    int bad_args = 0, parsed_args = 0, total_args = 1, parsed_r = 0, i, nchar,
        nread;
    char *params;
    
    /* Default values for options */
    args->verbose = 0;
    args->logtofile = 0;
    args->use_range = 0;
    /* Parse arguments */
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
                    params = &argv[i][2];
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
                    params = &argv[i][2];
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
                    params = &argv[i][2];
                    nchar = 0;
                    nread = sscanf(params, "%lf/%lf%n", &(args->low_dens),
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


/* Parse command line arguments for tessmodgen program */
int parse_tessmodgen_args(int argc, char **argv, const char *progname,
                          TESSMODGEN_ARGS *args, void (*print_help)(void))
{
    int bad_args = 0, parsed_args = 0, total_args = 2, parsed_s = 0,
        parsed_z = 0, parsed_d = 0, i, nchar, nread;
    char *params;

    /* Default values for options */
    args->verbose = 0;
    args->logtofile = 0;
    args->fix_density = 0;
    /* Parse arguments */
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
                    params = &argv[i][2];
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
                    params = &argv[i][2];
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
                    params = &argv[i][2];
                    nchar = 0;
                    nread = sscanf(params, "%lf/%lf%n", &(args->dlon),
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
                    params = &argv[i][2];
                    nchar = 0;
                    nread = sscanf(params, "%lf%n", &(args->ref), &nchar);
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
                    params = &argv[i][2];
                    nchar = 0;
                    nread = sscanf(params, "%lf%n", &(args->dens), &nchar);
                    if(nread != 1 || *(params + nchar) != '\0')
                    {
                        log_error("bad input argument '%s'", argv[i]);
                        bad_args++;
                    }
                    parsed_d = 1;
                    args->fix_density = 1;
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
                      TESSG_ARGS *args, void (*print_help)(const char *))
{
    int bad_args = 0, parsed_args = 0, total_args = 1,  parsed_order = 0, i,
        nchar, nread;
    char *params;

    /* Default values for options */
    args->verbose = 0;
    args->logtofile = 0;
    args->lon_order = 2;
    args->lat_order = 2;
    args->r_order = 2;
    args->adaptative = 1;
    /* Parse arguments */
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
                    print_help(progname);
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
                    params = &argv[i][2];
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
                    params = &argv[i][2];
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
                case 'a':
                    if(argv[i][2] != '\0')
                    {
                        log_error("invalid argument '%s'", argv[i]);
                        bad_args++;
                        break;
                    }
                    if(!args->adaptative)
                    {
                        log_error("repeated option -a");
                        bad_args++;
                        break;
                    }
                    args->adaptative = 0;
                    break;
                case 'o':
                {
                    if(parsed_order)
                    {
                        log_error("repeated option -o");
                        bad_args++;
                        break;
                    }
                    params = &argv[i][2];
                    nchar = 0;
                    nread = sscanf(params, "%d/%d/%d%n", &(args->lon_order),
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
    if(bad_args > 0 || parsed_args != total_args)
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
int parse_tessgrd_args(int argc, char **argv, TESSGRD_ARGS *args,
                       void (*print_help)(void))
{
    int bad_args = 0, parsed_args = 0, total_args = 3, parsed_r = 0,
        parsed_b = 0, parsed_z = 0, i, nchar, nread;    
    char progname[] = "tessgrd", *params;

    /* Default values for options */
    args->verbose = 0;
    args->logtofile = 0;
    /* Parse arguments */
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
                    params = &argv[i][2];
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
                    params = &argv[i][2];
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
                    params = &argv[i][2];
                    nchar = 0;
                    nread = sscanf(params, "%lf/%lf/%lf/%lf%n", &(args->w),
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
                    params = &argv[i][2];
                    nchar = 0;
                    nread = sscanf(params, "%d/%d%n", &(args->nlon),
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
                    params = &argv[i][2];
                    nchar = 0;
                    nread = sscanf(params, "%lf%n", &(args->height), &nchar);
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

