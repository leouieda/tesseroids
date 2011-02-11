#!/bin/bash


echo "set size ratio -1; set view map; splot '"$1"' using 1:2:"$2" with pm3d" | gnuplot -persist
