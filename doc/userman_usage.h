/** \file
User manual: Usage

\page userman-usage Usage

\htmlonly
<table border=0>
<tr>
<td width="15%"></td>
<td>
\endhtmlonly

\section heights A note about heights and units

In order to have a single convention, the word "height" means "height above the
Earths surface" and are interpreted as positive up and negative down, ie. oriented
with the z axis of the Local coordinate system.

Also, all input units are in SI and decimal degrees. Output of tessg* programs
are in mGal and Eotvos. All other output is also in SI and decimal degrees.

\section tessg Computing the gravitational effect of a tesseroid

The tessgx, tessgy, tessgz, tessgxx, etc. programs calculate the combined effect of
a list of tesseroids on given computation points. The computation points are passed
via standard input and do NOT have to be in a regular grid. This allows, for example,
computation on points where data was measured. The values calulated are put in
the last column of the input points and printed to standard output.

For example, if calculating \f$ g_z \f$ on these points:

\verbatim
lon1 lat1 height1 value1 othervalue1
lon2 lat2 height2 value2 othervalue2
...
lonN latN heightN valueN othervalueN
\endverbatim

the output would look something like:

\verbatim
lon1 lat1 height1 value1 othervalue1 gz1
lon2 lat2 height2 value2 othervalue2 gz2
...
lonN latN heightN valueN othervalueN gzN
\endverbatim

The input model file should contain one tesseroid per line and have columns
formated as:

\verbatim
W E S N HEIGHT_OF_TOP HEIGHT_OF_BOTTOM DENSITY
\endverbatim

HEIGHT_OF_TOP and HEIGHT_OF_BOTTOM are positive if the above the Earth's surface
and negavitive if bellow. Remember that HEIGHT_OF_TOP > HEIGHT_OF_BOTTOM!

Use the command line option -h to view a list of all commands available.

\subsection tess-example Example:

Calculate the \f$ g_z \f$ field of a tesseroid model having verbose printed and
logged to file "gz.log" and GLQ order 3/3/3.

\verbatim
tessgz modelfile.txt -v -lgz.log -o3/3/3 < points.txt > gz_data.txt
\endverbatim

\subsection tess-adapt The -a flag

The -a flag on tessg* programs enables the automatic re-sizing of tesseroids
when it is needed to maintain the GLQ precision desired. As a general rule,
the tesseroid should be no bigger than it's distance from the computation point.
Using this flag breaks the tesseroids automatically when this criterion is breached.
This means that the computations can be performed with order 2/2/2 (default) which is
much faster and still maintain correctness. <b>It is strongly recommended using
this flag and 2/2/2 order always</b>.

\section verbose Verbose and loggin to files

The -v flag enables printing of information messages to stderr. If ommited, only
error messages will appear.

The -l flag enables logging of information and error messages to a file.

\section comments Comments and provenance information

Comments can be inserted into input files by placing a "#" character at the
start of a line. All comment lines are ignored. tessg* programs print the
comment lines of the input to standard output.

All programs insert comments about the provenance of their results (where they came from)
to their output. These include names of input files, version of program used,
date, etc.

\section grid Generating a regular grid

Included in the package is program "tessgrd" which creates a regular grid of
points and prints them to standard output.

\subsection grdiexample Example

\verbatim
tessgrd -r-10/10/-10/10 -b100/100 -z250e03 -v > points.txt
\endverbatim

\section pipe Piping

Tesseroids was designed with the Unix filosophy in mind:

\verbatim
Write programs that do one thing and do it well.
Write programs to work together.
Write programs to handle text streams, because that is a universal interface.
\endverbatim

Therefore, all tessg* and tessgrd programs can be piped together to calculate
many components on a regular grid.

\subsection pipe-example Example

Given a tesseroids file "model.txt" as follows:

\verbatim
-5 5 -5 5 0 -10e03 -500
\endverbatim

Running the following would calculate \f$ g_z \f$ and gradient tensor of
tesseroids in "model.txt" of a regular grid from -10W to 10E and -10S
to 10N on 100x100 points at 250 km height.
And the best of all is that it is done in parallel! If your system has multiple cores
this would mean a great increase in the computation time.

\verbatim
tessgrd -r-10/10/-10/10 -b100/100 -z250e03 | tessgz model.txt -a | \
tessgxx model.txt -a | tessgxy model.txt -a | tessgxz model.txt -a | \
tessgyy model.txt -a | tessgyz model.txt -a | tessgzz model.txt -a > output.txt
\endverbatim

The result of this should look something like:

\image html example.png
\image latex example.png "Gravitational fields produced by a one tesseroid model." width=15cm


\htmlonly
</td>
<td width="15%"></td>
</table>
\endhtmlonly
*/