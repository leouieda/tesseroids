#!/bin/bash


echo "set view map; splot 'data.txt' using 1:2:"$1" with pm3d" | gnuplot -persist
