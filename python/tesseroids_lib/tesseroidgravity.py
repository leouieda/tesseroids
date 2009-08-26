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
    si2eotvos = 10.0**9
    # Conversion factor from SI units to mGal: 1 m/s**2 = 10**5 mGal
    si2mgal = 10.0**5
    # Maximum value of cos(Psi) to use the 2D algorithm without raising a
    # SingularityError
    max_cosPsi = 0.99999999


    def __init__(self, ablon, wlon, ablat, wlat, abr, wr):
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
        NOTICE: This function is slower than the 2D GLQ implemented in 
        'calculate'. Also, it has no optimizations and is not being maintained.

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
                for lambl, wlon in zip(*[self.ablon._val, self.wlon._val]):
                    for phil, wlat in zip(*[self.ablat._val, self.wlat._val]):
                        for rl, wr in zip(*[self.abr._val, self.wr._val]):

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

        grid = zip(*[range(0, len(lonsrad)), lonsrad, latsrad, rs])

        # Make a local copies, they run faster
        kernel_local = self.kernel
        kernel3d_local = self.kernel3D
        ablon_local = self.ablon
        wlon_local = self.wlon
        ablat_local = self.ablat
        wlat_local = self.wlat
        abr_local = self.abr
        wr_local = self.wr
        R_local = self.R
        G_local = self.G
        unit_local = self.unit
       
        # First initialize the array that will hold the field values
        field = [0.0]*len(lons)

        # Iterate over the tesseroids in the model and sum their fields on the
        # grid points.
        for tess in model:

            # Scale the abscissas to the dimensions of the tesseroid
            ablon_local.scale(deg2rad*tess['w'], deg2rad*tess['e'])
            ablat_local.scale(deg2rad*tess['s'], deg2rad*tess['n'])

            # These are the integration limits
            r1 = R_local - tess['bottom']
            r2 = R_local - tess['top']

            # This is the scale fator that will multiply the GLQ summation due
            # to the scaling of the abscissas
            # For practicality I included the density of the tesseroid and the
            # gravitational constants as well.
            factor = ((deg2rad**2)*\
                      (tess['e'] - tess['w'])*\
                      (tess['n'] - tess['s'])*\
                      tess['density']*G_local*unit_local) / 4.0
            # The 3D GLQ version of the scale factor (only used when 3D GLQ is
            # needed).
            factor3d = ((deg2rad**2)*(tess['e'] - tess['w'])*\
                      (tess['n'] - tess['s'])*\
                      (tess['bottom'] - tess['top'])*\
                      tess['density']*G_local*unit_local) / 8.0

            # Instead of putting this in the loop, do it only once
            abs_w_lon = zip(*[ablon_local._val, wlon_local._val])
            abs_w_lat = zip(*[ablat_local._val, wlat_local._val])

            # Caculate the field at every point on the grid and sum to what was
            # there before.                        
            for i, lamb, phi, r in grid:

                # There is a singularity in the 2D algorithm when the
                # computation point is aligned with a GLQ node. If this
                # singularity is encountered, use the slower 3D algorithm to do
                # the calculation on this point.
                try:
                    # Initialize the result with zero
                    res = 0.0

                    # Do the GLQ summation to calculate the field
                    for lambl, wlon in abs_w_lon:
                        for phil, wlat in abs_w_lat:

                            # Calculate the integral kernel K
                            K = kernel_local(r, lamb, phi, r1, r2, lambl, phil)

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

                except SingularityError, e:

                    self.logger.debug("In tesseroid '%s': " % (tess['tag']) + \
                        str(e) + " Using 3D algorithm instead.")

                    # Do the 3D algorithm since the 2D failed

                    # Take this out of the loop
                    abr_local.scale(r1, r2)
                    abs_w_r = zip(*[abr_local._val, wr_local._val])

                    # Initialize the result with zero
                    res = 0.0

                    # Do the GLQ summation to calculate the field
                    for lambl, wlon in abs_w_lon:
                        for phil, wlat in abs_w_lat :
                            for rl, wr in abs_w_r:

                                # Calculate the integral kernel K
                                K = kernel3d_local(r, lamb, phi, \
                                                   rl, lambl, phil)

                                # Add the kernel at the current integration
                                # point to the result
                                res += wlon*wlat*wr*K

                    # Add this to the field of the other tesseroids
                    # Factor is there to account for the change in variables
                    # done when scaling the abscissas to the correct interval.
                    # It also contains the density of the tesseroid, the
                    # gravitational constant and a unit conversion factor (only
                    # if needed).
                    field[i] += factor3d*res

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

    def __init__(self, ablon, wlon, ablat, wlat, abr=None, wr=None):
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
        TesseroidGravity.__init__(self, ablon, wlon, ablat, wlat, abr, wr)
        # Create a logger
        self.logger = logging.getLogger('tesseroidgravity.potential')


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

    def __init__(self, ablon, wlon, ablat, wlat, abr=None, wr=None):
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
        TesseroidGravity.__init__(self, ablon, wlon, ablat, wlat, abr, wr)
        self.unit = self.si2mgal
        # Create a logger
        self.logger = logging.getLogger('tesseroidgravity.gx')
    
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

    def __init__(self, ablon, wlon, ablat, wlat, abr=None, wr=None):
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
        TesseroidGravity.__init__(self, ablon, wlon, ablat, wlat, abr, wr)
        self.unit = self.si2mgal
        # Create a logger
        self.logger = logging.getLogger('tesseroidgravity.gy')

    
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

    def __init__(self, ablon, wlon, ablat, wlat, abr=None, wr=None):
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
        TesseroidGravity.__init__(self, ablon, wlon, ablat, wlat, abr, wr)
        self.unit = self.si2mgal
        # Create a logger
        self.logger = logging.getLogger('tesseroidgravity.gz')


    def kernel3D(self, r, lamb, phi, rl, lambl, phil):
        """
        The kernel of the 3D integral defining the gravitational field.
        """
        rl_2 = rl**2
        cosPhil = cos(phil)
        cosPsi = sin(phi)*sin(phil) + cos(phi)*cosPhil*cos(lamb - lambl)
        l = sqrt( (r**2) + (rl_2) - (2*r*rl*cosPsi) )
        return rl_2*cosPhil*(((rl*cosPsi) - r) / (l**3))


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
            return (cosPhil/r)*(r2*l2 - r1*l1 + 3*r*cosPsi*(l2 - l1) + \
                                   r_2*(3*(cosPsi**2) - 1)*ln - (r2*r2_2)/l2 + \
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

    def __init__(self, ablon, wlon, ablat, wlat, abr=None, wr=None):
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
        TesseroidGravity.__init__(self, ablon, wlon, ablat, wlat, abr, wr)
        self.unit = self.si2eotvos
        # Create a logger
        self.logger = logging.getLogger('tesseroidgravity.gxx')


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
            # Check if cosPsi is above the minimum for a 'safe' 2D GLQ
            # If it's bellow this, raise a SingularityError to force a 3D GLQ
            # This is necessary because the proximity of the GLQ node to the
            # calculation point causes some term to be too big and a strange
            # very high value to apear in the resulting field.
            if cosPsi >= self.max_cosPsi:
                rad2deg = 180.0/pi
                msg = "Singularity occured due to computation point " + \
                  "(lon=%g, lat=%g) " % (lamb*rad2deg, phi*rad2deg) + \
                  "being too close to a GLQ node in TesseroidGxx. This " + \
                  "could cause the 2D algorithm to fail."
                raise SingularityError, msg

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
            Kr = (r2*l2 - r1*l1 + 3*r*cosPsi*(l2 - l1) + r_2*cosPsi_2_1*ln - \
                r2l2*r2_2 + r1l1*r1_2) / r

            return cosPhil*(Kphi_2 + r*Kr) / r_2

        except ZeroDivisionError:
            rad2deg = 180.0/pi
            msg = "Singularity occured due to computation point " + \
                  "(lon=%g, lat=%g) " % (lamb*rad2deg, phi*rad2deg) + \
                  "being aligned with a GLQ node in TesseroidGxx."
            raise SingularityError, msg
################################################################################


################################################################################
# TESSEROIDGXY CLASS

class TesseroidGxy(TesseroidGravity):
    """
    This class is used to calculate the XY COMPONENT OF THE GRAVITY GRADIENT
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

    def __init__(self, ablon, wlon, ablat, wlat, abr=None, wr=None):
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
        TesseroidGravity.__init__(self, ablon, wlon, ablat, wlat, abr, wr)
        self.unit = self.si2eotvos
        # Create a logger
        self.logger = logging.getLogger('tesseroidgravity.gxy')


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
        sinLambLambl = sin(lamb - lambl)
        cosPsi = sinPhi*sinPhil + cosPhi*cosPhil*cosLambLambl
        cosPsiLamb = (-1)*cosPhi*cosPhil*sinLambLambl
        cosPsiPhi = cosPhi*sinPhil - sinPhi*cosPhil*cosLambLambl
        l = sqrt( (r_2) + (rl_2) - (2*r*rl*cosPsi) )
        l_3 = l**3
        rrl_l3 = (r*rl)/l_3
        return rl_2*cosPhil*( ( rrl_l3*((3*r*rl*cosPsiLamb*cosPsiPhi)/(l**2) + \
                                        (sinPhi*cosPhil*sinLambLambl)) + \
                                ((sinPhi/cosPhi)*(rrl_l3*cosPsiLamb))) \
                              / (r_2*cosPhi) )


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
            sinLambLambl = sin(lamb - lambl)
            cosPsi = sinPhi*sinPhil + cosPhi*cosPhil*cosLambLambl
            # Check if cosPsi is above the minimum for a 'safe' 2D GLQ
            # If it's bellow this, raise a SingularityError to force a 3D GLQ
            # This is necessary because the proximity of the GLQ node to the
            # calculation point causes some term to be too big and a strange
            # very high value to apear in the resulting field.
            if cosPsi >= self.max_cosPsi:
                rad2deg = 180.0/pi
                msg = "Singularity occured due to computation point " + \
                  "(lon=%g, lat=%g) " % (lamb*rad2deg, phi*rad2deg) + \
                  "being too close to a GLQ node in TesseroidGxy. This " + \
                  "could cause the 2D algorithm to fail."
                raise SingularityError, msg

            cosPsiPhi = cosPhi*sinPhil - sinPhi*cosPhil*cosLambLambl
            cosPsiLamb = (-1)*cosPhi*cosPhil*sinLambLambl
            cosPsiPhiXLamb = cosPsiPhi*cosPsiLamb
            cosPsiPhiLamb = sinPhi*cosPhil*sinLambLambl
            l1 = sqrt( r_2 + (r1_2) - (2*r*r1*cosPsi) )
            l2 = sqrt( r_2 + (r2_2) - (2*r*r2*cosPsi) )
            l1_2 = l1*l1
            l2_2 = l2*l2
            rr1_l1 = (r*r1) / l1
            rr2_l2 = (r*r2) / l2
            lntop = l2 + r2 - (r*cosPsi)
            lnbot = l1 + r1 - (r*cosPsi)
            ln = log(abs( lntop / lnbot ))
            cosPsi_2_1 = 3*(cosPsi**2) - 1
            sumt3 = l2 - l1 + cosPsi*(rr1_l1 - rr2_l2)
            sumt6 = (r1 + l1)/(l1*lnbot) - (r2 + l2)/(l2*lntop)
            
            # KVphilamb
            t1 = (rr2_l2*r2/l2_2)*(l2_2*cosPsiPhiLamb + r*r2*cosPsiPhiXLamb)
            t2 = (rr1_l1*r1/l1_2)*(l1_2*cosPsiPhiLamb + r*r1*cosPsiPhiXLamb)
            t3 = 3*r*cosPsiPhiLamb*sumt3
            t4 = 6*r_2*ln*(cosPsi*cosPsiPhiLamb + cosPsiPhiXLamb)
            t5 = 3*r*cosPsiPhiXLamb*( 2*(rr1_l1 - rr2_l2) + cosPsi*cosPsiLamb*(\
                                     (rr1_l1*rr1_l1/l1) - (rr2_l2*rr2_l2/l2) ) )
            t6 = r_2*r*(cosPsi_2_1*cosPsiPhiLamb + 12*cosPsi*cosPsiPhiXLamb)*\
                                                                          sumt6
            t7 = r_2*r_2*cosPsi_2_1*cosPsiPhiXLamb*(\
                     (r1*lnbot - (r1 + l1)*((r1/l1)*lnbot + r1 + l1))/ \
                                                        (l1_2*lnbot*lnbot) - \
                     (r2*lntop - (r2 + l2)*((r2/l2)*lntop + r2 + l2))/ \
                                                        (l2_2*lntop*lntop))
            KVphilamb = 0.5*( t2 - t1 + t3 + t4 + t5 + t6 - t7 )

            # KVlamb
            KVlamb = 0.5*cosPsiLamb*(rr1_l1*r1 - rr2_l2*r2 + 3*r*sumt3 + \
                                     6*r_2*cosPsi*ln + r_2*r*cosPsi_2_1*sumt6)

            return cosPhil*((KVphilamb + ((sinPhi/cosPhi)*KVlamb))/(r_2*cosPhi))

        except ZeroDivisionError:
            rad2deg = 180.0/pi
            msg = "Singularity occured due to computation point " + \
                  "(lon=%g, lat=%g) " % (lamb*rad2deg, phi*rad2deg) + \
                  "being aligned with a GLQ node in TesseroidGxy."
            raise SingularityError, msg

################################################################################


################################################################################
# TESSEROIDGXZ CLASS

class TesseroidGxz(TesseroidGravity):
    """
    This class is used to calculate the XZ COMPONENT OF THE GRAVITY GRADIENT
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

    def __init__(self, ablon, wlon, ablat, wlat, abr=None, wr=None):
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
        TesseroidGravity.__init__(self, ablon, wlon, ablat, wlat, abr, wr)
        self.unit = self.si2eotvos
        # Create a logger
        self.logger = logging.getLogger('tesseroidgravity.gxz')


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
        rl_l3 = rl / l_3
        return rl_2*cosPhil*rl_l3*cosPsiPhi*( \
                        (3*r*(rl*cosPsi-r)/l**2 + 1) - 1 ) / r



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
            # Check if cosPsi is above the minimum for a 'safe' 2D GLQ
            # If it's bellow this, raise a SingularityError to force a 3D GLQ
            # This is necessary because the proximity of the GLQ node to the
            # calculation point causes some term to be too big and a strange
            # very high value to apear in the resulting field.
            if cosPsi >= self.max_cosPsi:
                rad2deg = 180.0/pi
                msg = "Singularity occured due to computation point " + \
                  "(lon=%g, lat=%g) " % (lamb*rad2deg, phi*rad2deg) + \
                  "being too close to a GLQ node in TesseroidGxz. This " + \
                  "could cause the 2D algorithm to fail."
                raise SingularityError, msg

            cosPsiPhi = cosPhi*sinPhil - sinPhi*cosPhil*cosLambLambl
            l1 = sqrt( r_2 + (r1_2) - (2*r*r1*cosPsi) )
            l2 = sqrt( r_2 + (r2_2) - (2*r*r2*cosPsi) )
            l1_2 = l1*l1
            l2_2 = l2*l2
            rr1_l1 = (r*r1) / l1
            rr2_l2 = (r*r2) / l2
            lntop = l2 + r2 - (r*cosPsi)
            lnbot = l1 + r1 - (r*cosPsi)
            ln = log(abs( lntop / lnbot ))
            cosPsi_2_1 = (3*cosPsi*cosPsi) - 1
            sumt3 = l2 - l1 + cosPsi*(rr1_l1 - rr2_l2)
            sumt6 = (r1 + l1)/(l1*lnbot) - (r2 + l2)/(l2*lntop)
            
            KVphir = (cosPsiPhi/r)*( rr1_l1*r1*r1_2/l1_2 - rr2_l2*r2*r2_2/l2_2 \
                                    + rr1_l1*r1 - rr2_l2*r2 + 3*r*sumt3 + \
                                      6*r_2*cosPsi*ln + r_2*r*cosPsi_2_1*sumt6)

            KVphi = 0.5*cosPsiPhi*(rr1_l1*r1 - rr2_l2*r2 + 3*r*sumt3 + \
                                   6*r_2*cosPsi*ln + r_2*r*cosPsi_2_1*sumt6)

            return cosPhil*( (KVphir - KVphi/r) / r )

        except ZeroDivisionError:
            rad2deg = 180.0/pi
            msg = "Singularity occured due to computation point " + \
                  "(lon=%g, lat=%g) " % (lamb*rad2deg, phi*rad2deg) + \
                  "being aligned with a GLQ node in TesseroidGxz."
            raise SingularityError, msg

################################################################################


################################################################################
# TESSEROIDGYY CLASS

class TesseroidGyy(TesseroidGravity):
    """
    This class is used to calculate the YY COMPONENT OF THE GRAVITY GRADIENT
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

    def __init__(self, ablon, wlon, ablat, wlat, abr=None, wr=None):
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
        TesseroidGravity.__init__(self, ablon, wlon, ablat, wlat, abr, wr)
        self.unit = self.si2eotvos
        # Create a logger
        self.logger = logging.getLogger('tesseroidgravity.gyy')


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
        cosPsiLamb = (-1)*cosPhi*cosPhil*sin(lamb - lambl)
        l = sqrt( (r_2) + (rl_2) - (2*r*rl*cosPsi) )
        l_3 = l**3
        rrl = r*rl
        return rl_2*cosPhil*( ( ((3*rrl*rrl*cosPsiLamb*cosPsiLamb)/(l_3*l*l) - \
                                 (rrl*cosPhi*cosPhil*cosLambLambl)/(l_3)) + \
                                r*cosPhi*cosPhi*((rl*cosPsi - r) / l_3) - \
                                cosPhi*sinPhi*( (rrl*(cosPhi*sinPhil - \
                                                 sinPhi*cosPhil*cosLambLambl))/\
                                          l_3 ) ) / (r_2*cosPhi*cosPhi) )


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
            # Check if cosPsi is above the minimum for a 'safe' 2D GLQ
            # If it's bellow this, raise a SingularityError to force a 3D GLQ
            # This is necessary because the proximity of the GLQ node to the
            # calculation point causes some term to be too big and a strange
            # very high value to apear in the resulting field.
            if cosPsi >= self.max_cosPsi:
                rad2deg = 180.0/pi
                msg = "Singularity occured due to computation point " + \
                  "(lon=%g, lat=%g) " % (lamb*rad2deg, phi*rad2deg) + \
                  "being too close to a GLQ node in TesseroidGyy. This " + \
                  "could cause the 2D algorithm to fail."
                raise SingularityError, msg

            cosPsiPhi = cosPhi*sinPhil - sinPhi*cosPhil*cosLambLambl
            cosPsiLamb = (-1)*cosPhi*cosPhil*sin(lamb - lambl)
            cosPsiLamb_2 = cosPsiLamb*cosPsiLamb
            cosPsiLambLamb = (-1)*cosPhi*cosPhil*cosLambLambl
            l1 = sqrt( r_2 + (r1_2) - (2*r*r1*cosPsi) )
            l2 = sqrt( r_2 + (r2_2) - (2*r*r2*cosPsi) )
            l1_2 = l1*l1
            l2_2 = l2*l2
            r1l1 = r1 / l1
            r2l2 = r2 / l2
            lntop = l2 + r2 - (r*cosPsi)
            lnbot = l1 + r1 - (r*cosPsi)
            ln = log(abs( lntop / lnbot ))
            cosPsi_2_1 = (3*cosPsi*cosPsi) - 1
            
            # KVlamb^2 
            t1 = (r*r2l2*r2l2/l2)*(r*r2*cosPsiLamb_2 + l2_2*cosPsiLambLamb)
            t2 = (r*r1l1*r1l1/l1)*(r*r1*cosPsiLamb_2 + l1_2*cosPsiLambLamb)
            t3 = 3*r_2*cosPsiLamb_2*(2*(r1l1 - r2l2) + \
                                     r*cosPsi*((r1l1*r1l1/l1) - (r2l2*r2l2/l2)))
            t4 = 3*r*cosPsiLambLamb*(l2 - l1  + r*cosPsi*(r1l1 - r2l2))
            t5 = 6*r_2*ln*(cosPsiLamb_2 + cosPsiLambLamb*cosPsi)
            t6 = r_2*r*(12*cosPsi*cosPsiLamb_2 + cosPsiLambLamb*cosPsi_2_1)*\
                       ((r1 + l1)/(l1*lnbot) - (r2 + l2)/(l2*lntop))
            t7 = r_2*r_2*cosPsiLamb_2*cosPsi_2_1*(\
                    (r1*lnbot - (r1 + l1)*(r1l1*lnbot + r1 + l1))/ \
                                                         (l1*l1*lnbot*lnbot) - \
                    (r2*lntop - (r2 + l2)*(r2l2*lntop + r2 + l2))/ \
                                                         (l2*l2*lntop*lntop))
            KVlamb_2 = 0.5*( t2 - t1 + t3 + t4 + t5 + t6 - t7 )

            # KVphi
            KVphi = (cosPsiPhi/2)*(r*r1*r1l1 - r*r2*r2l2 + \
                                   3*r*(l2 - l1 + r*cosPsi*(r1l1 - r2l2)) + \
                                   6*r_2*cosPsi*ln + \
                                   r_2*r*cosPsi_2_1*((r1 + l1)/(l1*lnbot) - \
                                                     (r2 + l2)/(l2*lntop)) )
            # KVr
            KVr = (r2*l2 - r1*l1 + 3*r*cosPsi*(l2 - l1) + r_2*cosPsi_2_1*ln - \
                   r2l2*r2_2 + r1l1*r1_2) / r

            return cosPhil*((KVlamb_2 + r*cosPhi*cosPhi*KVr - \
                             cosPhi*sinPhi*KVphi)/(r_2*cosPhi*cosPhi))

        except ZeroDivisionError:
            rad2deg = 180.0/pi
            msg = "Singularity occured due to computation point " + \
                  "(lon=%g, lat=%g) " % (lamb*rad2deg, phi*rad2deg) + \
                  "being aligned with a GLQ node in TesseroidGyy."
            raise SingularityError, msg

################################################################################


################################################################################
# TESSEROIDGYZ CLASS

class TesseroidGyz(TesseroidGravity):
    """
    This class is used to calculate the YZ COMPONENT OF THE GRAVITY GRADIENT
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

    def __init__(self, ablon, wlon, ablat, wlat, abr=None, wr=None):
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
        TesseroidGravity.__init__(self, ablon, wlon, ablat, wlat, abr, wr)
        self.unit = self.si2eotvos
        # Create a logger
        self.logger = logging.getLogger('tesseroidgravity.gyz')


    def kernel3D(self, r, lamb, phi, rl, lambl, phil):
        """
        The kernel of the 3D integral defining the gravitational field.
        """
        rl_2 = rl**2
        r_2 = r**2
        cosPhil = cos(phil)
        cosPhi = cos(phi)
        cosPsi = sin(phi)*sin(phil) + cosPhi*cosPhil*cos(lamb - lambl)
        l = sqrt( r_2 + rl_2 - (2*r*rl*cosPsi) )
        rlcosPsiLamb_l3 = (-rl*cosPhi*cosPhil*sin(lamb - lambl))/l**3
        # The minus sign is because z points out while r points in
        return rl_2*cosPhil*( \
                            (r*rlcosPsiLamb_l3*3*(rl*cosPsi - r)/l**2 + \
                             rlcosPsiLamb_l3) / (r*cosPhi) )

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
            cosPsi = sin(phi)*sin(phil) + cosPhi*cosPhil*cos(lamb - lambl)
            # Check if cosPsi is above the minimum for a 'safe' 2D GLQ
            # If it's bellow this, raise a SingularityError to force a 3D GLQ
            # This is necessary because the proximity of the GLQ node to the
            # calculation point causes some term to be too big and a strange
            # very high value to apear in the resulting field.
            if cosPsi >= self.max_cosPsi:
                rad2deg = 180.0/pi
                msg = "Singularity occured due to computation point " + \
                  "(lon=%g, lat=%g) " % (lamb*rad2deg, phi*rad2deg) + \
                  "being too close to a GLQ node in TesseroidGyz. This " + \
                  "could cause the 2D algorithm to fail."
                raise SingularityError, msg

            cosPsiLamb = (-1)*cosPhi*cosPhil*sin(lamb - lambl)
            l1 = sqrt( r_2 + r1_2 - (2*r*r1*cosPsi) )
            l2 = sqrt( r_2 + r2_2 - (2*r*r2*cosPsi) )
            l1_2 = l1*l1
            l2_2 = l2*l2
            rr1_l1 = (r*r1) / l1
            rr2_l2 = (r*r2) / l2
            lntop = l2 + r2 - (r*cosPsi)
            lnbot = l1 + r1 - (r*cosPsi)
            ln = log(abs( lntop / lnbot ))
            cosPsi_2_1 = (3*cosPsi*cosPsi) - 1
            sumt3 = l2 - l1 + cosPsi*(rr1_l1 - rr2_l2)
            sumt6 = (r1 + l1)/(l1*lnbot) - (r2+l2)/(l2*lntop)
            
            KVlambr = (cosPsiLamb/r)*(rr1_l1*r1*r1_2/l1_2 - \
                                      rr2_l2*r2*r2_2/l2_2 + rr1_l1*r1 - \
                                      rr2_l2*r2 + 3*r*sumt3 + 6*r_2*cosPsi*ln +\
                                      r_2*r*cosPsi_2_1*sumt6)

            KVlamb = 0.5*cosPsiLamb*(rr1_l1*r1 - rr2_l2*r2 + 3*r*sumt3 + \
                                     6*r_2*cosPsi*ln + r_2*r*cosPsi_2_1*sumt6)

            return cosPhil*( (KVlambr - KVlamb/r)/(r*cosPhi) )

        except ZeroDivisionError:
            rad2deg = 180.0/pi
            msg = "Singularity occured due to computation point " + \
                  "(lon=%g, lat=%g) " % (lamb*rad2deg, phi*rad2deg) + \
                  "being aligned with a GLQ node in TesseroidGyz."
            raise SingularityError, msg

################################################################################


################################################################################
# TESSEROIDGZZ CLASS

class TesseroidGzz(TesseroidGravity):
    """
    This class is used to calculate the ZZ COMPONENT OF THE GRAVITY GRADIENT
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

    def __init__(self, ablon, wlon, ablat, wlat, abr=None, wr=None):
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
        TesseroidGravity.__init__(self, ablon, wlon, ablat, wlat, abr, wr)
        self.unit = self.si2eotvos
        # Create a logger
        self.logger = logging.getLogger('tesseroidgravity.gzz')


    def kernel3D(self, r, lamb, phi, rl, lambl, phil):
        """
        The kernel of the 3D integral defining the gravitational field.
        """
        rl_2 = rl**2
        cosPhil = cos(phil)
        cosPsi = sin(phi)*sin(phil) + cos(phi)*cosPhil*cos(lamb - lambl)
        l = sqrt( (r**2) + rl_2 - (2*r*rl*cosPsi) )
        l_3 = l**3
        rlcosPsi_r = (rl*cosPsi) - r
        return rl_2*cosPhil*( ((3*rlcosPsi_r*rlcosPsi_r)/l**2 - 1)/l_3 )


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
            cosPsi = sin(phi)*sin(phil) + cosPhi*cosPhil*cos(lamb - lambl)
            # Check if cosPsi is above the minimum for a 'safe' 2D GLQ
            # If it's bellow this, raise a SingularityError to force a 3D GLQ
            # This is necessary because the proximity of the GLQ node to the
            # calculation point causes some term to be too big and a strange
            # very high value to apear in the resulting field.
            if cosPsi >= self.max_cosPsi:
                rad2deg = 180.0/pi
                msg = "Singularity occured due to computation point " + \
                  "(lon=%g, lat=%g) " % (lamb*rad2deg, phi*rad2deg) + \
                  "being too close to a GLQ node in TesseroidGzz. This " + \
                  "could cause the 2D algorithm to fail."
                raise SingularityError, msg

            l1 = sqrt( r_2 + r1_2 - (2*r*r1*cosPsi) )
            l2 = sqrt( r_2 + r2_2 - (2*r*r2*cosPsi) )
            r1l1 = (r1_2*r1) / (2*l1)
            r2l2 = (r2_2*r2) / (2*l2)
            lntop = l2 + r2 - (r*cosPsi)
            lnbot = l1 + r1 - (r*cosPsi)
            ln = log(abs( lntop / lnbot ))
            return cosPhil*(r1l1 - r2l2 - r1*l1 + r2*l2 - \
                            r1l1*(r_2 - r1_2)/l1**2 + r2l2*(r_2 - r2_2)/l2**2 +\
                            3*r*cosPsi*(l2 - l1) + \
                            r_2*(3*cosPsi**2 - 1)*ln) / r_2

        except ZeroDivisionError:
            rad2deg = 180.0/pi
            msg = "Singularity occured due to computation point " + \
                  "(lon=%g, lat=%g) " % (lamb*rad2deg, phi*rad2deg) + \
                  "being aligned with a GLQ node in TesseroidGzz."
            raise SingularityError, msg

################################################################################


if __name__ == '__main__':
    print __doc__

