"""
Plot the columns of the output files
"""
import sys
import pylab

data = pylab.loadtxt(sys.argv[1], unpack=True)
shape = (int(sys.argv[2]), int(sys.argv[3]))
lon = pylab.reshape(data[0], shape)*0.001
lat = pylab.reshape(data[1], shape)*0.001
xmin, xmax = lon.min(), lon.max()
ymin, ymax = lat.min(), lat.max()
for i, value in enumerate(data[3:]):
    value = pylab.reshape(value, shape)
    pylab.figure(figsize=(4, 3))
    pylab.title("Column %d" % (i + 4))
    pylab.axis('scaled')
    pylab.pcolor(lon, lat, value)
    pylab.colorbar()
    pylab.contour(lon, lat, value, 12, color='k')
    #pylab.xlabel("Longitude")
    #pylab.ylabel("Latitude")
    pylab.xlim(xmin, xmax)
    pylab.ylim(ymin, ymax)
    pylab.savefig('column%d.png' % (i + 4))
pylab.show()
