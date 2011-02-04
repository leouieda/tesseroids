#!/bin/env python
"""
Generate a large model of tesseroids to test
"""

import numpy

dlon = 0.1
lons = numpy.arange(-10, 10, dlon)
dlat = 0.1
lats = numpy.arange(-5, 5, dlat)

top = -1000
bot = 10000
dens = 500

for lat in lats:
    for lon in lons:
        print "%f %f %f %f %f %f %f" % (lon, lon+dlon, lat, lat+dlat, top, bot,
                                        dens)