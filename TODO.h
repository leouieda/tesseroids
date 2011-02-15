/** \file
To do list.

TESTS:
\todo Error check for prism, sphere and tess (everton is doing)
\todo Check error in not rotating prism
\todo Check error os using tesseroid in poles

PROGRAMS:
\todo Programs to calculate the effect of a sphere model in spherical coordinates

API:
\todo Make minunit into functions and put variable arguments for messages like printf

DOC:
\todo Make doxygen groups to separate programs from api
\todo Write about the speed test
\todo Write about model generator
\todo Write about prismg* programs

MAYBE:
\todo Generate VTK file to plot tesseroids in Mayavi2 or Paraview

DONE:
- (Done 27/Jan/2011) Include automatic documentation with doxygen
- (Done 28/Jan/2011) Make integration test to compare tess to sphere and prism to sphere
- (Done 02/Feb/2011) Move grid generation to a separate program
- (Done 08/Feb/2011) Programs to calculate the effect of a prism model in cartesian coordinates
- (Done 09/Feb/2011) Program to calculate the total mass of a model (with options for cut-off density values)
- (Done 09/Feb/2011) Program to generate tesseroid model from an regular grid
- (Done 09/Feb/2011) Program to print default values for constants
- (Done 10/Feb/2011) Fix local z axis positive upward. Change all depths when defining tesseroids to negative heights.
- (Done 11/Feb/2011) Adaptatively resize tesseroid when too close to computation point (with fixed GLQ order)
*/