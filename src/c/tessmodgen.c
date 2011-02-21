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
Generate tesseroid model from a regular grid.

@author Leonardo Uieda
@date 09 Feb 2011
*/


#include <stdio.h>
#include <time.h>
#include "version.h"
#include "cmd.h"
#include "logger.h"
#include "utils.h"


/** Print the help message */
void print_help()
{
    printf("Usage: tessmodgen [ARGUMENTS] [OPTIONS]\n\n");
    printf("Generate a tesseroid model from a regular grid.\n\n");
    printf("Each tesseroid has its top face centered of the respective grid\n");
    printf("point. The top and bottom of the tesseroid are define as:\n");
    printf("  * top = height of grid point and bottom = reference level\n");
    printf("    if height of grid point > reference level\n");
    printf("  * other way around if otherwise\n\n");
    printf("All units either SI or degrees!\n\n");
    printf("Input:\n");
    printf("  Regular grid passed through standard input (stdin).\n");
    printf("  Reads 3 values per line: longitude latitude height\n");
    printf("  height should be read as 'height above the mean Earth radius'\n");
    printf("  If bellow the Earth radius use negative heights.\n");
    printf("  Lines that start with # are ignored as comments.\n");
    printf("  Lines should be no longer than 10000 (ten thousand) characters.");
    printf("  \n\n");
    printf("Output:\n");
    printf("  Tesseroids printed to standard output (stdout)\n");
    printf("  * Each tesseroid is specified by the values of its borders\n");
    printf("    and density\n");
    printf("  * Will print one tesseroid per line\n");
    printf("  * Each line has the following column format:\n");
    printf("       West East South North Top Bottom Density\n");
    printf("  * Top and Bottom should be read as 'height to top' and \n");
    printf("    'height to bottom' from the mean Earth radius. Use negative\n");
    printf("    values if bellow the surface, for example when modeling\n");
    printf("    deep structures, and positive if above the surface, for\n");
    printf("    example when modeling topography.\n");
    printf("  * If a line starts with # it will be considered a comment\n");
    printf("    and will be ignored\n\n");
    printf("Arguments:\n");
    printf("  -s  DLON/DLAT: grid spacing in the longitude and latitude\n");
    printf("                 directions, respectively. In DECIMAL DEGREES.\n");
    printf("  -d  DENS: density of the tesseroids. If ommited will expect a\n");
    printf("            4th column on the input with DENS values for each\n");
    printf("            point. Tesseroids above the reference will have\n");
    printf("            density DENS, and bellow will have density -DENS.\n");
    printf("  -z  REF: depth of the reference level with respect to the\n");
    printf("           mean Earth radius.\n\n");
    printf("Options:\n");
    printf("  -h         Print instructions.\n");
    printf("  --version  Print version and license information.\n");
    printf("  -v         Enable verbose printing to stderr.\n");
    printf("  -l         FILENAME: Print log messages to file FILENAME.\n");
    printf("\nPart of the Tesseroids package.\n");
    printf("Project site: <http://code.google.com/p/tesseroids>\n");
    printf("Report bugs at: ");
    printf("<http://code.google.com/p/tesseroids/issues/list>\n");
}


/** Main */
int main(int argc, char **argv)
{
    log_init(LOG_INFO);
    char *progname = "tessmodgen";
    TESSMODGEN_ARGS args;

    int rc = parse_tessmodgen_args(argc, argv, progname, &args, &print_help);

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
        if(logfile == NULL)
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

    /* Print a header on the output with provenance information */
    printf("# Tesseroid model generated by %s %s:\n", progname,
           tesseroids_version);
    printf("#   local time: %s", asctime(timeinfo));
    printf("#   grid spacing: %g deg lon / %g deg lat\n", args.dlon, args.dlat);
    printf("#   reference level (depth): %g\n", args.ref);
    if(args.fix_density)
    {
        printf("#   density: %g\n", args.dens);
        log_info("Using fixed density value: %g", args.dens);
    }
    else
    {
        printf("#   density: read from input\n");
        log_info("Reading density values from input grid");
    }

    /* Read each regular grid from stdin and generate the tesseroids */
    int line, error_exit = 0, bad_input = 0, size = 0;
    char buff[10000];
    double lon, lat, height, w, e, s, n, top, bot, dens;
    int nchars, nread;

    for(line = 1; !feof(stdin); line++)
    {
        if(fgets(buff, 10000, stdin) == NULL)
        {
            if(ferror(stdin))
            {
                log_error("problem encountered reading line %d", line);
                error_exit = 1;
                break;
            }
        }
        else
        {
            /* Check for comments and blank lines */
            if(buff[0] == '#' || buff[0] == '\r' || buff[0] == '\n')
            {
                continue;
            }

            strstrip(buff);

            if(args.fix_density)
            {
                nread = sscanf(buff, "%lf %lf %lf%n", &lon, &lat, &height,
                               &nchars);
                if(nread != 3 || buff[nchars] != '\0')
                {
                    log_warning("bad/invalid grid point at line %d", line);
                    log_warning("skipping this line and continuing");
                    bad_input++;
                    continue;
                }
            }
            else
            {
                nread = sscanf(buff, "%lf %lf %lf %lf%n", &lon, &lat, &height,
                               &dens, &nchars);
                if(nread != 4 || buff[nchars] != '\0')
                {
                    log_warning("bad/invalid grid point at line %d", line);
                    log_warning("skipping this line and continuing");
                    bad_input++;
                    continue;
                }
            }

            /* Need to remove \n and \r from end of buff first to print the
               result in the end */
            strstrip(buff);

            w = lon - 0.5*args.dlon;
            e = lon + 0.5*args.dlon;
            s = lat - 0.5*args.dlat;
            n = lat + 0.5*args.dlat;
            if(height >= args.ref)
            {
                top = height;
                bot = args.ref;
                if(args.fix_density)
                    dens = args.dens;
            }
            else
            {
                top = args.ref;
                bot = height;
                if(args.fix_density)
                    dens = -args.dens;
                else
                    dens *= -1;
            }

            printf("%g %g %g %g %g %g %g\n", w, e, s, n, top, bot, dens);
            size++;
        }
    }

    if(bad_input)
    {
        log_warning("Encountered %d bad grid points which were skipped",
                    bad_input);
    }

    if(error_exit)
    {
        log_warning("Terminating due to error in input");
        log_warning("Try '%s -h' for instructions", progname);
    }
    else
    {
        log_info("Generated %d tesseroids.", size);
    }

    /* Clean up */
    if(args.logtofile)
        fclose(logfile);

    return 0;
}