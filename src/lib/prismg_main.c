/*
Generic main function for the prismg* programs.
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "logger.h"
#include "version.h"
#include "grav_prism.h"
#include "geometry.h"
#include "parsers.h"
#include "prismg_main.h"


char global_progname[100];


/* Print the help message  */
void print_help()
{
    printf("Usage: %s MODELFILE [OPTIONS]\n\n", global_progname);
    if(strcmp(global_progname + 5, "pot") == 0)
    {
        printf("Calculate the potential due to a rectangular prism model on\n");
    }
    else
    {
        printf("Calculate the %s component due to a rectangular prism model on\n",
                global_progname + 5);
    }
    printf("specified observation points using Cartesian coordinates.\n\n");
    printf("All input units are SI! Output is SI, mGal or Eotvos.\n\n");
    printf("Coordinates:\n");
    printf("  The coordinate system for the prism is x->North, y->East\n");
    printf("  and z->Down\n\n");
    printf("Input:\n");
    printf("  Computation points passed through standard input (stdin).\n");
    printf("  Reads 3 or more values per line and inteprets the first 3 as:\n");
    printf("    Easting (y), Northing (x) and height coordinates of a\n");
    printf("  computation point. Other values in the line are ignored.\n");
    printf("  Lines that start with # are ignored as comments.\n");
    printf("  Lines should be no longer than 10000 (ten thousand) characters.");
    printf("  \n\n");
    printf("Output:\n");
    printf("  Printed to standard output (stdout) in the form:\n");
    printf("    y x height ... result\n");
    printf("  ... represents any values that were read from input and\n");
    printf("  ignored. In other words, the result is appended to the last\n");
    printf("  column of the input. Use this to pipe prismg* programs\n");
    printf("  together.\n\n");
    printf("  * Comments about the provenance of the data are inserted into\n");
    printf("    the top of the output\n\n");
    printf("MODELFILE: File containing the prism model\n");
    printf("  * Each prism is specified by the values of its borders\n");
    printf("    and density\n");
    printf("  * The file should contain one prism per line\n");
    printf("  * Each line should have the following column format:\n");
    printf("      X1 X2 Y1 Y2 Z1 Z2 Density\n");
    printf("  * If calculating on spherical coordinates (option -g),\n");
    printf("    the format should be:\n");
    printf("      DX DY DZ Density lon lat r\n");
    printf("    lon, lat, r are the spherical coordinates of the center of\n");
    printf("    the top face of the prism. This point is the origin of the\n");
    printf("    coordinate system defining the prism. Thus, X1, X2, etc,\n");
    printf("    refer to this system (i.e., X1=-X2, Y1=-Y2, and Z1=0).\n");
    printf("    \n");
    printf("  * If a line starts with # it will be considered a comment and\n");
    printf("    will be ignored.\n\n");
    printf("Options:\n");
    printf("  -h           Print instructions.\n");
    printf("  --version    Print version and license information.\n");
    printf("  -v           Enable verbose printing to stderr.\n");
    printf("  -l           FILENAME: Print log messages to file FILENAME.\n");
    printf("\nPart of the Tesseroids package.\n");
    printf("Project site: <http://code.google.com/p/tesseroids>\n");
    printf("Report bugs at: ");
    printf("<http://code.google.com/p/tesseroids/issues/list>\n");
}


/* Run the main for a generic prismg* program */
int run_prismg_main(int argc, char **argv, const char *progname,
                    double (*field)(PRISM, double, double, double))
{
    BASIC_ARGS args;
    PRISM *model;
    int modelsize, rc, line, points = 0, error_exit = 0, bad_input = 0, i;
    char buff[10000];
    double x, y, height, res;
    FILE *logfile = NULL, *modelfile = NULL;
    time_t rawtime;
    clock_t tstart;
    struct tm * timeinfo;
    
    log_init(LOG_INFO);
    strcpy(global_progname, progname);
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
    model = read_prism_model(modelfile, 0, &modelsize);
    fclose(modelfile);
    if(modelsize == 0 || model == NULL)
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
    if(strcmp(progname + 5, "pot") == 0)
    {
        printf("# Potential calculated with %s %s:\n", progname,
               tesseroids_version);
    }
    else
    {
        printf("# %s component calculated with %s %s:\n", progname+5, progname,
               tesseroids_version);
    }
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
            if(sscanf(buff, "%lf %lf %lf", &y, &x, &height) != 3)
            {
                log_warning("bad/invalid computation point at line %d", line);
                log_warning("skipping this line and continuing");
                bad_input++;
                continue;
            }
            /* Need to remove \n and \r from end of buff first to print the
               result in the end */
            strstrip(buff);
            for(res = 0, i = 0; i < modelsize; i++)
            {
                res += field(model[i], x, y, -height);
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
    log_info("Done");
    if(args.logtofile)
        fclose(logfile);
    return 0;
}
