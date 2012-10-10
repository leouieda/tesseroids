#!/bin/bash

# Calculate effect of the model at a low height using difference distance-size
# ratios for the recursive division of tesseroids.
# WARNING: This is only an example. You should not use the -t option in practice

tessgrd -r-3/3/-3/3 -b50/50 -z4e03 | \
tessgzz model.txt -t0.0001 -lratio1.log | \
tessgzz model.txt -t0.5 -lratio2.log | \
tessgzz model.txt -t1 -lratio3.log | \
tessgzz model.txt -v -lratio-default.log > output.txt
