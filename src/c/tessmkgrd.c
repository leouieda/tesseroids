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
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "logger.h"
#include "version.h"

/** Store input arguments and option flags */
struct tessmkgrd_args
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
};


/** Print out the help instructions */
void print_help();


/** Parse command line arguments and execute */
int main(int argc, char **argv)
{
    log_init(LOG_INFO);

    int bad_args = 0, parsed_args = 0, total_args = 3;
    char progname[] = "tessmkgrd";
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
                        log_error("bad input argument -l. Missing filename.");
                        bad_args++;
                    }
                    else
                    {
                        args.logtofile = 1;
                        args.logfname = params;
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
                        log_error("bad input argument '%s'", argv[i]);
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
                        log_error("bad input argument '%s'", argv[i]);
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
                        log_error("bad input argument '%s'", argv[i]);
                        bad_args++;
                    }
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
    if(bad_args > 0 || parsed_args < total_args)
    {
        if(parsed_args < total_args)
        {
            log_info("%s: missing arguments. given %d out of %d.\n", progname,
                     parsed_args, total_args);
        }
        if(bad_args > 0)
        {
            log_info("%d bad input argument(s)", bad_args);
        }
        log_info("Terminating due to bad input");
        log_info("Try '%s -h' for instructions", progname);
        
        return 1;
    }

    /* Set the appropriate logging level and log to file if necessary */
    if(!args.verbose) { log_init(LOG_WARNING); }
    
    FILE *logfile;
    if(args.logtofile)
    {
        logfile = fopen(args.logfname, "w");
        if(logfile == NULL)
        {
            log_error("Unable to create log file %s\n");
            log_info("Terminating due to bad input");
            log_info("Try '%s -h' for instructions", progname);
            return 1;
        }
        log_tofile(logfile, LOG_INFO);
    }

    /* Print standard verbose */
    log_info("%s (Tesseroids project) %s", progname, tesseroids_version);
    
    time_t rawtime;
    struct tm * timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    log_info("(local time) %s", asctime(timeinfo));
    
    /* CREATE THE GRID AND PRINT IT TO STDOUT */
    log_info("Generating regular grid in region: %g W / %g E / %g S / %g N",
             args.w, args.e, args.s, args.n);
    log_info("Grid size: %d lon X %d lat = %d points in total", args.nlon,
             args.nlat, args.nlon*args.nlat);

    /* Define the grid spacing. used nlon or nlat -1 because the borders should
       be in the grid */
    double dlon = (args.e - args.w)/(args.nlon - 1),
           dlat = (args.n - args.s)/(args.nlat - 1);
    log_info("Grid spacing: %g longitude  %g latitude", dlon, dlat);

    /* Make the grid points. Print lon first as x */
    double lon, lat;
    /* Keep track of how many printed. Used to check if produced right amount */
    int lons = 0, lats = 0, total = 0;
    
    for(lat = args.s; lat <= args.n; lat += dlat)
    {
        lons = 0;
        for(lon = args.w; lon <= args.e; lon += dlon)
        {
            printf("%lf %lf %lf\n", lon, lat, args.height);
            lons++;
            total++;
        }

        /* Sometimes prints one less because of rounding errors */
        if(lons != args.nlon)
        {
            printf("%lf %lf %lf\n", lon, lat, args.height);
            lons++;
            total++;
        }

        lats++;

        printf("\n"); /* To ease plotting in Gnuplot */
    }

    /* Sometimes prints one less because of rounding errors */
    if(lats != args.nlat)
    {
        lons = 0;
        for(lon = args.w; lon <= args.e; lon += dlon)
        {
            printf("%lf %lf %lf\n", lon, lat, args.height);
            lons++;
            total++;
        }

        if(lons != args.nlon)
        {
            printf("%lf %lf %lf\n", lon, lat, args.height);
            lons++;
            total++;
        }
    }
    
    if(total != args.nlat*args.nlon)
    {
        log_warning("%d total points made instead of required %d", total,
                    args.nlat*args.nlon);
    }

    log_info("Total points generated: %d", total);

    /* Clean up */
    if(args.logtofile)
    {
        log_info("Closing log file");
        fclose(logfile);
    }

    log_info("Done");

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
    printf("\nOutput format:\n");
    printf("  lon1    lat1    height\n");
    printf("  lon2    lat1    height\n");
    printf("  ...     ...     ...\n");
    printf("  lonNLON lat1    height\n");
    printf("  lon1    lat2    height\n");
    printf("  ...     ...     ...\n");
    printf("  ...     ...     ...\n");
    printf("  lonNLON latNLAT height\n");
    printf("\n");
    printf("Part of the Tesseroids package.\n");
    printf("Project site: <http://code.google.com/p/tesseroids>\n");
    printf("Report bugs at: ");
    printf("<http://code.google.com/p/tesseroids/issues/list>\n");
}