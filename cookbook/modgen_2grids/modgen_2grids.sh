#!/bin/bash

# Generate the tesseroid model with density of 500 kg/m3
tessmodgen -s1/1 -d500 < grids.txt > model.txt

# Now calculate the gz effect of this model
tessgrd -r-5/5/-5/5 -b50/50 -z250e3 | \
tessgz model.txt -v > output.txt
