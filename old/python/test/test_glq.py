#!/usr/bin/env python
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
GLQ_test:
    Unit-tests of the Abscissas and Weights classes in module glq.py
"""
################################################################################
# Created on 14-Apr-2009 10:12:17 PM
# Last modified by $Author$
__author__ = 'Leonardo Uieda (leouieda@gmail.com)'
__version__ = '$Revision$'
__date__ = '$Date$'
__copyright__ = "Copyright (C) 2009  Leonardo Uieda"
__license__ = "GPL"
################################################################################


from math import sqrt
import unittest
import sys
import os

import glq


################################################################################
################################################################################
#                             ABSCISSAS TESTING
################################################################################
################################################################################


################################################################################
# TEST FOR SUCCESS

class AbscissasKnownValues(unittest.TestCase):
    """
    Test if the methods in Abscissas give the right results with specific inputs
    and raising the right exceptions.
    """

    known_values = ( (2, [sqrt(3)/3, -sqrt(3)/3]),
                     (3, [sqrt(15)/5, 0, -sqrt(15)/5]),
                     (4, [sqrt(525+70*sqrt(30))/35,
                          sqrt(525-70*sqrt(30))/35,
                         -sqrt(525-70*sqrt(30))/35,
                         -sqrt(525+70*sqrt(30))/35]),
                     (5, [sqrt(245+14*sqrt(70))/21,
                          sqrt(245-14*sqrt(70))/21,
                          0,
                          -sqrt(245-14*sqrt(70))/21,
                          -sqrt(245+14*sqrt(70))/21]) )

    known_values_scaled = ( \
        (2, -2.54, 14.9, [8.72*sqrt(3)/3 + 6.18, -8.72*sqrt(3)/3 + 6.18]),
        (3, -2.54, 14.9, [8.72*sqrt(15)/5 + 6.18,
                          6.18,
                          -8.72*sqrt(15)/5 + 6.18]),
        (4, -2.54, 14.9, [8.72*sqrt(525+70*sqrt(30))/35 + 6.18,
                          8.72*sqrt(525-70*sqrt(30))/35 + 6.18,
                          -8.72*sqrt(525-70*sqrt(30))/35 + 6.18,
                          -8.72*sqrt(525+70*sqrt(30))/35 + 6.18]),
        (5, -2.54, 14.9, [8.72*sqrt(245+14*sqrt(70))/21 + 6.18,
                          8.72*sqrt(245-14*sqrt(70))/21 + 6.18,
                          6.18,
                          -8.72*sqrt(245-14*sqrt(70))/21 + 6.18,
                          -8.72*sqrt(245+14*sqrt(70))/21 + 6.18]),
        (2, 125.6, 234.84, [54.62*sqrt(3)/3 + 180.22,
                            -54.62*sqrt(3)/3 + 180.22]),
        (3, 125.6, 234.84, [54.62*sqrt(15)/5 + 180.22,
                            180.22,
                            -54.62*sqrt(15)/5 + 180.22]),
        (4, 125.6, 234.84, [54.62*sqrt(525+70*sqrt(30))/35 + 180.22,
                            54.62*sqrt(525-70*sqrt(30))/35 + 180.22,
                            -54.62*sqrt(525-70*sqrt(30))/35 + 180.22,
                            -54.62*sqrt(525+70*sqrt(30))/35 + 180.22]),
        (5, 125.6, 234.84, [54.62*sqrt(245+14*sqrt(70))/21 + 180.22,
                            54.62*sqrt(245-14*sqrt(70))/21 + 180.22,
                            180.22,
                            -54.62*sqrt(245-14*sqrt(70))/21 + 180.22,
                            -54.62*sqrt(245+14*sqrt(70))/21 + 180.22]),
        (2, 3.5, -12.4, [-7.95*sqrt(3)/3 - 4.45, 7.95*sqrt(3)/3 - 4.45]),
        (3, 3.5, -12.4, [-7.95*sqrt(15)/5 - 4.45,
                         -4.45,
                         7.95*sqrt(15)/5 - 4.45]),
        (4, 3.5, -12.4, [-7.95*sqrt(525+70*sqrt(30))/35 - 4.45,
                         -7.95*sqrt(525-70*sqrt(30))/35 - 4.45,
                         7.95*sqrt(525-70*sqrt(30))/35 - 4.45,
                         7.95*sqrt(525+70*sqrt(30))/35 - 4.45]),
        (5, 3.5, -12.4, [-7.95*sqrt(245+14*sqrt(70))/21 - 4.45,
                         -7.95*sqrt(245-14*sqrt(70))/21 - 4.45,
                         -4.45,
                         7.95*sqrt(245-14*sqrt(70))/21 - 4.45,
                         7.95*sqrt(245+14*sqrt(70))/21 - 4.45])    )

    def test_calculate_known_values(self):
        """Abscissas.calculate should give known result with known input"""
        
        for order, known_abs in self.known_values:

            test_abs = glq.Abscissas(order)
            
            for i in range(0, order):

                self.assertAlmostEqual(test_abs._val[i], known_abs[i], places=15,
                    msg="Failed for abscissa %d of order %d." % (i+1, order))


    def test_return_right_order(self):
        """Abscissas len() should return the correct order"""

        for order in range(2, 50):

            test_abs = glq.Abscissas(order)

            self.assertEqual(len(test_abs), order, \
                             msg="Failed for order %d." % (order))


    def test_return_right_element(self):
        """Abscissas.__getitem__ must return the correct item"""

        for order in range(2, 50):
            
            test_abs = glq.Abscissas(order)
            
            for i in range(0, order):
            
                self.assertEqual(test_abs._val[i], test_abs[i], \
                    msg="Failed for abscissa %d of order %d." % (i+1, order))


    def test_scale_known_value(self):
        """Abscissas.scale should give known result with known input"""

        for order, lower, upper, known_abs in self.known_values_scaled:

            test_abs = glq.Abscissas(order)
            test_abs.scale(lower, upper)

            for i in range(0, order):

                self.assertAlmostEqual(test_abs._val[i], known_abs[i], places=12,
                    msg="Failed to scale abscissa %d of order %d to [%f,%f]."\
                         % (i+1, order, lower, upper))

################################################################################


################################################################################
# TEST FOR FAILURE

class AbscissasBadOrder(unittest.TestCase):
    """
    Test if the appropriate exceptions are raised on bad order inputs to
    Abscissas.
    """
    
    def test_string_order(self):
        """Abscissas.calculate should fail for non-integer (string) order"""
        a = glq.Abscissas(4)
        self.assertRaises(glq.OrderNotIntegerError, a.calculate, "5")

    def test_list_order(self):
        """Abscissas.calculate should fail for non-integer (list) order"""
        a = glq.Abscissas(4)
        self.assertRaises(glq.OrderNotIntegerError, a.calculate, [53.45, 345])
        
    def test_float_order(self):
        """Abscissas.calculate should fail for non-integer (float) order"""
        a = glq.Abscissas(5)
        self.assertRaises(glq.OrderNotIntegerError, a.calculate, 3.454)

    def test_one_order(self):
        """Abscissas.calculate should fail for order = 1"""
        a = glq.Abscissas(3)
        self.assertRaises(glq.OrderOutOfRangeError, a.calculate, 1)

    def test_zero_order(self):
        """Abscissas.calculate should fail for order = 0"""
        a = glq.Abscissas(2)
        self.assertRaises(glq.OrderOutOfRangeError, a.calculate, 0)

    def test_negative_order(self):
        """Abscissas.calculate should fail for negative order"""
        a = glq.Abscissas(6)
        self.assertRaises(glq.OrderOutOfRangeError, a.calculate, -2)


class AbscissasBadIndex(unittest.TestCase):
    """
    Test if the appropriate exceptions are raised on bad index inputs to
    Abscissas.
    """

    def test_string_index(self):
        """Abscissas.__getitem__ should fail for non-integer (string) index"""
        a = glq.Abscissas(7)
        self.assertRaises(glq.IndexNotIntegerError, a.__getitem__, "6")

    def test_list_index(self):
        """Abscissas.__getitem__ should fail for non-integer (list) index"""
        a = glq.Abscissas(4)
        self.assertRaises(glq.IndexNotIntegerError, a.__getitem__, [3, 5])

    def test_float_index(self):
        """Abscissas.__getitem__ should fail for non-integer (float) index"""
        a = glq.Abscissas(3)
        self.assertRaises(glq.IndexNotIntegerError, a.__getitem__, 2.466)
        
    def test_negative_index(self):
        """Abscissas.__getitem__ should fail if the key given is negative"""
        a = glq.Abscissas(5)
        self.assertRaises(glq.IndexOutOfRangeError, a.__getitem__, -2)

    def test_large_index(self):
        """Abscissas.__getitem__ should fail if the key given is > order"""
        a = glq.Abscissas(2)
        self.assertRaises(glq.IndexOutOfRangeError, a.__getitem__, 4)

    def test_order_index(self):
        """Abscissas.__getitem__ should fail if the key given is == order"""
        a = glq.Abscissas(3)
        self.assertRaises(glq.IndexOutOfRangeError, a.__getitem__, 3)


class AbscissasMaxIterations(unittest.TestCase):
    """
    Test if is max iterations is ever reached when calculating the roots.
    """
    def test_max_it(self):
        """Abscissas.__findroot should not reach max iterations"""
        for i in range(2, 50):
            a = glq.Abscissas(i)
            self.assertEqual(a.maxit, False, 'Failed for order %d.' % (i))

################################################################################



################################################################################
################################################################################
#                             WEIGHTS TESTING
################################################################################
################################################################################

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
        self._val_unscaled = self.known_values[order]
        self._val = self._val_unscaled

    def __len__(self):
        """
        Return the order of the GLQ (number of abscissas) if len function is
        called.
        """
        return self.order

################################################################################


################################################################################
# TEST FOR SUCCESS

class WeightsKnownValues(unittest.TestCase):
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
        """Weights.calculate should give known result with known input"""

        for order, known_weights in self.known_values:
            abscissas = MockAbscissas(order)
            weights = glq.Weights(abscissas)

            for i in range(0, order):
                self.assertAlmostEqual(weights._val[i], known_weights[i], \
                    places=15, msg="Failed for weight %d in order %d." \
                                 % (i, order))


    def test_return_right_order(self):
        """Weights len() should return the correct order"""

        for order, known_weights in self.known_values:
            abscissas = MockAbscissas(order)
            weights = glq.Weights(abscissas)

            self.assertEqual(len(weights), order, \
                             msg="Failed for order %d." % (order))


    def test_return_right_item(self):
        """Weights.__getitem__ must return the correct item"""

        for order, known_weights in self.known_values:
            abscissas = MockAbscissas(order)
            weights = glq.Weights(abscissas)

            for i in range(0, order):

                self.assertEqual(weights._val[i], weights[i], \
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
        """Weights.calculate fails if passing string instead of abscissas"""
        abscissas = MockAbscissas(2)
        weights = glq.Weights(abscissas)
        self.assertRaises(glq.NotAbscissasInstanceError, weights.calculate, \
                        "a_string")

    def test_abscissas_float(self):
        """Weights.calculate fails if passing float instead of abscissas"""
        abscissas = MockAbscissas(3)
        weights = glq.Weights(abscissas)
        self.assertRaises(glq.NotAbscissasInstanceError, weights.calculate, \
                        65.5677)

    def test_abscissas_int(self):
        """Weights.calculate fails if passing int instead of abscissas"""
        abscissas = MockAbscissas(4)
        weights = glq.Weights(abscissas)
        self.assertRaises(glq.NotAbscissasInstanceError, weights.calculate, \
                        -4357)

    def test_abscissas_list(self):
        """Weights.calculate fails if passing list instead of abscissas"""
        abscissas = MockAbscissas(5)
        weights = glq.Weights(abscissas)
        self.assertRaises(glq.NotAbscissasInstanceError, weights.calculate, \
                        [32, 22.786, 'a_string'])

    def test_abscissas_dict(self):
        """Weights.calculate fails if passing dict instead of abscissas"""
        abscissas = MockAbscissas(4)
        weights = glq.Weights(abscissas)
        self.assertRaises(glq.NotAbscissasInstanceError, weights.calculate, \
                        {1:32, 2:22.786, 3:'a_string'})

    def test_abscissas_tuple(self):
        """Weights.calculate fails if passing tuple instead of abscissas"""
        abscissas = MockAbscissas(3)
        weights = glq.Weights(abscissas)
        self.assertRaises(glq.NotAbscissasInstanceError, weights.calculate, \
                        (32, 22.786, 'a_string'))


class WeightsBadIndex(unittest.TestCase):
    """
    Test if the appropriate exceptions are raised on bad index inputs.
    """

    def test_string_index(self):
        """Weights.__getitem__ should fail for non-integer (string) order"""
        abscissas = MockAbscissas(3)
        weights = glq.Weights(abscissas)
        self.assertRaises(glq.IndexNotIntegerError, weights.__getitem__, "6")

    def test_list_index(self):
        """Weights.__getitem__ should fail for non-integer (list) order"""
        abscissas = MockAbscissas(3)
        weights = glq.Weights(abscissas)
        self.assertRaises(glq.IndexNotIntegerError, weights.__getitem__, [1,2])

    def test_float_index(self):
        """Weights.__getitem__ should fail for non-integer (float) order"""
        abscissas = MockAbscissas(2)
        weights = glq.Weights(abscissas)
        self.assertRaises(glq.IndexNotIntegerError, weights.__getitem__, 2.466)

    def test_negative_index(self):
        """Weights.__getitem__ should fail if the key given is negative"""
        abscissas = MockAbscissas(4)
        weights = glq.Weights(abscissas)
        self.assertRaises(glq.IndexOutOfRangeError, weights.__getitem__, -2)

    def test_large_index(self):
        """Weights.__getitem__ should fail if the key given is > order"""
        abscissas = MockAbscissas(5)
        weights = glq.Weights(abscissas)
        self.assertRaises(glq.IndexOutOfRangeError, weights.__getitem__, 6)

    def test_order_index(self):
        """Weights.__getitem__ should fail if the key given is == order"""
        abscissas = MockAbscissas(3)
        weights = glq.Weights(abscissas)
        self.assertRaises(glq.IndexOutOfRangeError, weights.__getitem__, 3)

################################################################################

# Run all tests if called as a program
if __name__ == "__main__":
    unittest.main()