/*
Print the default values of the constants used in the calculations.
*/


#include <stdio.h>
#include <string.h>
#include <time.h>
#include "version.h"
#include "logger.h"
#include "constants.h"


/** Print the help message */
void print_help()
{
    printf("Usage: tessdefaults [OPTIONS]\n\n");
    printf("Print default values of constants used.\n\n");
    printf("All units either SI or degrees!\n\n");
    printf("Output:\n");
    printf("  Constants are printed to standard output (stdout) in the form\n");
    printf("    CONST_NAME = VALUE\n");
    printf("  Lines that start with a # are treated as comments.\n\n");
    printf("Options:\n");
    printf("  -h         Print instructions.\n");
    printf("  --version  Print version and license information.\n");
    printf("\nPart of the Tesseroids package.\n");
    printf("Project site: <http://fatiando.org/software/tesseroids>\n");
    printf("Report bugs at: ");
    printf("<http://code.google.com/p/tesseroids/issues/list>\n");
}


/** Main */
int main(int argc, char **argv)
{
    char progname[] = "tessdefaults";
    int i, bad_args = 0;
    char *params;

    log_init(LOG_INFO);

    /* Parse arguments */
    for(i = 1; i < argc; i++)
    {
        if(argv[i][0] == '-')
        {
            switch(argv[i][1])
            {
                case 'h':
                    if(argv[i][2] != '\0')
                    {
                        log_error("invalid argument '%s'", argv[i]);
                        bad_args++;
                        break;
                    }
                    print_help();
                    return 0;
                case '-':
                {
                    params = &argv[i][2];
                    if(strcmp(params, "version"))
                    {
                        log_error("invalid argument '%s'", argv[i]);
                        bad_args++;
                    }
                    else
                    {
                        print_version(progname);
                        return 0;
                    }
                    break;
                }
                default:
                    log_error("invalid argument '%s'", argv[i]);
                    bad_args++;
                    break;
            }
        }
        else
        {
            log_error("invalid argument '%s'", argv[i]);
            bad_args++;
        }
    }
    /* Check if parsing went well */
    if(bad_args > 0)
    {
        log_error("%d bad input argument(s)", bad_args);
        log_warning("Terminating due to bad input");
        log_warning("Try '%s -h' for instructions", progname);
        return 1;
    }
    /* Print the constants from constants.c */
    printf("# Mean Earth radius (m)\n");
    printf("MEAN_EARTH_RADIUS = %.1f\n\n", MEAN_EARTH_RADIUS);
    printf("# Gravitational constant (m^3 kg^-1 s^-1)\n");
    printf("G = %.4g\n\n", G);
    printf("# Conversion factor from SI units to Eotvos s^-2 = 10^9 Eotvos\n");
    printf("SI2EOTVOS = %g\n\n", SI2EOTVOS);
    printf("# Conversion factor from SI units to mGal m s^-2} = 10^5 mGal\n");
    printf("SI2MGAL = %g\n\n", SI2MGAL);
    printf("# Just pi\n");
    printf("PI = %.31f\n\n", PI);
    printf("# Minimum distance/size ratio for computations to be\n");
    printf("# accurate. Used for knowing when to divide the tesseroids.\n");
    printf("TESSEROID_POT_SIZE_RATIO = %g\n", TESSEROID_POT_SIZE_RATIO);
    printf("TESSEROID_GX_SIZE_RATIO = %g\n", TESSEROID_GX_SIZE_RATIO);
    printf("TESSEROID_GY_SIZE_RATIO = %g\n", TESSEROID_GY_SIZE_RATIO);
    printf("TESSEROID_GZ_SIZE_RATIO = %g\n", TESSEROID_GZ_SIZE_RATIO);
    printf("TESSEROID_GXX_SIZE_RATIO = %g\n", TESSEROID_GXX_SIZE_RATIO);
    printf("TESSEROID_GXY_SIZE_RATIO = %g\n", TESSEROID_GXY_SIZE_RATIO);
    printf("TESSEROID_GXZ_SIZE_RATIO = %g\n", TESSEROID_GXZ_SIZE_RATIO);
    printf("TESSEROID_GYY_SIZE_RATIO = %g\n", TESSEROID_GYY_SIZE_RATIO);
    printf("TESSEROID_GYZ_SIZE_RATIO = %g\n", TESSEROID_GYZ_SIZE_RATIO);
    printf("TESSEROID_GZZ_SIZE_RATIO = %g\n\n", TESSEROID_GZZ_SIZE_RATIO);
    /* Print the constants from glq.c */
    printf("# Max iterations of the Legendre polynomial root-finder \
algorithm\n");
    printf("GLQ_MAXIT = %d\n\n", GLQ_MAXIT);
    printf("# Max error allowed for the Legendre polynomial root-finder \
algorithm\n");
    printf("GLQ_MAXERROR = %g\n", GLQ_MAXERROR);
    return 0;
}
