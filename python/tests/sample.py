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
Sample:
    Calculate the gravitational fields for a 1 tesseroid model
    (2deg x 2deg x 15km, -0.3 g/cm^3) using a 2x2 2D GLQ on a 100x100 grid at
    250 km altitude.
    CMD Parameters:
        First arg is one of:
            all - calculate all fields;
            pot - calculate the gravitational potential;
            g - calculate only 3-component gravitational acceleration;
            ggt - calculate only 6-component gradient tensor;
        Second arg is the GLQ order.
"""
################################################################################
# Created on 16-Apr-2009 02:28 PM
# Last modified by $Author$
__author__ = 'Leonardo Uieda (leouieda@gmail.com)'
__version__ = '$Revision$'
__date__ = '$Date$'
__copyright__ = "Copyright (C) 2009  Leonardo Uieda"
__license__ = "GPL"
################################################################################

import sys
import os
import scipy as s
import pylab as p
import logging

# Get the path of where the test program is
testpath = os.path.abspath(os.path.dirname(sys.argv[0]))
# Get the path os the mods
tmp = testpath + os.path.sep + os.path.pardir + os.path.sep + 'mods'
modspath = os.path.abspath(tmp)
# Add this path to the sys.path so that the modules can be imported
sys.path.append(modspath)
# Now the mod can be imported
import glq
import tesseroid as t
import tesseroidgravity as tg

def main(opt):

    height = 250000.0
    order = 3

    # Create a logger for stokes
    logger = logging.getLogger()
    logger.setLevel(logging.DEBUG)
    # create console handler and set level to debug
    ch = logging.StreamHandler(strm=sys.stderr)
    ch.setLevel(logging.DEBUG)
    # create formatter
    formatter = logging.Formatter("%(levelname)s: %(message)s")
    # add formatter to ch
    ch.setFormatter(formatter)
    # add ch to logger
    logger.addHandler(ch)

    # Make the abscissas and weights
    print "Calculating abscissas and weights..."
    ablon = glq.Abscissas(order)
    ablat = glq.Abscissas(order)
    abr = glq.Abscissas(order)
    wlon = glq.Weights(ablon)
    wlat = glq.Weights(ablat)
    wr = glq.Weights(abr)

    # Make the field calculator obejects
    print "Creating calculator objects..."
    calculators = []
    tesspot = tg.TesseroidPotential(ablon, wlon, ablat, wlat, abr, wr)
    tessgx = tg.TesseroidGx(ablon, wlon, ablat, wlat, abr, wr)
    tessgy = tg.TesseroidGy(ablon, wlon, ablat, wlat, abr, wr)
    tessgz = tg.TesseroidGz(ablon, wlon, ablat, wlat, abr, wr)
    tessgxx = tg.TesseroidGxx(ablon, wlon, ablat, wlat, abr, wr)
    tessgxy = tg.TesseroidGxy(ablon, wlon, ablat, wlat, abr, wr)
    tessgxz = tg.TesseroidGxz(ablon, wlon, ablat, wlat, abr, wr)
    tessgyy = tg.TesseroidGyy(ablon, wlon, ablat, wlat, abr, wr)
    tessgyz = tg.TesseroidGyz(ablon, wlon, ablat, wlat, abr, wr)
    tessgzz = tg.TesseroidGzz(ablon, wlon, ablat, wlat, abr, wr)
    names = []

    if opt == 'all':
        calculators.append(tesspot)
        calculators.append(tessgx)
        calculators.append(tessgy)
        calculators.append(tessgz)
        calculators.append(tessgxx)
        calculators.append(tessgxy)
        calculators.append(tessgxz)
        calculators.append(tessgyy)
        calculators.append(tessgyz)
        calculators.append(tessgzz)
        names.append('Potential at %g m - GLQ Order %d' % (height, order))
        names.append('Gx at %g m - GLQ Order %d' % (height, order))
        names.append('Gy at %g m - GLQ Order %d' % (height, order))
        names.append('Gz at %g m - GLQ Order %d' % (height, order))
        names.append('Gxx at %g m - GLQ Order %d' % (height, order))
        names.append('Gxy at %g m - GLQ Order %d' % (height, order))
        names.append('Gxz at %g m - GLQ Order %d' % (height, order))
        names.append('Gyy at %g m - GLQ Order %d' % (height, order))
        names.append('Gyz at %g m - GLQ Order %d' % (height, order))
        names.append('Gzz at %g m - GLQ Order %d' % (height, order))
    if opt == 'pot':
        calculators.append(tesspot)
        names.append('Potential at %g m - GLQ Order %d' % (height, order))
    if opt == 'g':
        calculators.append(tessgx)
        calculators.append(tessgy)
        calculators.append(tessgz)
        names.append('Gx at %g m - GLQ Order %d' % (height, order))
        names.append('Gy at %g m - GLQ Order %d' % (height, order))
        names.append('Gz at %g m - GLQ Order %d' % (height, order))
    if opt == 'ggt':
        calculators.append(tessgxx)
        calculators.append(tessgxy)
        calculators.append(tessgxz)
        calculators.append(tessgyy)
        calculators.append(tessgyz)
        calculators.append(tessgzz)
        names.append('Gxx at %g m - GLQ Order %d' % (height, order))
        names.append('Gxy at %g m - GLQ Order %d' % (height, order))
        names.append('Gxz at %g m - GLQ Order %d' % (height, order))
        names.append('Gyy at %g m - GLQ Order %d' % (height, order))
        names.append('Gyz at %g m - GLQ Order %d' % (height, order))
        names.append('Gzz at %g m - GLQ Order %d' % (height, order))

    # Make the model
    print "Creating model..."
    tess = t.Tesseroid(19, 21, -1, 1, 30000, 45000, -0.3, 'The One')
    model = [tess]
    tesslons = [tess['w'], tess['e'], tess['e'], tess['w'], tess['w']]
    tesslats = [tess['s'], tess['s'], tess['n'], tess['n'], tess['s']]

    # Make the grid
    print "Creating grid..."
    lons = s.arange(15, 25.1, 0.1)
    lats = s.arange(-5, 5.1, 0.1)
    glons, glats = p.meshgrid(lons, lats)
    hs = []
    lns = []
    lts = []
    for lat in lats:
        for lon in lons:
            lns.append(lon)
            lts.append(lat)
            hs.append(height)

    # Calculate the fields
    print "Calculating fields with 2D algorithm... (This may take a while)"
    fields = []
    for calculator in calculators:
        fields.append(calculator.calculate(model, lns, lts, hs))

    # Plot the maps
    print "Making the maps..."
    for field, name in zip(*[fields, names]):

        print "  %s..." % (name)

        fieldlist = []
        for i in range(len(lons),len(field)+1,len(lons)):
            fieldlist.append(field[i-len(lons):i]+[field[i-len(lons)]])
        # Since I reversed lat, have to reverse field too
        #fieldlist.reverse()
        fieldgrd = s.array(fieldlist)

        p.figure(figsize=(10,10))
        p.title(name)
        p.xlabel('Longitude')
        p.ylabel('Latitude')
        p.pcolor(glons, glats, fieldgrd, cmap=p.cm.jet)
        p.colorbar(orientation='horizontal')

        # Plot the tesseroids with 'white'
        p.plot(tesslons,tesslats,'-w',linewidth=1)
        p.xlim(15, 25)
        p.ylim(-5, 5)
        

    print "Done!"
    p.show()

if __name__ == '__main__':
    if len(sys.argv) == 1 or sys.argv[1] != 'all' and sys.argv[1] != 'pot' \
       and sys.argv[1] != 'g' and sys.argv[1] != 'ggt':
        print __doc__
    else:
        main(sys.argv[1], int(sys.argv[2]))
    