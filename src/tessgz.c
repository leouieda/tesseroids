/*
Program to calculate gz of a tesseroid model on a set of points.
*/


#include "constants.h"
#include "libtesseroid.h"
#include "tessg_main.h"


/** Main */
int main(int argc, char **argv)
{
    return run_tessg_main(argc, argv, "tessgz", &tess_gz,
                          TESSEROID_GZ_SIZE_RATIO);
}
