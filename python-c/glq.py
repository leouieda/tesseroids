################################################################################
#                                                                              #
#                  MODULE GLQ (GAUSS-LEGENDRE QUADRATURE)                      #
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
    This module contains classes for calculating the abscissas and weights used
    in the Gauss-Legenre Quadrature.

    Example: To integrate x**2 from -20 to 15 using a 5 point GLQ

    >>> a = Abscissas(5)
    >>> w = Weights(a)
    >>>
    >>> a.scale(-20, 15)
    >>>
    >>> result = 0
    >>> for i in range(0, 5):
    ...     result += w[i]*(a[i]**2)
    ...
    >>> result = 0.5*(15-(-20))*result
    >>> print result
    3791.66666667

    
    Abscissas implements Newton's Method for Multiple Roots presented in:
        Barrera-Figueroa, V., Sosa-Pedroza, J. and Lopez-Bonilla, J.,
        "Multiple root finder algorithm for Legendre and Chebyshev polynomials
         via Newton's method", 2006,
        Annales mathematicae et Informaticae, 33, pp 3-13.         
"""
################################################################################
# Created on 21-Mar-2009 1:47:06 AM
# Last modified by $Author: leouieda $
__author__ = 'Leonardo Uieda (leouieda@gmail.com)'
__version__ = '$Revision: 52 $'
__date__ = '$Date: 2009-09-23 16:27:07 +0200 (Wed, 23 Sep 2009) $'
__copyright__ = "Copyright (C) 2009  Leonardo Uieda"
__license__ = "GPL" 
################################################################################


import math as m

# Define the defalut GLQ order
defaultOrder = 2

################################################################################
# LOGGING
import logging

# A Handler that does nothing so that if the app using glq.py doesn't
# want to log, it won't get an error because it didn't provide a Handler
class NullHandler(logging.Handler):
    """
    Log Handler that does nothing.
    """
    def emit(self, record):
        """ Do nothing. """
        pass

NULLH = NullHandler()
logging.getLogger('glq').addHandler(NULLH)
################################################################################


################################################################################
#  EXCEPTIONS

class GLQError(Exception):
    """
    Base exception for errors in the execution of the Abscissas and Weights
    classes.
    """
    pass

class OrderNotIntegerError(GLQError):
    """
    Thrown when an order that is not integer is passed to __init__.
    """
    pass

class OrderOutOfRangeError(GLQError):
    """
    Thrown when an order that is not >= 2 is passed to __init__.
    """
    pass

class MaxIterationsError(GLQError):
    """
    Thrown when the maximum number of iterations is reached when searching for
    a Legendre polynomial root. This will indicate that the mininum error was
    likelly not achieved.
    """
    pass

class IndexOutOfRangeError(GLQError):
    """
    Thrown when trying to access abscissas with __getitem__ passing an out of
    range key. order > key >= 0.
    """
    pass

class IndexNotIntegerError(GLQError):
    """
    Thrown when trying to access abscissa with __getitem__ passing a non-integer
    key.
    """
    pass

class NotAbscissasInstanceError(GLQError):
    """
    Thrown when passing an object that is not an instance of Abscissas to the
    Weights '__init__' or 'calculate' methods.
    """
    pass

################################################################################


################################################################################
# ABSCISSAS CLASS

class Abscissas:
    """
    Calculates the abscissas for the GLQ.
    __init__ recieves an integer parameter 'order'. This is the order of the
    GLQ and must be >= 2. Raises OrderNotIntegerError and OrderOutOfRangeError
    if invalid orders are passed.
    The abscissas can also be re-calculated by calling 'calculate(order)'.

    These values are scaled in the interval [-1,1]. To scale them to an interval
    [lower,upper] call 'scale(lower,upper)'.
    A non-scaled version is still kept so further calls to 'scale' can be made
    on the original abscissas.

    Abscissas uses a modified Newton's method (Barrera-Figueroa et al., 2006) to
    search for the abscissas.
    In case this algorithm ends because it reached it's maximum iterations, a
    MaxIterationsError will be raised. If this happens, the maximum error
    (difference between iteration n and n-1) can be increased by modifying the
    class variable 'max_error'. The default error is 10e-15.

    The abcissas can be accessed using [] as if it was a list. In case of an
    invalid index passed, the exceptions IndexOutOfRangeError and/or
    IndexNotIntegerError will be raised.
    """

    # Class variables that define the maximum number of iterations when
    # searching for a Legendre polynomial root and the maximum error accepted.
    # These values should only be changed if max_iterations is being reached!
    max_iterations = 10000
    max_error = 0.000000000000001

    def __init__(self, order=None):
        """
        Pass the order of the GLQ (number of nodes).
        Must me integer > 1. Defaults to 2.
        """
        # Do some trickery so that the default order can be set later
        # To do this, set glq.defaultOrder before any intantiation of Weights or
        # Abscissas is done
        if order == None:
            order = defaultOrder
        # Create a Logger for the Abscissas class
        self.log = logging.getLogger('glq.Abscissas')
        # Initiate the parameters and calculate the abscissas they are OK
        self.order = 0
        self._val_unscaled = []
        self._val = []
        # Flag to tell if max_it was reached
        self.maxit = False
        # Calculate the abscissas
        self.calculate(order)

        

    def __findroot(self, initial_guess, current_root):
        """
        Finds the next root from current_root of Legendre polynomial of order =
        order using Newton's Method for Multiple Roots (Barrera-Figueroa et al.,
        2006).

        """
              
        # The initial guess for the root
        x1 = initial_guess
        # To count the number of iterations needed
        iterations = 0
        # Mark if the max_error was reached
        error_reached = False
                     
        while iterations < self.max_iterations:
        
            x0 = x1

            # Calculate Pn(x0)
            ####################################################################
            # Starting from P0(x) and P1(x),
            # find the others using the recursive relation:
            #     Pn(x) = (2n-1)xPn_1(x)/n - (n-1)Pn_2(x)/n            
            pn_1 = 1.0  # This is Po(x)
            pn = x0   # and this P1(x)

            for n in range(2, self.order+1):
                pn_2 = pn_1
                pn_1 = pn
                pn = ( ( (2 * n-1)*x0*pn_1 ) - ( (n-1)*pn_2 ) ) / n            
            ####################################################################

            # Now calculate Pn'(x0) using another recursive relation:
            #     Pn'(x) = n(xPn(x)-Pn_1(x))/(x*x-1)
            ####################################################################
            pn_line = self.order*( (x0*pn) - pn_1 ) / ( x0**2 - 1 )
            ####################################################################

            # Sum the roots found so far
            rootsum = 0.0
            for n in range(0, current_root):
                rootsum += 1.0 / (x0 - self._val_unscaled[n])
            
            # Update the guess for the root
            x1 = x0 - ( pn / (pn_line - (pn*rootsum)) )

            # Update the number of iterations necessary
            iterations += 1

            if abs(x1 - x0) <= self.max_error:
                error_reached = True
                break

        # If max_iterations was reached, create a warning msg in the log.
        if iterations >= self.max_iterations and not error_reached:
            msg = "Maximum iterations (%d) " % (self.max_iterations) +\
                  "reached when looking for root %d of " % (current_root) + \
                  "Legendre polynomial P%d." % (self.order)
            self.log.warning(msg)
            self.maxit = True

        # Return the final value of the root.
        return x1
    

    def calculate(self, order):
        """
        Calculates the abscissas for the determined order.
        """
        
        # Check if the order given is valid
        ########################################################################
        # Check if it is an integer.
        try:
            # This part catches if order is a float or a string
            if int(order) != order:
                raise OrderNotIntegerError, \
                      "ERROR! Order of GLQ (%s) provided is not an integer!" \
                          % (str(order))
        except TypeError:
            # If order is neither float nor string nor int, a TypeError is
            # raised. If this happens, order was not valid.
            raise OrderNotIntegerError, \
                     "ERROR! Order of GLQ provided is not an integer!"

        # Order must be at least 2.
        if order < 2:
            raise OrderOutOfRangeError, \
                  "ERROR! Order of GLQ (%d) cannot be < 2!" % (order)
        ########################################################################

        # Flag to tell if max_it was reached when looking for the root
        self.maxit = False

        # Set the order of the GLQ. This will be number of abscissas.
        self.order = order

        # Initialize the (unscaled) values with 0
        self._val_unscaled = [0.0]*order
        self._val = [0.0]*order
        
        # Iterate to find all roots of the Legendre polynomial
        # Find the root and put it in the unscaled version of the roots
        for i in range(0, order):
            self._val_unscaled[i] = self.__findroot(m.cos(m.pi*((i+1)-0.25)/ \
                                                   (order+0.5)), i)
            # Then copy the to the ones that will be scaled in the future
            self._val[i] = self._val_unscaled[i]
        

    def scale(self, lower, upper):
        """
        Scale the abscissas to the interval [lower,upper].
        """

        # This is done to optimize the code a bit
        tmpplus = (upper + lower)/2.0
        tmpminus = (upper - lower)/2.0
        
        for i in range(0, self.order):
            self._val[i] = tmpminus*self._val_unscaled[i] + tmpplus


    def __getitem__(self, key):
        """
        Get the abscissa value corresponding to key. 
        key should be integer and order > key >= 0.
        Raises IndexOutOfRangeError and IndexNotIntegerError.
        """
        
        # Check if the key given is valid
        ########################################################################
        # Check if it is an integer.
        try:
            # This part catches if key is a float or a string
            if int(key) != key:
                raise IndexNotIntegerError, \
                      "ERROR! Abscissa index given (%s) is not an integer!" \
                          % (str(key))
        except TypeError:
            # If key is neither float nor string nor int, a TypeError is
            # raised. If this happens, key was not valid.
            raise IndexNotIntegerError, \
                      "ERROR! Abscissa index given is not an integer!"

        # Key must be >= 0 and < order.
        if key >= self.order or key < 0:
            raise IndexOutOfRangeError, \
                  "ERROR! Abscissa index given '%d' is not valid! " % (key) + \
                  "Must be %d >= index >= 0!" % (self.order-1)
        ########################################################################

        return self._val[key]


    def __len__(self):
        """
        Return the order of the GLQ (number of abscissas) if len function is
        called.
        """
        return self.order

################################################################################


################################################################################
# WEIGHTS CLASS

class Weights():
    """
    Calculates the weights for the GLQ.
    __init__ recieves an instance of Abscissas and calculates the weights that
    correspond to the different abscissas. To re-calculate the weights for a
    different set of abscissas, call 'calculate(abscissas)'. If an object that
    is not an instance of abscissas is passed, a NotAbscissasInstanceError will
    be raised.
    
    The weights can be accessed using [] as if it was a list. In case of an
    invalid index passed, the exceptions IndexOutOfRangeError and/or
    IndexNotIntegerError will be raised.

    len() will return the number of weights (which is the same as the number of
    abscissas and the order of the GLQ).
    """

    def __init__(self, abscissas=None):
        """
        abscissas is an instance of the Abscissas class for which the weights
        will be calculated. Default is an Abscissas with order 2.
        """
        # Do some trickery so that the default order can be set later.
        # To do this, set glq.defaultOrder before any intantiation of Weights or
        # Abscissas is done
        if abscissas == None:
            abscissas = Abscissas()
        self._val = []
        self.order = 0
        self.calculate(abscissas)


    def calculate(self, abscissas):
        """
        Calculates the weights based on the abscissas passed. 'abscissas' must
        be an instance of the Abscissas class! 
        If not, a NotAbscissasInstanceError exception will be raised.
        """

        # Check is 'abscissas' really is an instance of Abscissas
        if not isinstance(abscissas, Abscissas):
            raise NotAbscissasInstanceError, \
                "ERROR! Passed object that is not an instance of " + \
                "Abscissas to Weights.calculate(abscissas)."


        # Set the order of the GLQ
        self.order = len(abscissas)

        # Initialize the values with 0
        self._val = [0.0]
        self._val *= self.order

        # Calculate the weights
        for i in range(0, self.order):

            # Need to find Pn'(xi)
            # To do this, use the recursive relation to find Pn and Pn-1:
            #     Pn(x) = (2n-1)xPn_1(x)/n - (n-1)Pn_2(x)/n
            # Then use:
            #     Pn'(x) = n(xPn(x)-Pn_1(x))/(x*x-1)
            xi = abscissas._val_unscaled[i]

            # Find Pn and Pn-1 stating from P0 and P1
            pn_1 = 1.0   # This is Po(x)
            pn = xi    # and this P1(x)
            for n in range(2, self.order+1):
                pn_2 = pn_1
                pn_1 = pn
                pn = ( ( ((2*n)-1)*xi*pn_1 ) - ( (n-1)*pn_2 ) ) / n

            # Now find Pn'(xi)
            pn_line = self.order*(xi*pn-pn_1) / ((xi**2)-1)

            # Calculate the weight Wi
            self._val[i] = 2.0 / ( (1-(xi**2))*(pn_line**2) )
        

    def __getitem__(self, key):
        """
        Get the weight value corresponding to key.
        key should be integer and order > key >= 0.
        Raises IndexOutOfRangeError and IndexNotIntegerError.
        """

        # Check if the key given is valid
        ########################################################################
        # Check if it is an integer.
        try:
            if int(key) != key:
                # This part catches if key is a float or a string
                raise IndexNotIntegerError, \
                      "ERROR! Weight index given (%s) is not an integer!" \
                          % (str(key))

        except TypeError:
            # If key is neither float nor string nor int, a TypeError is
            # raised. If this happens, key was not valid.
            raise IndexNotIntegerError, \
                      "ERROR! Weight index given is not an integer!"

        # Key must be >= 0 and < order.
        if key >= self.order or key < 0:
            raise IndexOutOfRangeError, \
                  "ERROR! Weight index given '%d' is not valid! " % (key) + \
                  "Must be %d >= index >= 0!" % (self.order-1)
        ########################################################################

        return self._val[key]


    def __len__(self):
        """
        Return the order of the GLQ (number of weights) if len function is
        called.
        """
        return self.order

################################################################################


################################################################################
# DOCTEST

if __name__ == '__main__':
    import doctest
    print "\nDOCTEST FOR GLQ.PY:"
    doctest.testmod()
    print "Done\n"
    
################################################################################