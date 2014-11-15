# ![Tesseroids](https://raw.githubusercontent.com/leouieda/tesseroids/master/doc/_static/banner.png)

[Website](http://www.leouieda.com/tesseroids/) |
[Documentation](http://tesseroids.readthedocs.org/) |
[Download](https://github.com/leouieda/tesseroids/releases)

[![Travis CI build status](http://img.shields.io/travis/leouieda/tesseroids.svg?style=flat)](https://travis-ci.org/leouieda/tesseroids)
[![BSD license](http://img.shields.io/badge/license-BSD-lightgrey.svg?style=flat)](https://github.com/leouieda/tesseroids/blob/master/LICENSE.txt)
[![doi:10.6084/m9.figshare.786514](http://img.shields.io/badge/doi-10.6084/m9.figshare.786514-blue.svg?style=flat)](http://dx.doi.org/10.6084/m9.figshare.786514)

*Forward modeling of gravitational fields in spherical coordinates.*

Developed by [Leonardo Uieda](http://www.leouieda.com)
in cooperation with [Carla Braitenberg](http://lithoflex.org/).

## About

*Tesseroids* is a collection of **command-line tools**
for modeling the gravitational potential, acceleration, and
gradient (Marussi) tensor.
The mass models can be made of right rectangular prisms or tesseroids
(spherical prisms).
Computation for rectangular prisms can be made in Cartesian or spherical
(geocentric) coordinates.

[![This is a tesseroid.](https://raw.githubusercontent.com/leouieda/tesseroids/master/doc/_static/tesseroid.png)](http://tesseroids.readthedocs.org/en/latest/theory.html#what-is-a-tesseroid-anyway)

*Tesseroids* is written in C and made available under the terms of the
[BSD 3-clause license](https://github.com/leouieda/tesseroids/blob/master/LICENSE.txt).

## Citing

*Tesseroids* is research software.
If you use it in your research,
please **cite it** in your publications as:

    Uieda, L. (2013), Source code and compiled binaries for software
    Tesseroids, figshare, http://dx.doi.org/10.6084/m9.figshare.786514,
    doi:10.6084/m9.figshare.786514.

See the
[CITATION.txt](https://github.com/leouieda/tesseroids/blob/master/LICENSE.txt)
file for more information.

## Installing

The easiest way to install is to download the compiled binary archives
(e.g., `tesseroids-1.1-win32.zip`) from:

https://github.com/leouieda/tesseroids/releases

We offer binaries for Windows (32 bit only)
and GNU/Linux (32 and 64 bit).

Once downloaded, simply unpack the archive in the desired directory.
The executables will be in the `bin` folder.
For easier access to the programs, consider
[adding the bin folder to your PATH environment
variable](http://www.computerhope.com/issues/ch000549.htm).

## Getting started

Take a look at the examples in the
[Cookbook](http://tesseroids.readthedocs.org/en/latest/cookbook.html).
They contain scripts that run *Tesseroids* and some Python code to plot the
results.

The documentation contains sections on
[the theory and equations](http://tesseroids.readthedocs.org/en/latest/theory.html)
and [usage instructions](http://tesseroids.readthedocs.org/en/latest/usage.html).

Also, all programs accept the `-h` flag to print the instructions for using
that particular program. For example:

    $ tessgrd -h
    Usage: tessgrd [PARAMS] [OPTIONS]

    Make a regular grid of points.

    All units either SI or degrees!

    Output:
      Printed to standard output (stdout) in the format:
        lon1    lat1    height
        lon2    lat1    height
        ...     ...     ...
        lonNLON lat1    height
        lon1    lat2    height
        ...     ...     ...
        ...     ...     ...
        lonNLON latNLAT height

      * Comments about the provenance of the data are inserted into
        the top of the output

    Parameters:
      -r           W/E/S/N: Bounding region of the grid.
      -b           NLON/NLAT: Number of grid points in the
                   longitudinal and latitudinal directions.
      -z           HEIGHT: Height of the grid with respect to the
                   mean Earth radius.
      -h           Print instructions.
      --version    Print version and license information.

    Options:
      -v           Enable verbose printing to stderr.
      -lFILENAME   Print log messages to file FILENAME.

    Part of the Tesseroids package.
    Project site: <http://fatiando.org/software/tesseroids>
    Report bugs at: <http://code.google.com/p/tesseroids/issues/list>

## Compiling from source

If you want to build *Tesseroids* from source, you'll need:

* A C compiler (preferably [GCC](http://gcc.gnu.org))
* The build tool [SCons](http://www.scons.org/)

### Setting up SCons

Tesseroids uses the build tool SCons.
A `SConstruct` file (`Makefile` equivalent)
is used to define the compilation rules.
The advantage of SCons over Make is that it automatically detects your system
settings.
You will have to download and install SCons
in order to easily compile Tesseroids.
SCons is available for both GNU/Linux and Windows
so compiling should work the same on both platforms.

SCons requires that you have [Python](http://www.python.org) installed.
Follow the instructions in the [SCons website](http://www.scons.org/)
to install it.
Python is usually installed by default on most GNU/Linux systems.

Under Windows you will have to put SCons on
your `PATH` environment variable
in order to use it from the command line.
It is usually located in the `Scripts` directory of your Python installation.

On GNU/Linux, SCons will generally use
the GCC compiler to compile sources.
On Windows it will search for an existing compiler.
We recommend that you install GCC on Windows using
[MinGW](http://mingw.org/).

### Compiling

Download a source distribution and
unpack the archive anywhere you want
(e.g., `~/tesseroids` or `C:\tesseroids` or whatever).
To compile,
open a terminal (or `cmd.exe` on Windows)
and go to the directory where you unpacked (use the `cd` command).
Then, type the following and hit `Enter`:

    scons

If everything goes well, the compiled executables will be placed on a `bin`
folder.

To clean up the build (delete all generated files), run:

    scons -c

If you get any strange errors or the code doesn't compile for some reason,
please [submit a bug report](https://github.com/leouieda/tesseroids/issues).
Don't forget to copy the output of running `scons`.

### Testing the build

After the compilation,
a program called `tesstest`
will be placed in the directory where you unpacked the source.
This program runs the [unit tests](https://en.wikipedia.org/wiki/Unit_testing)
for *Tesseroids* (sources in the `test` directory).
If all tests pass,
the compilation probably went well.
If any test fail,
please [submit a bug report](https://github.com/leouieda/tesseroids/issues)
with the output of running `tesstest`.
