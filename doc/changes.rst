.. _changes:

Changelog
=========

Changes in version 1.1
----------------------

* the tesseroids license was changed from the GNU GPL
  to the more permissive BSD license
  (see :ref:`the license text <license>`).
* tess2prism has a new flag --flatten
  to make the prism model by flattening the tesseroids
  (i.e., 1 degree = 111km) into Cartesian coordinates
  (so that they can be used with the prismg* programs).
* tessg* programs have a new flag -t
  used to control the distance-size ratio for the automatic recursive division
  of tesseroids.
* **NEW PROGRAMS** prismpots, prismgs, and prismggts,
  to calculate the prism effects
  in spherical coordinates.
  These programs are compatible with the output of tess2prism
  (see :ref:`this recipe <recipe_tess2prism>` for an example).
* **NEW PROGRAM** tesslayers to generate a tesseroid model of a stack of layers
  from grids of the thickness and density of each layer.
  tesslayers complements the functionality of tessmodgen
  and can be used to generate crustal models,
  sedimentary basin models, etc.
  (see :ref:`this recipe <recipe_tesslayers>` for an example).
* tesseroids now strictly follows the ANSI C standard.
* Bug fix: prismpot, prismgx, prismgy, prismgz, and prismgxy had problems with
  a log(z + r) when the computation point was bellow the top of the prism
  (zp > prism.z1). Fixed by calculating on top of the prism when this happens,
  then changing the sign of the result when needed (only for gz).
* Bug fix: the tessg and prismg family of programs was crashing when the model
  file is empty. Now they fail with an error message.


Changes in version 1.0
----------------------

Tesseroids 1.0 was completely re-coded in the C programming language
and is much faster and more stable than the 0.3 release.
Here is a list of new features:

* tesspot and tessg* programs now take the computation points as input,
  allowing for custom grids.
* tesspot and tessg* programs now automatically subdivide a tesseroid
  if needed to maintain GLQ precision
  (this makes computations up to 5x faster and safer).
* Automated model generation using program tessmodgen.
* Regular grid generation with program tessgrd.
* Total mass calculation with program tessmass.
* Programs to calculate the gravitational fields
  of right rectangular prisms in Cartesian coordinates.
* HTML User Manual and API Reference generated with Doxygen.
* Easy source code compilation with SCons.
