/** \file
User manual: Installation

\page userman-instal Installation

\htmlonly
<table border=0>
<tr>
<td width="15%"></td>
<td>
\endhtmlonly

\section bin Binary distribution

If you downloaded a pre-compiled binary distribution, simply unpack in the desired
directory. The executables will be in the "bin" folder and the HTML documentation
in the "doc" folder. To view the docs open "index.html" from the "html" folder
in a web browser.

The "examples" folder contains a few usage examples. The "run_examples.sh" scripts
contain the commands to run each example.
Included are also some <a href="http://www.python.org/download/">Python</a>
scripts that do the plotting. These are not necessary for running the examples, so don't
worry if you don't know Python or have it installed.
If you want to know more about Python and scientific computing in general, I recommend
the excelent <a href="http://software-carpentry.org">Software Carpentry</a> course.

\section src From source

Tesseroids uses the build tool <a href="http://www.scons.org/">SCons</a>.
A SConstruct file (Makefile equivalent) is used to define the compilation rules.
You will have to download and install SCons in order to easily compile Tesseroids.
SCons is available for both GNU/Linux and Windows and building should work the
same on both platforms.
SCons requires that you have <a href="http://www.python.org/download/">Python</a> installed.
Check the SCons website for more information.
Python is usually installed by default on most GNU/Linux systems.
Under Windows you will have to put SCons on your PATH environment variable in order to
use it from the command line.
It is usually located in the Scripts directory of your Python installation.

On GNU/Linux SCons will use the GCC compiler to compile sources. On Windows it will
search for an existing compiler. We recomment that you install GCC on Windows using
<a href="http://www.mingw.org/">MinGW</a>.

To compile, open a terminal (cmd.exe on Windows) and go to the directory where you
unpacked the source distribution. Once there, simply type:

\verbatim
scons
\endverbatim

The executables are placed on a "bin" folder.

\section src-test Testing

The source code for the unit tests for Tesseroids are in the "test" folder.
Tesseroids uses a modified version of the 
<a href="http://www.jera.com/techinfo/jtns/jtn002.html">MinUnit</a> unit testing
framework. When compiling the source code with SCons, the unit tests will be automatically
compiled into a test program called "tesstest", placed in the "bin" folder.
To run the tests, executed "tesstest".

\section src-docs Compiling the documentation

Tesseroids uses <a href="http://www.stack.nl/~dimitri/doxygen/index.html">Doxygen</a>
to generate the documentation. You will need Doxygen installed as well as
<a href="http://www.gnu.org/software/make/">make</a>. If you want to compile the PDF
documentation you will also need <a href="http://www.latex-project.org/">Latex</a> installed.
make comes pre-installed on most GNU/Linux systems.

The source for the documentation doesn't come with the regular source code distribution.
If you want to get it, go to the
<a href="http://code.google.com/p/tesseroids/source/checkout">Source tab</a> in the project
site and see how you can get a copy of the source code from the version control repository.

To build the docs, on GNU/Linux run the command "make" from the "doc" folder to
generate the HTML and Latex
code for the documentation. On Windows, run "make win". To compile the Latex code,
go to "doc/build/latex" and run "make". To view the HTML documentation open
"doc/build/html/index.html" in an internet browser. The Latex documentation is compiled
to "doc/build/latex/refman.pdf".

\htmlonly
</td>
<td width="15%"></td>
</table>
\endhtmlonly
*/