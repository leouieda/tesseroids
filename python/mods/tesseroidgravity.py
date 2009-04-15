################################################################################
#                                                                              #
#                        MODULE TESSEROID GRAVITY                              #
#                                                                              #
################################################################################
################################################################################
#
#    Copyright (C) 2009  Leonardo Uieda
#
#    This program is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
#    Doubts and/or suggestions please mail the author:
#         Leonardo Uieda - leouieda@gmail.com
#
################################################################################
"""
    This module contains classes for calculating the gravity fields of tesseroid
    models. Fields are:
        potential, 3-component acceleration, 9-component gradient tensor.
    The constants used in the calculations are defined as class variables in the
    TesseroidGravity class that is mother to all others.
"""

################################################################################
# Created on 6-Apr-2009 4:07:57 PM
# Last modified by $Author$
__author__ = 'Leonardo Uieda (leouieda@gmail.com)'
__version__ = '$Revision$'
__date__ = '$Date$'
__copyright__ = "Copyright (C) 2009  Leonardo Uieda"
__license__ = "GPL"
################################################################################

################################################################################
# LOGGING
import logging

# A Handler that does nothing so that if the app using this module doesn't
# want to log, it won't get an error because it didn't provide a Handler
class NullHandler(logging.Handler):
    """
    Log Handler that does nothing.
    """
    def emit(self, record):
        """ Do nothing. """
        pass

NULLH = NullHandler()
logging.getLogger('tesseroidgravity').addHandler(NULLH)

################################################################################


from math import pi, cos, sin, sqrt, log
from timeit import Timer


################################################################################
#  EXCEPTIONS

class TesseroidGravityError(Exception):
    """
    Base class for the exceptions raised by the classes in this module class.
    """
    pass

class SingularityError(TesseroidGravityError):
    """
    Raised when a singularity occurs in the 2D GLQ. This singularity is due to
    the computation point being aligned with a GLQ node.
    """
    pass

################################################################################


################################################################################
# TESSEROIDGRAVITY CLASS

class TesseroidGravity:
    """
    Mother class to the Tesseroid* classes (* are the diff. fields).
    This class has the constants shared by it's children.
    If these need be changed, change them here.
    Constants are:
        * G = Gravitational constant in m**3 kg**(-1) s**(-1)
        * R = Mean Earth radius in m
        * si2eotvos = Conversion factor from SI units to Eotvos
        * si2mgal = Conversion factor from SI units to mGal
    """


    # The gravitational constant (m^3*kg^-1*s^-1)
    G = 0.00000000006673
    # The mean earth radius (m)
    R = 6378137.0
    # Conversion factor from SI units to Eotvos: 1 /s**2 = 10**9 Eotvos
    si2eotvos = 1000000000.0
    # Conversion factor from SI units to mGal: 1 m/s**2 = 10**5 mGal
    si2mgal = 100000.0


    def __init__(self, abr, ablon, ablat, wr, wlon, wlat):
        """
        Pass the Abscissas and associated Weights that will be used in the GLQ.
            abr   - Abscissas in the radial direction;
            ablon - Abscissas in the longitude direction;
            ablat - Abscissas in the latitude direction;
            wr    - Weights in the radial direction;
            wlon  - Weights in the longitude direction;
            wlat  - Weights in the latitude direction;
        """
        # Initialize the parameters
        self.abr = abr
        self.ablon = ablon
        self.ablat = ablat
        self.wr = wr
        self.wlon = wlon
        self.wlat = wlat
        # A factor to convert the field to the proper unit
        self.unit = 1.0
        self.field = []


    def calculate3D(self, model, lons, lats, heights):
        """
        Calculate the gravity potential field of a Tesseroid model on a given
        grid of points using 3D GLQ.
        Parameters:
            model   - list of Tesseroid objects;
            lons    - list of longitudes in which to calculate the field;
            lats    - list of latitudes in which to calculate the field;
            heights - list with the heights in which to calculate the field;

        Returns a list with the field values on the respective grid points.
        Ex:
            The first grid point is (lons[0], lats[0], heights[0]), the second
            is (lons[1], lats[1], heights[1]), and so forth.
        """

        # Convert the coordinates to radians and calculate the radial distance
        # to the point
        deg2rad = pi/180.0
        lonsrad = list(lons)
        latsrad = list(lats)
        rs = list(heights)
        for i in range(0, len(lons)):
            lonsrad[i] *= deg2rad
            latsrad[i] *= deg2rad
            rs[i] += self.R

        # First initialize the array that will hold the field values
        field = [0.0]*len(lons)

        # Iterate over the tesseroids in the model and sum their fields on the
        # grid points.
        for tess in model:

            # Scale the abscissas to the dimensions of the tesseroid
            self.abr.scale(self.R - tess['bottom'], self.R - tess['top'])
            self.ablon.scale(deg2rad*tess['w'], deg2rad*tess['e'])
            self.ablat.scale(deg2rad*tess['s'], deg2rad*tess['n'])

            # This is the scale fator that will multiply the GLQ summation due
            # to the scaling of the abscissas
            # For practicality I included the density of the tesseroid and the
            # gravitational constants as well.
            factor = ((deg2rad**2)*(tess['e'] - tess['w'])*\
                      (tess['n'] - tess['s'])*\
                      (tess['bottom'] - tess['top'])*\
                      tess['density']*self.G*self.unit) / 8.0

            # Caculate the field at every point on the grid and sum to what was
            # there before.
            grid = zip(*[range(0, len(lonsrad)), lonsrad, latsrad, rs])
            for i, lamb, phi, r in grid:

                # Initialize the result with zero
                res = 0.0

                # Do the GLQ summation to calculate the field
                for lambl, wlon in zip(*[self.ablon.val, self.wlon.val]):
                    for phil, wlat in zip(*[self.ablat.val, self.wlat.val]):
                        for rl, wr in zip(*[self.abr.val, self.wr.val]):

                            # Calculate the integral kernel K
                            K = self.kernel3D(r, lamb, phi, rl, lambl, phil)

                            # Add the kernel at the current integration
                            # point to the result
                            res += wlon*wlat*wr*K

                # Add this to the field of the other tesseroids
                # Factor is there to account for the change in variables
                # done when scaling the abscissas to the correct interval.
                # It also contains the density of the tesseroid, the
                # gravitational constant and a unit conversion factor (only
                # if needed).
                field[i] += factor*res

        return field


    def kernel3D(self, r, lamb, phi, rl, lambl, phil):
        """
        The kernel of the 3D integral defining the gravitational field.
        """
        pass


    def calculate(self, model, lons, lats, heights):
        """
        Calculate this particular gravity field of a Tesseroid model on a given
        grid of points using analytical integraion in the r direction and 2D GLQ
        Parameters:
            model   - list of Tesseroid objects;
            lons    - list of longitudes in which to calculate the field;
            lats    - list of latitudes in which to calculate the field;
            heights - list with the heights in which to calculate the field;

        Returns a list with the field values on the respective grid points.
        Ex:
            The first grid point is (lons[0], lats[0], heights[0]), the second
            is (lons[1], lats[1], heights[1]), and so forth.
        """

        # Convert the coordinates to radians and calculate the radial distance
        # to the point
        deg2rad = pi/180.0
        lonsrad = list(lons)
        latsrad = list(lats)
        rs = list(heights)
        for i in range(0, len(lons)):
            lonsrad[i] *= deg2rad
            latsrad[i] *= deg2rad
            rs[i] += self.R

        # First initialize the array that will hold the field values
        field = [0.0]*len(lons)

        # Iterate over the tesseroids in the model and sum their fields on the
        # grid points.
        for tess in model:

            # Scale the abscissas to the dimensions of the tesseroid
            self.ablon.scale(deg2rad*tess['w'], deg2rad*tess['e'])
            self.ablat.scale(deg2rad*tess['s'], deg2rad*tess['n'])

            # These are the integration limits
            r1 = self.R - tess['bottom']
            r2 = self.R - tess['top']

            # This is the scale fator that will multiply the GLQ summation due
            # to the scaling of the abscissas
            # For practicality I included the density of the tesseroid and the
            # gravitational constants as well.
            factor = ((deg2rad**2)*\
                      (tess['e'] - tess['w'])*\
                      (tess['n'] - tess['s'])*\
                      tess['density']*self.G*self.unit) / 4.0

            # Caculate the field at every point on the grid and sum to what was
            # there before.
            grid = zip(*[range(0, len(lonsrad)), lonsrad, latsrad, rs])
            for i, lamb, phi, r in grid:

                # Initialize the result with zero
                res = 0.0

                # Do the GLQ summation to calculate the field
                for lambl, wlon in zip(*[self.ablon.val, self.wlon.val]):
                    for phil, wlat in zip(*[self.ablat.val, self.wlat.val]):

                        # Calculate the integral kernel K
                        K = self.kernel(r, lamb, phi, r1, r2, lambl, phil)

                        # Add the weighted kernel at the current
                        # integration point to the result
                        res += wlon*wlat*K

                # Add this to the field of the other tesseroids
                # Factor is there to account for the change in variables
                # done when scaling the abscissas to the correct interval.
                # It also contains the density of the tesseroid, the
                # gravitational constant and a unit conversion factor (only
                # if needed).
                field[i] += factor*res

        return field


    def kernel(self, r, lamb, phi, r1, r2, lambl, phil):
        """
        The kernel of the 2D integral defining the gravitational field.
        """
        pass
################################################################################


################################################################################
# TESSEROIDPOTENTIAL CLASS

class TesseroidPotential(TesseroidGravity):
    """
    This class is used to calculate the GRAVITY POTENTIAL field of a tesseroid
    model. Calculations are performed on a grid. A Gauss-Legendre Quadrature is
    used in order to solve the integral that defines the field. The Abscissas
    and Weights classes used in this GLQ must be passed to __init__ (see
    __init__ doc for more on the input parameters).
    See glq module doc for more on Abscissas and Weights.
    Abscissas must be in the radial, lon, and lat directions (and so must the
    weights). Use the 'calculate' method to calculate a the field (see calculate
    doc for input parameters and output).
    """

    def __init__(self, abr, ablon, ablat, wr, wlon, wlat):
        """
        Pass the Abscissas and associated Weights that will be used in the GLQ.
        Parameters:
            abr   - Abscissas in the radial direction;
            ablon - Abscissas in the longitude direction;
            ablat - Abscissas in the latitude direction;
            wr    - Weights in the radial direction;
            wlon  - Weights in the longitude direction;
            wlat  - Weights in the latitude direction;
        """
        # Call the base class constructor to set the parameters
        TesseroidGravity.__init__(self, abr, ablon, ablat, wr, wlon, wlat)


    def kernel3D(self, r, lamb, phi, rl, lambl, phil):
        """
        The kernel of the integral defining the gravitational field.
        """        
        rl_2 = rl**2
        cosPhil = cos(phil)
        cosPsi = sin(phi)*sin(phil) + cos(phi)*cosPhil*cos(lamb-lambl)
        l = sqrt( (r**2) + (rl_2) - (2*r*rl*cosPsi) )    
        return (rl_2*cosPhil) / l

   
    def kernel(self, r, lamb, phi, r1, r2, lambl, phil):
        """
        The kernel of the 2D integral defining the gravitational field.
        Raises a SingularityError when the computation point is aligned with a
        GLQ node (i.e. l2 + r2 - (r*cosPsi) = 0 and l1 + r1 - (r*cosPsi) = 0).
        """
        try:
            r_2 = r**2
            cosPhil = cos(phil)
            cosPsi = sin(phi)*sin(phil) + cos(phi)*cosPhil*cos(lamb - lambl)
            l1 = sqrt( r_2 + (r1**2) - (2*r*r1*cosPsi) )
            l2 = sqrt( r_2 + (r2**2) - (2*r*r2*cosPsi) )
            lntop = l2 + r2 - (r*cosPsi)
            lnbot = l1 + r1 - (r*cosPsi)
            ln = log(abs(lntop/lnbot))
            return cosPhil*0.5*( r2*l2 - r1*l1 + 3*r*cosPsi*(l2 - l1) + \
                                 r_2*((3*cosPsi*cosPsi) - 1)*ln )
        except ZeroDivisionError:
            rad2deg = 180.0/pi
            msg = "Singularity occured due to computation point " + \
                  "(lon=%g, lat=%g) " % (lamb*rad2deg, phi*rad2deg) + \
                  "being aligned with a GLQ node in TesseroidPotential."
            raise SingularityError, msg
                    

################################################################################


################################################################################
# TESSEROIDGX CLASS

class TesseroidGx(TesseroidGravity):
    """
    This class is used to calculate the X COMPONENT OF GRAVITY field of a
    tesseroid model.
    Calculations are performed on a grid. A Gauss-Legendre Quadrature is used in
    order to solve the integral that defines the field. The Abscissas and
    Weights classes used in this GLQ must be passed to __init__ (see __init__
    doc for more on the input parameters). See glq module doc for more on
    Abscissas and Weights.
    Abscissas must be in the radial, lon, and lat directions (and so must the
    weights). Use the 'calculate' method to calculate a the field (see calculate
    doc for input parameters and output).
    """

    def __init__(self, abr, ablon, ablat, wr, wlon, wlat):
        """
        Pass the Abscissas and associated Weights that will be used in the GLQ.
        Parameters:
            abr   - Abscissas in the radial direction;
            ablon - Abscissas in the longitude direction;
            ablat - Abscissas in the latitude direction;
            wr    - Weights in the radial direction;
            wlon  - Weights in the longitude direction;
            wlat  - Weights in the latitude direction;
        """
        # Call the base class constructor to set the parameters
        TesseroidGravity.__init__(self, abr, ablon, ablat, wr, wlon, wlat)
        self.unit = self.si2mgal
    
    def kernel3D(self, r, lamb, phi, rl, lambl, phil):
        """
        The kernel of the 3D integral defining the gravitational field.
        """
        rl_2 = rl**2
        cosPhil = cos(phil)
        cosPhi = cos(phi)
        sinPhi = sin(phi)
        sinPhil = sin(phil)
        cosLambLambl = cos(lamb - lambl)
        cosPsi = sinPhi*sinPhil + cosPhi*cosPhil*cosLambLambl
        l = sqrt( (r**2) + (rl_2) - (2*r*rl*cosPsi) )
        return ((sinPhil*cosPhi - cosPhil*sinPhi*cosLambLambl)*rl_2*rl*cosPhil)\
                                                                        / (l**3)

    def kernel(self, r, lamb, phi, r1, r2, lambl, phil):
        """
        The kernel of the 2D integral defining the gravitational field.
        """
        try:
            r_2 = r**2
            r1_2 = r1**2
            r2_2 = r2**2
            cosPhil = cos(phil)
            cosPhi = cos(phi)
            sinPhi = sin(phi)
            sinPhil = sin(phil)
            cosLambLambl = cos(lamb-lambl)
            cosPsi = sinPhi*sinPhil + cosPhi*cosPhil*cosLambLambl
            cosPsiPhi = cosPhi*sinPhil - sinPhi*cosPhil*cosLambLambl
            l1 = sqrt( r_2 + (r1_2) - (2*r*r1*cosPsi) )
            l2 = sqrt( r_2 + (r2_2) - (2*r*r2*cosPsi) )
            r1l1 = r1/l1
            r2l2 = r2/l2
            lntop = l2 + r2 - (r*cosPsi)
            lnbot = l1 + r1 - (r*cosPsi)
            ln = log(abs(lntop/lnbot))
            return (cosPhil*cosPsiPhi/(2*r))*( \
                    r*(r1*r1l1 - r2*r2l2) + \
                    3*r*(l2 - l1 + r*cosPsi*(r1l1 - r2l2)) + \
                    6*r_2*cosPsi*ln + \
                    r_2*r*((3*cosPsi*cosPsi) - 1)*( \
                        ((r1 + l1)/(l1*lnbot)) - ((r2 + l2)/(l2*lntop)) ) )
        except ZeroDivisionError:
            rad2deg = 180.0/pi
            msg = "Singularity occured due to computation point " + \
                  "(lon=%g, lat=%g) " % (lamb*rad2deg, phi*rad2deg) + \
                  "being aligned with a GLQ node in TesseroidGx."
            raise SingularityError, msg

################################################################################



################################################################################
# TESSEROIDGY CLASS

class TesseroidGy(TesseroidGravity):
    """
    This class is used to calculate the Y COMPONENT OF GRAVITY field of a
    tesseroid model.
    Calculations are performed on a grid. A Gauss-Legendre Quadrature is used in
    order to solve the integral that defines the field. The Abscissas and
    Weights classes used in this GLQ must be passed to __init__ (see __init__
    doc for more on the input parameters). See glq module doc for more on
    Abscissas and Weights.
    Abscissas must be in the radial, lon, and lat directions (and so must the
    weights). Use the 'calculate' method to calculate a the field (see calculate
    doc for input parameters and output).
    """

    def __init__(self, abr, ablon, ablat, wr, wlon, wlat):
        """
        Pass the Abscissas and associated Weights that will be used in the GLQ.
        Parameters:
            abr   - Abscissas in the radial direction;
            ablon - Abscissas in the longitude direction;
            ablat - Abscissas in the latitude direction;
            wr    - Weights in the radial direction;
            wlon  - Weights in the longitude direction;
            wlat  - Weights in the latitude direction;
        """
        # Call the base class constructor to set the parameters
        TesseroidGravity.__init__(self, abr, ablon, ablat, wr, wlon, wlat)
        self.unit = self.si2mgal

    
    def kernel3D(self, r, lamb, phi, rl, lambl, phil):
        """
        The kernel of the 3D integral defining the gravitational field.
        """
        rl_2 = rl**2
        cosPhil = cos(phil)
        cosPsi = sin(phi)*sin(phil) + cos(phi)*cosPhil*cos(lamb - lambl)
        l = sqrt( (r**2) + (rl_2) - (2*r*rl*cosPsi) )
        return (rl_2*cosPhil*(-1)*rl*cosPhil*sin(lamb - lambl)) / (l**3)


    def kernel(self, r, lamb, phi, r1, r2, lambl, phil):
        """
        The kernel of the 2D integral defining the gravitational field.
        """
        try:
            r_2 = r**2
            r1_2 = r1**2
            r2_2 = r2**2
            cosPhil = cos(phil)
            cosPhi = cos(phi)
            cosPsi = sin(phi)*sin(phil) + cosPhi*cosPhil*cos(lamb-lambl)
            cosPsiLamb = (-1)*cosPhi*cosPhil*sin(lamb - lambl)
            l1 = sqrt( r_2 + (r1_2) - (2*r*r1*cosPsi) )
            l2 = sqrt( r_2 + (r2_2) - (2*r*r2*cosPsi) )
            r1l1 = r1/l1
            r2l2 = r2/l2
            lntop = l2 + r2 - (r*cosPsi)
            lnbot = l1 + r1 - (r*cosPsi)
            ln = log(abs(lntop/lnbot))
            return ((cosPsiLamb*cosPhil)/(2*r*cosPhi))*( \
                    r*(r1*r1l1 - r2*r2l2) + \
                    3*r*(l2 - l1 + r*cosPsi*(r1l1 - r2l2)) + \
                    6*r_2*cosPsi*ln + \
                    r_2*r*((3*cosPsi*cosPsi) - 1)*( \
                        (r1 + l1)/(l1*lnbot) - (r2+l2)/(l2*lntop) ) )
        except ZeroDivisionError:
            rad2deg = 180.0/pi
            msg = "Singularity occured due to computation point " + \
                  "(lon=%g, lat=%g) " % (lamb*rad2deg, phi*rad2deg) + \
                  "being aligned with a GLQ node in TesseroidGy."
            raise SingularityError, msg

################################################################################



################################################################################
# TESSEROIDGZ CLASS

class TesseroidGz(TesseroidGravity):
    """
    This class is used to calculate the Z COMPONENT OF GRAVITY field of a
    tesseroid model.
    Calculations are performed on a grid. A Gauss-Legendre Quadrature is used in
    order to solve the integral that defines the field. The Abscissas and
    Weights classes used in this GLQ must be passed to __init__ (see __init__
    doc for more on the input parameters). See glq module doc for more on
    Abscissas and Weights.
    Abscissas must be in the radial, lon, and lat directions (and so must the
    weights). Use the 'calculate' method to calculate a the field (see calculate
    doc for input parameters and output).
    """

    def __init__(self, abr, ablon, ablat, wr, wlon, wlat):
        """
        Pass the Abscissas and associated Weights that will be used in the GLQ.
        Parameters:
            abr   - Abscissas in the radial direction;
            ablon - Abscissas in the longitude direction;
            ablat - Abscissas in the latitude direction;
            wr    - Weights in the radial direction;
            wlon  - Weights in the longitude direction;
            wlat  - Weights in the latitude direction;
        """
        # Call the base class constructor to set the parameters
        TesseroidGravity.__init__(self, abr, ablon, ablat, wr, wlon, wlat)
        self.unit = self.si2mgal


    def kernel3D(self, r, lamb, phi, rl, lambl, phil):
        """
        The kernel of the 3D integral defining the gravitational field.
        """
        rl_2 = rl**2
        cosPhil = cos(phil)
        cosPsi = sin(phi)*sin(phil) + cos(phi)*cosPhil*cos(lamb - lambl)
        l = sqrt( (r**2) + (rl_2) - (2*r*rl*cosPsi) )
        return rl_2*cosPhil*((r - (rl*cosPsi)) / (l**3))


    def kernel(self, r, lamb, phi, r1, r2, lambl, phil):
        """
        The kernel of the 2D integral defining the gravitational field.
        """
        try:
            r_2 = r**2
            r1_2 = r1**2
            r2_2 = r2**2
            cosPhil = cos(phil)
            cosPsi = sin(phi)*sin(phil) + cos(phi)*cosPhil*cos(lamb - lambl)
            l1 = sqrt( r_2 + (r1_2) - (2*r*r1*cosPsi) )
            l2 = sqrt( r_2 + (r2_2) - (2*r*r2*cosPsi) )
            lntop = l2 + r2 - (r*cosPsi)
            lnbot = l1 + r1 - (r*cosPsi)
            ln = log( abs(lntop / lnbot) )
            return (cosPhil/r)*( r1*l1 - r2*l2 - 3*r*cosPsi*(l2 - l1) - \
                                    r_2*((3*cosPsi*cosPsi) - 1)*ln + (r2*r2_2)/l2 - \
                                    (r1*r1_2)/l1 )
        except ZeroDivisionError:
            rad2deg = 180.0/pi
            msg = "Singularity occured due to computation point " + \
                  "(lon=%g, lat=%g) " % (lamb*rad2deg, phi*rad2deg) + \
                  "being aligned with a GLQ node in TesseroidGz."
            raise SingularityError, msg

################################################################################



################################################################################
# TESSEROIDGXX CLASS

class TesseroidGxx(TesseroidGravity):
    """
    This class is used to calculate the XX COMPONENT OF THE GRAVITY GRADIENT
    TENSOR field of a tesseroid model.
    Calculations are performed on a grid. A Gauss-Legendre Quadrature is used in
    order to solve the integral that defines the field. The Abscissas and
    Weights classes used in this GLQ must be passed to __init__ (see __init__
    doc for more on the input parameters). See glq module doc for more on
    Abscissas and Weights.
    Abscissas must be in the radial, lon, and lat directions (and so must the
    weights). Use the 'calculate' method to calculate a the field (see calculate
    doc for input parameters and output).
    """

    def __init__(self, abr, ablon, ablat, wr, wlon, wlat):
        """
        Pass the Abscissas and associated Weights that will be used in the GLQ.
        Parameters:
            abr   - Abscissas in the radial direction;
            ablon - Abscissas in the longitude direction;
            ablat - Abscissas in the latitude direction;
            wr    - Weights in the radial direction;
            wlon  - Weights in the longitude direction;
            wlat  - Weights in the latitude direction;
        """
        # Call the base class constructor to set the parameters
        TesseroidGravity.__init__(self, abr, ablon, ablat, wr, wlon, wlat)
        self.unit = self.si2eotvos


    def kernel3D(self, r, lamb, phi, rl, lambl, phil):
        """
        The kernel of the 3D integral defining the gravitational field.
        """
        rl_2 = rl**2
        r_2 = r**2
        cosPhil = cos(phil)
        cosPhi = cos(phi)
        sinPhil = sin(phil)
        sinPhi = sin(phi)
        cosLambLambl = cos(lamb - lambl)
        cosPsi = sinPhi*sinPhil + cosPhi*cosPhil*cosLambLambl
        cosPsiPhi = cosPhi*sinPhil - sinPhi*cosPhil*cosLambLambl
        l = sqrt( (r_2) + (rl_2) - (2*r*rl*cosPsi) )
        l_3 = l**3
        rrl = r*rl
        return rl_2*cosPhil*(( (3*rrl*rrl*cosPsiPhi*cosPsiPhi)/(l_3*l*l) - \
                               (rrl*cosPsi)/(l_3) + \
                               r*(((rl*cosPsi) - r)/l_3) ) / r_2 )


    def kernel(self, r, lamb, phi, r1, r2, lambl, phil):
        """
        The kernel of the 2D integral defining the gravitational field.
        """
        try:
            r_2 = r**2
            r1_2 = r1**2
            r2_2 = r2**2
            cosPhil = cos(phil)
            cosPhi = cos(phi)
            sinPhil = sin(phil)
            sinPhi = sin(phi)
            cosLambLambl = cos(lamb - lambl)
            cosPsi = sinPhi*sinPhil + cosPhi*cosPhil*cosLambLambl
            cosPsiPhi = cosPhi*sinPhil - sinPhi*cosPhil*cosLambLambl
            cosPsiPhi_2 = cosPsiPhi**2
            l1 = sqrt( r_2 + (r1_2) - (2*r*r1*cosPsi) )
            l2 = sqrt( r_2 + (r2_2) - (2*r*r2*cosPsi) )
            l1_2 = l1**2
            l2_2 = l2**2
            r1l1 = r1 / l1
            r2l2 = r2 / l2
            lntop = l2 + r2 - (r*cosPsi)
            lnbot = l1 + r1 - (r*cosPsi)
            cosPsi_2_1 = (3*cosPsi*cosPsi) - 1
            ln = log(abs( lntop / lnbot ) )

            # Kphi**2
            t1 = (r2l2*r2l2*r/l2)*(r*r2*cosPsiPhi_2 - l2_2*cosPsi)
            t2 = (r1l1*r1l1*r/l1)*(r*r1*cosPsiPhi_2 - l1_2*cosPsi)
            t3 = 3*r_2*cosPsiPhi_2*( 2*(r1l1 - r2l2) + \
                                    r*cosPsi*(r1l1*r1l1/l1 - r2l2*r2l2/l2) )
            t4 = 3*r*cosPsi*(l2 - l1 + cosPsi*(r*r1l1 - r*r2l2))
            t5 = 6*r_2*ln*(cosPsiPhi_2 - cosPsi*cosPsi)
            t6 = r_2*r*cosPsi*(12*cosPsiPhi_2 - cosPsi_2_1)*( \
                    (r1 + l1)/(l1*lnbot) - (r2 + l2)/(l2*lntop) )
            t7 = r_2*r_2*cosPsiPhi_2*cosPsi_2_1*( \
                    (r1*lnbot - (r1 + l1)*(r1l1*lnbot + r1 + l1)) / \
                    (l1*l1*lnbot*lnbot) - \
                    (r2*lntop - (r2 + l2)*(r2l2*lntop + r2 + l2)) / \
                    (l2*l2*lntop*lntop) )
            Kphi_2 = 0.5*( t2 - t1 + t3 - t4 + t5 + t6 - t7 )

            # Kr
            Kr = ( r2*l2 - r1*l1 + 3*r*cosPsi*(l2 - l1) + r_2*cosPsi_2_1*ln - \
                r2l2*r2_2 + r1l1*r1_2 ) / r

            return cosPhil*(Kphi_2 + r*Kr) / r_2

        except ZeroDivisionError:
            rad2deg = 180.0/pi
            msg = "Singularity occured due to computation point " + \
                  "(lon=%g, lat=%g) " % (lamb*rad2deg, phi*rad2deg) + \
                  "being aligned with a GLQ node in TesseroidGxx."
            raise SingularityError, msg
################################################################################


################################################################################
# TIMING TESTS

def time_potential(repetitions, num_times):
    """
    Counts the time it takes to execute TesseroidPotential.calculate number of
    times.
    Parameters:
        repetitions - the number calculate will be executed per iteration. The
                      time returned is the time it takes to all repetions;
        num_times   - the number of iterations performed;

    Prints to stdout the times per iteration, max of all iterations, min of all
    iterations, and total time.
    The model used is a one tesseroid model calculated over a 100x100 grid.
    """

    # The setup statement that import everything, instantiates the Abscissas
    # and Weights, and creates the grids.
    str = \
"""
import glq
import tesseroid as t
import __main__ as tg
import scipy as s
tess = t.Tesseroid(27.5,32.5,-2.4,2.5,0,40000,1.0,'TEST')
mod = [tess]
o = 5
ablon = glq.Abscissas(o)
ablat = glq.Abscissas(o)
abr = glq.Abscissas(o)
wlon = glq.Weights(ablon)
wlat = glq.Weights(ablat)
wr = glq.Weights(abr)
tp = tg.TesseroidPotential(abr,ablon,ablat,wr,wlon,wlat)
lons = s.arange(5,55.5,0.5)
lats = s.arange(-25,25.5,0.5)
hs = []
lns = []
lts = []
for lon in lons:
    for lat in lats:
        lns.append(lon)
        lts.append(lat)
        hs.append(100000)
"""

    print "Timing %d iterations of calculate %d times on a 100x100 grid...\n" \
            % (repetitions, num_times)
    times = []
    total = 0.0
    timer = Timer(stmt='tp.calculate(mod, lns, lts, hs)', setup=str)
    print "2D ALGORITHM:\n  Iteration: Time (s)"
    for i in range(1, num_times+1):
        t = timer.timeit(repetitions)
        print "  %d: %g" % (i, t)
        times.append(t)
        total += t

    print "  Max: %g" % (max(times))
    print "  Min: %g" % (min(times))
    print "  Total: %g" % (total)

    times = []
    total = 0.0
    timer = Timer(stmt='tp.calculate3D(mod, lns, lts, hs)', setup=str)
    print "3D ALGORITHM:\n  Iteration: Time (s)"
    for i in range(1, num_times+1):
        t = timer.timeit(repetitions)
        print "  %d: %g" % (i, t)
        times.append(t)
        total += t

    print "  Max: %g" % (max(times))
    print "  Min: %g" % (min(times))
    print "  Total: %g" % (total)

################################################################################


################################################################################
# CALCULATE THE FIELDS FOR A TEST

def calc_all(height):
    """
    Calculate all the fields for a 2 tesseroid model, one with positive and one
    with negative density on a 100x100 grid at 250km altitude and a 5x5x5 GLQ.
    """
    import glq
    import scipy as s
    import pylab as p
    import tesseroid as t

    # Make the model
    print "Creating model..."
    tess1 = t.Tesseroid(19, 21, -1, 1, 0, 40000, 1.0, 'pos')
    tess2 = t.Tesseroid(29, 31, -1, 1, 0, 40000, -1.0, 'neg')
    model = [tess1, tess2]

    # Make the abscissas and weights
    print "Calculating abscissas and weights..."
    ablon = glq.Abscissas(5)
    ablat = glq.Abscissas(5)
    abr = glq.Abscissas(5)
    wlon = glq.Weights(ablon)
    wlat = glq.Weights(ablat)
    wr = glq.Weights(abr)

    # Make the grid
    print "Creating grid..."
    lons = s.arange(10, 40.3, 0.3)
    lats = s.arange(-15, 15.3, 0.3)
    glons, glats = p.meshgrid(lons, lats)
    heights = [[height]*len(lons) for i in range(0, len(lats))]

    # Make the field calculator obejects
    print "Creating calculator objects..."
    tesspot = TesseroidPotential(abr, ablon, ablat, wr, wlon, wlat)
    tessgx = TesseroidGx(abr, ablon, ablat, wr, wlon, wlat)
    tessgy = TesseroidGy(abr, ablon, ablat, wr, wlon, wlat)
    tessgz = TesseroidGz(abr, ablon, ablat, wr, wlon, wlat)

    # Calculate the fields
    print "Calculating fields:"
    print "Potential..."
    pot = tesspot.calculate_kernel(model, lons, lats, heights)
    #print "Gx..."
    #gx = tessgx.calculate2D(model, lons, lats, heights)
#    print "Gy..."
#    gy = tessgy.calculate(model, lons, lats, heights)
#    print "Gz..."
#    gz = tessgz.calculate(model, lons, lats, heights)

    # Plot the maps
    print "Plotting maps:"
    fields = []
    fields.append(pot)
#    fields.append(gx)
#    fields.append(gy)
#    fields.append(gz)
    names = []
    names.append('Potential at %g' % (height))
    #names.append('Gx at %g' % (height))
#    names.append('Gy at %g' % (height))
#    names.append('Gz at %g' % (height))
    for field, name in zip(*[fields, names]):

        print "%s..." % (name)
        p.figure()
        p.title(name)
        p.pcolor(glons, glats, field, cmap=p.cm.jet)
        p.colorbar(orientation='horizontal')

        # Plot the tesseroids with 'white' for the positive density and 'black'
        # for the negative density
        tess1lons = [tess1['w'], tess1['e'], tess1['e'], tess1['w'], tess1['w']]
        tess1lats = [tess1['s'], tess1['s'], tess1['n'], tess1['n'], tess1['s']]
        p.plot(tess1lons,tess1lats,'-w',linewidth=1)
        tess2lons = [tess2['w'], tess2['e'], tess2['e'], tess2['w'], tess2['w']]
        tess2lats = [tess2['s'], tess2['s'], tess2['n'], tess2['n'], tess2['s']]
        p.plot(tess2lons,tess2lats,'-k',linewidth=1)
        p.xlim(10, 40)
        p.ylim(-15, 15)

    print "Done!"
    p.show()

################################################################################

if __name__ == '__main__':

    time_potential(100, 10)

