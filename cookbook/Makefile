# Run all the examples and clean up after them

.PHONY: simple_tess simple_prism tess2prism_flatten dem_brasil

all: simple_tess simple_prism tess2prism_flatten

dem_brasil:
	cd dem_brasil; bash dem_brasil.sh; python plot.py

tess2prism_flatten:
	cd tess2prism_flatten; bash tess2prism_flatten.sh; python plot.py output.txt 50 50

simple_tess:
	cd simple_tess; bash simple_tess.sh; python plot.py output.txt 50 50
	
simple_prism:
	cd simple_prism; bash simple_prism.sh; python plot.py output.txt 50 50

clean: clean_simple_tess clean_simple_prism clean_tess2prism_flatten clean_dem_brasil

clean_simple_tess:
	cd simple_tess; rm -f output.txt column*.png
	
clean_simple_prism:
	cd simple_prism; rm -f output.txt column*.png
	
clean_tess2prism_flatten:
	cd tess2prism_flatten; rm -f output.txt column*.png prism-model.txt

clean_dem_brasil:
	cd dem_brasil; rm -f dem-ggt.txt dem-dens.txt *.log dem.png dem-dens.png \
	dem-ggt.png dem-tess.txt
