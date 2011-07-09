/** \file
User manual: Theoretical background


\page userman-theory

\htmlonly
<table border=0>
<tr>
<td width="15%"></td>
<td>
\endhtmlonly

\section coordsys About Coordinate Systems

The two coordinate systems involved in the computations are the Global and Local
coordinate systems.

The Global system has origin on the center of the Earth and Z axis aligned
with the Earth's mean rotation axis. The X and Y axis are contained on the
equatorial parallel with X intercepting the mean Greenwich meridian and Y
completing a right-handed system.

The Local system has origin on the computation point. It's z is oriented
along the radial direction and points away from the center of the Earth. The x
and y axis are contained on a plane normal to the z axis and x points North and
y East.

The tesseroids are defined using the Global Coordinate system with spherical
coordinates, while the gravitational fields are calculated on the Local
Coordinate system of the computation point.

<b>WARNING</b>: The \f$ g_z \f$ component is an exception to this. In order to
conform with the regular convention of z-axis pointing toward the center of the
Earth, this component ONLY is calculated with an inverted z axis.

\image latex tesseroid_coordsys.pdf "View of a tesseroid, the integration point Q, the global coordinate system, the computation P and it's local coordinate system."

<center>
\image html tesseroid_coordsys.png "Figure1: View of a tesseroid, the integration point Q, the global coordinate system, the computation P and it's local coordinate system."
</center>

\section formulas Gravitational Fields of a Tesseroid

The gravitational attraction of a tesseroid can be calculated using the formula
(Grombein et al., 2010):

\f[
g_{\alpha}(r_p,\phi_p,\lambda_p) = G \rho \displaystyle\int_{\lambda_1}^{\lambda_2}
    \displaystyle\int_{\phi_1}^{\phi_2} \displaystyle\int_{r_1}^{r_2}
    \frac{\Delta x_{\alpha}}{\ell^3} \kappa \ d r' d \phi' d \lambda'
    \ \ \alpha \in \{1,2,3\}
\f]

The gravity gradients can be calculated using the general formula
(Grombein et al., 2010):

\f[
g_{\alpha\beta}(r_p,\phi_p,\lambda_p) = G \rho \displaystyle\int_{\lambda_1}^{\lambda_2}
    \displaystyle\int_{\phi_1}^{\phi_2} \displaystyle\int_{r_1}^{r_2}
    I_{\alpha\beta}\ d r' d \phi' d \lambda'
    \ \ \alpha,\beta \in \{1,2,3\}
\f]

\f[
I_{\alpha\beta} = \left(\frac{3\Delta x_{\alpha} \Delta x_{\beta}}{\ell^5} -
    \frac{\delta_{\alpha\beta}}{\ell^3} \right)\kappa\
    \ \ \alpha,\beta \in \{1,2,3\}
\f]

where \f$ \rho \f$ is density, the subscripts 1, 2, and 3 should be
interpreted as the x, y, and z axis, \f$ \delta_{\alpha\beta} \f$ is the
Kronecker delta function, and

\f{eqnarray*}{
\Delta x_1 &=& r' K_{\phi} \\
\Delta x_2 &=& r' \cos \phi' \sin(\lambda' - \lambda_p) \\
\Delta x_3 &=& r' \cos \psi - r_p\\
\ell &=& \sqrt{r'^2 + r_p^2 - 2 r' r_p \cos \psi} \\
\cos\psi &=& \sin\phi_p\sin\phi' + \cos\phi_p\cos\phi'
             \cos(\lambda' - \lambda_p) \\
K_{\phi} &=& \cos\phi_p\sin\phi' - \sin\phi_p\cos\phi'
             \cos(\lambda' - \lambda_p)\\
\kappa &=& {r'}^2 \cos \phi'
\f}

\f$ \phi \f$ is latitude, \f$ \lambda \f$ is longitude, \f$ r \f$ is radius. The
subscript \f$ p \f$ is for the computation point.

\section glq Numerical Integration

The above integrals are solved using the Gauss-Legendre Quadrature rule
(Asgharzadeh et al., 2007):

\f[
g_{\alpha\beta}(r_p,\phi_p,\lambda_p) \approx G \rho \frac{(\lambda_2 - \lambda_1)
    (\phi_2 - \phi_1)(r_2 - r_1)}{8} \displaystyle\sum_{k=0}^{N^{\lambda} - 1}
    \displaystyle\sum_{j=0}^{N^{\phi} - 1} \displaystyle\sum_{i=0}^{N^r - 1}
    W^r_i W^{\phi}_j W^{\lambda}_k
    I_{\alpha\beta}({r'}_i, {\phi'}_j, {\lambda'}_k )\kappa\ \ \alpha,\beta \in \{1,2,3\}
\f]

where \f$ W^r \f$,\f$ W^{\phi} \f$, and \f$ W^{\lambda} \f$ are weighting
coefficients and \f$ N^r \f$,\f$ N^{\phi} \f$, and \f$ N^{\lambda} \f$ are the
number of quadrature nodes, ie the order of the quadrature.

\section theory-refs References

- Asgharzadeh, M.F., von Frese, R.R.B., Kim, H.R., Leftwich, T.E. & Kim, J.W.
(2007): Spherical prism gravity effects by Gauss-Legendre quadrature integration.
Geophysical Journal International, 169, 1-11.

- Grombein, T.; Seitz, K.; Heck, B. (2010): Untersuchungen zur effizienten
Berechnung topographischer Effekte auf den Gradiententensor am Fallbeispiel der
Satellitengradiometriemission GOCE.
KIT Scientific Reports 7547, ISBN 978-3-86644-510-9, KIT Scientific Publishing,
Karlsruhe, Germany.
(<a href="http://digbib.ubka.uni-karlsruhe.de/volltexte/documents/1336300">
http://digbib.ubka.uni-karlsruhe.de/volltexte/documents/1336300</a>).

\htmlonly
</td>
<td width="15%"></td>
</table
\endhtmlonly
*/
