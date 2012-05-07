.. _theory:

Theorerical background
======================

What is a tesseroid anyway?
---------------------------

.. image:: _static/tesseroid.png
    :align: center
    :width: 300px

A tesseroid, or spherical prism,
is segment of a sphere.
It is delimited by:

1. 2 meridians, :math:`\lambda_1` and :math:`\lambda_2`

    .. image:: _static/tesseroid_meridians.png
        :align: center
        :width: 600px

2. 2 parallels, :math:`\phi_1` and :math:`\phi_2`

    .. image:: _static/tesseroid_parallels.png
        :align: center
        :width: 600px

3. 2 spheres or radii :math:`r_1` and :math:`r_2`

    .. image:: _static/tesseroid_sphere.png
        :align: center
        :width: 600px

About coordinate systems
------------------------

The figure bellow shows a tesseroid,
the global coordinate system (X, Y, Z),
and the local coodinate system (:math:`x,\ y,\ z`) of a point P.

.. figure:: _static/tesseroid-coordinates.png
    :align: center
    :width: 300px
    :figwidth: 60%

    View of a tesseroid, the integration point Q,
    the global coordinate system (X, Y, Z),
    the computation P
    and it's local coordinate system (:math:`x,\ y,\ z`)
    of a point P.
    :math:`r,\ \phi,\ \lambda` are
    the radius, latitude, and longitude, respectively,
    of point P.

The global system has origin on the center of the Earth
and Z axis aligned with the Earth's mean rotation axis.
The X and Y axis are contained on the equatorial parallel
with X intercepting the mean Greenwich meridian
and Y completing a right-handed system.

The local system has origin on the computation point P.
It's :math:`z` axis is oriented along the radial direction
and points away from the center of the Earth.
The :math:`x` and :math:`y` axis
are contained on a plane normal to the :math:`z` axis.
:math:`x` points North and :math:`y` East.

The tesseroid is defined
through the (:math:`r,\ \phi,\ \lambda`) coordinates
of the global system.
Its gravitational attraction
and gravity gradient tensor
are calculated with respect to
the local coordinate system of the computation point P.

.. warning:: The :math:`g_z` component is an exception to this.
    In order to conform with the regular convention
    of z-axis pointing toward the center of the Earth,
    this component **ONLY** is calculated with an inverted z axis.
    This way, gravity anomalies of
    tesseroids with positive density
    are positive, not negative.

Gravitational fields of a tesseroid
-----------------------------------

The gravitational attraction of a tesseroid
can be calculated using the formula
(Grombein et al., 2010):

.. math::
    g_{\alpha}(r_p,\phi_p,\lambda_p) = G \rho
        \displaystyle\int_{\lambda_1}^{\lambda_2}
        \displaystyle\int_{\phi_1}^{\phi_2} \displaystyle\int_{r_1}^{r_2}
        \frac{\Delta x_{\alpha}}{\ell^3} \kappa \ d r' d \phi' d \lambda'
        \ \ \alpha \in \{1,2,3\}

The gravity gradients can be calculated
using the general formula
(Grombein et al., 2010):

.. math::
    g_{\alpha\beta}(r_p,\phi_p,\lambda_p) = G \rho
        \displaystyle\int_{\lambda_1}^{\lambda_2}
        \displaystyle\int_{\phi_1}^{\phi_2} \displaystyle\int_{r_1}^{r_2}
        I_{\alpha\beta}\ d r' d \phi' d \lambda'
        \ \ \alpha,\beta \in \{1,2,3\}

.. math::
    I_{\alpha\beta} =
        \left(
            \frac{3\Delta x_{\alpha} \Delta x_{\beta}}{\ell^5} -
            \frac{\delta_{\alpha\beta}}{\ell^3}
        \right)
        \kappa\
        \ \ \alpha,\beta \in \{1,2,3\}

where :math:`\rho` is density,
the subscripts 1, 2, and 3 should be interpreted
as the x, y, and z axis,
:math:`\delta_{\alpha\beta}` is the Kronecker delta function, and

.. math::
   :nowrap:
   
    \begin{eqnarray*}
        \Delta x_1 &=& r' K_{\phi} \\
        \Delta x_2 &=& r' \cos \phi' \sin(\lambda' - \lambda_p) \\
        \Delta x_3 &=& r' \cos \psi - r_p\\
        \ell &=& \sqrt{r'^2 + r_p^2 - 2 r' r_p \cos \psi} \\
        \cos\psi &=& \sin\phi_p\sin\phi' + \cos\phi_p\cos\phi'
                     \cos(\lambda' - \lambda_p) \\
        K_{\phi} &=& \cos\phi_p\sin\phi' - \sin\phi_p\cos\phi'
                     \cos(\lambda' - \lambda_p)\\
        \kappa &=& {r'}^2 \cos \phi'
    \end{eqnarray*}


:math:`\phi` is latitude, :math:`\lambda` is longitude, :math:`r` is radius. The
subscript :math:`p` is for the computation point.


Numerical integration
---------------------

The above integrals are solved using the Gauss-Legendre Quadrature rule
(Asgharzadeh et al., 2007):

.. math::
    g_{\alpha\beta}(r_p,\phi_p,\lambda_p) \approx G \rho
        \frac{(\lambda_2 - \lambda_1)(\phi_2 - \phi_1)(r_2 - r_1)}{8}
        \displaystyle\sum_{k=0}^{N^{\lambda} - 1}
        \displaystyle\sum_{j=0}^{N^{\phi} - 1}
        \displaystyle\sum_{i=0}^{N^r - 1}
        W^r_i W^{\phi}_j W^{\lambda}_k
        I_{\alpha\beta}({r'}_i, {\phi'}_j, {\lambda'}_k )
        \kappa\ \ \alpha,\beta \in \{1,2,3\}

where :math:`W^r`, :math:`W^{\phi}`, and :math:`W^{\lambda}` are weighting
coefficients and :math:`N^r`, :math:`N^{\phi}`, and :math:`N^{\lambda}` are the
number of quadrature nodes, ie the order of the quadrature.


References
----------

Asgharzadeh, M.F., von Frese, R.R.B., Kim, H.R., Leftwich, T.E. & Kim, J.W.
(2007): Spherical prism gravity effects by Gauss-Legendre quadrature integration.
Geophysical Journal International, 169, 1-11.

`Grombein, T.; Seitz, K.; Heck, B. (2010): Untersuchungen zur effizienten
Berechnung topographischer Effekte auf den Gradiententensor am Fallbeispiel der
Satellitengradiometriemission GOCE.
KIT Scientific Reports 7547, ISBN 978-3-86644-510-9, KIT Scientific Publishing,
Karlsruhe, Germany.
<http://digbib.ubka.uni-karlsruhe.de/volltexte/documents/1336300>`_
