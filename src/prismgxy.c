/*
Program to calculate gxy of a rectangular prism model on a set of points.
*/


#include "grav_prism.h"
#include "prismg_main.h"


/** Main */
int main(int argc, char **argv)
{
    return run_prismg_main(argc, argv, "prismgxy", &prism_gxy);
}
