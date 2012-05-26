.. _install:

Installation instructions
=========================

Downloading
-----------

You can download the source and binary distributions
from the `Google Code project site`_.

.. _Google Code project site: http://code.google.com/p/tesseroids/


Pre-compiled binaries
---------------------

If you downloaded a pre-compiled binary distribution,
simply unpack in the desired directory.

The executables will be in the ``bin`` folder,
the pdf documentation in the ``doc`` folder,
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
We recomment that you install GCC on Windows using MinGW_.

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
please `submit a bug`_ with the output of ``tesstest``.

.. _unit tests: https://en.wikipedia.org/wiki/Unit_testing
.. _submit a bug: http://code.google.com/p/tesseroids/issues/list
