.. _install:

Installing
==========

We offer binaries for Windows (32 and 64 bit)
and GNU/Linux (32 and 64 bit).
You can download the latest version for your operating system from Github:

https://github.com/leouieda/tesseroids/releases/latest

Once downloaded, simply unpack the archive in the desired directory.
The executables will be in the ``bin`` folder.
For easier access to the programs, consider
`adding the bin folder to your PATH environment
variable <http://www.computerhope.com/issues/ch000549.htm>`__.

Tesseroids is permanently archived in `Zenodo <http://zenodo.org/>`__.
Each release is stored (source code and binaries) and given a
`DOI <http://www.doi.org/>`__.
The DOIs, source code, and compiled binaries for previous versions
can be found on the :ref:`Releases <releases>` page.

If we don't provide the binaries for your operating system,
you can compile the source code (download a source distribution from Github) by
following the instructions below.

Compiling from source
---------------------

If you want to build *Tesseroids* from source, you'll need:

* A C compiler (preferably `GCC <http://gcc.gnu.org>`__)
* The build tool `SCons <http://www.scons.org/>`__

Setting up SCons
++++++++++++++++

Tesseroids uses the build tool SCons.
A ``SConstruct`` file (``Makefile`` equivalent)
is used to define the compilation rules.
The advantage of SCons over Make is that it automatically detects your system
settings.
You will have to download and install SCons
in order to easily compile Tesseroids.
SCons is available for both GNU/Linux and Windows
so compiling should work the same on both platforms.

SCons requires that you have `Python <https://www.python.org>`__ installed.
Follow the instructions in the `SCons website <http://www.scons.org/>`__
to install it.
Python is usually installed by default on most GNU/Linux systems.

Under Windows you will have to put SCons on
your ``PATH`` environment variable
in order to use it from the command line.
It is usually located in the ``Scripts`` directory of your Python installation.

On GNU/Linux, SCons will generally use
the GCC compiler to compile sources.
On Windows it will search for an existing compiler.
We recommend that you install GCC on Windows using
`MinGW <http://mingw.org/>`__.

Compiling
+++++++++

Download a source distribution and
unpack the archive anywhere you want
(e.g., ``~/tesseroids`` or ``C:\tesseroids`` or whatever).
To compile,
open a terminal (or ``cmd.exe`` on Windows)
and go to the directory where you unpacked (use the ``cd`` command).
Then, type the following and hit ``Enter``:

    scons

If everything goes well, the compiled executables will be placed on a ``bin``
folder.

To clean up the build (delete all generated files), run:

    scons -c

If you get any strange errors or the code doesn't compile for some reason,
please `submit a bug report <https://github.com/leouieda/tesseroids/issues>`__.
Don't forget to copy the output of running ``scons``.

Testing the build
+++++++++++++++++

After the compilation,
a program called ``tesstest``
will be placed in the directory where you unpacked the source.
This program runs the `unit tests
<https://en.wikipedia.org/wiki/Unit_testing>`__
for *Tesseroids* (sources in the ``test`` directory).

To run the test suite, simply execute ``tesstest`` with no arguments:

    tesstest

or on GNU/Linux:

    ./tesstest

A summary of all tests (pass or fail) will be printed on the screen.
If all tests pass,
the compilation probably went well.
If any test fail,
please `submit a bug report <https://github.com/leouieda/tesseroids/issues>`__
with the output of running ``tesstest``.
