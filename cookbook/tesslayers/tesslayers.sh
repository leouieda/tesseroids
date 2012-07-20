#!/bin/bash

# Convert the layer grids in layers.txt to tesseroids.
# The grid spacing passed to -s is used as the size of the tesseroids,
# so be careful!
tesslayers -s0.5/0.5 -v < layers.txt > tessmodel.txt

# Now calculate the gz and tensor effect of this model at 100km height
tessgrd -r-8/8/32/48 -b50/50 -z100000 | \
tessgz tessmodel.txt | \
tessgxx tessmodel.txt | tessgxy tessmodel.txt | \
tessgxz tessmodel.txt | tessgyy tessmodel.txt | \
tessgyz tessmodel.txt | tessgzz tessmodel.txt -v > output.txt
