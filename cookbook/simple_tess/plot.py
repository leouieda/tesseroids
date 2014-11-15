"""
Plot the columns of the output files
"""
import sys
from matplotlib import pyplot as plt
from mpl_toolkits.basemap import Basemap
import numpy as np

# Set up a projection
bm = Basemap(projection='ortho', lon_0=0, lat_0=0,
             resolution='l', area_thresh=10000)

# Load the data and make them into matrices
data = np.loadtxt(sys.argv[1], unpack=True)
shape = (int(sys.argv[2]), int(sys.argv[3]))
lon = data[0].reshape(shape)
lat = data[1].reshape(shape)
glon, glat = bm(lon, lat)

plt.figure(figsize=(14, 12))
for i, value in enumerate(data[3:]):
    plt.subplot(3, 4, i + 1)
    plt.title("Column %d" % (i + 4))
    bm.drawcoastlines()
    bm.drawmapboundary()
    bm.contourf(glon, glat, value.reshape(shape), 15, cmap=plt.cm.RdBu_r)
    plt.colorbar(orientation="horizontal", pad=0, aspect=30)
plt.tight_layout()
plt.savefig('output.png')
