"""
Plot the columns of the output files
"""
import sys
import pylab

data = pylab.loadtxt(sys.argv[1], unpack=True)
shape = (int(sys.argv[2]), int(sys.argv[3]))
lon = pylab.reshape(data[0], shape)
lat = pylab.reshape(data[1], shape)
for i, value in enumerate(data[3:]):
    value = pylab.reshape(value, shape)
    pylab.figure(figsize=(4, 3))
    pylab.axis('scaled')
    pylab.title("Column %d" % (i + 4))
    pylab.pcolor(lon, lat, value)
    pylab.colorbar()
    pylab.xlim(lon.min(), lon.max())
    pylab.ylim(lat.min(), lat.max())
    pylab.savefig('column%d.png' % (i + 4))
