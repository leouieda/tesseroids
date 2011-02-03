#!/bin/bash

echo "set view map; splot 'data.txt' with pm3d" | gnuplot -persist
