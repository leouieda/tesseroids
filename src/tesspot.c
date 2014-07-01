/*
Program to calculate potential of a tesseroid model on a set of points.
*/


#include "constants.h"
#include "libtesseroid.h"
#include "tessg_main.h"


/** Main */
int main(int argc, char **argv)
{
    return run_tessg_main(argc, argv, "tesspot", &tess_pot,
                          TESSEROID_POT_SIZE_RATIO);
}
