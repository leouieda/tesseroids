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
Unit tests for GLQ functions.

Author: Leonardo Uieda
Date: 24 Jan 2011
***************************************************************************** */

#include <stdio.h>
#include "minunit.h"
#include "../src/c/glq.h"

int tests_run = 0, tests_passed = 0, tests_failed = 0;

char msg[1000];

static char *test_glq_nodes()
{
    int rc;
    double nodes[3];
    
    rc = glq_nodes(3, nodes);

    sprintf(msg, "failed with return code %d", rc);
    
    mu_assert(rc == 0, msg);

    return 0;
}


void glq_run_all()
{
    mu_run_test(test_glq_nodes, "glq_nodes produces correct results");
}

int main()
{
    glq_run_all();

    mu_print_summary(0.0);
    
    return 0;
}