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
Program to calculate gz of a tesseroid model on a set of points.

@author Leonardo Uieda
@date 02 Feb 2011
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "logger.h"
#include "version.h"
#include "grav_tess.h"
#include "glq.h"
#include "constants.h"
#include "utils.h"
#include "cmd.h"


/** Main */
int main(int argc, char **argv)
{
    log_init(LOG_INFO);
    char progname[] = "tessgz";
    TESSG_ARGS args;

    int rc = parse_tessg_args(argc, argv, progname, &args);

    if(rc == 2)
    {
        return 0;
    }
    if(rc == 1)
    {
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

    /* Clean up */
    if(args.logtofile)
    {
        log_info("Closing log file");
        fclose(logfile);
    }

    log_info("Done");

    return 0;
}