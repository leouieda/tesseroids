.. _install:

Installation instructions
=========================

Pre-compiled binaries
---------------------

If you downloaded a pre-compiled binary distribution,
simply unpack in the desired directory.

The executables will be in the ``bin`` folder
and example scripts in the ``examples`` folder.

Compiling from source
---------------------

To build Tesseroids you'll need:

* A C compiler (like GCC_)
* SCons_

SCons
+++++

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
We recomment that you install GCC on Windows using MinGW_.

Compiling
+++++++++

To compile, type in a terminal (or ``cmd.exe`` on Windows)::

    scons

The executables will be placed on a ``bin`` folder.

To clean up the build, run::

    scons -c

This will delete all object files and executables.

.. _GCC: http://gcc.gnu.org
.. _SCons: http://www.scons.org/
.. _SCons website: http://www.scons.org/
.. _Python: http://www.python.org
.. _MinGW: http://mingw.org/
