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
Test_tesseroid:
    Unit-tests of the Tesseroid class.    
"""
################################################################################
# Created on 25-Mar-2009 10:06:32 AM
# Last modified by $Author$
__author__ = 'Leonardo Uieda (leouieda@gmail.com)'
__version__ = '$Revision$'
__date__ = '$Date$'
__copyright__ = "Copyright (C) 2009  Leonardo Uieda"
__license__ = "GPL"
################################################################################

import unittest
import os
import sys

from tesseroids_lib import tesseroid

################################################################################
# TEST FOR SUCCESS

class TesseroidKnownValues(unittest.TestCase):
    """
    Test if the class returns the correct values for some inputs.
    """
    known = ( (180, 181.4, -12.4, -11.3, 500, 654.3, -2.64),
              (103, 112, 4.56, 6.7886, -1203, -1120, 3.466),
              (-1, 1, -2.4, 2.67, -12, 5.65, -1.346))

    known_change = ( (312, 3, -48, 3),
                     (179, -179, 179, 181) )

    def test_getitem(self):
        """Tesseroid.__getitem__ should return the correct values"""

        for w, e, s, n, t, b, d in self.known:
            tess = tesseroid.Tesseroid(w, e, s, n, t, b, d)
            self.assertEqual(w, tess['w'], msg="Failed for w=%f." % (w))
            self.assertEqual(e, tess['e'], msg="Failed for e=%f." % (e))
            self.assertEqual(s, tess['s'], msg="Failed for s=%f." % (s))
            self.assertEqual(n, tess['n'], msg="Failed for n=%f." % (n))
            self.assertEqual(t, tess['top'], msg="Failed for top=%f." % (t))
            self.assertEqual(b, tess['bottom'], msg="Failed for bottom=%f." \
                                                        % (b))
            self.assertEqual(d, tess['density'], msg="Failed for density=%f." \
                                                    % (d))

    def test_setbounds(self):
        """Tesseroid.set_bounds changes the W and E values appropriately"""
        s = -18
        n = -10
        t = 10
        b = 20
        for w, e, known_w, known_e in self.known_change:
            tess = tesseroid.Tesseroid(w, e, s, n, t, b, -2.64)
            self.assertEqual(known_w, tess['w'], msg="Failed for %f, %f." \
                                                        % (w, e))
            self.assertEqual(known_e, tess['e'], msg="Failed for %f, %f." \
                                                        % (w, e))
        
################################################################################


################################################################################
# TEST FOR FAILURE

class TesseroidBadParameters(unittest.TestCase):
    """
    Test if the proper exceptions are raised when passing bad boundary/density
    parameters.
    """

    def test_string_boudary(self):
        """Tesseroid.set_bounds should fail if passed string parameter"""
        tess = tesseroid.Tesseroid(1, 2, 1, 2, 12, 38, -2.34)
        self.assertRaises(tesseroid.InvalidBoundaryError, tess.set_bounds, \
                    '1', 2.34, 80.5, 81.34, 100, 140)
        self.assertRaises(tesseroid.InvalidBoundaryError, tess.set_bounds, \
                    1, '2.34', 80.5, 81.34, 100, 140)
        self.assertRaises(tesseroid.InvalidBoundaryError, tess.set_bounds, \
                    1, 2.34, '80.5', 81.34, 100, 140)
        self.assertRaises(tesseroid.InvalidBoundaryError, tess.set_bounds, \
                    1, 2.34, 80.5, '81.34', 100, 140)
        self.assertRaises(tesseroid.InvalidBoundaryError, tess.set_bounds, \
                    1, 2.34, 80.5, 81.34, '100', 140)
        self.assertRaises(tesseroid.InvalidBoundaryError, tess.set_bounds, \
                    1, 2.34, 80.5, 81.34, 100, '140')

    def test_list_boudary(self):
        """Tesseroid.set_bounds should fail if passed list parameter"""
        tess = tesseroid.Tesseroid(1, 2, 1, 2, 12, 38, -2.34)
        self.assertRaises(tesseroid.InvalidBoundaryError, tess.set_bounds, \
                    [1, 2], 2.34, 80.5, 81.34, 100, 140)
        self.assertRaises(tesseroid.InvalidBoundaryError, tess.set_bounds, \
                    1, [2.34, 3], 80.5, 81.34, 100, 140)
        self.assertRaises(tesseroid.InvalidBoundaryError, tess.set_bounds, \
                    1, 2.34, [80.5, 'meh'], 81.34, 100, 140)
        self.assertRaises(tesseroid.InvalidBoundaryError, tess.set_bounds, \
                    1, 2.34, 80.5, [81.34, 3.45], 100, 140)
        self.assertRaises(tesseroid.InvalidBoundaryError, tess.set_bounds, \
                    1, 2.34, 80.5, 81.34, [100, 1002.45], 140)
        self.assertRaises(tesseroid.InvalidBoundaryError, tess.set_bounds, \
                    1, 2.34, 80.5, 81.34, 100, [140, 'bla'])

    def test_string_density(self):
        """Tesseroid.set_density should fail is passed a string"""
        tess = tesseroid.Tesseroid(1, 2, 1, 2, 12, 38, -2.34)
        self.assertRaises(tesseroid.InvalidDensityError, tess.set_density, \
                          '12.345')

    def test_list_density(self):
        """Tesseroid.set_density should fail is passed a list"""
        tess = tesseroid.Tesseroid(1, 2, 1, 2, 12, 38, -2.34)
        self.assertRaises(tesseroid.InvalidDensityError, tess.set_density, \
                          [12.345, 23.56])

    def test_dict_density(self):
        """Tesseroid.set_density should fail is passed a dictionary"""
        tess = tesseroid.Tesseroid(1, 2, 1, 2, 12, 38, -2.34)
        self.assertRaises(tesseroid.InvalidDensityError, tess.set_density, \
                          {1:12.345, 2:23.56})

    def test_tuple_density(self):
        """Tesseroid.set_density should fail is passed a tuple"""
        tess = tesseroid.Tesseroid(1, 2, 1, 2, 12, 38, -2.34)
        self.assertRaises(tesseroid.InvalidDensityError, tess.set_density, \
                          (12.345, 23.56))

    def test_w_greaterthan360(self):
        """Tesseroid.set_bounds should fail if passed W > 360"""
        tess = tesseroid.Tesseroid(1, 2, 1, 2, 12, 38, -2.34)
        self.assertRaises(tesseroid.InvalidBoundaryError, tess.set_bounds, \
                    362.45, 2, 1, 2, 12, 38)

    def test_w_lessthanm180(self):
        """Tesseroid.set_bounds should fail if passed W < -180"""
        tess = tesseroid.Tesseroid(1, 2, 1, 2, 12, 38, -2.34)
        self.assertRaises(tesseroid.InvalidBoundaryError, tess.set_bounds, \
                    -198.566, 2, 1, 2, 12, 38)

    def test_e_greaterthan360(self):
        """Tesseroid.set_bounds should fail if passed E > 360"""
        tess = tesseroid.Tesseroid(1, 2, 1, 2, 12, 38, -2.34)
        self.assertRaises(tesseroid.InvalidBoundaryError, tess.set_bounds, \
                    1, 398.57, 1, 2, 12, 38)

    def test_e_lessthanm180(self):
        """Tesseroid.set_bounds should fail if passed E < -180"""
        tess = tesseroid.Tesseroid(1, 2, 1, 2, 12, 38, -2.34)
        self.assertRaises(tesseroid.InvalidBoundaryError, tess.set_bounds, \
                    134.3, -198.566, 1, 2, 12, 38)

    def test_s_greaterthan90(self):
        """Tesseroid.set_bounds should fail if passed S > 90"""
        tess = tesseroid.Tesseroid(1, 2, 1, 2, 12, 38, -2.34)
        self.assertRaises(tesseroid.InvalidBoundaryError, tess.set_bounds, \
                    1, 2, 92, 2, 12, 38)

    def test_s_lessthanm90(self):
        """Tesseroid.set_bounds should fail if passed S < -90"""
        tess = tesseroid.Tesseroid(1, 2, 1, 2, 12, 38, -2.34)
        self.assertRaises(tesseroid.InvalidBoundaryError, tess.set_bounds, \
                    134.3, 198.566, -124 , 2, 12, 38)

    def test_n_greaterthan90(self):
        """Tesseroid.set_bounds should fail if passed N > 90"""
        tess = tesseroid.Tesseroid(1, 2, 1, 2, 12, 38, -2.34)
        self.assertRaises(tesseroid.InvalidBoundaryError, tess.set_bounds, \
                    1, 2, 80, 139.56, 12, 38)

    def test_n_lessthanm90(self):
        """Tesseroid.set_bounds should fail if passed N < -90"""
        tess = tesseroid.Tesseroid(1, 2, 1, 2, 12, 38, -2.34)
        self.assertRaises(tesseroid.InvalidBoundaryError, tess.set_bounds, \
                    134.3, 198.566, -12 , -200.865, 12, 38)

    def test_s_greaterthan_n(self):
        """Tesseroid.set_bounds should fail if passed S > N"""
        tess = tesseroid.Tesseroid(1, 2, 1, 2, 12, 38, -2.34)
        self.assertRaises(tesseroid.InvalidBoundaryError, tess.set_bounds, \
                    1, 2, 88.787, 82.56, 12, 38)

    def test_s_equal_n(self):
        """Tesseroid.set_bounds should fail if passed S == N"""
        tess = tesseroid.Tesseroid(1, 2, 1, 2, 12, 38, -2.34)
        self.assertRaises(tesseroid.InvalidBoundaryError, tess.set_bounds, \
                    1, 2, -34.67765, -34.67765, 12, 38)

    def test_top_greaterthan_bottom(self):
        """Tesseroid.set_bounds should fail if passed Top > Bottom"""
        tess = tesseroid.Tesseroid(1, 2, 1, 2, 12, 38, -2.34)
        self.assertRaises(tesseroid.InvalidBoundaryError, tess.set_bounds, \
                    1, 2, 76.77, 82.56, 100, 38.45)

    def test_top_equal_bottom(self):
        """Tesseroid.set_bounds should fail if passed Top == Bottom"""
        tess = tesseroid.Tesseroid(1, 2, 1, 2, 12, 38, -2.34)
        self.assertRaises(tesseroid.InvalidBoundaryError, tess.set_bounds, \
                    1, 2, -34.67765, -30.68, 123.45, 123.45)


class TesseroidBadKey(unittest.TestCase):
    """
    Test if the proper exceptions are raised when passing bad key to __getitem__
    """

    def test_invalid_string(self):
        """Tesseroid.___getitem__ fails if key is not boundary or density"""
        tess = tesseroid.Tesseroid(1.45, 6.2435, -1.25, 2.3567, 1, 33.35, -2.34)
        self.assertRaises(tesseroid.InvalidKeyError, tess.__getitem__, 'meh')

    def test_float_key(self):
        """Tesseroid.___getitem__ should fail if key is a float"""
        tess = tesseroid.Tesseroid(1.45, 6.2435, -1.25, 2.3567, 1, 33.35, -2.34)
        self.assertRaises(tesseroid.InvalidKeyError, tess.__getitem__, 173.564)

    def test_int_key(self):
        """Tesseroid.___getitem__ should fail if key is a int"""
        tess = tesseroid.Tesseroid(1.45, 6.2435, -1.25, 2.3567, 1, 33.35, -2.34)
        self.assertRaises(tesseroid.InvalidKeyError, tess.__getitem__, 466)

################################################################################

if __name__ == "__main__":
    unittest.main()
