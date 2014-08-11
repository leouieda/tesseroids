Tesseroids
==========

Forward modeling of gravitational fields in spherical coordinates
through the command-line.

Contains tools for modeling the gravitational potential, acceleration, and
gradient tensor. Mass models can be made of right rectangular prisms
or tesseroids_ (spherical prisms).

.. image:: http://img.shields.io/github/release/leouieda/tesseroids.svg?style=flat
    :target: https://github.com/leouieda/tesseroids/releases
    :alt: Latest version.
.. image:: http://img.shields.io/travis/leouieda/tesseroids.svg?style=flat
    :target: https://travis-ci.org/leouieda/tesseroids
    :alt: Travis CI build status.
.. image:: http://img.shields.io/badge/license-BSD-lightgrey.svg?style=flat
    :target: https://github.com/leouieda/tesseroids/blob/master/LICENSE.txt
    :alt: BSD 3 clause license.
.. image:: http://img.shields.io/badge/10.6084/m9.figshare.786514-blue.svg?style=flat
    :target: http://dx.doi.org/10.6084/m9.figshare.786514
    :alt: doi:10.6084/m9.figshare.786514

Developed by `Leonardo Uieda`_ in cooperation with `Carla Braitenberg`_.

.. _Leonardo Uieda: http://www.leouieda.com
.. _Carla Braitenberg: http://lithoflex.org/
.. _tesseroids: http://tesseroids.readthedocs.org/en/latest/theory.html#what-is-a-tesseroid-anyway

Documentation
-------------

You'll find the documentation for Tesseroids on Read the Docs:

http://tesseroids.readthedocs.org/

The docs contain installation instructions, usage, theoretical background,
and some examples.

Citing
------

*Tesseroids* is research software. If you use it in your research,
please **cite it** in your publications as:

    Uieda, L. (2013), Source code and compiled binaries for software
    Tesseroids, figshare, http://dx.doi.org/10.6084/m9.figshare.786514,
    doi:10.6084/m9.figshare.786514.

See the `CITATION.txt`_ file for more information.

.. _CITATION.txt: https://github.com/leouieda/tesseroids/blob/master/CITATION.txt

Downloading
-----------

You can download the source and binary distributions
from the `project site`_.

.. _project site: http://www.leouieda.com/tesseroids

Pre-compiled binaries
---------------------

If you downloaded a pre-compiled binary distribution,
simply unpack in the desired directory.

The executables will be in the ``bin`` folder
and example scripts in the ``cookbook`` folder.

Compiling from source
---------------------

To build Tesseroids you'll need:

* A C compiler (like GCC_)
* SCons_

Setting up SCons
++++++++++++++++

Tesseroids uses the build tool SCons_.
A ``SConstruct`` file (``Makefile`` equivalent)
is used to define the compilation rules.
You will have to download and install SCons
in order to easily compile Tesseroids.
SCons is available for both GNU/Linux and Windows.
Building should work the same on both platforms.

SCons requires that you have Python_ installed.
Check the `SCons website`_ for more information.
Python is usually installed by default on most GNU/Linux systems.

Under Windows you will have to put SCons on
your ``PATH`` environment variable
in order to use it from the command line.
It is usually located in the ``Scripts`` directory of your Python installation.

On GNU/Linux, SCons will generally use
the GCC_ compiler to compile sources.
On Windows it will search for an existing compiler.
We recommend that you install GCC on Windows using MinGW_.

.. _GCC: http://gcc.gnu.org
.. _SCons: http://www.scons.org/
.. _SCons website: http://www.scons.org/
.. _Python: http://www.python.org
.. _MinGW: http://mingw.org/

Compiling
+++++++++

First, download a source distribution.
Unpack the archive anywhere you want
(e.g., ``~/tesseroids`` or ``C:\tesseroids`` or whatever).
To compile,
go to the directory where you unpacked
(e.g., ``~/tesseroids`` etc.)
and type in a terminal (or ``cmd.exe`` on Windows)::

    scons

The executables will be placed on a ``bin`` folder.

To clean up the build, run::

    scons -c

This will delete all object files and executables.

Testing the build
+++++++++++++++++

After the compilation,
a program called ``tesstest``
will be placed in the directory where you unpacked the source.
This program runs all the `unit tests`_
in the ``test`` directory.
If all tests pass,
the compilation probably went well.
If any test fails,
please `submit a bug report`_ with the output of ``tesstest``.

.. _unit tests: https://en.wikipedia.org/wiki/Unit_testing
.. _submit a bug report: https://github.com/leouieda/tesseroids/issues
