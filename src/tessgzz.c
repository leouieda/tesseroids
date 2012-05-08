/*
Program to calculate gzz of a tesseroid model on a set of points.
*/


#include "constants.h"
#include "grav_tess.h"
#include "tessg_main.h"


/** Main */
int main(int argc, char **argv)
{
    return run_tessg_main(argc, argv, "tessgzz", &tess_gzz,
                          TESSEROID_GG_SIZE_RATIO);
}
