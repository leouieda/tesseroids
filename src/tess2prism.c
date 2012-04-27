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
Convert a tesseroid model into a prism model in spherical coordinates

@author Leonardo Uieda
@date 04 Feb 2011
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
    printf("Usage: tess2prim TESSFILE [OPTIONS]\n\n");
    printf("Convert a tesseroid model into a rectangular prism model\n");
    printf("(for use with the prismsh* programs).\n\n");
    printf("The converted prism have the same volume as the tesseroid.\n");
    printf("Along with each prism is given the spherical coordinates of the\n");
    printf("center of the top face of the tesseroid (used as the origin of\n");
    printf("the prisms coordinate system). The dimensions of the prism are \n");
    printf("measured with respect to this origin.\n\n");
    printf("To use the output with the prism* programs, use option\n");
    printf("--flatten which converts the tesseroids by approximating\n");
    printf("1 degree by 111.11km. Note that the prism will have the same\n");
    printf("volume as the tesseroid for this case.\n\n");
    printf("WARNING: The density of the prism is adjusted so that it has\n");
    printf("the same mass as the tesseroid (even when using --flatten).\n\n");
    printf("All units either SI or degrees!\n\n");
    printf("Input:\n");
    printf("  If TESSFILE is omited, will read from standard input (stdin)\n");
    printf("  TESSFILE: File containing the tesseroid model\n");
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
    printf("  * If a line starts with # it will be considered a comment\n");
    printf("    and will be ignored\n\n");
    printf("Output:\n");
    printf("  Printed to standard output (stdout) one prism per line in the\n");
    printf("  format:\n\n");
    printf("    x1 x2 y1 y2 z1 z2 density lon lat r\n\n");
    printf("  Comments about the provenance of the data are inserted into\n");
    printf("  the top of the output.\n\n");
    printf("  WARNING: If the option --flatten is used, lon lat and r are\n");
    printf("  not given.\n\n");
    printf("Options:\n");
    printf("  --flatten    Convert the tesseroids by approximating 1 degree\n");
    printf("               by 111.11 km (for compatibility with prism*\n");
    printf("               programs).\n");
    printf("  -h           Print instructions.\n");
    printf("  --version    Print version and license information.\n");
    printf("  -v           Enable verbose printing to stderr.\n");
    printf("  -l           FILENAME: Print log messages to file FILENAME.\n");
    printf("\nPart of the Tesseroids package.\n");
    printf("Project site: <http://code.google.com/p/tesseroids>\n");
    printf("Report bugs at: ");
    printf("<http://code.google.com/p/tesseroids/issues/list>\n");
}


/** Main */
int main(int argc, char **argv)
{
    char *progname = "tess2prism";
    TESS2PRISM_ARGS args;
    int rc, line, converted = 0, error_exit = 0, bad_input = 0;
    char buff[10000];
    TESSEROID tess;
    PRISM prism;
    FILE *logfile = NULL, *modelfile = NULL;
    time_t rawtime;
    struct tm * timeinfo;
    
    log_init(LOG_INFO);
    rc = parse_tess2prism_args(argc, argv, progname, &args, &print_help);
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

    /* Print provenance data to stdout */
    printf("# Prisms converted from tesseroid model with %s %s\n", progname,
           tesseroids_version);
    printf("#   local time: %s", asctime(timeinfo));
    printf("#   tesseroids file: %s\n", rc == 3 ? "stdin" : args.inputfname);
    printf("#   conversion type: %s\n",
            args.flatten ? "flatten" : "equal volume");
    if(args.flatten)
    {
        printf("#   format: x1 x2 y1 y2 z1 z2 density\n");
    }
    else
    {
        printf("#   format: x1 x2 y1 y2 z1 z2 density lon lat r\n");
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
                printf("%s", buff);
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
            if(args.flatten)
            {
                tess2prism_flatten(tess, &prism);
                printf("%.15g %.15g %.15g %.15g %.15g %.15g %.15g\n",
                       prism.x1, prism.x2, prism.y1, prism.y2, prism.z1,
                       prism.z2, prism.density);
            }
            else
            {
                tess2prism(tess, &prism);
                printf("%.15g %.15g %.15g %.15g %.15g %.15g %.15g %.15g %.15g %.15g\n",
                       prism.x1, prism.x2, prism.y1, prism.y2, prism.z1,
                       prism.z2, prism.density,
                       0.5*(tess.e + tess.w), 0.5*(tess.n + tess.s), tess.r2);
            }
            converted++;
        }
    }
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
        log_info("Converted %d tesseroids", converted);
    }
    /* Clean up */
    fclose(modelfile);    
    if(args.logtofile)
        fclose(logfile);    
    return 0;
}
