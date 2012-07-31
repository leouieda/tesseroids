/*
Program to calculate gyy of a tesseroid model on a set of points.
*/


#include "constants.h"
#include "grav_tess.h"
#include "tessg_main.h"


/** Main */
int main(int argc, char **argv)
{
    return run_tessg_main(argc, argv, "tessgyy", &tess_gyy,
                          TESSEROID_GYY_SIZE_RATIO);
}
