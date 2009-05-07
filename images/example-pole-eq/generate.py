import sys
import os
import scipy as s
import pylab as p
import logging
from mpl_toolkits.basemap import Basemap

# Get the path of where the program is
mypath = os.path.abspath(os.path.dirname(sys.argv[0]))
# Get the path os the mods
tmp = mypath + os.path.sep + os.path.pardir + os.path.sep + 'python' + os.path.sep + 'mods'
modspath = os.path.abspath(tmp)
# Add this path to the sys.path so that the modules can be imported
sys.path.append(modspath)
# Now the mod can be imported
import glq
import tesseroid as t
import tesseroidgravity as tg

order = 3
height = 250000

# Create a logger for stokes
logger = logging.getLogger()
logger.setLevel(logging.DEBUG)
# create console handler and set level to debug
ch = logging.StreamHandler(strm=sys.stderr)
ch.setLevel(logging.DEBUG)
# create formatter
formatter = logging.Formatter("%(levelname)s: %(message)s")
# add formatter to ch
ch.setFormatter(formatter)
# add ch to logger
logger.addHandler(ch)


# Make the abscissas and weights
print "Calculating abscissas and weights..."
ablon = glq.Abscissas(order)
ablat = glq.Abscissas(order)
abr = glq.Abscissas(order)
wlon = glq.Weights(ablon)
wlat = glq.Weights(ablat)
wr = glq.Weights(abr)

# Make the field calculator obejects
print "Creating calculator objects..."
calculators = []
tesspot = tg.TesseroidPotential(ablon, wlon, ablat, wlat, abr, wr)
tessgx = tg.TesseroidGx(ablon, wlon, ablat, wlat, abr, wr)
tessgy = tg.TesseroidGy(ablon, wlon, ablat, wlat, abr, wr)
tessgz = tg.TesseroidGz(ablon, wlon, ablat, wlat, abr, wr)
tessgxx = tg.TesseroidGxx(ablon, wlon, ablat, wlat, abr, wr)
tessgxy = tg.TesseroidGxy(ablon, wlon, ablat, wlat, abr, wr)
tessgxz = tg.TesseroidGxz(ablon, wlon, ablat, wlat, abr, wr)
tessgyy = tg.TesseroidGyy(ablon, wlon, ablat, wlat, abr, wr)
tessgyz = tg.TesseroidGyz(ablon, wlon, ablat, wlat, abr, wr)
tessgzz = tg.TesseroidGzz(ablon, wlon, ablat, wlat, abr, wr)
calculators.append(tesspot)
calculators.append(tessgx)
calculators.append(tessgy)
calculators.append(tessgz)
calculators.append(tessgxx)
calculators.append(tessgxy)
calculators.append(tessgxz)
calculators.append(tessgyy)
calculators.append(tessgyz)
calculators.append(tessgzz)
names = []
names.append('Potential')
names.append('Gx')
names.append('Gy')
names.append('Gz')
names.append('Gxx')
names.append('Gxy')
names.append('Gxz')
names.append('Gyy')
names.append('Gyz')
names.append('Gzz')

################################################################################
print "Close to Equator:"
# Make the model
print "    Creating models..."
tesseq = t.Tesseroid(19, 21, -1, 1, 0, 30000, 0.3, 'Equator')
modeleq = [tesseq]
tesseqlons = [tesseq['w'], tesseq['e'], tesseq['e'], tesseq['w'], tesseq['w']]
tesseqlats = [tesseq['s'], tesseq['s'], tesseq['n'], tesseq['n'], tesseq['s']]

# Make the grid
print "    Creating grid..."
lons = s.arange(15, 25.1, 0.1)
lats = s.arange(-5, 5.1, 0.1)
hs = []
lns = []
lts = []
for lat in lats:
    for lon in lons:
        lns.append(lon)
        lts.append(lat)
        hs.append(height)

# Calculate the fields
print "    Calculating fields..."
fields = []
for calculator, name in zip(*[calculators, names]):
    print "      %s..." % (name)
    fields.append(calculator.calculate(modeleq, lns, lts, hs))

print "    Creating Basemap class..."
# Create a Basemap class to do the projections
# Use Mercator projection
bm = Basemap(projection='merc',llcrnrlat=-5,urcrnrlat=5,\
             llcrnrlon=15,urcrnrlon=25,lat_ts=0)

print "  Converting lat and lon to projection coordinates..."
# Convert the grid to the projection coordinates.
longrid, latgrid = bm(*p.meshgrid(lons, lats))
tesseqlons, tesseqlats = bm(tesseqlons, tesseqlats)

# Plot the maps
print "    Making the maps..."
for field, name in zip(*[fields, names]):

    print "      %s..." % (name)

    fieldlist = []
    for i in range(len(lons),len(field)+1,len(lons)):
        fieldlist.append(field[i-len(lons):i]+[field[i-len(lons)]])
    fieldgrd = s.array(fieldlist)

    p.figure(figsize=(10,10))
    #p.title(name)

    #print "        Drawing coastlines and countries..."
    ## Draw coastlines, etc
    #bm.drawcoastlines(linewidth=0.3)
    #bm.fillcontinents(color='coral')
    #bm.drawmapboundary()
    #bm.bluemarble()
    #bm.drawcountries(linewidth=0.2)
    #bm.drawstates(linewidth=0.8)

    print "        Drawing meridians and parallels..."
    # Draw Meridians
    bm.drawmeridians(p.arange(17,25.1,2),labels=[0,0,0,1])
    bm.drawparallels(p.arange(-5,5,2),labels=[1,0,0,0])

    print "        Drawing map..."
    # Plot the tesseroids with 'white'
    p.plot(tesseqlons, tesseqlats, '-w', linewidth=1.5)
    PC = bm.pcolor(longrid, latgrid, fieldgrd, cmap=p.cm.jet)
    p.colorbar(PC, orientation='vertical', aspect=50, shrink=0.8)

    print "        Saving the image..."
    p.savefig('Equator-'+name+".png",format='png')

################################################################################
print "Close to Pole:"
# Make the model
print "    Creating models..."
tessp = t.Tesseroid(18, 22, -85.5, -84.5, 0, 30000, 0.3, 'Pole')
modelp = [tessp]
tessplons = [tessp['w'], tessp['e'], tessp['e'], tessp['w'], tessp['w']]
tessplats = [tessp['s'], tessp['s'], tessp['n'], tessp['n'], tessp['s']]

# Make the grid
print "    Creating grid..."
lons = s.arange(-20, 60.1, 0.5)
lats = s.arange(-89, -80.1, 0.1)
hs = []
lns = []
lts = []
for lat in lats:
    for lon in lons:
        lns.append(lon)
        lts.append(lat)
        hs.append(height)

# Calculate the fields
print "    Calculating fields..."
fields = []
for calculator, name in zip(*[calculators, names]):
    print "      %s..." % (name)
    fields.append(calculator.calculate(modelp, lns, lts, hs))

print "    Creating Basemap class..."
# Create a Basemap class to do the projections
bm = Basemap(projection='laea',\
             width=1500000, height=1200000, \
             resolution='l',lat_0=-85,lon_0=20)

print "  Converting lat and lon to projection coordinates..."
# Convert the grid to the projection coordinates.
longrid, latgrid = bm(*p.meshgrid(lons, lats))
tessplons, tessplats = bm(tessplons, tessplats)

# Plot the maps
print "    Making the maps..."
for field, name in zip(*[fields, names]):

    print "      %s..." % (name)

    fieldlist = []
    for i in range(len(lons),len(field)+1,len(lons)):
        fieldlist.append(field[i-len(lons):i]+[field[i-len(lons)]])
    fieldgrd = s.array(fieldlist)

    p.figure(figsize=(10,10))
    #p.title(name)

    #print "        Drawing coastlines and countries..."
    ## Draw coastlines, etc
    #bm.drawcoastlines(linewidth=0.3)
    #bm.fillcontinents(color='coral')
    #bm.drawmapboundary()
    #bm.bluemarble()
    #bm.drawcountries(linewidth=0.2)
    #bm.drawstates(linewidth=0.8)

    print "        Drawing meridians and parallels..."
    # Draw Meridians
    bm.drawparallels(s.arange(-88,-78,2),labels=[0,0,0,1])
    bm.drawmeridians(s.arange(-30,70,10),labels=[0,0,1,0])

    print "        Drawing map..."
    # Plot the tesseroids with 'white'
    p.plot(tessplons, tessplats, '-w', linewidth=1)
    PC = bm.pcolor(longrid, latgrid, fieldgrd, cmap=p.cm.jet)
    p.colorbar(PC, orientation='vertical', aspect=50, shrink=0.6)

    print "        Saving the image..."
    p.savefig('Pole-'+name+".png",format='png')
#p.show()