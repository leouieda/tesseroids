#!/bin/bash

# First, insert the density information into the DEM file using a Python script
python dem_density.py dem-10min.xyz > dem-10min-dens.xyz

# Now, use tessmodgen to create a tesseroid model
tessmodgen -v -s0.166666667/0.166666667 -z0 < dem-10min-dens.xyz > dem-10min-mod.txt

# Calculate the GGT
tessgrd -r-60/-45/-30/-15 -b50/50 -z250e03 | \
tessgxx dem-10min-mod.txt -lgxx.log | tessgxy dem-10min-mod.txt -lgxy.log | \
tessgxz dem-10min-mod.txt -lgxz.log | tessgyy dem-10min-mod.txt -lgyy.log | \
tessgyz dem-10min-mod.txt -lgyz.log | tessgzz dem-10min-mod.txt -lgzz.log > dem-10min-ggt.xyz

# Use another Python script to make some nice plots
python mkplots.py