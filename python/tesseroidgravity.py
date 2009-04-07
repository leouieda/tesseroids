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
import time
################################################################################
__author__ = '$Author$'
# Created: 6-Apr-2009 4:07:57 PM
# Revision: $Revision$
__date__ = 'Last edited: $Date$'
################################################################################


from math import pi, cos, sin, sqrt


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
    R = 6378137
    # Conversion factor from SI units to Eotvos: 1 /s**2 = 10**9 Eotvos
    si2eotvos = 1000000000
    # Conversion factor from SI units to mGal: 1 m/s**2 = 10**5 mGal
    si2mgal = 100000

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


    def calculate(self, model, grid):
        """
        Method implemented in the derived classes to calculate the gravity field
        of 'model' in the points of 'grid'
            model - should be a 1D list of Tesseroids.
            grid  - 2D list with grid[:][0] having the lon values, grid[:][1]
                    the lat values, and grid[:][2] the height values.
                    Lon and lat should be in degrees and height in meters.
        Returns a list with the field values on the respective grid points.
        """
        pass


################################################################################


################################################################################
# TESSEROIDPOTENTIAL CLASS
class TesseroidPotential(TesseroidGravity):
    """
    Calculates and stores the gravity potential of a tesseroid model.

    """

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
        # Call the base class constructor to set the parameters
        TesseroidGravity.__init__(self, abr, ablon, ablat, wr, wlon, wlat)

        # Create a Logger for the TesseroidPotential class
        self.log = logging.getLogger('tesseroid.TesseroidPotential')


    def calculate(self, model, lons, lats, heights):
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

        # First initialize the array that will hold the field values
        field = []

        # Iterate over the tesseroids in the model and sum their fields on the
        # grid points.
        for tess in model:

            # Scale the abscissas to the dimensions of the tesseroid
            self.abr.scale(self.R-tess['bottom'], self.R-tess['top'])
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

                # Start a new line in field
                field.append([])

                # Convert the latitude to radians
                phi *= deg2rad
                
                for lamb, h in zip(*[lons, heights[i]]):
                    
                    # Calculate the radial distance to the point
                    r = self.R + h

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
                                l = sqrt( (r*r) + (rl_2) - (2*r*rl*cosPsi) )
                                K = (rl_2*cosPhil) / l

                                # Add the kernel at the current integration point
                                # to the result
                                res += wlon*wlat*wr*K

                    # Now that it's all added, multiply the scale factor
                    res *= factor

                    # Add this to the field list
                    field[i].append(res)

        return field


#    def check_abscissas(self):
#        """
#        Checks if the abscissas passed are actually instances of Abscissas.
#        If one of them isn't, raises a NotAbscissasInstanceError.
#        """
#        pass

################################################################################


if __name__ == '__main__':

    from timeit import Timer
    
    str = "import glq\n" + \
        "import pylab as p\n" + \
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
        "lon = s.arange(0,60.1,0.1)\n" + \
        "lat = s.arange(-30,30.1,0.1)\n" + \
        "h = 100000*p.ones((len(lat), len(lon)))\n" + \
        "tp = TesseroidPotential(abr,ablon,ablat,wr,wlon,wlat)\n"
    
    print "Timing calculate..."

    time = Timer(stmt='tp.calculate([tess], lon, lat, h)', setup=str)
    res = time.repeat(20)
    print "min: ", min(res), " max: ", max(res)
    print "times: ", res
    #pot = tp.calculate([tess], lon, lat, h)
    #lons, lats = p.meshgrid(lon,lat)
#
#    p.pcolor(lons, lats, pot, cmap=p.cm.jet)
#    p.colorbar(orientation='horizontal',aspect=50,shrink=0.5)
#    p.show()

    


