#!/bin/bash

# Convert the layer grids in layers.txt to tesseroids.
# The grid spacing passed to -s is used as the size of the tesseroids,
# so be careful!
tesslayers -s1/1 -v < layers.txt > tessmodel.txt

# Now calculate the gz and gzz effect of this model at 100km height
tessgrd -r-8/8/32/48 -b50/50 -z100000 | \
tessgzz tessmodel.txt -v > tesseffect.txt
