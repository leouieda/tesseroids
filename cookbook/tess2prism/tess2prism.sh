#!/bin/bash

# Generate a prism model from a tesseroid model.
# Prisms will have the same mass as the tesseroids and
# associated spherical coordinates of the center of
# the top of the tesseroid.

tess2prism < tess-model.txt > prism-model.txt

# Generate a regular grid in spherical coordinates,
# pipe the grid to the computation programs,
# and dump the result on output.txt
# prismpots calculates the potential in spherical
# coordinates, prismgs calculates the full
# gravity vector, and prismggts calculates the full
# gravity gradient tensor.
#
#tessgrd -r-3/3/-3/3 -b50/50 -z250e03 | \
#prismpots prism-model.txt | \
#prismgs prism-model.txt | \
#prismggts prism-model.txt > output.txt

tessgrd -r-3/3/-3/3 -b50/50 -z250e03 | \
prismpots prism-model.txt -v > output.txt
