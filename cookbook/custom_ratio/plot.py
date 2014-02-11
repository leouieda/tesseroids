"""
Plot the columns of the output files
"""
import sys
from matplotlib import pyplot as plt
import numpy as np

data = np.loadtxt(sys.argv[1], unpack=True)
shape = (int(sys.argv[2]), int(sys.argv[3]))
lon = np.reshape(data[0], shape)
lat = np.reshape(data[1], shape)
for i, value in enumerate(data[3:]):
    value = np.reshape(value, shape)
    plt.figure(figsize=(4, 3))
    plt.title("Column %d" % (i + 4))
    plt.contourf(lon, lat, value, 50)
    plt.colorbar()
    plt.savefig('column%d.png' % (i + 4))
