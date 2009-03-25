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
Test_abscissas:
    Unit-tests of the Abscissas class in module GLQ.
    To execute the tests run:

        python test_abscissas.py -v


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
# $Author$
# Created: 21-Mar-2009 1:49:24 AM
# $Revision: 4 $
# Last edited: $Date: 2009-03-24 00:54:04 -0400 (Tue, 24 Mar 2009) $
################################################################################


import glq
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
        """calculate(order) should give known result with known input"""
        
        for order, known_abs in self.known_values:

            test_abs = glq.Abscissas(order)
            
            for i in range(0, order):

                self.assertAlmostEqual(test_abs.val[i], known_abs[i], places=15,
                    msg="Failed for abscissa %d of order %d." % (i+1, order))


    def test_return_right_order(self):
        """len(Abscissas(order)) should return the correct order."""

        for order in range(2, 50):

            test_abs = glq.Abscissas(order)

            self.assertEqual(len(test_abs), order, \
                             msg="Failed for order %d." % (order))


    def test_return_right_element(self):
        """__getitem__ must return the correct item."""

        for order in range(2, 50):
            
            test_abs = glq.Abscissas(order)
            
            for i in range(0, order):
            
                self.assertEqual(test_abs.val[i], test_abs[i], \
                    msg="Failed for abscissa %d of order %d." % (i+1, order))


    def test_scale_known_value(self):
        """scale(lower,upper) should give known result with known input"""

        for order, lower, upper, known_abs in self.known_values_scaled:

            test_abs = glq.Abscissas(order)
            test_abs.scale(lower, upper)

            for i in range(0, order):

                self.assertAlmostEqual(test_abs.val[i], known_abs[i], places=12,
                    msg="Failed to scale abscissa %d of order %d to [%f,%f]."\
                         % (i+1, order, lower, upper))

################################################################################


################################################################################
# TEST FOR FAILURE

class BadOrder(unittest.TestCase):
    """
    Test if the appropriate exceptions are raised on bad order inputs.
    """
    
    def test_string_order(self):
        """calculate(order) should fail for non-integer (string) order"""
        a = glq.Abscissas(4)
        self.assertRaises(glq.OrderNotIntegerError, a.calculate, "5")

    def test_float_order(self):
        """calculate(order) should fail for non-integer (float) order"""
        a = glq.Abscissas(5)
        self.assertRaises(glq.OrderNotIntegerError, a.calculate, 3.454)

    def test_one_order(self):
        """calculate(order) should fail for order = 1"""
        a = glq.Abscissas(3)
        self.assertRaises(glq.OrderOutOfRangeError, a.calculate, 1)

    def test_zero_order(self):
        """calculate(order) should fail for order = 0"""
        a = glq.Abscissas(2)
        self.assertRaises(glq.OrderOutOfRangeError, a.calculate, 0)

    def test_negative_order(self):
        """calculate(order) should fail for negative order"""
        a = glq.Abscissas(6)
        self.assertRaises(glq.OrderOutOfRangeError, a.calculate, -2)


class BadIndex(unittest.TestCase):
    """
    Test if the appropriate exceptions are raised on bad index inputs.
    """

    def test_string_index(self):
        """__getitem__ should fail for non-integer (string) order"""
        a = glq.Abscissas(7)
        self.assertRaises(glq.IndexNotIntegerError, a.__getitem__, "6")

    def test_float_index(self):
        """__getitem__ should fail for non-integer (float) order"""
        a = glq.Abscissas(3)
        self.assertRaises(glq.IndexNotIntegerError, a.__getitem__, 2.466)
        
    def test_negative_index(self):
        """__getitem__ should fail if the key given is negative."""
        a = glq.Abscissas(5)
        self.assertRaises(glq.IndexOutOfRangeError, a.__getitem__, -2)

    def test_large_index(self):
        """__getitem__ should fail if the key given is > order."""
        a = glq.Abscissas(2)
        self.assertRaises(glq.IndexOutOfRangeError, a.__getitem__, 4)

    def test_order_index(self):
        """__getitem__ should fail if the key given is == order."""
        a = glq.Abscissas(3)
        self.assertRaises(glq.IndexOutOfRangeError, a.__getitem__, 3)


class MaxIterations(unittest.TestCase):
    """
    Test if is max iterations is ever reached when calculating the roots.
    """

    def test_max_it(self):
        """Max iterations should not be reached when calculating abscissas."""
        for i in range(2, 100):
            try:
                a = glq.Abscissas(i)
            except glq.MaxIterationsError, e:
                self.fail(msg=e.message)

################################################################################

if __name__ == "__main__":
    unittest.main()