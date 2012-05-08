#!/bin/bash

# Generate a regular grid, pipe it to all the computation programs,
# and write the result to output.txt

tessgrd -r-20/20/-20/20 -b50/50 -z250e03 | \
tesspot model.txt | \
tessgx model.txt | tessgy model.txt | tessgz model.txt | \
tessgxx model.txt  | tessgxy model.txt  | tessgxz model.txt  | \
tessgyy model.txt  | tessgyz model.txt  | tessgzz model.txt > output.txt
