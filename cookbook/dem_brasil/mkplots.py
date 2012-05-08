# Make some nice plots of the DEM, the densities used and the calculated GGT
import numpy
import pylab
from mpl_toolkits.basemap import Basemap

# Plot the DEM and density maps
################################################################################
lons, lats, heights, dens = numpy.loadtxt('dem-10min-dens.xyz', unpack=True)
nlons = 151  # Number of points in the longitude direction
nlats = len(lats)/nlons

# Convert the lists to 2D grids
glons = numpy.reshape(lons, (nlats, nlons))
glats = numpy.reshape(lats, (nlats, nlons))
gheights = numpy.reshape(heights, (nlats, nlons))
gdens = numpy.reshape(dens, (nlats, nlons))

# Set up a Mercator projection
bm = Basemap(projection='merc',
             llcrnrlon=lons[0], llcrnrlat=lats[-1],
             urcrnrlon=lons[-1], urcrnrlat=lats[0],
             lon_0=lons[nlons//2], lat_0=lats[len(lats)//2],
             resolution='l',
             area_thresh=10000)
glons, glats = bm(glons, glats)

# Plot the DEM first
print "Plotting DEM"
pylab.figure()
bm.drawmeridians(numpy.arange(lons[0]+5., lons[-1], 5.),
                 labels=[0,0,0,1], fontsize=12, linewidth=0.5)
bm.drawparallels(numpy.arange(lats[-1]+5., lats[0], 5.),
                 labels=[1,0,0,0], fontsize=12, linewidth=0.5)
bm.drawcoastlines(linewidth=1)
bm.drawmapboundary()
bm.drawcountries(linewidth=0.8)
# Do the pseudocolor plot
cf = bm.pcolor(glons, glats, gheights, cmap=pylab.cm.gist_earth, \
               vmin=-1000, vmax=1000)
cb = pylab.colorbar()
cb.set_label("Height [m]")
# Plot the calculation area used later
w = -60
e = -45
s = -30
n = -15
areax, areay = bm([w, w, e, e, w], \
                  [n, s, s, n, n])
bm.plot(areax, areay, '-r', label="Computation grid", linewidth=1.8)
pylab.legend(shadow=True, loc='lower right', prop={'size':10})
# Save a png figure
pylab.savefig('dem-10min.png')

# Now plot the densities
print "Plotting density model"
pylab.figure()
bm.drawmeridians(numpy.arange(lons[0]+5., lons[-1], 5.),
                 labels=[0,0,0,1], fontsize=12, linewidth=0.5)
bm.drawparallels(numpy.arange(lats[-1]+5., lats[0], 5.),
                 labels=[1,0,0,0], fontsize=12, linewidth=0.5)
bm.drawcoastlines(linewidth=1)
bm.drawmapboundary()
bm.drawcountries(linewidth=0.8)
# Do the pseudocolor plot
cf = bm.pcolor(glons, glats, gdens, cmap=pylab.cm.jet)
cb = pylab.colorbar()
cb.set_label(r"Density [$g.cm^{-3}$]")
# Save a png figure
pylab.savefig('dem-10min-dens.png')

# Plot the GGT
################################################################################
print "Plotting GGT"
data = numpy.loadtxt('dem-10min-ggt.xyz')
lons, lats, heights, gxx, gxy, gxz, gyy, gyz, gzz = data.T
nlons = 50  # Number of points in the longitude direction
nlats = len(lats)/nlons

# Convert the lists to 2D grids
glons = numpy.reshape(lons, (nlats, nlons))
glats = numpy.reshape(lats, (nlats, nlons))

# Set up a Mercator projection
bm = Basemap(projection='merc', \
             llcrnrlon=lons[0], llcrnrlat=lats[0], \
             urcrnrlon=lons[-1], urcrnrlat=lats[-1], \
             lon_0=lons[nlons//2], lat_0=lats[len(lats)//2],
             resolution='l', area_thresh=10000)
glons, glats = bm(glons, glats)

# Plot each component
fig = pylab.figure(figsize=(14,9))
pylab.subplots_adjust(wspace=0.35)
titles = [r"$g_{xx}$", r"$g_{xy}$", r"$g_{xz}$", r"$g_{yy}$", r"$g_{yz}$",
          r"$g_{zz}$"]
fields = [gxx, gxy, gxz, gyy, gyz, gzz]
for i, args in enumerate(zip(fields, titles)):
    field, title = args
    ax = pylab.subplot(2, 3, i + 1, aspect='equal')
    pylab.title(title, fontsize=18)
    # Make it a 2D grid
    gfield = numpy.reshape(field, (nlats, nlons))
    # Plot the coastlines and etc
    mer = bm.drawmeridians(numpy.arange(lons[0]+3, lons[-1]-3, 3),
                           labels=[0,0,0,1], fontsize=9, linewidth=0.5)
    bm.drawparallels(numpy.arange(lats[0]+3, lats[-1]-3, 3),
                     labels=[1,0,0,0], fontsize=9, linewidth=0.5)
    bm.drawcoastlines(linewidth=1)
    bm.drawmapboundary()
    bm.drawcountries(linewidth=1)
    bm.drawstates(linewidth=0.2)
    # Make a pseudocolor plot
    cf = bm.pcolor(glons, glats, gfield, cmap=pylab.cm.jet)
    cb = pylab.colorbar(orientation='vertical', format='%.2f', shrink=0.8)
    cb.set_label(r"$E\"otv\"os$")
# Save a png figure
pylab.savefig('dem-10min-ggt.png')