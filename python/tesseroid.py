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
    This module contains the Tesseroid class that contains the parameters
    defining a tesseroid:
        - Boundary (W, E, S, N, Top, Bottom);
        - Physical Property (Density);
        - tag (a tag identifying the tesseroid, ex: line it is in model file)
    Also contains the exceptions raised by Tesseroid's methods.
    
    
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

class Tesseroid(dict):
    """
    The Tesseroid class.
    Holds the parameters that define a tesseroid:
        - Boundary (W, E, S, N, Top, Bottom);
        - Physical Property (Density);
        - tag (a tag identifying the tesseroid, ex: line it is in model file)
    Pass the parameters to __init__.
    If the parameters need to be changed later on, use the 'set_bounds' and
    'set_density' methods!
    DO NOT set these parameters by hand! set_bounds performs some checks and
    adaptations on the parameters without which the GLQ might fail later on!
    The parameters can also be set/accessed just like in a dictionary.    
    """

    def __init__(self, w, e, s, n, top, bottom, density):
        # Initialize the parameters and let set_* methods set them
        # Tag is a value that can differentiate each tesseroid
        # Recomended is it's position in the model file
        self['w'] = 0
        self['e'] = 0
        self['s'] = 0
        self['n'] = 0
        self['top'] = 0
        self['bottom'] = 0
        self['density'] = 0
        self['tag'] = ''
        self.set_bounds(w, e, s, n, top, bottom)
        self.set_density(density)

    def set_bounds(self, w, e, s, n, top, bottom):
        """
        Sets the boundary parameters of the tesseroid.
        Performs checks to see if parameters are correct.
        Raises a InvalidBoundaryError if parameters are not in correct format.
        """
        # Check if parameters are float or int
        try:
            # This part catches if parameters are strings.
            # if it is an integer it will pass.
            if float(w) != w:
                raise InvalidBoundaryError, "ERROR! Boundary parameter W " + \
                "given to tesseroid %s is not valid! %s" % (str(self['tag']), \
                                                            str(w))
            if float(e) != e:
                raise InvalidBoundaryError, "ERROR! Boundary parameter E " + \
                "given to tesseroid %s is not valid! %s" % (str(self['tag']), \
                                                            str(e))
            if float(s) != s:
                raise InvalidBoundaryError, "ERROR! Boundary parameter S " + \
                "given to tesseroid %s is not valid! %s" % (str(self['tag']), \
                                                            str(s))
            if float(n) != n:
                raise InvalidBoundaryError, "ERROR! Boundary parameter N " + \
                "given to tesseroid %s is not valid! %s" % (str(self['tag']), \
                                                            str(n))
            if float(top) != top:
                raise InvalidBoundaryError, "ERROR! Boundary parameter Top " + \
                "given to tesseroid %s is not valid! %s" % (str(self['tag']), \
                                                            str(w))
            if float(bottom) != bottom:
                raise InvalidBoundaryError, "ERROR! Boundary parameter " + \
                "Bottom given to tesseroid %s is not valid! %s" \
                % (str(self['tag']), str(bottom))

        except TypeError:
            # If any parameter is neither float nor string nor int, a TypeError
            # is raised. If this happens, the parameter is not valid.
            raise InvalidBoundaryError, \
                      "ERROR! Boundary given to tesseroid %s is not valid!" \
                          % (self['tag'])

        
    def set_density(self, density):
        """
        Sets the density of the tesseroid. Also checks if the density given is
        a float or integer value. Raises InvalidDensityError is it is not.
        """
        # Check if density is a float
        try:
            # This part catches if density is a string.
            # if it is an integer it will pass.
            if float(density) != density:
                raise InvalidDensityError, \
                      "ERROR! Density given to tesseroid %s is not valid! %s" \
                          % (str(self['tag']), str(density))
                          
        except TypeError:
            # If density is neither float nor string nor int, a TypeError is
            # raised. If this happens, density was not valid.
            raise InvalidDensityError, \
                      "ERROR! Density given to tesseroid %s is not valid!" \
                          % (self['tag'])

        # If made it this far, then the density is valid
        self['density'] = density



    def __getitem__(self, key):
        """
        Returns the value of the parameter 'key'.
        Key can be 'w', 'e', 's', 'n', 'top', 'bottom' or 'density'.
        Raises InvalidKeyError if it is none of the above.
        """
        if key not in self.keys():
            raise InvalidKeyError, "ERROR! Tesseroid %s has no field '%s'!" \
                % (str(self['tag']), str(key)) + \
                " Valid fields are %s." % self.keys()

        return self.get(key)
    
################################################################################


################################################################################
# DOCTEST

if __name__ == '__main__':
    import doctest
    doctest.testmod()

################################################################################