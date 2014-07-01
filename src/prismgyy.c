/*
Program to calculate gyy of a rectangular prism model on a set of points.
*/


#include "libprism.h"
#include "prismg_main.h"


/** Main */
int main(int argc, char **argv)
{
    return run_prismg_main(argc, argv, "prismgyy", &prism_gyy);
}
