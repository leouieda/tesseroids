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
    
*Tesseroids* is coded in the C programming language.
It is developed by `Leonardo Uieda`_ in cooperation with Carla Braitenberg.

As of version 1.1,
*Tesseroids* is licensed under the :ref:`BSD license <license>`.
This means that it can be reused and remixed
with fewer restrictions.
See the license text for more information.

This documentation explains how to
:ref:`install <install>`
and :ref:`use <usage>` Tesseroids.
It also contains some :ref:`theoretical background <theory>`
to get you up-to-date with the terms and equations that we use.
The :ref:`cookbook` has a few example recipes and the expected output
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
