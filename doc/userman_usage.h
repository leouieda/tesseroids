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


\section help Getting help information

All programs accept the -h and --version flags. -h will print a help message
describing the usage, input and output formats and options accepted.
--version prints version and license information about the program.

Program <i>tessdefaults</i> prints the default values of constants used in the computations
such as: mean Earth radius, pi, gravitational constant, etc.

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

<b>WARNING</b>: Do not calculate directly on top or inside of a tesseroid. The
computations will not be correct even using the -a option. If tried, will print a warning
message but will calculate anyway.

\subsection tess-example Example:

Calculate the \f$ g_z \f$ field of a tesseroid model having verbose printed and
logged to file "gz.log" and GLQ order 3/3/3.

\verbatim
tessgz modelfile.txt -v -lgz.log -o3/3/3 < points.txt > gz_data.txt
\endverbatim

\subsection tess-adapt The -a flag

The -a flag on tessg* programs disables the automatic subdividing of tesseroids
when needed to maintain the GLQ accuracy desired. As a general rule,
the tesseroid should be no bigger than a ratio times the distance from the computation point
(program tessdefaults prints the value of the size ratio used).
The tessg* programs automatically break the tesseroids when this criterion is breached.
This means that the computations can be performed with order 2/2/2 (default) which is
much faster and still maintain correctness.
<b>It is strongly recommended that you don't use this flag unless you know what you are
doing! It is also recommended that you keep 2/2/2 order always</b>.


\section verbose Verbose and logging to files

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
points and prints them to standard output. This program is unit independent and
can be used to generate grids in spherical or Cartesian coordinates.

\subsection grdiexample Example

To generate a grid in the range -10W, 10E, -10S, 10N with 100x100 points at
250 km height:

\verbatim
tessgrd -r-10/10/-10/10 -b100/100 -z250e03 -v > points.txt
\endverbatim


\section modgen Automated model generation

Tesseroids 1.0 includes a new program called "tessmodgen" for automatically
generating a tesseroid model from a map of an interface. The interface can be
any surface deviating from a reference level. For example, topography (a DEM)
deviates from 0, a Moho map deviates from a mean crustal thickness, etc.

This program takes as input a REGULAR grid with longitude, latitude and height
values of the interface. Each tesseroid is generated with a grid point at the center
of it's top face. The top and bottom faces of the tesseroid are defined as:

- Top = Interface and Bottom = Reference: if the interface is above the reference
- Top = Reference and Bottom = Interface: if the interface is bellow the reference

The density \f$ \rho \f$ of the tesseroids can be passed using the -d option.
This will asign a density value of \f$ \rho \f$ when the interface is above the reference
and a value of \f$ -\rho \f$ if the interface is bellow the reference.

Alternatively, the density of each tesseroid can be passed as a forth column on the
input grid. <b>As with the -d option, if the interface is bellow the reference, the
density value will be multiplied by -1!</b> Also, an error will occur if both a
forth column and the -d option are passed!

\subsection modgen-ex Example:

To generate a tesseroid model from a Digital Elevation Model (DEM) with \f$ 1^\circ \f$
resolution using a density \f$ \rho = 2.67\ g.cm^{-3} \f$:

\verbatim
tessmodgen -s1/1 -d2670 -z0 -v < dem_file.txt > dem_tess_model.txt
\endverbatim


\section mass Calculating the total mass of a model

The tessmass program can be used to compute the total mass of a given tesseroid
model. If desired, a density range can be given and only tesseroids that fall
within the given range will be used in the calculation.

\subsection mass-ex Example:

To calculate the total mass of all tesseroids in "model.txt" with density between
0 and 1 g.cm^-3 :

\verbatim
tessmass -r0/1000 < model.txt
\endverbatim


\section prismg Computing the gravitational effect of a rectangular prism

Tesseroids 1.0 also introduces programs to calculate the gravitational effect
of rectangular prisms in Cartesian coordinates.
This is done using the formulas of Nagy <i> et al.</i> (2000).
The programs are name prismgx, prismgy, prismgz, prismgxx, etc.

Input and output for these programs is very similar to that of the tessg* programs.
Computation points are read from standard input and the prism model is read from
a file. The model file should have the column format:

\verbatim
X1 X2 Y1 Y2 Z1 Z2 DENSITY
\endverbatim

\subsection prism-ex Example

Calculate the \f$ g_z \f$ field of a prism model having verbose printed and
logged to file "prismgz.log".

\verbatim
prismgz modelfile.txt -v -lprismgz.log < points.txt > gz_data.txt
\endverbatim


\subsection prism-coords A note on the coordinate system

As in Nagy <i> et al.</i> (2000), the coordinate system for the rectangular prism
calculations has X axis pointing North, Y axis pointing East and Z axis pointing
<b>Down</b>. This is important to note because it differs from the convention
adopted for the tesseroids. In practice, this means that the \f$ g_{xz} \f$ and
\f$ g_{yz} \f$ components of the prism and tesseroid will have different signs.
This will not be such for the \f$ g_{z} \f$ component, though, because the convention
for tesseroids is to have Z axis Down for this component only. See the \subpage userman-theory
section for more details on this.


\section pipe Piping

Tesseroids was designed with the Unix filosophy in mind:

\verbatim
Write programs that do one thing and do it well.
Write programs to work together.
Write programs to handle text streams, because that is a universal interface.
\endverbatim

Therefore, all tessg* and tessgrd programs can be piped together to calculate
many components on a regular grid.

\subsection pipe-example Example with tesseroids

Given a tesseroids file "model.txt" as follows:

\verbatim
-1 1 -1 1 0 -10e03 -500
\endverbatim

Running the following would calculate \f$ g_z \f$ and gradient tensor of tesseroids
in "model.txt" of a regular grid from -5W to 5E and -5S to 5N on 100x100 points at
250 km height.
And the best of all is that it is done in parallel! If your system has multiple cores
this would mean a great increase in the computation time. All information regarding the
computations will be logged to files gz.log, gxx.log, etc. These should include the
information about how many times the tesseroid had to be split into smaller ones
to guarantee GLQ accuracy.

\verbatim
tessgrd -r-5/5/-5/5 -b100/100 -z250e03 | tessgz model.txt -lgz.log | \
tessgxx model.txt -lgxx.log | tessgxy model.txt -lgxy.log | \
tessgxz model.txt -lgxz.log | tessgyy model.txt -lgyy.log | \
tessgyz model.txt -lgyz.log | tessgzz model.txt -lgzz.log > output.txt
\endverbatim

The result of this should look something like:

\image html example.png "Plot of columns in result file output.txt (values in mGal and Eotvos)."
\image latex example.png "Plot of columns in result file output.txt (values in mGal and Eotvos)." width=15cm


\subsection pipe-example-prism Example with rectangular prisms

Given a prism file "prism_model.txt" as follows:

\verbatim
4e03 6e03 2e03 8e03 0 2e03 500
\endverbatim

Running the following would calculate \f$ g_z \f$ and gradient tensor of
prisms in "prism_model.txt" of a regular grid from 0km W to 10km E and 0km S
to 10km N on 100x100 points at 1 km height.

\verbatim
tessgrd -r0/10e03/0/10e03 -b100/100 -z1e03 | prismgz prism_model.txt | \
prismgxx prism_model.txt | prismgxy prism_model.txt | prismgxz prism_model.txt | \
prismgyy prism_model.txt | prismgyz prism_model.txt | prismgzz prism_model.txt > prism_output.txt
\endverbatim

Note that we could re-use "tessgrd" for this since it's calculations are unit independent.
The result of this should look something like:

\image html example-prism.png "Plot of columns in result file prism_output.txt (values in mGal and Eotvos)."
\image latex example-prism.png "Plot of columns in result file prism_output.txt (values in mGal and Eotvos)." width=15cm


\section refs References

- Nagy, D., Papp, G., Benedek, J. [2000]. The gravitational potential and its derivatives for the prism. Journal of Geodesy, 74, 552–560.


\htmlonly
</td>
<td width="15%"></td>
</table>
\endhtmlonly
*/