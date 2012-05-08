#!/bin/bash

# Generate a prism model from a tesseroid model by
# flattening the tesseroids (1 degree = 111.11 km).
# This way the converted prisms can be used
# with the prism* programs in Cartesian coordinates.

tess2prism --flatten < tess-model.txt > prism-model.txt

# Generate a regular grid in Cartesian coordinates,
# pipe the grid to the computation programs,
# and dump the result on output.txt

tessgrd -r-3e06/3e06/-3e06/3e06 -b50/50 -z250e03 | \
prismpot prism-model.txt | \
prismgx prism-model.txt | \
prismgy prism-model.txt | \
prismgz prism-model.txt | \
prismgxx prism-model.txt  | prismgxy prism-model.txt  | \
prismgxz prism-model.txt  | prismgyy prism-model.txt  | \
prismgyz prism-model.txt  | prismgzz prism-model.txt > output.txt
