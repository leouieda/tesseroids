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
TesseroidGravity:


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


import glq
from math import pi, cos, sin


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
# TESSEROIDPOTENTIAL CLASS
class TesseroidGravity:
    """
    Mother class to the Tesseroid* classes (* are the diff. fields).
    This class has the constants shared by it's children.
    If these need be changed, change them here.
    """
    # The gravitational constant (m^3*kg^-1*s^-1)
    G = 0.00000000006673
    # The mean earth radius (m)
    R = 6378137
    # Conversion factor from SI units to Eotvos: 1 /s**2 = 10**9 Eotvos
    si2eotvos = 1000000000
    # Conversion factor from SI units to mGal: 1 m/s**2 = 10**5 mGal
    si2mgal = 100000
  

################################################################################


################################################################################
# TESSEROIDPOTENTIAL CLASS
class TesseroidPotential:
    """
    Calculates and stores the gravity potential of a tesseroid model.

    """

    def __init__(self, abr, ablon, ablat, wr, wlon, wlat):
        """
        Pass the Abscissas and associated Weights that will be used in the GLQ.
        """        
        # Initialize the parameters
        self.abr = abr
        self.ablon = ablon
        self.ablat = ablat
        self.wr = wr
        self.wlon = wlon
        self.wlat = wlat

        # Create a Logger for the TesseroidPotential class
        self.log = logging.getLogger('tesseroid.TesseroidPotential')


    def calculate(self, model, grid):
        """
        Calculate the gravity potential field of 'model' in the points of 'grid'
            model - should be a 1D list of Tesseroids.
            grid  - 2D list with grid[:][0] having the lon values, grid[:][1]
                    the lat values, and grid[:][2] the height values.
                    Lon and lat should be in degrees and height in meters.
        Returns a list with the field values on the respective grid points.
        """

        # Conversion factor from degrees to radians.
        deg2rad = pi/180.0

        # Iterate over the tesseroids in the model and sum their fields on the
        # grid points.
        for tess in model:

            self.abr.scale()
        
        return ()


#    def check_abscissas(self):
#        """
#        Checks if the abscissas passed are actually instances of Abscissas.
#        If one of them isn't, raises a NotAbscissasInstanceError.
#        """
#        pass

################################################################################