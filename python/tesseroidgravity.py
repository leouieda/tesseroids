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
    models. Fields are: potential, geoid, 3-component acceleration, 9-component
    gradient tensor.
    The constants used in the calculations are defined as class variables in the
    TesseroidGravity class that is mother to all others.
    Also included are functions to time the execution of these calculations.
    

    Copyright (C) 2009  Leonardo Uieda

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    Doubts and/or suggestions please mail the author:
         Leonardo Uieda - leouieda@gmail.com

"""

################################################################################
__author__ = '$Author$'
# Created: 6-Apr-2009 4:07:57 PM
# Revision: $Revision$
__date__ = 'Last edited: $Date$'
################################################################################


from math import pi, cos, sin, sqrt
from timeit import Timer


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


################################################################################
#  EXCEPTIONS

class TesseroidGravityError(Exception):
    """
    Base class for the exceptions raised by the classes in this module class.
    """
    pass

#class NotAbscissasInstanceError(TesseroidGravity):
#    """
#    Raised when trying to pass __init__ something that is not an Abscissas
#    instance.
#    """
#    pass

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


    def calculate(self, model, lons, lats, heights):
        """
        Calculate the gravity potential field of a Tesseroid model on a given
        grid of points.
        Parameters:
            model   - list of Tesseroid objects;
            lons    - list of longitudes in which to calculate the field;
            lats    - list of latitudes in which to calculate the field;
            heights - 2D list (grid) with the calculation heights. The columns
                      correspond to the longitudes in 'lons' and the lines to
                      the latitudes in 'lats';

        Returns a 2D list (grid) with the field values on the respective grid
        points.

        Example:
            lons = [0, 90, 180, 270]
            lats = [-80, -40, 0, 40, 80]
            heights = [[10, 10, 10, 10],
                       [20, 20, 20, 20],
                       [40, 40, 40, 40],
                       [30, 30, 30, 30],
                       [20, 20, 20, 20]]
            The field will be calculated at latitude -80 and longitudes [0, 90,
            180, 270] with a height of 10, at lattitude 0 and longitudes [0, 90,
            180, 270] with a height of 40, etc.
            The field returned will be: field[0][0] will correspond to the field
            at lon 0 and lat -80, field[1][2] will correspond to the
            field at lon 180 and lat -40, and so forth.
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


    def calculate(self, model, lons, lats, heights):
        """
        Calculate this particular gravity field of a Tesseroid model on a given
        grid of points.
        Parameters:
            model   - list of Tesseroid objects;
            lons    - list of longitudes in which to calculate the field;
            lats    - list of latitudes in which to calculate the field;
            heights - 2D list (grid) with the calculation heights. The columns
                      correspond to the longitudes in 'lons' and the lines to
                      the latitudes in 'lats';

        Returns a 2D list (grid) with the field values on the respective grid
        points.

        Example:
            lons = [0, 90, 180, 270]
            lats = [-80, -40, 0, 40, 80]
            heights = [[10, 10, 10, 10],
                       [20, 20, 20, 20],
                       [40, 40, 40, 40],
                       [30, 30, 30, 30],
                       [20, 20, 20, 20]]
            The field will be calculated at latitude -80 and longitudes [0, 90,
            180, 270] with a height of 10, at lattitude 0 and longitudes [0, 90,
            180, 270] with a height of 40, etc.
            The field returned will be: field[0][0] will correspond to the field
            at lon 0 and lat -80, field[1][2] will correspond to the
            field at lon 180 and lat -40, and so forth.
        """

        # Conversion factor from degrees to radians.
        deg2rad = pi/180.0

        # First initialize the array that will hold the field values
        field = [[0.0]*len(lons) for i in range(0, len(lats))]
        
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
            factor = ((deg2rad**2)*(tess['e']-tess['w'])*(tess['n']-tess['s'])*\
                     (tess['bottom']-tess['top'])*tess['density']*self.G) / 8.0

            # Caculate the field at every point on the grid and sum to what was
            # there before.
            for i, phi in zip(*[range(0, len(lats)), lats]):
                
                # Convert the latitude to radians
                phi *= deg2rad
                
                for j, lamb in zip(*[range(0, len(lons)), lons]):
                    
                    # Calculate the radial distance to the point
                    r = self.R + heights[i][j]

                    # Convert the longitude to radians
                    lamb *= deg2rad
                    
                    # Initialize the result with zero
                    res = 0.0

                    # Do the GLQ summation to calculate the field
                    for lambl, wlon in zip(*[self.ablon.val, self.wlon.val]):
                        for phil, wlat in zip(*[self.ablat.val, self.wlat.val]):
                            for rl, wr in zip(*[self.abr.val, self.wr.val]):

                                # Calculate the integral kernel K
                                rl_2 = rl**2
                                cosPhil = cos(phil)
                                cosPsi = sin(phi)*sin(phil) + cos(phi)*cosPhil*\
                                                                cos(lamb-lambl)
                                l = sqrt( (r**2) + (rl_2) - (2*r*rl*cosPsi) )
                                K = (rl_2*cosPhil) / l

                                # Add the kernel at the current integration
                                # point to the result
                                res += wlon*wlat*wr*K

                    # Add this to the field of the other tesseroids
                    # Factor is there to account for the change in variables
                    # done when scaling the abscissas to the correct interval.
                    # It also contains the density of the tesseroid, the
                    # gravitational constant and a unit conversion factor (only
                    # if needed).
                    field[i][j] += factor*res

        return field

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


    def calculate(self, model, lons, lats, heights):
        """
        Calculate this particular gravity field of a Tesseroid model on a given
        grid of points.
        Parameters:
            model   - list of Tesseroid objects;
            lons    - list of longitudes in which to calculate the field;
            lats    - list of latitudes in which to calculate the field;
            heights - 2D list (grid) with the calculation heights. The columns
                      correspond to the longitudes in 'lons' and the lines to
                      the latitudes in 'lats';

        Returns a 2D list (grid) with the field values on the respective grid
        points.

        Example:
            lons = [0, 90, 180, 270]
            lats = [-80, -40, 0, 40, 80]
            heights = [[10, 10, 10, 10],
                       [20, 20, 20, 20],
                       [40, 40, 40, 40],
                       [30, 30, 30, 30],
                       [20, 20, 20, 20]]
            The field will be calculated at latitude -80 and longitudes [0, 90,
            180, 270] with a height of 10, at lattitude 0 and longitudes [0, 90,
            180, 270] with a height of 40, etc.
            The field returned will be: field[0][0] will correspond to the field
            at lon 0 and lat -80, field[1][2] will correspond to the
            field at lon 180 and lat -40, and so forth.
        """

        # Conversion factor from degrees to radians.
        deg2rad = pi/180.0

        # First initialize the array that will hold the field values
        field = [[0.0]*len(lons) for i in range(0, len(lats))]

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
            factor = ((deg2rad**2)*(tess['e']-tess['w'])*(tess['n']-tess['s'])*\
                     (tess['bottom']-tess['top'])*\
                     tess['density']*self.G*self.si2mgal) / 8.0

            # Caculate the field at every point on the grid and sum to what was
            # there before.
            for i, phi in zip(*[range(0, len(lats)), lats]):

                # Convert the latitude to radians
                phi *= deg2rad

                for j, lamb in zip(*[range(0, len(lons)), lons]):

                    # Calculate the radial distance to the point
                    r = self.R + heights[i][j]

                    # Convert the longitude to radians
                    lamb *= deg2rad

                    # Initialize the result with zero
                    res = 0.0

                    # Do the GLQ summation to calculate the field
                    for lambl, wlon in zip(*[self.ablon.val, self.wlon.val]):
                        for phil, wlat in zip(*[self.ablat.val, self.wlat.val]):
                            for rl, wr in zip(*[self.abr.val, self.wr.val]):

                                # Calculate the integral kernel K
                                rl_2 = rl**2
                                cosPhil = cos(phil)
                                cosPhi = cos(phi)
                                sinPhi = sin(phi)
                                sinPhil = sin(phil)
                                cosLambLambl = cos(lamb - lambl)
                                cosPsi = sinPhi*sinPhil + cosPhi*cosPhil*\
                                                                    cosLambLambl
                                l = sqrt( (r**2) + (rl_2) - (2*r*rl*cosPsi) )
                                K = ((sinPhil*cosPhi - cosPhil*sinPhi*\
                                         cosLambLambl)*rl_2*rl*cosPhil) / (l**3)

                                # Add the weighted kernel at the current
                                # integration point to the result
                                res += wlon*wlat*wr*K

                    # Add this to the field of the other tesseroids
                    # Factor is there to account for the change in variables
                    # done when scaling the abscissas to the correct interval.
                    # It also contains the density of the tesseroid, the
                    # gravitational constant and a unit conversion factor (only
                    # if needed).
                    field[i][j] += factor*res

        return field

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


    def calculate(self, model, lons, lats, heights):
        """
        Calculate this particular gravity field of a Tesseroid model on a given
        grid of points.
        Parameters:
            model   - list of Tesseroid objects;
            lons    - list of longitudes in which to calculate the field;
            lats    - list of latitudes in which to calculate the field;
            heights - 2D list (grid) with the calculation heights. The columns
                      correspond to the longitudes in 'lons' and the lines to
                      the latitudes in 'lats';

        Returns a 2D list (grid) with the field values on the respective grid
        points.

        Example:
            lons = [0, 90, 180, 270]
            lats = [-80, -40, 0, 40, 80]
            heights = [[10, 10, 10, 10],
                       [20, 20, 20, 20],
                       [40, 40, 40, 40],
                       [30, 30, 30, 30],
                       [20, 20, 20, 20]]
            The field will be calculated at latitude -80 and longitudes [0, 90,
            180, 270] with a height of 10, at lattitude 0 and longitudes [0, 90,
            180, 270] with a height of 40, etc.
            The field returned will be: field[0][0] will correspond to the field
            at lon 0 and lat -80, field[1][2] will correspond to the
            field at lon 180 and lat -40, and so forth.
        """

        # Conversion factor from degrees to radians.
        deg2rad = pi/180.0

        # First initialize the array that will hold the field values
        field = [[0.0]*len(lons) for i in range(0, len(lats))]

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
            factor = ((deg2rad**2)*(tess['e']-tess['w'])*(tess['n']-tess['s'])*\
                     (tess['bottom']-tess['top'])*\
                     tess['density']*self.G*self.si2mgal) / 8.0

            # Caculate the field at every point on the grid and sum to what was
            # there before.
            for i, phi in zip(*[range(0, len(lats)), lats]):

                # Convert the latitude to radians
                phi *= deg2rad

                for j, lamb in zip(*[range(0, len(lons)), lons]):

                    # Calculate the radial distance to the point
                    r = self.R + heights[i][j]

                    # Convert the longitude to radians
                    lamb *= deg2rad

                    # Initialize the result with zero
                    res = 0.0

                    # Do the GLQ summation to calculate the field
                    for lambl, wlon in zip(*[self.ablon.val, self.wlon.val]):
                        for phil, wlat in zip(*[self.ablat.val, self.wlat.val]):
                            for rl, wr in zip(*[self.abr.val, self.wr.val]):

                                # Calculate the integral kernel K
                                rl_2 = rl**2
                                cosPhil = cos(phil)
                                cosPsi = sin(phi)*sin(phil) + cos(phi)*cosPhil*\
                                                            cos(lamb - lambl)
                                l = sqrt( (r**2) + (rl_2) - (2*r*rl*cosPsi) )
                                K = (rl_2*cosPhil*(-1)*rl*cosPhil*\
                                                     sin(lamb - lambl)) / (l**3)

                                # Add the weighted kernel at the current
                                # integration point to the result
                                res += wlon*wlat*wr*K

                    # Add this to the field of the other tesseroids
                    # Factor is there to account for the change in variables
                    # done when scaling the abscissas to the correct interval.
                    # It also contains the density of the tesseroid, the
                    # gravitational constant and a unit conversion factor (only
                    # if needed).
                    field[i][j] += factor*res

        return field

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


    def calculate(self, model, lons, lats, heights):
        """
        Calculate this particular gravity field of a Tesseroid model on a given
        grid of points.
        Parameters:
            model   - list of Tesseroid objects;
            lons    - list of longitudes in which to calculate the field;
            lats    - list of latitudes in which to calculate the field;
            heights - 2D list (grid) with the calculation heights. The columns
                      correspond to the longitudes in 'lons' and the lines to
                      the latitudes in 'lats';

        Returns a 2D list (grid) with the field values on the respective grid
        points.

        Example:
            lons = [0, 90, 180, 270]
            lats = [-80, -40, 0, 40, 80]
            heights = [[10, 10, 10, 10],
                       [20, 20, 20, 20],
                       [40, 40, 40, 40],
                       [30, 30, 30, 30],
                       [20, 20, 20, 20]]
            The field will be calculated at latitude -80 and longitudes [0, 90,
            180, 270] with a height of 10, at lattitude 0 and longitudes [0, 90,
            180, 270] with a height of 40, etc.
            The field returned will be: field[0][0] will correspond to the field
            at lon 0 and lat -80, field[1][2] will correspond to the
            field at lon 180 and lat -40, and so forth.
        """

        # Conversion factor from degrees to radians.
        deg2rad = pi/180.0

        # First initialize the array that will hold the field values
        field = [[0.0]*len(lons) for i in range(0, len(lats))]

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
            factor = ((deg2rad**2)*(tess['e']-tess['w'])*(tess['n']-tess['s'])*\
                     (tess['bottom']-tess['top'])*\
                     tess['density']*self.G*self.si2mgal) / 8.0

            # Caculate the field at every point on the grid and sum to what was
            # there before.
            for i, phi in zip(*[range(0, len(lats)), lats]):

                # Convert the latitude to radians
                phi *= deg2rad

                for j, lamb in zip(*[range(0, len(lons)), lons]):

                    # Calculate the radial distance to the point
                    r = self.R + heights[i][j]

                    # Convert the longitude to radians
                    lamb *= deg2rad

                    # Initialize the result with zero
                    res = 0.0

                    # Do the GLQ summation to calculate the field
                    for lambl, wlon in zip(*[self.ablon.val, self.wlon.val]):
                        for phil, wlat in zip(*[self.ablat.val, self.wlat.val]):
                            for rl, wr in zip(*[self.abr.val, self.wr.val]):

                                # Calculate the integral kernel K
                                rl_2 = rl**2
                                cosPhil = cos(phil)
                                cosPsi = sin(phi)*sin(phil) + cos(phi)*cosPhil*\
                                                               cos(lamb - lambl)
                                l = sqrt( (r**2) + (rl_2) - (2*r*rl*cosPsi) )
                                K = rl_2*cosPhil*((r - (rl*cosPsi)) / (l**3))

                                # Add the weighted kernel at the current
                                # integration point to the result
                                res += wlon*wlat*wr*K

                    # Add this to the field of the other tesseroids
                    # Factor is there to account for the change in variables
                    # done when scaling the abscissas to the correct interval.
                    # It also contains the density of the tesseroid, the
                    # gravitational constant and a unit conversion factor (only
                    # if needed).
                    field[i][j] += factor*res

        return field

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
    str = "import glq\n" + \
        "import scipy as s\n" + \
        "import tesseroid as t\n" + \
        "from __main__ import TesseroidPotential\n" + \
        "tess = t.Tesseroid(27.5,32.5,-2.5,2.5,0,40000,1.0,'TEST')\n" + \
        "ablon = glq.Abscissas(5)\n" + \
        "ablat = glq.Abscissas(5)\n" + \
        "abr = glq.Abscissas(5)\n" + \
        "wlon = glq.Weights(ablon)\n" + \
        "wlat = glq.Weights(ablat)\n" + \
        "wr = glq.Weights(abr)\n" + \
        "lon = s.arange(5,55.5,0.5)\n" + \
        "lat = s.arange(-25,25.5,0.5)\n" + \
        "h = [[100000.0]*len(lon) for i in range(0, len(lat))]\n" + \
        "tp = TesseroidPotential(abr,ablon,ablat,wr,wlon,wlat)\n" + \
        "mod = [tess]"

    print "Timing %d iterations of calculate %d times..." % (repetitions, \
                                                                num_times)
    times = []
    total = 0.0
    timer = Timer(stmt='tp.calculate(mod, lon, lat, h)', setup=str)
    print "Iterations: Time (s)"
    for i in range(1, num_times+1):
        t = timer.timeit(repetitions)
        print "%d: %g" % (i, t)
        times.append(t)
        total += t

    print "Max (s): %g" % (max(times))
    print "Min (s): %g" % (min(times))
    print "Total (s): %g" % (total)

################################################################################


################################################################################
# CALCULATE THE FIELDS FOR A TEST

def calc_all():
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
    heights = [[250000.0]*len(lons) for i in range(0, len(lats))]

    # Make the field calculator obejects
    print "Creating calculator objects..."
    tesspot = TesseroidPotential(abr, ablon, ablat, wr, wlon, wlat)
    tessgx = TesseroidGx(abr, ablon, ablat, wr, wlon, wlat)
    tessgy = TesseroidGy(abr, ablon, ablat, wr, wlon, wlat)
    tessgz = TesseroidGz(abr, ablon, ablat, wr, wlon, wlat)

    # Calculate the fields
    print "Calculating fields:"
    print "Potential..."
    pot = tesspot.calculate(model, lons, lats, heights)
    print "Gx..."
    gx = tessgx.calculate(model, lons, lats, heights)
    print "Gy..."
    gy = tessgy.calculate(model, lons, lats, heights)
    print "Gz..."
    gz = tessgz.calculate(model, lons, lats, heights)

    # Plot the maps
    print "Plotting maps:"
    fields = [pot, gx, gy, gz]
    names = ['Potential', 'Gx', 'Gy', 'Gz']
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
        
