/* This is a modified version of the MinUnit testing tool.

As with the original, you may use this code for any purpose, with the
understanding that it comes with NO WARRANTY.

The original can be found at http://www.jera.com/techinfo/jtns/jtn002.html

USAGE:
Simmply include minunit.h and implement the testing functions using the
assertions. To run the tests, call mu_run_test. It will run the tests, print
out the verbose and if the test passed or failed. Use mu_print_summary to
print out a summary of the tests (how many were run, how long it took, how many
failed and how many passed).

Ex:

#include "minunit.h"
#include "myfuncs.h"

int tests_run = 0, tests_passed = 0, tests_failed = 0;

char * test_func1()
{
    // Assert that func1 returns 3 when passed 1.2
    // assertion will return the fail message if fails
    mu_assert(func1(1.2) == 3, "func1 did not return 3 when passed 1.2");

    // If the assertion passed, return 0
    return 0;
}

int main()
{
    mu_run_test(test_func1, "testing func1");

    // If passed the time it took to run the tests will print it. Pass 0.0 to
    // ommit the time
    mu_print_summary(0.0);

    return 0;
}

Simply compile and run the test program.

Author: Leonardo Uieda
Date: 25 Jan 2011
*/

#ifndef _MINUNIT_H_
#define _MINUNIT_H_

#include <stdio.h>

/* Basic assertion. If fails, returns msg. If passes, returns 0 */
#define mu_assert(test, msg) do { if (!(test)) return msg; } while (0)


/* Assert if val equals expect within a certain precision prec */
#define mu_assert_almost_equals(val, expect, prec, msg) do { \
    if(!(val <= expect + prec && val >= expect - prec)) { return msg; }\
    } while (0)


/* Run a test case, print the verbose and check if passed or failed */
#define mu_run_test(test, verbose) \
    do { printf("\n%s... ", verbose); char *msg = test(); tests_run++; \
         if(msg){ tests_failed++; printf("FAIL:%s", msg); } \
         else { tests_passed++; printf("pass"); } \
    } while (0)

/* Print a summary of the tests ran and how long it took */
#define mu_print_summary(test_time) \
    printf("\n\n-----------------------------------------------------------"); \
    printf("\nRan %d test(s)", tests_run); \
    if(test_time) { printf(" in %g seconds", test_time); } \
    printf(". %d passed and %d failed.\n", tests_passed, tests_failed);


/* Utility for copying one array onto another */
#define mu_arraycp(original, copy, size) \
    do { int c; for(c=0; c<size; c++){ copy[c]=original[c]; } } while (0)

        
/* Global counters. WARNING: Don't forget to initialize in the main program
before running the tests! */
extern int tests_run, tests_passed, tests_failed;

#endif