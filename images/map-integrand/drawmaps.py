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
DrawMaps:
    Make some sample maps at varying heights and GLQ orders to see when it fails
"""
################################################################################
# Created on 12-May-2009 15:41 PM
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
import time
import logging

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

def breaktess(tess, nlon, nlat, nr):
    """
    Break the tesseroid into nlon x nlat x nr pieces.
    """
    dlon = (tess['e'] - tess['w'])/(nlon)
    dlat = (tess['n'] - tess['s'])/(nlat)
    dr = (tess['bottom'] - tess['top'])/(nr)

    ws = np.arange(tess['w'], tess['e'], dlon)
    es = np.arange(tess['w']+dlon, tess['e']+dlon, dlon)
    ss = np.arange(tess['s'], tess['n'], dlat)
    ns = np.arange(tess['s']+dlat, tess['n']+dlat, dlat)
    tops = np.arange(tess['top'], tess['bottom'], dr)
    bottoms = np.arange(tess['top']+dr, tess['bottom']+dr, dr)

    density = tess['density']
    model = []
    i = 1
    for top, bottom in zip(*[tops, bottoms]):
        for w, e in zip(*[ws, es]):
            for s, n in zip(*[ss, ns]):
                tess = ts.Tesseroid(w, e, s, n, top, bottom, density, '%d' % (i))
                model.append(tess)
                i += 1

    return model


def drawmaps(model, lons, lats, height, order, folder):
    """
    Draw the tensor maps at height and save them in folder.
    """

    deg2rad = math.pi/180.0
    rad2deg = 180.0/math.pi

    # Create a logger for the prog
    logger = logging.getLogger()
    logger.setLevel(logging.DEBUG)
    # Create console handler and set level to debug
    ch = logging.StreamHandler(strm=sys.stderr)
    # Create formatter
    formatter = logging.Formatter("\n%(name)-20s %(levelname)-8s: %(message)s")
    # Add formatter to ch
    ch.setFormatter(formatter)
    ch.setLevel(logging.DEBUG)
    # Add ch to logger
    logger.addHandler(ch)

    # Create a calculation grid
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

    names = []
    names.append("Gxx")
    names.append("Gxy")
    names.append("Gxz")
    names.append("Gyy")
    names.append("Gyz")
    names.append("Gzz")

    # Create the abscissas and weights
    abslon = glq.Abscissas(order)
    abslat = glq.Abscissas(order)
    absr = glq.Abscissas(order)
    wlon = glq.Weights(abslon)
    wlat = glq.Weights(abslat)
    wr = glq.Weights(absr)
      
    # Create a calculator class for the integrands of the GGT
    calculators = []
    tessgxx = tg.TesseroidGxx(abslon, wlon, abslat, wlat, absr, wr)
    calculators.append(tessgxx)
    tessgxy = tg.TesseroidGxy(abslon, wlon, abslat, wlat, absr, wr)
    calculators.append(tessgxy)
    tessgxz = tg.TesseroidGxz(abslon, wlon, abslat, wlat, absr, wr)
    calculators.append(tessgxz)
    tessgyy = tg.TesseroidGyy(abslon, wlon, abslat, wlat, absr, wr)
    calculators.append(tessgyy)
    tessgyz = tg.TesseroidGyz(abslon, wlon, abslat, wlat, absr, wr)
    calculators.append(tessgyz)
    tessgzz = tg.TesseroidGzz(abslon, wlon, abslat, wlat, absr, wr)
    calculators.append(tessgzz)

    # Calculate the average distances between nodes
    print "\nCalculating distance between nodes:"
    for tess in model:
    #for tess in [model[0]]:
        print "  Tesseroid %s:" % (tess['tag'])
        abslon.scale(tess['w'], tess['e'])
        abslat.scale(tess['s'], tess['n'])
        R = tessgxx.R - (tess['bottom'] - tess['top'])/2.0
        londists = []
        latdists = []
        for i in range(0, len(abslon)-1):
            londists.append(abs(R*deg2rad*abslon[i+1] - \
                            R*deg2rad*abslon[i]))
            latdists.append(abs(R*deg2rad*abslat[i+1] - \
                            R*deg2rad*abslat[i]))
        londists = np.array(londists)
        latdists = np.array(latdists)
        print "    Longitude nodes:"
        print "      Min: %f km" % (londists.min()/1000.0)
        print "      Max: %f km" % (londists.max()/1000.0)
        print "      Mean: %f km" % (londists.mean()/1000.0)
        print "    Latitude nodes:"
        print "      Min: %f km" % (latdists.min()/1000.0)
        print "      Max: %f km" % (latdists.max()/1000.0)
        print "      Mean: %f km\n" % (latdists.mean()/1000.0)

    print "Calculating GGT at %f km" % (height/1000.0)
    # Create a figure
    pl.figure(figsize=(15,10))
    pl.suptitle("Gradiente da Gravidade a %g km (Ordem:%d)" \
                % (height/1000.0, order), \
                fontsize=16)

    # For each calculator, calculate the field and plot the map
    index = [1,2,3,5,6,9]
    ggt = {}
    totaltime = 0
    for calc, name, i in zip(*[calculators, names, index]):
        print "  %s" % (name)

        fieldfile = folder + os.path.sep + '%s-h%g-o%d.txt' % \
                                            (name, height/1000.0, order)
        if os.path.exists(fieldfile):
        #if False:
            fieldgrd = pl.load(fieldfile)
        else:
            # Calculate the field
            start = time.clock()
            field = calc.calculate(model, lonslist, latslist, heights)
            end = time.clock()
            deltat = end - start
            totaltime += deltat
            print "    Time it took (s): %f\n" % (deltat)

            # Convert the field to a matrix
            fieldlist = []
            for j in range(len(lons),len(field)+1,len(lons)):
                fieldlist.append(field[j-len(lons):j])
            # Make a Numpy array
            fieldgrd = np.array(fieldlist)

            # Save the data
            pl.save(fieldfile, fieldgrd)
        ggt[name] = fieldgrd
        # Plot the integrand
        pl.subplot(3,3,i)
        #pl.title(name)
        #pl.xlabel('Longitude')
        #pl.ylabel('Latitude')
        pc = pl.pcolor(glons, glats, fieldgrd, cmap=pl.cm.jet)
        pl.colorbar(orientation='vertical', format='%g')

        ## Plot the tesseroids
        #for tess in model:
            #tesslons = [tess['w'], tess['e'], tess['e'], tess['w'], tess['w']]
            #tesslats = [tess['s'], tess['s'], tess['n'], tess['n'], tess['s']]
            #pl.plot(tesslons, tesslats, 'w-', linewidth=1)
            ## Plot the abscissas
            #abslon.scale(tess['w'], tess['e'])
            #abslat.scale(tess['s'], tess['n'])
            #for alon in abslon._val:
                #pl.plot([alon]*len(abslat._val), abslat._val, 'k+')

        pl.xlim(lons[0], lons[-1])
        pl.ylim(lats[0], lats[-1])

    filename = folder + os.path.sep + 'ggt-h%g-o%d.png' % \
                (height/1000.0, order)
    pl.savefig(filename, fmt='png')
    print "Total time (s): %f\n" % (totaltime)
    #pl.show()
    pl.close()

if __name__ == '__main__':

    # Create the model
    tess = ts.Tesseroid(-0.5, 0.5, 44.5, 45.5, 0, 10000, 200, '1')
    model = breaktess(tess, 1, 1, 1)

    # Computaion grid
    w = -3
    e = 3
    dlon = (e-w)/100.0
    s = 42
    n = 48
    dlat = (n-s)/100.0
    lons = np.arange(w, e + dlon, dlon)
    lats = np.arange(s, n + dlat, dlat)

    height = float(sys.argv[1])*1000
    order = int(sys.argv[2])
    folder = 'model-1x1'

    # Check if folder exists
    if not os.path.exists(folder):
        print "  Creating dir"
        os.mkdir(folder)

    # Draw the map of the field
    drawmaps(model, lons, lats, height, order, folder)