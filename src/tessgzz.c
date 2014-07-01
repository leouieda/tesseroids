/*
Program to calculate gzz of a tesseroid model on a set of points.
*/


#include "constants.h"
#include "libtesseroid.h"
#include "tessg_main.h"


/** Main */
int main(int argc, char **argv)
{
    return run_tessg_main(argc, argv, "tessgzz", &tess_gzz,
                          TESSEROID_GZZ_SIZE_RATIO);
}
