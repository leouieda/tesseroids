/* This is a modified version of the MinUnit testing tool.

As with the original, you may use this code for any purpose, with the
understanding that it comes with NO WARRANTY.

The original can be found at http://www.jera.com/techinfo/jtns/jtn002.html

USAGE:



Author: Leonardo Uieda
Date: 25 Jan 2011
*/

#ifndef _MINUNIT_H_
#define _MINUNIT_H_

#include <stdio.h>

/* Basic assertion. If fails, returns message. If passes, returns 0 */
#define mu_assert(test, message) do { if (!(test)) return message; } while (0)

/* Run a test case, print the verbose and check if passed or failed */
#define mu_run_test(test, verbose) \
    do { printf("\n%s... ", verbose); char *message = test(); tests_run++; \
         if(message){ tests_failed++; printf("FAIL:%s", message); } \
         else { tests_passed++; printf("pass"); } \
    } while (0)

/* Print a summary of the tests ran and how long it took */
#define mu_print_summary(test_time) \
    printf("\n\n-----------------------------------------------------------"); \
    printf("\nRan %d test(s)", tests_run); \
    if(test_time) { printf(" in %g seconds", test_time); } \
    printf(". %d passed and %d failed.\n", tests_passed, tests_failed);

extern int tests_run, tests_passed, tests_failed;

#endif