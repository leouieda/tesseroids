"""
Plot the columns of the output files
"""
import sys
import pylab
from mpl_toolkits.basemap import Basemap

# Set up a projection
bm = Basemap(projection='ortho', lon_0=-80, lat_0=-40,
             resolution='l', area_thresh=10000)

data = pylab.loadtxt(sys.argv[1], unpack=True)
shape = (int(sys.argv[2]), int(sys.argv[3]))
lon = pylab.reshape(data[0], shape)
lat = pylab.reshape(data[1], shape)
glon, glat = bm(lon, lat)

for i, value in enumerate(data[3:]):
    value = pylab.reshape(value, shape)
    pylab.figure(figsize=(4, 3))
    pylab.title("Column %d" % (i + 4))
    bm.drawcoastlines()
    #bm.fillcontinents(color='coral',lake_color='aqua')
    #bm.drawmapboundary(fill_color='aqua')
    bm.drawmapboundary()
    bm.drawparallels(pylab.arange(-90.,120.,30.))
    bm.drawmeridians(pylab.arange(0.,420.,60.))
    #bm.bluemarble()
    bm.pcolor(glon, glat, value)
    pylab.colorbar()
    #bm.contour(glon, glat, value, 12, linewidth=3)
    pylab.savefig('column%d.png' % (i + 4))
pylab.show()
