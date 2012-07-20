/*
Generate tesseroid model of a series of layers given their thickness.
*/


#include <stdio.h>
#include <time.h>
#include "version.h"
#include "parsers.h"
#include "logger.h"
#include "geometry.h"
#include "constants.h"


#define BUFFSIZE 1000

/** Print the help message */
void print_help()
{
    printf("Usage: tesslayers [ARGUMENTS] [OPTIONS]\n\n");
    printf("Generate a tesseroid model of a series of stacked layers.\n\n");
    printf("All units are either SI or degrees!\n\n");
    printf("Input:\n");
    printf("  Regular grids passed through standard input (stdin).\n");
    printf("  Grids should be in a single file in xyz format, i.e., in\n");
    printf("  columns:\n");
    printf("      lon lat height thickness1 dens1 thickness2 dens2 ...\n");
    printf("  lon and lat are the longitude and latitude of a grid point,\n");
    printf("  height is the top of the first layer at the grid point\n");
    printf("  (e.g., the topography or relief of the first layer),\n");
    printf("  height should be read as 'height above the mean Earth radius'\n");
    printf("  (if bellow the Earth radius use negative heights),\n");
    printf("  thickness1 is the thickness of the first layer,\n");
    printf("  dens1 is the density of the first layer, and so forth.\n\n");
    printf("  Layers MUST be ordered from top-most to bottom-most\n");
    printf("  (i.e., thickness1 is of the top layer).\n\n");
    printf("  Lines that start with # are ignored as comments.\n");
    printf("  Lines should be no longer than 10000 (ten thousand) characters.");
    printf("  \n\n");
    printf("Output:\n");
    printf("  Tesseroids that fill between the interfaces of the layers.\n");
    printf("  Tesseroids are printed to standard output (stdout.)\n");
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
    printf("  -sDLON/DLAT The grid spacing in the longitude and latitude\n");
    printf("              directions, respectively, in DECIMAL DEGREES.\n");
    printf("              Will be used as the size of the tesseroids.\n");
    printf("              WARNING: You may get wrong results if -s is \n");
    printf("                       different from the grid spacing!\n");
    printf("Options:\n");
    printf("  -h          Print instructions.\n");
    printf("  --version   Print version and license information.\n");
    printf("  -v          Enable verbose printing to stderr.\n");
    printf("  -lFILENAME  Print log messages to file FILENAME.\n");
    printf("\nPart of the Tesseroids package.\n");
    printf("Project site: <http://code.google.com/p/tesseroids>\n");
    printf("Report bugs at: ");
    printf("<http://code.google.com/p/tesseroids/issues/list>\n");
}


/** Main */
int main(int argc, char **argv)
{
    char *progname = "tesslayers";
    TESSLAYERS_ARGS args;
    TESSEROID tessbuff[BUFFSIZE];
    int t, rc, line, error_exit = 0, bad_input = 0, size = 0, nlayers_old = -1,
        nlayers_new;
    char buff[10000];
    FILE *logfile = NULL;
    time_t rawtime;
    struct tm * timeinfo;

    log_init(LOG_INFO);

    rc = parse_tesslayers_args(argc, argv, progname, &args, &print_help);
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
    log_info("Using grid spacing (size of tesseroids): %g lon / %g lat",
        args.dlon, args.dlat);

    /* Print a header on the output with provenance information */
    printf("# Tesseroid model generated by %s %s:\n", progname,
           tesseroids_version);
    printf("#   local time: %s", asctime(timeinfo));
    printf("#   grid spacing (size of tesseroids): %g deg lon / %g deg lat\n",
        args.dlon, args.dlat);

    /* Read each regular grid from stdin and generate the tesseroids */
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

            nlayers_new = gets_layers(buff, args.dlon, args.dlat, tessbuff,
                                      BUFFSIZE);
            if(nlayers_new == -1)
            {
                log_error("invalid input in line %d", line);
                error_exit++;
                break;
            }
            if(nlayers_old != -1 && nlayers_old != nlayers_new)
            {
                log_error("different number of layers in line %d than in previous lines",
                          line);
                error_exit++;
                break;
            }
            nlayers_old = nlayers_new;

            for(t = 0; t < nlayers_new; t++)
            {
                printf("%.15g %.15g %.15g %.15g %.15g %.15g %.15g\n",
                       tessbuff[t].w,
                       tessbuff[t].e,
                       tessbuff[t].s,
                       tessbuff[t].n,
                       tessbuff[t].r2 - MEAN_EARTH_RADIUS,
                       tessbuff[t].r1 - MEAN_EARTH_RADIUS,
                       tessbuff[t].density);
                size++;
            }
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
