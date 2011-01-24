################################################################################
#                                                                              #
#                            MODULE TESSEROID                                  #
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
Tesseroid:
    This module contains the Tesseroid class that contains the parameters
    defining a tesseroid:
        - Boundary (W, E, S, N, Top, Bottom);
        - Physical Property (Density);
        - tag (a tag identifying the tesseroid, ex: line it is in model file)
    Also contains the exceptions raised by Tesseroid's methods. These are
    derived from TesseroidError.       
"""
################################################################################
# Created on 25-Mar-2009 9:59:51 AM
# Last modified by $Author: leouieda $
__author__ = 'Leonardo Uieda (leouieda@gmail.com)'
__version__ = '$Revision: 41 $'
__date__ = '$Date: 2009-07-16 01:53:36 +0200 (Thu, 16 Jul 2009) $'
__copyright__ = "Copyright (C) 2009  Leonardo Uieda"
__license__ = "GPL"
################################################################################


################################################################################
# LOGGING
import logging

# A Handler that does nothing so that if the app using tesseroid.py doesn't
# want to log, it won't get an error because it didn't provide a Handler
class NullHandler(logging.Handler):
    """
    Log Handler that does nothing.
    """
    def emit(self, record):
        """ Do nothing. """
        pass

NULLH = NullHandler()
logging.getLogger('tesseroid').addHandler(NULLH)
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
    Holds the parameters that define a tesseroid:
        - Boundary (w, e, s, n, top, bottom);
        - Physical Property (density);
        - tag (a tag identifying the tesseroid, ex: line it is in model file)
    Pass the parameters to __init__ and it will call the appropriate set_* 
    method. If the parameters need to be changed later on, use the 'set_bounds'
    and 'set_density' methods!
    DO NOT set these parameters by hand! set_bounds performs some checks and
    adaptations on the parameters without which the GLQ might fail later on!

    The parameters can be accessed just like in a dictionary.
    
    NOTICE:
    If W > E, then there will be a problem when calculating the abscissas for
    the glq because of the jump from 360 to 1 or from 179 to -179. The abscissas 
    algorithm won't understand this and will go 360 to 1 the other way around. 
    So, to correct for this, first W and E will be converted to [0,360]. Then,
    if W still > E, W will be converted [-180,180]. So don't be surprised if the
    value set to tesseroid is not the one you passed (though it is actually the
    same!). A INFO will be printed to a log (if you are logging) telling what
    changes have been made.
    
    Ex:
        >>> import logging
        >>> import sys
        >>>
        >>> # Setup a log
        >>> logging.basicConfig(level=logging.DEBUG, stream=sys.stdout)
        >>>
        >>> tess = Tesseroid(179,-179,-1,1,10,100,1.5,"MyTesseroid")
        DEBUG:tesseroid.Tesseroid:Tesseroid MyTesseroid had it's boundary \
parameter E changed from -179 to 181 for convenience.
        >>>
        >>> print tess['w'], tess['e']
        179 181
        
    """

    def __init__(self, w, e, s, n, top, bottom, density, tag=''):
        """
        Pass boundary and density parameters.
        Optionaly pass a tag value to identify the tesseroid. DEFAULT=''
        """
        dict.__init__(self)
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
        self['tag'] = tag
        # Create a Logger for the Tesseroid class
        self.log = logging.getLogger('tesseroid.Tesseroid')
        # Check if the parameters are Ok and set them if they are.
        self.set_bounds(w, e, s, n, top, bottom)
        self.set_density(density)
        

    def set_bounds(self, w, e, s, n, top, bottom):
        """
        Sets the boundary parameters of the tesseroid.
        Performs checks to see if parameters are correct.
        Raises a InvalidBoundaryError if parameters are not in correct format.
        """
        ########################################################################
        # CHECK THE TYPE OF PARAMETERS
        # Check if parameters are float or int
        try:
            # This part catches if parameters are strings.
            # if it is an integer it will pass.
            if float(w) != w:
                raise InvalidBoundaryError, "ERROR! Boundary parameter W " + \
                "given to tesseroid %s is not valid!\n Value given: %s" \
                % (str(self['tag']), str(w))
            if float(e) != e:
                raise InvalidBoundaryError, "ERROR! Boundary parameter E " + \
                "given to tesseroid %s is not valid!\n Value given: %s" \
                % (str(self['tag']), str(e))
            if float(s) != s:
                raise InvalidBoundaryError, "ERROR! Boundary parameter S " + \
                "given to tesseroid %s is not valid!\n Value given: %s" \
                % (str(self['tag']), str(s))
            if float(n) != n:
                raise InvalidBoundaryError, "ERROR! Boundary parameter N " + \
                "given to tesseroid %s is not valid!\n Value given: %s" \
                % (str(self['tag']), str(n))
            if float(top) != top:
                raise InvalidBoundaryError, "ERROR! Boundary parameter Top " + \
                "given to tesseroid %s is not valid!\n Value given: %s" \
                % (str(self['tag']), str(top))
            if float(bottom) != bottom:
                raise InvalidBoundaryError, "ERROR! Boundary parameter " + \
                "Bottom given to tesseroid %s is not valid!\n Value given: %s" \
                % (str(self['tag']), str(bottom))

        except TypeError:
            # If any parameter is neither float nor string nor int, a TypeError
            # is raised. If this happens, the parameter is not valid.
            raise InvalidBoundaryError, \
                      "ERROR! Boundary given to tesseroid %s is not valid!" \
                          % (self['tag'])
        ########################################################################

        ########################################################################
        # CHECK THE VALUES OF PARAMETERS
        # Longitude boundaries cannot be > 360 degrees
        if w > 360:
            raise InvalidBoundaryError, "ERROR! Boundary parameter W " + \
                "given to tesseroid %s cannot be > 360" % (str(self['tag'])) + \
                " degrees!\n Value given: %f" % (w)
        if e > 360:
            raise InvalidBoundaryError, "ERROR! Boundary parameter E " + \
                "given to tesseroid %s cannot be > 360" % (str(self['tag'])) + \
                " degrees!\n Value given: %f" % (e)
        # Longitude boundaries cannot be < -180 degrees
        if w < -180:
            raise InvalidBoundaryError, "ERROR! Boundary parameter W " + \
            "given to tesseroid %s cannot be < -180" % (str(self['tag'])) + \
            " degrees!\n Value given: %f" % (w)
        if e < -180:
            raise InvalidBoundaryError, "ERROR! Boundary parameter E " + \
            "given to tesseroid %s cannot be < -180" % (str(self['tag'])) + \
            " degrees!\n Value given: %f" % (e)
        # Longitude boundaries cannot be equal
        if w == e:
            raise InvalidBoundaryError, "ERROR! Boundary parameters W and " + \
            "E given to tesseroid %s cannot be equal!" % (str(self['tag'])) + \
            "\n Values given: w:%f  e:%f" % (w, e)
        # If W > E, then there will be a problem when calculating the abscissas
        # because of the jump from 360 to 1 or from 179 to -179. The abscissas
        # algorithm won't understand this and will go 360 to 1 the other way
        # around. So, to correct for this, convert W and E to an interval with
        # no jump.
        # To make it easier to analyse, first convert them both to [0,360]:
        if w < 0:
            msg = "Tesseroid %s had it's boundary parameter" % (self['tag']) + \
                  " W changed from %g to %g for convenience." % (w, 360 + w)
            w = 360 + w            
            self.log.debug(msg)
        if e <  0:
            msg = "Tesseroid %s had it's boundary parameter" % (self['tag']) + \
                  " E changed from %g to %g for convenience." % (e, 360 + e)
            e = 360 + e
            self.log.debug(msg)
        if w > e:
            msg = "Tesseroid %s had it's boundary parameter" % (self['tag']) + \
                  " W changed from %g to %g so that W < E." % (w, w - 360)
            w = w - 360
            self.log.debug(msg)
        # Now W and E should be ok with no problems

        # Latitude boundaries cannot be > 90 degrees
        if s > 90:
            raise InvalidBoundaryError, "ERROR! Boundary parameter S " + \
                "given to tesseroid %s cannot be > 90" % (str(self['tag'])) + \
                " degrees!\n Value given: %f" % (s)
        if n > 90:
            raise InvalidBoundaryError, "ERROR! Boundary parameter N " + \
                "given to tesseroid %s cannot be > 90" % (str(self['tag'])) + \
                " degrees!\n Value given: %f" % (n)
        # Latitude boundaries cannot be < -90 degrees
        if s < -90:
            raise InvalidBoundaryError, "ERROR! Boundary parameter S " + \
            "given to tesseroid %s cannot be < -90" % (str(self['tag'])) + \
            " degrees!\n Value given: %f" % (s)
        if n < -90:
            raise InvalidBoundaryError, "ERROR! Boundary parameter N " + \
            "given to tesseroid %s cannot be < -90" % (str(self['tag'])) + \
            " degrees!\n Value given: %f" % (n)
        # S cannot be greater than or equal to N
        if s >= n:
            raise InvalidBoundaryError, "ERROR! Boundary parameters S " + \
            "given to tesseroid %s cannot be >= N!" % (str(self['tag'])) + \
            "\n Values given: s:%f  n:%f" % (s, n)

        # Since bottom and top are the depth to the bottom and top of the
        # tesseroid, top cannot be >= bottom.
        if top >= bottom:
            raise InvalidBoundaryError, "ERROR! Boundary parameters Top " + \
            "given to tesseroid %s cannot be >= Bottom!" % (str(self['tag'])) +\
            "\n Values given: top:%f  bottom:%f" % (top, bottom)
        ########################################################################

        # If we got here, then all went well and the parameters are OK
        self['w'] = w
        self['e'] = e
        self['s'] = s
        self['n'] = n
        self['top'] = top
        self['bottom'] = bottom

        
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
    print "\nDOCTEST FOR TESSEROID.PY:"
    doctest.testmod()
    print "Done\n"

################################################################################