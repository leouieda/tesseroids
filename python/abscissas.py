"""
Abscissas:
    Calculates the abscissas used in the Gauss-Legenre Quadrature (GLQ).
    Meant to be used in conjuction with the Weights class.

    Example: To integrate x**2 from -20 to 15 using a 5 point GLQ

    >>> from math import e
    >>> from weights import Weights
    >>>
    >>> my_abs = Abscissas(order=5)
    >>> my_weights = Weights(my_abs)
    >>>
    >>> my_abs.scale(-20, 15)
    >>>
    >>> a = my_abs.val
    >>> w = my_weights.val
    >>>
    >>> result = 0
    >>> for i in range(0, 5):
    ...     result += w[i]*(e**(-a[i]**2))
    ...
    >>> print result
    
    
            

    This class implements Newton's Method for Multiple Roots presented in:
        Barrera-Figueroa, V., Sosa-Pedroza, J. and Lopez-Bonilla, J.,
        "Multiple root finder algorithm for Legendre and Chebyshev polynomials
         via Newton's method", 2006,
        Annales mathematicae et Informaticae, 33, pp 3-13.

Author: Leonardo Uieda (leouieda@gmail.com)
        Geophysics undergraduate student
        Instituto de Astronomia, Geofisica e Ciencias Atmosfericas
        Universidade de Sao Paulo
        Sao Paulo - SP, Brasil

Created: 21-Mar-2009 1:47:06 AM
Revision: $Revision$
Last edited: $Date$
"""

import math as m



################################################################################
#  EXCEPTIONS

class AbscissasError(Exception): 
    """
    Base exception for errors in the execution of the Abscissas class.
    """
    pass

class OrderNotIntegerError(AbscissasError):
    """
    Thrown when an order that is not integer is passed to __init__.
    """
    pass

class OrderOutOfRangeError(AbscissasError):
    """
    Thrown when an order that is not >= 2 is passed to __init__.
    """
    pass

class MaxIterationsError(AbscissasError):
    """
    Thrown when the maximum number of iterations is reached when searching for
    a Legendre polynomial root. This will indicate that the mininum error was
    likelly not achieved.
    """
    pass

class IndexOutOfRangeError(AbscissasError):
    """
    Thrown when trying to access abscissas with __getitem__ passing an out of
    range key. order > key >= 0.
    """
    pass

class IndexNotIntegerError(AbscissasError):
    """
    Thrown when trying to access abscissa with __getitem__ passing a non-integer
    key.
    """
    pass

################################################################################


################################################################################
# ABSCISSAS CLASS

class Abscissas:
    """
    Abscissas class.
    """

    # Class variables that define the maximum number of iterations when
    # searching for a Legendre polynomial root and the maximum error accepted.
    # These values should only be changed if max_iterations is being reached!
    max_iterations = 10000
    max_error = 0.000000000000001


    def __init__(self, order):                
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
                     
        for iterations in range(1, self.max_iterations + 1):
        
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
                rootsum += 1.0 / (x0 - self.val_unscaled[n])
            
            # Update the guess for the root
            x1 = x0 - ( pn / (pn_line - (pn*rootsum)) )

            # Break the loop if the required error is reached
            if abs(x1 - x0) <= self.max_error:
                break

        if iterations == self.max_iterations:
            raise MaxIterationsError, \
                "WARNING! Maximum iterations (%d) " % (self.max_iterations) +\
                "reached when looking for root %d of " % (current_root) + \
                "Legendre polynomial P%d." % (self.order)

        return x1
    

    def calculate(self, order):
        """
        Calculates the abscissas for the determined order.
        """
        
        # Check if the order given is valid
        ########################################################################
        # Check if it is an integer.
        if int(order) != order:
            raise OrderNotIntegerError, \
                  "ERROR! Order of GLQ (%s) provided is not an integer!" \
                      % (str(order))
                      
        # Order must be at least 2.
        if order < 2:
            raise OrderOutOfRangeError, \
                  "ERROR! Order of GLQ (%d) cannot be < 2!" % (order)
        ########################################################################
        
        # Set the order of the GLQ. This will be number of abscissas.
        self.order = order

        # Initialize the unscaled values with 0
        self.val_unscaled = [0.0]
        self.val_unscaled *= order
        
        # Iterate to find all roots of the Legendre polynomial
        # Find the root and put it in the unscaled version of the roots
        for i in range(0, order):
            self.val_unscaled[i] = self.__findroot(m.cos(m.pi*((i+1)-0.25)/ \
                                                   (order+0.5)), i)
        # Then copy the to the ones that will be scaled in the future
        self.val = self.val_unscaled
        

    def scale(self, lower, upper):
        """
        Scale the abscissas to the interval [lower,upper].
        """
        pass

    def __getitem__(self, key):
        """
        Get the abscissa value corresponding to key. 
        key should be integer and order > key >= 0.
        Raises IndexOutOfRangeError and IndexNotIntegerError.
        """
        pass

    def __len__(self):
        """
        Return the order of the abscissas if len function is called.
        """
        pass

################################################################################


################################################################################
# DOCTEST

if __name__ == '__main__':
    import doctest
    doctest.testmod()
    
################################################################################