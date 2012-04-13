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
Print the default values of the constants used in the calculations.

@author Leonardo Uieda
@date 09 Feb 2011
*/


#include <stdio.h>
#include <string.h>
#include <time.h>
#include "version.h"
#include "logger.h"
#include "constants.h"
#include "glq.h"


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
    printf("Project site: <http://code.google.com/p/tesseroids>\n");
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
    printf("# Minimum distance-to-size ratio for computations to be\n");
    printf("# accurate. Used for knowing when to subdivide the tesseroids.\n");
    printf("TESSEROID_POT_SIZE_RATIO = %d\n\n", TESSEROID_POT_SIZE_RATIO);
    printf("TESSEROID_G_SIZE_RATIO = %d\n\n", TESSEROID_G_SIZE_RATIO);
    printf("TESSEROID_GG_SIZE_RATIO = %d\n\n", TESSEROID_GG_SIZE_RATIO);
    /* Print the constants from glq.c */
    printf("# Max iterations of the Legendre polynomial root-finder \
algorithm\n");
    printf("GLQ_MAXIT = %d\n\n", GLQ_MAXIT);
    printf("# Max error allowed for the Legendre polynomial root-finder \
algorithm\n");
    printf("GLQ_MAXERROR = %g\n", GLQ_MAXERROR);
    return 0;
}
