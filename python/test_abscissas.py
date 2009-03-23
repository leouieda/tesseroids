#!/usr/bin/env python
"""
Unit-tests of abscissas.py.

Author: Leonardo Uieda (leouieda@gmail.com)
        Geophysics undergraduate student
        Instituto de Astronomia, Geofisica e Ciencias Atmosfericas
        Universidade de Sao Paulo
        Sao Paulo - SP, Brasil

Created: 21-Mar-2009 1:49:24 AM
Revision: $Revision$
Last edited: $Date$
"""

import abscissas as ab
import unittest
from math import sqrt


################################################################################
# TEST FOR SUCCESS

class KnownValues(unittest.TestCase):
    """
    Test if the methods give the right results with specific inputs and raising
    the right exceptions.
    """

    known_values = ( (2, [sqrt(3)/3, -sqrt(3)/3]),
                     (3, [sqrt(15)/5, 0, -sqrt(15)/5]),
                     (4, [sqrt(525+70*sqrt(30))/35, sqrt(525-70*sqrt(30))/35,
                           -sqrt(525-70*sqrt(30))/35, -sqrt(525+70*sqrt(30))/35]),
                     (5, [sqrt(245+14*sqrt(70))/21, sqrt(245-14*sqrt(70))/21,
                           0, -sqrt(245-14*sqrt(70))/21,
                           -sqrt(245+14*sqrt(70))/21]) )

    def test_calculate_known_values(self):
        """__calculate() should give known result with known input"""
        
        for order, known_abs in self.known_values:

            test_abs = ab.Abscissas(order)
            
            for i in range(0, order):

                self.assertAlmostEqual(test_abs.val[i], known_abs[i], places=15,
                    msg="Failed for abscissa %d of order %d." % (i+1, order))

################################################################################


################################################################################
# TEST FOR FAILURE

class BadOrder(unittest.TestCase):
    """
    Test if the appropriate exceptions are raised on bad order inputs.
    """
    
    def test_string_order(self):
        """__calculate() should fail for non-integer (string) order"""
        a = ab.Abscissas(4)
        self.assertRaises(ab.OrderNotIntegerError, a.calculate, "5")

    def test_float_order(self):
        """__calculate() should fail for non-integer (float) order"""
        a = ab.Abscissas(5)
        self.assertRaises(ab.OrderNotIntegerError, a.calculate, 3.454)

    def test_one_order(self):
        """__calculate() should fail for order = 1"""
        a = ab.Abscissas(3)
        self.assertRaises(ab.OrderOutOfRangeError, a.calculate, 1)

    def test_zero_order(self):
        """__calculate() should fail for order = 0"""
        a = ab.Abscissas(2)
        self.assertRaises(ab.OrderOutOfRangeError, a.calculate, 0)

    def test_negative_order(self):
        """__calculate() should fail for negative order"""
        a = ab.Abscissas(6)
        self.assertRaises(ab.OrderOutOfRangeError, a.calculate, -2)


class BadIndex(unittest.TestCase):
    """
    Test if the appropriate exceptions are raised on bad index inputs.
    """

    def test_string_index(self):
        """__getitem__ should fail for non-integer (string) order"""
        a = ab.Abscissas(7)
        self.assertRaises(ab.IndexNotIntegerError, a.__getitem__, "6")

    def test_float_index(self):
        """__getitem__ should fail for non-integer (float) order"""
        a = ab.Abscissas(3)
        self.assertRaises(ab.IndexNotIntegerError, a.__getitem__, 2.466)
        
    def test_negative_index(self):
        """__getitem__ should fail if the key given is negative."""
        a = ab.Abscissas(5)
        self.assertRaises(ab.IndexOutOfRangeError, a.__getitem__, -2)

    def test_large_index(self):
        """__getitem__ should fail if the key given is > order."""
        a = ab.Abscissas(2)
        self.assertRaises(ab.IndexOutOfRangeError, a.__getitem__, 4)

    def test_order_index(self):
        """__getitem__ should fail if the key given is == order."""
        a = ab.Abscissas(3)
        self.assertRaises(ab.IndexOutOfRangeError, a.__getitem__, 3)


class MaxIterations(unittest.TestCase):
    """
    Test if is max iterations is ever reached when calculating the roots.
    """

    def test_max_it(self):
        """Max iterations should not be reached when calculating abscissas."""
        for i in range(2, 100):
            try:
                a = ab.Abscissas(i)
            except ab.MaxIterationsError, e:
                self.fail(msg=e.message)

################################################################################

if __name__ == "__main__":
    unittest.main()