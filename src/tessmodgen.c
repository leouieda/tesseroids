/*
Generate tesseroid model from a regular grid.
*/


#include <stdio.h>
#include <time.h>
#include "version.h"
#include "parsers.h"
#include "logger.h"
#include "geometry.h"


/** Print the help message */
void print_help()
{
    printf("Usage: tessmodgen [ARGUMENTS] [OPTIONS]\n\n");
    printf("Generate a tesseroid model of an interface, like topography,\n");
    printf("moho, sediment thickness, etc.\n\n");
    printf("Each tesseroid has its top face centered of the respective grid\n");
    printf("point. The top and bottom of the tesseroid are define as:\n");
    printf("  * top = height of grid point and bottom = reference level\n");
    printf("    if height of grid point > reference level\n");
    printf("  * other way around if otherwise\n\n");
    printf("All units either SI or degrees!\n\n");
    printf("Input:\n");
    printf("  REGULAR grid passed through standard input (stdin).\n");
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
    printf("  -sDLON/DLAT The grid spacing in the longitude and latitude\n");
    printf("              directions, respectively, in DECIMAL DEGREES.\n");
    printf("              Will be used as the size of the tesseroids.\n");
    printf("              WARNING: You may get wrong results if -s is \n");
    printf("                       different from the grid spacing!\n");
    printf("  -dDENS      Density of the tesseroids. If ommited will expect\n");
    printf("              a 4th column on the input with DENS values for\n");
    printf("              each point. Tesseroids above the reference will\n");
    printf("              have density DENS, and bellow will have density\n");
    printf("              -DENS.\n");
    printf("  -zREF       Height of the reference level with respect to the\n");
    printf("              mean Earth radius. If bellow the mean Earth\n");
    printf("              radius, use a negative value.\n\n");
    printf("Options:\n");
    printf("  -h          Print instructions.\n");
    printf("  --version   Print version and license information.\n");
    printf("  -v          Enable verbose printing to stderr.\n");
    printf("  -lFILENAME  Print log messages to file FILENAME.\n");
    printf("\nPart of the Tesseroids package.\n");
    printf("Project site: <http://fatiando.org/software/tesseroids>\n");
    printf("Report bugs at: ");
    printf("<http://code.google.com/p/tesseroids/issues/list>\n");
}


/** Main */
int main(int argc, char **argv)
{
    char *progname = "tessmodgen";
    TESSMODGEN_ARGS args;
    int rc, line, error_exit = 0, bad_input = 0, size = 0, nchars, nread;
    char buff[10000];
    double lon, lat, height, w, e, s, n, top, bot, dens;
    FILE *logfile = NULL;
    time_t rawtime;
    struct tm * timeinfo;

    log_init(LOG_INFO);

    rc = parse_tessmodgen_args(argc, argv, progname, &args, &print_help);
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

    /* Print a header on the output with provenance information */
    printf("# Tesseroid model generated by %s %s:\n", progname,
           tesseroids_version);
    printf("#   local time: %s", asctime(timeinfo));
    printf("#   grid spacing (size of tesseroids): %g deg lon / %g deg lat\n",
        args.dlon, args.dlat);
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
                /* The lon != lon looks weird but this checks for NaN values */
                if(nread != 3 || buff[nchars] != '\0' ||
                   lon != lon || lat != lat || height != height)
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
                if(nread != 4 || buff[nchars] != '\0' ||
                   lon != lon || lat != lat || height != height || dens != dens)
                {
                    log_warning("bad/invalid grid point at line %d", line);
                    log_warning("skipping this line and continuing");
                    bad_input++;
                    continue;
                }
            }
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
            printf("%.15g %.15g %.15g %.15g %.15g %.15g %.15g\n", w, e, s, n,
                   top, bot, dens);
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
