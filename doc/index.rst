.. index:

Documentation for Tesseroids (|version|)
=========================================

.. image:: _static/banner.png
    :alt: Tesseroids

A collection of **command-line programs**
for modeling the gravitational potential, acceleration, and gradient tensor
in spherical coordinates.

Developed by `Leonardo Uieda`_
in cooperation with `Carla Braitenberg`_.

**Official site**: http://tesseroids.leouieda.com

**License**: :ref:`BSD 3-clause <license>`

**Source code**: https://github.com/leouieda/tesseroids

.. note::

    *Tesseroids* is **research software**.
    Please consider **citing** it in your publications
    if you use it for your research.
    See :ref:`Citation <citation>` for more information.

.. warning::

    See the list of :ref:`known issues <issues>` for things you should be
    aware of.

The geometric element used in the modeling processes is
a **spherical prism**, also called a **tesseroid**.
*Tesseroids* also contains programs for modeling using
**right rectangular prisms**,
both in **Cartesian** and **spherical coordinates**.

.. image:: _static/tesseroid.png
    :align: center
    :width: 300px


Getting started
---------------

Take a look at the examples in the
:ref:`Cookbook <cookbook>`.
They contain scripts that run *Tesseroids* and some Python code to plot the
results.

If you're the kind of person who likes to see the equations
(who doesn't?),
see the :ref:`Theoretical background <theory>` and the references cited there.

For a more detailed description of the software, options, and conventions used,
see the :ref:`usage instructions <usage>`.

Also, all programs accept the ``-h`` flag to print the instructions for using
that particular program. For example::

    $ tessgrd -h
    Usage: tessgrd [PARAMS] [OPTIONS]

    Make a regular grid of points.

    All units either SI or degrees!

    Output:
      Printed to standard output (stdout) in the format:
        lon1    lat1    height
        lon2    lat1    height
        ...     ...     ...
        lonNLON lat1    height
        lon1    lat2    height
        ...     ...     ...
        ...     ...     ...
        lonNLON latNLAT height

      * Comments about the provenance of the data are inserted into
        the top of the output

    Parameters:
      -r           W/E/S/N: Bounding region of the grid.
      -b           NLON/NLAT: Number of grid points in the
                   longitudinal and latitudinal directions.
      -z           HEIGHT: Height of the grid with respect to the
                   mean Earth radius.
      -h           Print instructions.
      --version    Print version and license information.

    Options:
      -v           Enable verbose printing to stderr.
      -lFILENAME   Print log messages to file FILENAME.

    Part of the Tesseroids package.
    Project site: <http://fatiando.org/software/tesseroids>
    Report bugs at: <http://code.google.com/p/tesseroids/issues/list>


Getting help
------------

Write an e-mail to `Leonardo Uieda <http://www.leouieda.com/>`__,
or `tweet <https://twitter.com/leouieda>`__,
or `Google Hangout <https://plus.google.com/+LeonardoUieda>`__.
**Even better**, submit a bug report/feature request/question to the
`Github issue tracker <https://github.com/leouieda/tesseroids/issues>`__.

----

Contents
--------

.. toctree::
    :maxdepth: 2

    license
    citation
    issues
    changes
    install
    theory
    usage
    cookbook


.. _Leonardo Uieda: http://www.leouieda.com
.. _Carla Braitenberg: http://www2.units.it/geodin/biobraitenberg.html
.. _the official site: http://leouieda.github.com/tesseroids
.. _GitHub: https://github.com/leouieda/tesseroids
.. _Get in touch: http://www.leouieda.com
