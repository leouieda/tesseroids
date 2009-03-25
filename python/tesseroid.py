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
Tesseroid:
    Class that contains the parameters defining a tesseroid.

    
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
# Created: 25-Mar-2009 9:59:51 AM
# Revision: $Revision$
__date__ = 'Last edited: $Date$'
################################################################################


################################################################################
#  EXCEPTIONS

class TesseroidError(Exception):
    """
    Base class for the exceptions raised by the Tesseroid class.
    """
    pass

class InvalidBoundaryError(TesseroidError):
    """
    Raised when passing invalid boundary parameters to the Tesseroid class.
    """
    pass

class InvalidDensityError(TesseroidError):
    """
    Raised when passing invalid density parameter to the Tesseroid class.
    """
    pass

class InvalidKeyError(TesseroidError):
    """
    Raised when passing invalid key to __getitem__.
    """
    pass

################################################################################


################################################################################
# TESSEROID CLASS

class Tesseroid:
    """
    The Tesseroid class.
    Holds the parameters that define a tesseroid:
        - Boundary (W, E, S, N, Top, Bottom);
        - Physical Property (Density);
    Pass the parameters to __init__.
    If the parameters need to be changed later on, use the 'set_bounds' and
    'set_density' methods!
    DO NOT set the parameters by hand! set_bounds performs some checks and
    adaptations on the parameters without which the GLQ might fail later on!

    """

    def __init__(self, w, e, s, n, top, bottom, density):
        # Initialize the parameters and let set_* methods set them
        self.w = 0
        self.e = 0
        self.s = 0
        self.n = 0
        self.top = 0
        self.bottom = 0
        self.set_bounds(w, e, s, n, top, bottom)
        self.density = 0
        self.set_density(density)

    def set_bounds(self, w, e, s, n, top, bottom):
        """
        Sets the boundary parameters of the tesseroid.
        Performs checks to see if parameters are correct.
        Raises a InvalidBoundaryError if parameters are not in correct format.
        """
        pass

    def set_density(self, density):
        """
        Sets the density of the tesseroid. Also checks if the density given is
        a float or integer value. Raises InvalidDensityError is it is not.
        """
        pass

    def __getitem__(self, key):
        """
        Returns the value of the parameter 'key'.
        Key can be 'w', 'e', 's', 'n', 'top', 'bottom' or 'density'.
        Raises InvalidKeyError if it is none of the above.
        """
        pass
################################################################################


################################################################################
# DOCTEST

if __name__ == '__main__':
    import doctest
    doctest.testmod()

################################################################################