.. _changes:

Changelog
=========

Changes in version 1.1
----------------------

* tesseroids now strictly follows the ANSI C standard.
* the tesseroids license was changed from the GNU GPL
  to the more permissive BSD license
  (see :ref:`the license text <license>`).
* tess2prism has a new flag --flatten
  to make the prism model by flattening the tesseroids
  (i.e., 1 degree = 111km) into Cartesian coordinates
  (so that they can be used with the prismg* programs).
* prismpot and prismg* programs raises an error
  when the input has more columns than it should
  (e.g., it comes from tess2prism, see the --flatten flag).

  
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
