WHAT IS Tesseroids?

Tesseroids is a software package for the direct modeling of gravitational fields in spherical coordinates.
The input models are consisted of spherical prisms, also called tesseroids.
The fields currently supported are potential, three-component acceleration, and full gradient tensor.

This project is coded in pure Python for ease of programming and testing.
If needed, further optimizations might be implemented in C/C++.
The package is composed of many individual programs, each for calculating a specific gravitational field (10 in total).
Input parameters are made through command line arguments and a model file.


IMPLEMENTATION

The integral equations describing the fields are solved using a Gauss-Legendre Quadrature (GLQ) rule.
There are two ways of solving the integral:
    * using the GLQ on the 3D integral;
    * integrating analytically in the r direction and then using GLQ on the 2D integral;

The second approach is faster because it eliminates one loop from the calculation.
The disadvantage is that a singularity appears when the computation point is aligned with the integration point.
To overcome this problem a combination of the two algorithms is used:
    * used the 2D GLQ on all points that have no singularity;
    * if a singularity occurs, use the 3D GLQ to solve the integral;


DOWNLOAD AND INSTALLATION

The source files can be downloaded from the Downloads tab in http://code.google.com/p/tesseroids/
Choose one of the compressed files and unpack it anywhere.

This program requires Python 2.5 or greater (not Python 3!).
The Python interpreter can be downloaded from http://www.python.org/download/

Some of the tests require Numpy, Scipy and Matplotlib.
All these packages can be freely downloaded from the respective websites.
Though Tesseroids does not require these packages to run, it is recommended to have them for testing purposes.

Included in the tests directory are the unit tests, doc tests and other tests for the program.
To run all tests simply execute the Makefile in the directory.
Note that these require the packages listed above!


USAGE

The user interface is done via command line.
There are a number of parameters to be passed to the each program along with the model file.
Every program displays a help message when run with the command -h (or --help).
This message explains how the model file should be formated, how the output is done, and what are the options available.

The output is made in SI units for the potential, mGal for the gravity vector, and Eotvos for the gravity gradient.


EXAMPLE

To calculate the ZZ component of the gravity gradient of the model 'model.txt' on a grid W/E/S/N = -3/3/42/48, using 0.1 degrees spacing, at computation height 250km, and GLQ order 2x2x2 (default):

   tessgzz model.txt -r-3 3 42 48 -b0.1 0.1 -e250000 > output.txt

The same can be done for all other components.

If the model is a tesseroid of 1° x 1° x 10km with density contrast of 200 kg/m³, and centered at 0° longitude and 45° latitude with the top at the surface, the model file should be:

-0.5 0.5 44.5 45.5 0 10000 200