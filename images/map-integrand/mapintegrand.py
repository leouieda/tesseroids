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
# Last modified by $Author$
__author__ = 'Leonardo Uieda (leouieda@gmail.com)'
__version__ = '$Revision$'
__date__ = '$Date$'
__copyright__ = "Copyright (C) 2009  Leonardo Uieda"
__license__ = "GPL"
################################################################################

import os
import sys
import pylab as pl
import numpy as np
import math

# Get the path of where the program is
mypath = os.path.abspath(os.path.dirname(sys.argv[0]))
# Get the path os the mods
tmp = mypath + os.path.sep + os.path.pardir + os.path.sep + os.path.pardir + \
      os.path.sep + 'python' + os.path.sep + 'mods'
modspath = os.path.abspath(tmp)
# Add this path to the sys.path so that the modules can be imported
sys.path.append(modspath)
# Now the mod can be imported
import glq
import tesseroid as ts
import tesseroidgravity as tg

def mapint(lon, lat, height, order, folder):
    """
    Map the integrand for the calculation point (lon,lat,height).
    """

    pl.figure(figsize=(15,10))
    pl.suptitle(r"Lon: $%g^\circ$   Lat: $%g^\circ$   Altitude: $%g\ km$" % \
                (lon, lat, height/1000.0), fontsize=16)

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
    tess = ts.Tesseroid(-1, 1, 44, 46, 0, 30000, 0.2, '1')
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
        pl.colorbar(orientation='vertical', format='%2g')#,aspect=50)
        abslon.scale(tess['w'], tess['e'])
        abslat.scale(tess['s'], tess['n'])
        for alon in abslon._val:
            pl.plot([alon]*len(abslat._val), abslat._val, 'k+')
        pl.xlim(tess['w'], tess['e'])
        pl.ylim(tess['s'], tess['n'])

    filename = folder + os.path.sep + 'lon%g-lat%g-h%g-o%d.png' % \
                 (lon*rad2deg, lat*rad2deg, height/1000.0, order)
    pl.savefig(filename, fmt='png')
    pl.close()


def drawmap(height, folder, prof_lons, prof_lats):
    """
    Draw the tensor maps at height and save them in folder.
    """

    pl.figure(figsize=(15,10))
    pl.suptitle("Gradiente da Gravidade a %g km" % (height/1000.0), \
                fontsize=16)

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
    tess = ts.Tesseroid(-1, 1, 44, 46, 0, 30000, 0.2, '1')
    model = [tess]
    tesslons = [tess['w'], tess['e'], tess['e'], tess['w'], tess['w']]
    tesslats = [tess['s'], tess['s'], tess['n'], tess['n'], tess['s']]

    # Create a calculation grid
    lons = np.arange(-20, -10, 1).tolist()
    lons += np.arange(-10, 10, 0.2).tolist()
    lons += np.arange(10, 20+1, 1).tolist()
    lats = np.arange(25, 35, 1).tolist()
    lats += np.arange(35, 55, 0.2).tolist()
    lats += np.arange(55, 65+1, 1).tolist()
    lonslist = []
    latslist = []
    heights = []
    # Iterate over the lats
    for lat in lats:
        # Iterate over the lons
        for lon in lons:
            lonslist.append(lon)
            latslist.append(lat)
            heights.append(height)
    # Mesh a grid with lons, lats
    glons, glats = pl.meshgrid(lons, lats)

    # For each calculator, map the integrand and plot
    index = [1,2,3,5,6,9]
    for calc, name, i in zip(*[calculators, names, index]):
        # Calculate the field
        field = calc.calculate(model, lonslist, latslist, heights)

        # Convert the field to a matrix
        fieldlist = []
        for j in range(len(lons),len(field)+1,len(lons)):
            fieldlist.append(field[j-len(lons):j])
        fieldgrd = np.array(fieldlist)

        # Plot the integrand
        pl.subplot(3,3,i)
        #pl.title(name)
        #pl.xlabel('Longitude')
        #pl.ylabel('Latitude')
        pc = pl.pcolor(glons, glats, fieldgrd, cmap=pl.cm.jet)
        pl.colorbar(orientation='vertical', format='%.1e')#,aspect=50)
        pl.plot(tesslons,tesslats,'-w',linewidth=1)
        pl.xlim(-20, 20)
        pl.ylim(25, 65)
        
    filename = folder + os.path.sep + 'ggt-h%g-o%d.png' % \
                 (height/1000.0, order)
    pl.savefig(filename, fmt='png')

    # Make a second plot with the points of the profiles
    for i in index:
        pl.subplot(3,3,i)
        pl.plot(prof_lons, [45]*len(prof_lats), 'k,')
        pl.plot([0]*len(prof_lons), prof_lats, 'k,')
        pl.xlim(-20, 20)
        pl.ylim(25, 65)
    filename = folder + os.path.sep + 'ggt-h%g-o%d-prof.png' % \
                 (height/1000.0, order)
    pl.savefig(filename, fmt='png')
    pl.close()

if __name__ == '__main__':


    heights = [0, 1000, 10000, 20000, 30000, 50000, 100000, 150000, 250000]
    lons = [-20, -15, -10, -5, -4, -3, -2, -1.9, -1.8, -1.7, \
            -1.6, -1.5, -1.4, -1.3, -1.2, -1.1, -1, -0.9, -0.8, -0.7, \
            -0.6, -0.5, -0.4, -0.3, -0.2, -0.1, 0, 0.1, 0.2, 0.3, 0.4, 0.5, \
            0.6, 0.7, 0.8, 0.9, 1, 1.1, 1.2, 1.3, 1.4, 1.5, \
            1.6, 1.7, 1.8, 1.9, 2, 3, 4, 5, 10, 15, 20]
    lats = [25, 30, 35, 40, 41, 42, 43, 43.1, 43.2, 43.3, 43.4, 43.5, \
            43.6, 43.7, 43.8, 43.9, 44, 44.1, 44.2, 44.3, 44.4, 44.5, \
            44.6, 44.7, 44.8, 44.9, 45, 45.1, 45.2, 45.3, 45.4, 45.5, 45.6, \
            45.7, 45.8, 45.9, 46, 46.1, 46.2, 46.3, 46.4, 46.5, 46.6, \
            46.7, 46.8, 46.9, 47, 48, 49, 50, 55, 60, 65]
    order = 5
    for height in heights:
        print "Height: %g km" % (height/1000.0)
        print "  Creating dir"
        folder = 'heigh-%g' % (height/1000.0)
        # Check if folder exists
        if not os.path.exists(folder):
            os.mkdir(folder)

        # Draw the map of the field
        print "  Drawing map"
        drawmap(height, folder, lons, lats)

        print "  Latitude profile"
        subfolder = 'lat-profile'
        # Check if it exists
        if not os.path.exists(folder+os.path.sep+subfolder):
            os.mkdir(folder+os.path.sep+subfolder)
        lat = 45
        for lon in lons:
            print "    lat: %g  lon: %g" % (lat, lon)
            mapint(lon, lat, height, order, folder+os.path.sep+subfolder)

        print "  Longitude profile"
        subfolder = 'lon-profile'
        # Check if it exists
        if not os.path.exists(folder+os.path.sep+subfolder):
            os.mkdir(folder+os.path.sep+subfolder)
        lon = 0
        for lat in lats:
            print "    lon: %g  lat: %g" % (lon, lat)
            mapint(lon, lat, height, order, folder+os.path.sep+subfolder)
    