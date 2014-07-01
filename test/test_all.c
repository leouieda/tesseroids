/*
Run all unit tests. This is compiled into a single executable (tesstest).
*/

#include <stdio.h>
#include <time.h>
#include "../src/logger.h"
#include "minunit.h"
#include "test_geometry.c"
#include "test_parsers.c"
#include "test_grav_prism.c"
#include "test_grav_tess.c"

int tests_run = 0, tests_passed = 0, tests_failed = 0;

int main()
{
    clock_t start = clock();
    int failed = 0;

    log_init(LOG_INFO);

    failed += geometry_run_all();
    failed += parsers_run_all();
    failed += grav_prism_run_all();
    failed += grav_tess_run_all();

    mu_print_summary((double)(clock() - start)/CLOCKS_PER_SEC);

    if(failed)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
