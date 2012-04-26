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
Calculate the mass of a tesseroid model.

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
    printf("Usage: tessmass TESSFILE [OPTIONS]\n\n");
    printf("Calculate the mass of a tesseroid model.\n\n");
    printf("All units either SI or degrees!\n\n");
    printf("Input:\n");
    printf("  If TESSFILE is omited, will read from standard input (stdin)\n");
    printf("  TESSFILE: File containing the tesseroid model\n");
    printf("   * Each tesseroid is specified by the values of its borders\n");
    printf("     and density\n");
    printf("   * The file should contain one tesseroid per line\n");
    printf("   * Each line should have the following column format:\n");
    printf("       West East South North Top Bottom Density\n");
    printf("   * Top and Bottom should be read as 'depth to top' and \n");
    printf("     'depth to bottom' from the mean Earth radius. Use negative\n");
    printf("     values if above the surface, for example when modeling\n");
    printf("     topography\n");
    printf("   * If a line starts with # it will be considered a comment\n");
    printf("     and will be ignored\n\n");
    printf("Output:\n");
    printf("  Printed to standard output (stdout) in same units as input\n\n");
    printf("Options:\n");
    printf("  -r         LOW/HIGH: only take into account tesseroids with\n");
    printf("                       density between LOW and HIGH\n");
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
    char *progname = "tessmass";
    TESSMASS_ARGS args;
    TESSEROID tess;
    double mass = 0;
    int rc, line, size = 0, error_exit = 0, bad_input = 0;
    FILE *logfile = NULL, *modelfile = NULL;
    time_t rawtime;
    struct tm * timeinfo;
    char buff[10000];
    
    log_init(LOG_INFO);
    
    rc = parse_tessmass_args(argc, argv, progname, &args, &print_help);
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
    if(!args.verbose)
    {
        log_init(LOG_WARNING);
    }
    if(args.logtofile)
    {
        logfile = fopen(args.logfname, "w");
        if(logfile == NULL)
        {
            log_error("unable to create log file %s", args.logfname);
            log_warning("Terminating due to bad input");
            log_warning("Try '%s -h' for instructions", progname);
            return 1;
        }
        log_tofile(logfile, LOG_INFO);
    }

    /* Print standard verbose */
    log_info("%s (Tesseroids project) %s", progname, tesseroids_version);
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    log_info("(local time) %s", asctime(timeinfo));

    /* If an input file is not given, read from stdin. Else open the file */
    if(rc == 3)
    {
        log_info("Reading tesseroids from stdin");
        modelfile = stdin;
    }
    else
    {
        log_info("Reading tesseroids from file %s", args.inputfname);
        modelfile = fopen(args.inputfname, "r");
        if(modelfile == NULL)
        {
            log_error("failed to open file %s", args.inputfname);
            log_warning("Terminating due to bad input");
            log_warning("Try '%s -h' for instructions", progname);
            if(args.logtofile)
                fclose(logfile);
            return 1;
        }
    }

    /* Read the tesseroids, convert and print to stdout */
    for(line = 1; !feof(modelfile); line++)
    {
        if(fgets(buff, 10000, modelfile) == NULL)
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
            /* Remove any trailing spaces or newlines */
            strstrip(buff);
            if(gets_tess(buff, &tess))
            {
                log_warning("bad/invalid tesseroid at line %d", line);
                bad_input++;
                continue;
            }
            if(args.use_range)
            {
                mass += tess_range_mass(&tess, 1, args.low_dens,
                                        args.high_dens);
                size++;
            }
            else
            {
                mass += tess_total_mass(&tess, 1);
                size++;
            }
        }
    }
    if(args.use_range)
    {
        log_info("Mass within density range %g/%g:", args.low_dens,
                 args.high_dens);
    }
    else
    {
        log_info("Total mass:");
    }
    
    printf("%.15g\n", mass);
    
    if(bad_input)
    {
        log_warning("Encountered %d bad input line(s) which were skipped",
                    bad_input);
    }
    if(error_exit)
    {
        log_warning("Terminating due to error in input");
        log_warning("Try '%s -h' for instructions", progname);
    }
    else
    {
        log_info("Mass calculated from %d tesseroids", size);
    }

    /* Clean up */
    if(rc != 3)
        fclose(modelfile);
    if(args.logtofile)
        fclose(logfile);
    return 0;
}
