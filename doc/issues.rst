.. _issues:

Known issues
============

* Prism and tesseroid calculations are **only valid outside** of the mass
  elements.
  If you calculate on top or inside of the prism/tesseroid, there is
  no guarantee that the result will be correct.
* The gravity gradient components of tesseroids suffer from increased numerical
  error as the computation point gets closer to the tesseroid. It is not
  recommened to compute the effects at distances smaller than 1km above the
  tesseroid.
