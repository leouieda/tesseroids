/*
Program to calculate gy of a tesseroid model on a set of points.
*/


#include "constants.h"
#include "grav_tess.h"
#include "tessg_main.h"


/** Main */
int main(int argc, char **argv)
{
    return run_tessg_main(argc, argv, "tessgy", &tess_gy,
                          TESSEROID_G_SIZE_RATIO);
}
