

:: First, insert the density information into
:: the DEM file using the Python script.
python dem_density.py dem.xyz > dem-dens.txt

:: Next, use the modified DEM with tessmodgen
:: to create a tesseroid model
tessmodgen -s0.166667/0.166667 -z0 -v < dem-dens.txt ^
> dem-tess.txt

:: Calculate the GGT on a regular grid at 250km
:: use the -l option to log the processes to files
:: (usefull to diagnose when things go wrong)
:: The output is dumped to dem-ggt.txt
tessgrd -r-60/-45/-30/-15 -b50/50 -z250e03 | ^
tessgxx dem-tess.txt -lgxx.log | ^
tessgxy dem-tess.txt -lgxy.log | ^
tessgxz dem-tess.txt -lgxz.log | ^
tessgyy dem-tess.txt -lgyy.log | ^
tessgyz dem-tess.txt -lgyz.log | ^
tessgzz dem-tess.txt -lgzz.log -v > dem-ggt.txt
