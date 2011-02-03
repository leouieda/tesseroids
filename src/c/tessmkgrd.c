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
#include <time.h>
#include "logger.h"
#include "version.h"
#include "cmd.h"

/** Main */
int main(int argc, char **argv)
{
    log_init(LOG_INFO);

    TESSMKGRD_ARGS args;
    char progname[] = "tessmkgrd";

    int rc = parse_tessmkgrd_args(argc, argv, &args);

    if(rc == 2)
    {
        return 0;
    }
    if(rc == 1)
    {
        log_warning("Terminating due to bad input");
        log_warning("Try '%s -h' for instructions", progname);
        return 1;
    }

    /* Set the appropriate logging level and log to file if necessary */
    if(!args.verbose) { log_init(LOG_WARNING); }
    
    FILE *logfile;
    if(args.logtofile)
    {
        logfile = fopen(args.logfname, "w");
        if(!logfile)
        {
            log_error("unable to create log file %s\n", args.logfname);
            log_warning("Terminating due to bad input");
            log_warning("Try '%s -h' for instructions", progname);
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