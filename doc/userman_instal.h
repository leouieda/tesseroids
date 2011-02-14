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
directory. The executables will be in the "bin" folder and the HTML and PDF documentation
in the "doc" folder.

\subsection bin-win Windows installer

The Windows installer should be very straight-forward. Simply follow the instructions
and reboot.

\section src From source

Tesseroids uses the build tool <a href="http://www.scons.org/">SCons</a>.
A SConstruct file (Makefile equivalent) to define the compilation rules.
You will have to download and install SCons in order to easily compile Tesseroids.
SCons is available for both GNU/Linux and Windows and building should work the
same on both platforms.

To compile, type in a terminal (cmd.exe on Windows):

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
to generate the documentation. Run the command "make" from the "doc" folder
to generate the HTML and Latex code for the documentation. To compile the Latex code,
go to "doc/build/latex" and run "make". You will need
<a href="http://www.gnu.org/software/make/">make</a> installed for this.

To view the HTML documentation open "doc/build/html/index.html" in any internet
browser. The Latex documentation is compiled to "doc/build/latex/refman.pdf".

\htmlonly
</td>
<td width="15%"></td>
</table>
\endhtmlonly
*/