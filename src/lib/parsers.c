/*
Input and output parsing tools.
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "logger.h"
#include "version.h"
#include "parsers.h"
#include "constants.h"
#include "geometry.h"


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


/* Parse command line arguments for prism* programs */
int parse_prism_args(int argc, char **argv, const char *progname,
                     PRISM_ARGS *args, void (*print_help)(void))
{
    int bad_args = 0, parsed_args = 0, total_args = 1, i;
    char *params;

    /* Default values for options */
    args->verbose = 0;
    args->logtofile = 0;
    args->spherical = 0;
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
                case 'g':
                    if(argv[i][2] != '\0')
                    {
                        log_error("invalid argument '%s'", argv[i]);
                        bad_args++;
                        break;
                    }
                    if(args->spherical)
                    {
                        log_error("repeated option -g");
                        bad_args++;
                        break;
                    }
                    args->spherical = 1;
                    break;
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


/* Strip trailing spaces and newlines from the end of a string */
void strstrip(char *str)
{
    int i;
    for(i = strlen(str) - 1; i >= 0; i--)
    {
        if(str[i] != ' ' && str[i] != '\n' && str[i] != '\r' && str[i] != '\0')
            break;
    }
    str[i + 1] = '\0';
}


/* Read a single tesseroid from a string */
int gets_tess(const char *str, TESSEROID *tess)
{
    /** \todo Catch wrong order of model inputs, ie. w > e or s > n or
    top < bottom */

    double w, e, s, n, top, bot, dens;
    int nread, nchars;

    nread = sscanf(str, "%lf %lf %lf %lf %lf %lf %lf%n", &w, &e, &s,
                    &n, &top, &bot, &dens, &nchars);
    if(nread != 7 || str[nchars] != '\0')
    {
        return 1;
    }
    tess->w = w;
    tess->e = e;
    tess->s = s;
    tess->n = n;
    tess->r1 = MEAN_EARTH_RADIUS + bot;
    tess->r2 = MEAN_EARTH_RADIUS + top;
    tess->density = dens;
    return 0;
}


/* Read tesseroids from an open file and store them in an array */
TESSEROID * read_tess_model(FILE *modelfile, int *size)
{
    TESSEROID *model, *tmp;
    int buffsize = 100, line, badinput = 0, error_exit = 0;
    char sbuff[10000];

    /* Start with a single buffer allocation and expand later if necessary */
    model = (TESSEROID *)malloc(buffsize*sizeof(TESSEROID));
    if(model == NULL)
    {
        log_error("problem allocating initial memory to load tesseroid model");
        return NULL;
    }
    *size = 0;
    for(line = 1; !feof(modelfile); line++)
    {
        if(fgets(sbuff, 10000, modelfile) == NULL)
        {
            if(ferror(modelfile))
            {
                log_error("problem encountered reading line %d", line);
                error_exit = 1;
                break;
            }
        }
        else
        {
            /* Check for comments and blank lines */
            if(sbuff[0] == '#' || sbuff[0] == '\r' || sbuff[0] == '\n')
            {
                continue;
            }
            if(*size == buffsize)
            {
                buffsize += buffsize;
                tmp = (TESSEROID *)realloc(model, buffsize*sizeof(TESSEROID));
                if(tmp == NULL)
                {
                    /* Need to free because realloc leaves unchanged in case of
                       error */
                    free(model);
                    log_error("problem expanding memory for tesseroid model");
                    return NULL;
                }
                model = tmp;
            }
            /* Remove any trailing spaces or newlines */
            strstrip(sbuff);
            if(gets_tess(sbuff, &model[*size]))
            {
                log_warning("bad/invalid tesseroid at line %d", line);
                badinput = 1;
                continue;
            }
            (*size)++;
        }
    }
    if(badinput || error_exit)
    {
        free(model);
        return NULL;
    }
    /* Adjust the size of the model */
    tmp = (TESSEROID *)realloc(model, (*size)*sizeof(TESSEROID));
    if(tmp == NULL)
    {
        /* Need to free because realloc leaves unchanged in case of
            error */
        free(model);
        log_error("problem freeing excess memory for tesseroid model");
        return NULL;
    }
    model = tmp;
    return model;
}


/* Read a single rectangular prism from a string */
int gets_prism(const char *str, PRISM *prism)
{
    double x1, x2, y1, y2, z1, z2, dens;
    int nread, nchars;

    nread = sscanf(str, "%lf %lf %lf %lf %lf %lf %lf%n", &x1, &x2, &y1, &y2,
                   &z1, &z2, &dens, &nchars);
    /* Check if there are extra characters in the line. This indicates
     * that the model is wrong or was generated by tess2prism without the
     * --flatten flag */    
    if(nread != 7 || str[nchars] != '\0')
    {
        return 1;
    }
    prism->x1 = x1;
    prism->x2 = x2;
    prism->y1 = y1;
    prism->y2 = y2;
    prism->z1 = z1;
    prism->z2 = z2;
    prism->density = dens;
    return 0;
}


/* Read a single rectangular prism and the spherical coordinates of its top
from a string */
int gets_prism_sph(const char *str, PRISM *prism)
{
    double dx, dy, dz, dens, lon, lat, r;
    int nread, nchars;

    nread = sscanf(str, "%lf %lf %lf %lf %lf %lf %lf%n",
                   &dx, &dy, &dz, &dens, &lon, &lat, &r, &nchars);
    /* Check if there are extra characters in the line. This indicates
     * that the model is wrong or was generated by tess2prism without the
     * --flatten flag */    
    if(nread != 7 || str[nchars] != '\0')
    {
        return 1;
    }
    prism->x1 = -0.5*dx;
    prism->x2 = 0.5*dx;
    prism->y1 = -0.5*dy;
    prism->y2 = 0.5*dy;
    prism->z1 = 0.;
    prism->z2 = dz;
    prism->density = dens;
    prism->lon = lon;
    prism->lat = lat;
    prism->r = r;
    return 0;
}


/* Read rectangular prisms from an open file and store them in an array. */
PRISM * read_prism_model(FILE *modelfile, int pos, int *size)
{
    PRISM *model, *tmp;
    int buffsize = 100, line, badinput = 0, error_exit = 0;
    char sbuff[10000];

    /* Start with a single buffer allocation and expand later if necessary */
    model = (PRISM *)malloc(buffsize*sizeof(PRISM));
    if(model == NULL)
    {
        log_error("problem allocating initial memory to load prism model");
        return NULL;
    }
    *size = 0;
    for(line = 1; !feof(modelfile); line++)
    {
        if(fgets(sbuff, 10000, modelfile) == NULL)
        {
            if(ferror(modelfile))
            {
                log_error("problem encountered reading line %d", line);
                error_exit = 1;
                break;
            }
        }
        else
        {
            /* Check for comments and blank lines */
            if(sbuff[0] == '#' || sbuff[0] == '\r' || sbuff[0] == '\n')
            {
                continue;
            }
            if(*size == buffsize)
            {
                buffsize += buffsize;
                tmp = (PRISM *)realloc(model, buffsize*sizeof(PRISM));
                if(tmp == NULL)
                {
                    /* Need to free because realloc leaves unchanged in case of
                       error */
                    free(model);
                    log_error("problem expanding memory for prism model");
                    return NULL;
                }
                model = tmp;
            }
            /* Remove any trailing spaces or newlines */
            strstrip(sbuff);
            /* Check if r, lon, lat are expected as well */
            if(pos)
            {
                if(gets_prism_sph(sbuff, &model[*size]))
                {
                    log_warning(
                    "bad/invalid prism at line %d.\nMaybe missing lon, lat, r?",
                    line);
                    badinput = 1;
                    continue;
                }                
            }
            else
            {
                if(gets_prism(sbuff, &model[*size]))
                {
                    log_warning("bad/invalid prism at line %d", line);
                    badinput = 1;
                    continue;
                }
            }
            (*size)++;
        }
    }
    if(badinput || error_exit)
    {
        free(model);
        return NULL;
    }
    /* Adjust the size of the model */
    tmp = (PRISM *)realloc(model, (*size)*sizeof(PRISM));
    if(tmp == NULL)
    {
        /* Need to free because realloc leaves unchanged in case of
            error */
        free(model);
        log_error("problem freeing excess memory for prism model");
        return NULL;
    }
    model = tmp;
    return model;
}
