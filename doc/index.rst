.. index:

Welcome to Tesseroids!
======================

*Tesseroids* is a collection of command-line programs
for the direct modeling of gravitational fields in spherical coordinates.
It can model the gravitational potential, acceleration, and gradient tensor.

The geometric element used in the modeling processes is
a spherical prism, also called a tesseroid.
*Tesseroids* also contains programs for modeling using
right rectangular prisms, both in Cartesian and spherical coordinates.

.. image:: _static/tesseroid.png
    :align: center
    :width: 300px
    
*Tesseroids* is developed by `Leonardo Uieda`_
in cooperation with `Carla Braitenberg`_.

As of version 1.1,
*Tesseroids* is licensed under the :ref:`BSD license <license>`.
This means that it can be reused and remixed
with fewer restrictions.
See the license text for more information.

The source code of *Tesseroids* is hosted on several repositories:

* `Google Code`_: source code and compiled binaries (latest stable release)
* `Bitbucket Stable`_: source code (latest stable release)
* `Bitbucket Development`_: source code (development version)

.. _Google Code: http://code.google.com/p/tesseroids/
.. _Bitbucket Stable: https://bitbucket.org/leouieda/tesseroids
.. _Bitbucket Development: https://bitbucket.org/leouieda/tesseroids-dev

This documentation explains how to
:ref:`install <install>`
and :ref:`use <usage>` Tesseroids.
It also contains some :ref:`theoretical background <theory>`
to get you up-to-date with the terms and equations that we use.
The :ref:`cookbook <cookbook>` has a few example recipes and the expected output
(I recommend starting here if you just want a quick peek).

.. toctree::
    :maxdepth: 2

    license
    changes
    install
    theory
    usage
    cookbook


.. _Leonardo Uieda: http://fatiando.org/people/uieda/
.. _Carla Braitenberg: http://www2.units.it/geodin/biobraitenberg.html
