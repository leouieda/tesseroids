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
Times:
    Time how long it takes to calculate the gravitational fields for a 100
    tesseroid model on a 200x200 grid at 250 km altitude.
    Runs 10 times for each field and prints the individual times, the max, min
    and overall time.
    CMD Paramaters:
        First arg is one of:
            all - calculate all fields;
            pot - calculate the gravitational potential;
            g - calculate only 3-component gravitational acceleration;
            ggt - calculate only 6-component gradient tensor;
        Second arg is the GLQ order.

    Ex:
        python times.py all 2
    will time how it takes to calculate all the fields usign a GLQ order of 2.
"""
################################################################################
# Created on 21-Apr-2009 03:57 PM
# Last modified by $Author$
__author__ = 'Leonardo Uieda (leouieda@gmail.com)'
__version__ = '$Revision$'
__date__ = '$Date$'
__copyright__ = "Copyright (C) 2009  Leonardo Uieda"
__license__ = "GPL"
################################################################################

import sys
import timeit

def main(opt, order):

    print """Times using:
    * model size: 100
    * grid: 200x200
    * height: 250 km
    * GLQ order: %d
    """ % (order)

    names = []
    if opt == 'all':
        names.append('Potential')
        names.append('Gx')
        names.append('Gy')
        names.append('Gz')
        names.append('Gxx')
        names.append('Gxy')
        names.append('Gxz')
        names.append('Gyy')
        names.append('Gyz')
        names.append('Gzz')
    if opt == 'pot':
        names.append('Potential')
    if opt == 'g':
        names.append('Gx')
        names.append('Gy')
        names.append('Gz')
    if opt == 'ggt':
        names.append('Gxx')
        names.append('Gxy')
        names.append('Gxz')
        names.append('Gyy')
        names.append('Gyz')
        names.append('Gzz')

    # The setup statement that import everything, instantiates the Abscissas
    # and Weights, and creates the grids.
    pre = \
"""
import sys
import os
import scipy as sp
import logging

import glq
import tesseroid as t
import tesseroidgravity as tg

# Create a logger for stokes
logger = logging.getLogger()
logger.setLevel(logging.DEBUG)
# create console handler and set level to debug
ch = logging.StreamHandler(strm=sys.stderr)
ch.setLevel(logging.DEBUG)
# create formatter
formatter = logging.Formatter("%(name)s - %(levelname)s: %(message)s\\n")
# add formatter to ch
ch.setFormatter(formatter)
# add ch to logger
logger.addHandler(ch)

i = 1
mod = []
for w in range(-5, 5):
    e = w + 1
    for s in range(-5, 5):
        n = s + 1
        tess = t.Tesseroid(w, e, s, n, 0, 40000, 200, 'TEST%d' % (i))
        i += 1
        mod.append(tess) """ + \
"""
o = %d
ablon = glq.Abscissas(o)
ablat = glq.Abscissas(o)
abr = glq.Abscissas(o)
wlon = glq.Weights(ablon)
wlat = glq.Weights(ablat)
wr = glq.Weights(abr)
lons = sp.arange(-10,10.1,0.1)
lats = sp.arange(-10,10.1,0.1)
hs = []
lns = []
lts = []
for lon in lons:
    for lat in lats:
        lns.append(lon)
        lts.append(lat)
        hs.append(250000)
""" % (order)

    for name in names:
        msg = '\nsys.stderr.write("Working on %s\\n")' % (name)

        fieldcalc = "tp = tg.Tesseroid%s(ablon,wlon,ablat,wlat,abr,wr)" % (name)

        print "Timing %s..." % (name)
        times = []
        total = 0.0
        setupstr = pre + fieldcalc + msg
        timer = timeit.Timer(stmt='tp.calculate(mod, lns, lts, hs)', \
                             setup=setupstr)
        print "  Iteration: Time (s)"
        for i in range(1, 11):
            t = timer.timeit(1)
            print "  %d: %g" % (i, t)
            times.append(t)
            total += t

        print "  \nMax: %g" % (max(times))
        print "  Min: %g" % (min(times))
        print "  Total: %g\n" % (total)


if __name__ == '__main__':
    if len(sys.argv) == 1 or sys.argv[1] != 'all' and sys.argv[1] != 'pot' \
       and sys.argv[1] != 'g' and sys.argv[1] != 'ggt':
        print __doc__
    else:
        main(sys.argv[1], int(sys.argv[2]))