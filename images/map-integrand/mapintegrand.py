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
MapIntegrand:
    Map how the integral kernel varies with respect to the integration point for
    a given calculation point.
"""
################################################################################
# Created on 7-May-2009 12:24 PM
# Last modified by $Author: $
__author__ = 'Leonardo Uieda (leouieda@gmail.com)'
__version__ = '$Revision: $'
__date__ = '$Date: $'
__copyright__ = "Copyright (C) 2009  Leonardo Uieda"
__license__ = "GPL"
################################################################################

import os
import sys
import pylab as pl
import numpy as np
import math

# Get the path of where the test program is
testpath = os.path.abspath(os.path.dirname(sys.argv[0]))
# Get the path os the mods
tmp = testpath + os.path.sep + os.path.pardir + os.path.sep + 'mods'
modspath = os.path.abspath(tmp)
# Add this path to the sys.path so that the modules can be imported
sys.path.append(modspath)
# Now the mod can be imported
import glq
import tesseroid as ts
import tesseroidgravity as tg

def mapint(lon, lat, height, order):
    """
    Map the integrand for the calculation point (lon,lat,height).
    """

    pl.figure(figsize=(15,10))
    pl.suptitle(r"Lon: $%g^\circ$   Lat: $%g^\circ$   Height: $%g\ km$" % (lon, lat, height/1000.0), fontsize=16)

    # Convert lon, lat to radians
    deg2rad = math.pi/180.0
    rad2deg = 180.0/math.pi
    lon = deg2rad*lon
    lat = deg2rad*lat

    # Create the abscissas and weights
    abslon = glq.Abscissas(order)
    abslat = glq.Abscissas(order)
    absr = glq.Abscissas(order)
    wlon = glq.Weights(abslon)
    wlat = glq.Weights(abslat)
    wr = glq.Weights(absr)

    # Create a calculator class for the integrands of the GGT
    calculators = []
    names = []
    tessgxx = tg.TesseroidGxx(abslon, wlon, abslat, wlat, absr, wr)
    calculators.append(tessgxx)
    names.append("Gxx")
    tessgxy = tg.TesseroidGxy(abslon, wlon, abslat, wlat, absr, wr)
    calculators.append(tessgxy)
    names.append("Gxy")
    tessgxz = tg.TesseroidGxz(abslon, wlon, abslat, wlat, absr, wr)
    calculators.append(tessgxz)
    names.append("Gxz")
    tessgyy = tg.TesseroidGyy(abslon, wlon, abslat, wlat, absr, wr)
    calculators.append(tessgyy)
    names.append("Gyy")
    tessgyz = tg.TesseroidGyz(abslon, wlon, abslat, wlat, absr, wr)
    calculators.append(tessgyz)
    names.append("Gyz")
    tessgzz = tg.TesseroidGzz(abslon, wlon, abslat, wlat, absr, wr)
    calculators.append(tessgzz)
    names.append("Gzz")

    # Create a tesseroid
    tess = ts.Tesseroid(0, 2, 44, 46, 0, 30000, 0.2, '1')
    r1 = tessgxx.R - tess['bottom']
    r2 = tessgxx.R - tess['top']
    r = tessgxx.R + height

    # For each calculator, map the integrand and plot
    index = [1,2,3,5,6,9]
    intlons = np.arange(tess['w'], tess['e']+0.02, 0.02)
    intlats = np.arange(tess['s'], tess['n']+0.02, 0.02)
    intlonsrad = deg2rad*intlons
    intlatsrad = deg2rad*intlats
    lonsgrid, latsgrid = pl.meshgrid(intlons, intlats)
    for calc, name, i in zip(*[calculators, names, index]):
        # Map the integrand
        integrand = []
        for latl in intlatsrad:
            tmp = []
            for lonl in intlonsrad:
                try:
                    tmp.append(calc.kernel(r, lon, lat, r1, r2, lonl, latl))
                except tg.SingularityError:
                    try:
                        tmp.append(tmp[-1])
                    except:
                        tmp.append(0)
            integrand.append(tmp)

        # Plot the integrand
        pl.subplot(3,3,i)
        #pl.title(name)
        #pl.xlabel('Longitude')
        #pl.ylabel('Latitude')
        pc = pl.pcolor(lonsgrid, latsgrid, integrand, cmap=pl.cm.jet)
        pl.colorbar(orientation='vertical')#,aspect=50)
        abslon.scale(tess['w'], tess['e'])
        abslat.scale(tess['s'], tess['n'])
        for alon in abslon._val:
            pl.plot([alon]*len(abslat._val), abslat._val, 'k+')
        pl.xlim(tess['w'], tess['e'])
        pl.ylim(tess['s'], tess['n'])

    pl.savefig('lon%g-lat%g-h%g-o%d.png' % (lon*rad2deg, lat*rad2deg, height/1000.0, order), fmt='png')

if __name__ == '__main__':


    heights = [0, 1000, 10000, 20000, 30000, 50000, 100000, 150000, 250000]
    lons = np.arange(-10, 10, 1)    
    lats = np.arange(35, 55, 1)
    order = 4
    for height in heights:
        lat = 45
        for lon in lons:
            mapint(lon, lat, height, order)
        lon = 1
        for lat in lats:
            mapint(lon, lat, height, order)
    #pl.show()