#!/bin/bash


tessgrd -r-10/10/-10/10 -b100/100 -z250e03 | tessgz model.txt -a | \
tessgxx model.txt -a | tessgxy model.txt -a | tessgxz model.txt -a | \
tessgyy model.txt -a | tessgyz model.txt -a | tessgzz model.txt -a > output.txt