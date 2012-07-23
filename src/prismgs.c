/*
Program to calculate the potential of a rectangular prism model in spherical
coordinates.
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "logger.h"
#include "version.h"
#include "grav_prism_sph.h"
#include "geometry.h"
#include "constants.h"
#include "parsers.h"


/* Print the help message  */
void print_help()
{
    printf("Usage: prismgs MODELFILE [OPTIONS]\n\n");
    printf("Calculates the 3 component gravity vector due to a rectangular\n");
    printf("prism model on specified observation points using spherical\n");
    printf("coordinates.\n\n");
    printf("All input units are SI! Output is in mGal.\n\n");
    printf("Coordinate system:\n");
    printf("  The coordinate system used for the calculations is:\n");
    printf("    x->North, y->East, and z->Up\n");
    printf("In order to maintain mainstream convention, component gz is\n");
    printf("calculated with z-> Down.\n\n");
    printf("Input:\n");
    printf("  Computation points are passed through standard input (stdin).\n");
    printf("  Reads 3 or more values per line and inteprets the first 3 as:\n");
    printf("    longitude latitude height \n");
    printf("  longitude and latitude should be in decimal degrees, and\n");
    printf("  height in meters.\n");
    printf("  Other values in the line are ignored.\n");
    printf("  Lines that start with # are ignored as comments.\n");
    printf("  Lines should be no longer than 10000 (ten thousand) characters.");
    printf("  \n\n");
    printf("Output:\n");
    printf("  Printed to standard output (stdout) in the form:\n");
    printf("    lon lat height ... gx gy gz\n");
    printf("  ... represents any values that were read from input and\n");
    printf("  ignored. In other words, the result is appended to the last\n");
    printf("  column of the input. Use this to pipe prism* programs\n");
    printf("  together.\n\n");
    printf("  Comments about the provenance of the data are inserted into\n");
    printf("  the top of the output\n\n");
    printf("MODELFILE: File containing the prism model\n");
    printf("  * Each prism is specified by the values of its dimensions,\n");
    printf("    density, and spherical coordinates of the center of its\n");
    printf("    top.\n");
    printf("  * The file should contain one prism per line\n");
    printf("  * If a line starts with # it will be considered a comment and\n");
    printf("    will be ignored.\n");
    printf("  * Each line should have the following column format:\n");
    printf("      DX DY DZ Density lon lat r\n");
    printf("    This is the format output by tess2prism.\n\n");
    printf("Options:\n");
    printf("  -h           Print instructions.\n");
    printf("  --version    Print version and license information.\n");
    printf("  -v           Enable verbose printing to stderr.\n");
    printf("  -lFILENAME   Print log messages to file FILENAME.\n");
    printf("\nPart of the Tesseroids package.\n");
    printf("Project site: <http://fatiando.org/software/tesseroids>\n");
    printf("Report bugs at: ");
    printf("<http://code.google.com/p/tesseroids/issues/list>\n");
}


/* Run the main for a generic prismg* program */
int main(int argc, char **argv)
{
    BASIC_ARGS args;
    PRISM *model;
    int modelsize, rc, line, points = 0, error_exit = 0, bad_input = 0, i;
    char buff[10000];
    char progname[] = "prismgs";
    double lon, lat, height, gx, gy, gz, tmpx, tmpy, tmpz;
    FILE *logfile = NULL, *modelfile = NULL;
    time_t rawtime;
    clock_t tstart;
    struct tm * timeinfo;

    log_init(LOG_INFO);
    strcpy(progname, progname);
    rc = parse_basic_args(argc, argv, progname, &args, &print_help);
    if(rc == 3)
    {
        log_error("%s: missing input file", progname);
        log_warning("Terminating due to bad input");
        log_warning("Try '%s -h' for instructions", progname);
        return 1;
    }
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

    /* Read the model file */
    log_info("Reading prism model from file %s", args.inputfname);
    modelfile = fopen(args.inputfname, "r");
    if(modelfile == NULL)
    {
        log_error("failed to open model file %s", args.inputfname);
        log_warning("Terminating due to bad input");
        log_warning("Try '%s -h' for instructions", progname);
        if(args.logtofile)
            fclose(logfile);
        return 1;
    }
    model = read_prism_model(modelfile, 1, &modelsize);
    fclose(modelfile);
    if(modelsize == 0)
    {
        log_error("prism file %s is empty", args.inputfname);
        log_warning("Terminating due to bad input");
        log_warning("Try '%s -h' for instructions", progname);
        if(args.logtofile)
            fclose(logfile);
        return 1;
    }
    if(model == NULL)
    {
        log_error("failed to read model from file %s", args.inputfname);
        log_warning("Terminating due to bad input");
        log_warning("Try '%s -h' for instructions", progname);
        if(args.logtofile)
            fclose(logfile);
        return 1;
    }
    log_info("Total of %d prism(s) read", modelsize);

    /* Print a header on the output with provenance information */
    printf("# Gravity vector calculated in spherical coordinates with %s %s:\n",
        progname, tesseroids_version);
    printf("#   local time: %s", asctime(timeinfo));
    printf("#   model file: %s (%d prisms)\n", args.inputfname, modelsize);

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
            gx = 0;
            gy = 0;
            gz = 0;
            for(i = 0; i < modelsize; i++)
            {
                prism_g_sph(model[i], lon, lat, height + MEAN_EARTH_RADIUS,
                            &tmpx, &tmpy, &tmpz);
                gx += tmpx;
                gy += tmpy;
                gz += tmpz;
            }
            printf("%s %.15g %.15g %.15g\n", buff, gx, gy, gz);
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
    log_info("Done");
    if(args.logtofile)
        fclose(logfile);
    return 0;
}
