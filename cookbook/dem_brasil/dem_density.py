"""
Assign density values for the DEM points.
"""
import sys
import numpy

lons, lats, heights = numpy.loadtxt(sys.argv[1], unpack=True)

for i in xrange(len(heights)):
    if heights[i] >=0:
        print "%lf %lf %lf %lf" % (lons[i], lats[i], heights[i], 2670.0)
    else:
        print "%lf %lf %lf %lf" % (lons[i], lats[i], heights[i], 1670.0)
