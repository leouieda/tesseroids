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

/* *****************************************************************************
Run all unit tests

Author: Leonardo Uieda
Date: 24 Jan 2011
***************************************************************************** */

#include <stdio.h>
#include <time.h>
#include "../src/lib/logger.h"
#include "minunit.h"
#include "test_glq.c"
#include "test_utils.c"
#include "test_grav_prism.c"
#include "test_grav_tess.c"

int tests_run = 0, tests_passed = 0, tests_failed = 0;

int main()
{    
    clock_t start = clock();
    
    log_init(LOG_DEBUG);
    
    glq_run_all();
    utils_run_all();
    grav_prism_run_all();
    grav_tess_run_all();
    
    mu_print_summary((double)(clock() - start)/CLOCKS_PER_SEC);

    return 0;
}
