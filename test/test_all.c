/* 
Run all unit tests. This is compiled into a single executable (tesstest).
*/

#include <stdio.h>
#include <time.h>
#include "../src/lib/logger.h"
#include "minunit.h"
#include "test_glq.c"
#include "test_geometry.c"
#include "test_parsers.c"
#include "test_grav_prism.c"
#include "test_grav_tess.c"

int tests_run = 0, tests_passed = 0, tests_failed = 0;

int main()
{    
    clock_t start = clock();
    
    log_init(LOG_DEBUG);
    
    glq_run_all();
    geometry_run_all();
    parsers_run_all();
    grav_prism_run_all();
    grav_tess_run_all();
    
    mu_print_summary((double)(clock() - start)/CLOCKS_PER_SEC);

    return 0;
}
