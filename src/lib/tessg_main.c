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
Generic main function for the tessg* programs.

@author Leonardo Uieda
@date 03 Feb 2011
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
#include "parsers.h"
#include "tessg_main.h"


/* Print the help message for tessg* programs */
void print_tessg_help(const char *progname)
{
    printf("Usage: %s MODELFILE [OPTIONS]\n\n", progname);
    if(strcmp(progname + 4, "pot") == 0)
    {
        printf("Calculate the potential due to a tesseroid model on\n");
    }
    else
    {
        printf("Calculate the %s component due to a tesseroid model on\n",
               progname + 4);
    }
    printf("specified observation points.\n\n");
    printf("Values are calculated in the local coordinate system of the\n");
    printf("observation point: x-> North  y-> East  z-> Up (away from the\n");
    printf("center of the Earth).\n");
    printf("In order to maintain mainstream convention, component gz is\n");
    printf("calculated with z-> Down.\n\n");
    printf("All units either SI or degrees!\n\n");
    printf("The computation of the gravitational effect of the tesseroids\n");
    printf("is done using the Gauss-Legendre Quadrature (GLQ) numerical\n");
    printf("integration method.\n\n");
    printf("WARNING: Avoid computing directly on top or inside the\n");
    printf("         tesseroids! This will break the GLQ and the formulas!\n");
    printf("\n");
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
    printf("  together.\n");
    printf("  * Comments about the provenance of the data are inserted into\n");
    printf("    the top of the output\n\n");
    printf("MODELFILE: File containing the tesseroid model\n");
    printf("  * Each tesseroid is specified by the values of its borders\n");
    printf("    and density\n");
    printf("  * The file should contain one tesseroid per line\n");
    printf("  * Each line should have the following column format:\n");
    printf("      West East South North Top Bottom Density\n");
    printf("  * Top and Bottom should be read as 'height to top' and \n");
    printf("    'height to bottom' from the mean Earth radius. Use negative\n");
    printf("    values if bellow the surface, for example when modeling\n");
    printf("    deep structures, and positive if above the surface, for\n");
    printf("    example when modeling topography.\n");
    printf("  * If a line starts with # it will be considered a comment and\n");
    printf("    will be ignored.\n\n");
    printf("Options:\n");
    printf("  -a           Disable the automatic subdividing of tesseroids.\n");
    printf("               Subdividing is done to ensure the GLQ gives\n");
    printf("               accurate results. Only use this option if you\n");
    printf("               know what you are doing!\n");
    printf("  -o           LONORDER/LATORDER/RORDER: the GLQ order to use\n");
    printf("               in the longitudinal, latitudinal and radial\n");
    printf("               integrations, respectively. Defaults to 2/2/2.\n");
    printf("               Subdividing of tesseroids works best with the\n");
    printf("               default order.\n");
    printf("  -h           Print instructions.\n");
    printf("  --version    Print version and license information.\n");
    printf("  -v           Enable verbose printing to stderr.\n");
    printf("  -l           FILENAME: Print log messages to file FILENAME.\n");
    printf("\nPart of the Tesseroids package.\n");
    printf("Project site: <http://code.google.com/p/tesseroids>\n");
    printf("Report bugs at: ");
    printf("<http://code.google.com/p/tesseroids/issues/list>\n");
}


/* Run the main for a generic tessg* program */
int run_tessg_main(int argc, char **argv, const char *progname,
    double (*field)(TESSEROID, double, double, double, GLQ, GLQ, GLQ),
    int ratio)
{
    TESSG_ARGS args;
    GLQ *glq_lon, *glq_lat, *glq_r;
    TESSEROID *model;
    int modelsize, rc, line, points = 0, error_exit = 0, bad_input = 0;
    char buff[10000];
    double lon, lat, height, res;
    FILE *logfile = NULL, *modelfile = NULL;
    time_t rawtime;
    clock_t tstart;
    struct tm * timeinfo;
    
    log_init(LOG_INFO);

    rc = parse_tessg_args(argc, argv, progname, &args, &print_tessg_help);
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
        log_tofile(logfile, LOG_DEBUG);
    }

    /* Print standard verbose */
    log_info("%s (Tesseroids project) %s", progname, tesseroids_version);
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    log_info("(local time) %s", asctime(timeinfo));

    /* Make the necessary GLQ structures */
    log_info("Using GLQ orders: %d lon / %d lat / %d r", args.lon_order,
             args.lat_order, args.r_order);
    glq_lon = glq_new(args.lon_order, -1, 1);
    glq_lat = glq_new(args.lat_order, -1, 1);
    glq_r = glq_new(args.r_order, -1, 1);
    if(glq_lon == NULL || glq_lat == NULL || glq_r == NULL)
    {
        log_error("failed to create required GLQ structures");
        log_warning("Terminating due to bad input");
        log_warning("Try '%s -h' for instructions", progname);
        if(args.logtofile)
            fclose(logfile);
        return 1;
    }

    /* Read the tesseroid model file */
    log_info("Reading tesseroid model from file %s", args.modelfname);
    modelfile = fopen(args.modelfname, "r");
    if(modelfile == NULL)
    {
        log_error("failed to open model file %s", args.modelfname);
        log_warning("Terminating due to bad input");
        log_warning("Try '%s -h' for instructions", progname);
        if(args.logtofile)
            fclose(logfile);
        return 1;
    }
    model = read_tess_model(modelfile, &modelsize);
    fclose(modelfile);
    if(modelsize == 0 || model == NULL)
    {
        log_error("failed to read model from file %s", args.modelfname);
        log_warning("Terminating due to bad input");
        log_warning("Try '%s -h' for instructions", progname);
        if(args.logtofile)
            fclose(logfile);
        return 1;
    }
    log_info("Total of %d tesseroid(s) read", modelsize);

    /* Print a header on the output with provenance information */
    if(strcmp(progname + 4, "pot") == 0)
    {
        printf("# Potential calculated with %s %s:\n", progname,
               tesseroids_version);
    }
    else
    {
        printf("# %s component calculated with %s %s:\n", progname+4, progname,
               tesseroids_version);
    }
    printf("#   local time: %s", asctime(timeinfo));
    printf("#   model file: %s (%d tesseroids)\n", args.modelfname, modelsize);
    printf("#   GLQ order: %d lon / %d lat / %d r\n", args.lon_order,
           args.lat_order, args.r_order);
    printf("#   Use adaptative algorithm: %s\n",
           args.adaptative ? "True" : "False");

    /* Read each computation point from stdin and calculate */
    log_info("Calculating (this may take a while)...");
    tstart = clock();
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
                printf("%s", buff);
                continue;
            }
            if(sscanf(buff, "%lf %lf %lf", &lon, &lat, &height) != 3)
            {
                log_warning("bad/invalid computation point at line %d", line);
                log_warning("skipping this line and continuing");
                bad_input++;
                continue;
            }
            /* Need to remove \n and \r from end of buff first to print the
               result in the end */
            strstrip(buff);
            if(args.adaptative)
            {
                res = calc_tess_model_adapt(model, modelsize, lon, lat,
                                            height + MEAN_EARTH_RADIUS, glq_lon,
                                            glq_lat, glq_r, field, ratio);
            }
            else
            {
                res = calc_tess_model(model, modelsize, lon, lat,
                                      height + MEAN_EARTH_RADIUS, glq_lon,
                                      glq_lat, glq_r, field);
            }
            printf("%s %.15g\n", buff, res);
            points++;
        }
    }
    if(bad_input)
    {
        log_warning("Encountered %d bad computation points which were skipped",
                    bad_input);
    }
    if(error_exit)
    {
        log_warning("Terminating due to error in input");
        log_warning("Try '%s -h' for instructions", progname);
    }
    else
    {
        log_info("Calculated on %d points in %.5g seconds", points,
                 (double)(clock() - tstart)/CLOCKS_PER_SEC);
    }
    /* Clean up */
    free(model);
    glq_free(glq_lon);
    glq_free(glq_lat);
    glq_free(glq_r);
    log_info("Done");    
    if(args.logtofile)
        fclose(logfile);
    return 0;
}
