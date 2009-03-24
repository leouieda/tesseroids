#!/usr/bin/env python
"""
Unit-tests of the Weights class in glq.py.

Author: Leonardo Uieda (leouieda@gmail.com)
        Geophysics undergraduate student
        Instituto de Astronomia, Geofisica e Ciencias Atmosfericas
        Universidade de Sao Paulo
        Sao Paulo - SP, Brasil

Created: 23-Mar-2009 10:29:35 PM
Revision: $Revision$
Last edited: $Date$
"""

import glq
import unittest
from math import sqrt


################################################################################
# MOCK TO BE PASSED TO WEIGHTS

class MockAbscissas(glq.Abscissas):
    """
    Mock class for Abscissas.
    This class has the same methods that Weights needs but they are not
    implemented as in Abscissas. When called they will give known values from
    a look-up table.
    """

    known_values = { 2:[sqrt(3)/3, -sqrt(3)/3],
                     3:[sqrt(15)/5, 0, -sqrt(15)/5],
                     4:[sqrt(525+70*sqrt(30))/35,
                        sqrt(525-70*sqrt(30))/35,
                        -sqrt(525-70*sqrt(30))/35,
                        -sqrt(525+70*sqrt(30))/35],
                     5:[sqrt(245+14*sqrt(70))/21,
                        sqrt(245-14*sqrt(70))/21,
                        0,
                        -sqrt(245-14*sqrt(70))/21,
                        -sqrt(245+14*sqrt(70))/21] }

    def __init__(self, order):
        glq.Abscissas.__init__(self, order)

    def calculate(self, order):
        """
        Just set the correct value from the known_values.
        """
        self.order = order
        self.val_unscaled = self.known_values[order]
        self.val = self.val_unscaled

    def __len__(self):
        """
        Return the order of the GLQ (number of abscissas) if len function is
        called.
        """
        return self.order

################################################################################


################################################################################
# TEST FOR SUCCESS

class KnownValues(unittest.TestCase):
    """
    Test if the methods give the right results with specific inputs and raising
    the right exceptions.
    """

    known_values = ( (2, [1, 1]),
                     (3, [5.0/9, 8.0/9, 5.0/9]),
                     (4, [(18-sqrt(30))/36.0, (18+sqrt(30))/36.0,
                          (18+sqrt(30))/36.0, (18-sqrt(30))/36.0]),
                     (5, [(322-13*sqrt(70))/900.0, (322+13*sqrt(70))/900.0,
                          128.0/225,
                          (322+13*sqrt(70))/900.0, (322-13*sqrt(70))/900.0]) )


    def test_calculate_known_values(self):
        """'calculate' should give known result with known input."""

        for order, known_weights in self.known_values:
            abscissas = MockAbscissas(order)
            weights = glq.Weights(abscissas)

            for i in range(0, order):
                self.assertAlmostEqual(weights.val[i], known_weights[i], \
                    places=15, msg="Failed for weight %d in order %d." \
                                 % (i, order))


    def test_return_right_order(self):
        """'len' should return the correct order."""

        for order, known_weights in self.known_values:
            abscissas = MockAbscissas(order)
            weights = glq.Weights(abscissas)

            self.assertEqual(len(weights), order, \
                             msg="Failed for order %d." % (order))


    def test_return_right_item(self):
        """__getitem__ must return the correct item."""

        for order, known_weights in self.known_values:
            abscissas = MockAbscissas(order)
            weights = glq.Weights(abscissas)

            for i in range(0, order):

                self.assertEqual(weights.val[i], weights[i], \
                    msg="Failed for weight %d in order %d." % (i+1, order))

################################################################################


################################################################################
# TEST FOR FAILURE

class BadAbscissas(unittest.TestCase):
    """
    Test if the appropriate exceptions are raised when passing and invalid
    abscissas.
    """

    def test_abscissas_string(self):
        """'calculate' should fail is passing a string in place of abscissas."""
        abscissas = MockAbscissas(2)
        weights = glq.Weights(abscissas)
        self.assertRaises(glq.NotAbscissasInstanceError, weights.calculate, \
                        "a_string")

    def test_abscissas_float(self):
        """'calculate' should fail is passing a float in place of abscissas."""
        abscissas = MockAbscissas(3)
        weights = glq.Weights(abscissas)
        self.assertRaises(glq.NotAbscissasInstanceError, weights.calculate, \
                        65.5677)

    def test_abscissas_int(self):
        """'calculate' should fail is passing an int in place of abscissas."""
        abscissas = MockAbscissas(4)
        weights = glq.Weights(abscissas)
        self.assertRaises(glq.NotAbscissasInstanceError, weights.calculate, \
                        -4357)

    def test_abscissas_list(self):
        """'calculate' should fail is passing a list in place of abscissas."""
        abscissas = MockAbscissas(5)
        weights = glq.Weights(abscissas)
        self.assertRaises(glq.NotAbscissasInstanceError, weights.calculate, \
                        [32, 22.786, 'a_string'])

    def test_abscissas_dict(self):
        """'calculate' should fail is passing a dict in place of abscissas."""
        abscissas = MockAbscissas(4)
        weights = glq.Weights(abscissas)
        self.assertRaises(glq.NotAbscissasInstanceError, weights.calculate, \
                        {1:32, 2:22.786, 3:'a_string'})

    def test_abscissas_tuple(self):
        """'calculate' should fail is passing a tuple in place of abscissas."""
        abscissas = MockAbscissas(3)
        weights = glq.Weights(abscissas)
        self.assertRaises(glq.NotAbscissasInstanceError, weights.calculate, \
                        (32, 22.786, 'a_string'))


class BadIndex(unittest.TestCase):
    """
    Test if the appropriate exceptions are raised on bad index inputs.
    """

    def test_string_index(self):
        """__getitem__ should fail for non-integer (string) order"""
        abscissas = MockAbscissas(3)
        weights = glq.Weights(abscissas)
        self.assertRaises(glq.IndexNotIntegerError, weights.__getitem__, "6")

    def test_float_index(self):
        """__getitem__ should fail for non-integer (float) order"""
        abscissas = MockAbscissas(2)
        weights = glq.Weights(abscissas)
        self.assertRaises(glq.IndexNotIntegerError, weights.__getitem__, 2.466)

    def test_negative_index(self):
        """__getitem__ should fail if the key given is negative."""
        abscissas = MockAbscissas(4)
        weights = glq.Weights(abscissas)
        self.assertRaises(glq.IndexOutOfRangeError, weights.__getitem__, -2)

    def test_large_index(self):
        """__getitem__ should fail if the key given is > order."""
        abscissas = MockAbscissas(5)
        weights = glq.Weights(abscissas)
        self.assertRaises(glq.IndexOutOfRangeError, weights.__getitem__, 6)

    def test_order_index(self):
        """__getitem__ should fail if the key given is == order."""
        abscissas = MockAbscissas(3)
        weights = glq.Weights(abscissas)
        self.assertRaises(glq.IndexOutOfRangeError, weights.__getitem__, 3)

################################################################################

if __name__ == "__main__":
    unittest.main()