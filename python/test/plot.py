import pylab as p
import numpy as n
import sys

# Load the lat, lon, field data from file in argv[1]
filename = sys.argv[1]
print "Opening file '%s'" % (filename)
lon, lat,  field = p.load(filename, unpack=True)
lat = lat.tolist()
lon = lon.tolist()
field = field.tolist()
nlat = int(sys.argv[2])

# get the unique values from lon and lat
# so I can use it with meshgrid
print "Separating lat and lon..."
lat = n.array(lat[0:nlat])
lonlist = []
for i in range(0,len(lon),nlat):
    lonlist.append(lon[i])
lon = n.array(lonlist)
nlon = len(lon)
# field will be a matrix with the latxlon dimensions
# with the field values displayed in a grid-like matrix
fieldlist = []
for i in range(nlat,len(field)+1,nlat):
    fieldlist.append(field[i-nlat:i])
field = n.array(fieldlist).transpose()

longrid, latgrid = p.meshgrid(lon,lat)

print "Drawing map..."
PC = p.pcolor(longrid, latgrid, field, cmap=p.cm.jet)
p.colorbar(PC, orientation='horizontal',aspect=50)
p.xlim(lon[0],lon[nlon-1])
p.ylim(lat[0],lat[nlat-1])
p.savefig(filename+'.png', fmt='png')