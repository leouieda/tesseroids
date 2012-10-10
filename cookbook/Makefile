# Run all the examples and clean up after them

.PHONY: simple_tess simple_prism tess2prism_flatten dem_brasil tess2prism \
	tesslayers custom_ratio

all: simple_tess simple_prism tess2prism_flatten tess2prism tesslayers

dem_brasil:
	cd dem_brasil; bash dem_brasil.sh; python plot.py

tess2prism_flatten:
	cd tess2prism_flatten; bash tess2prism_flatten.sh; python plot.py output.txt 50 50

tess2prism:
	cd tess2prism; bash tess2prism.sh; python plot.py output.txt 100 100

simple_tess:
	cd simple_tess; bash simple_tess.sh; python plot.py output.txt 50 50

custom_ratio:
	cd custom_ratio; bash custom_ratio.sh; python plot.py output.txt 50 50

simple_prism:
	cd simple_prism; bash simple_prism.sh; python plot.py output.txt 50 50

tesslayers:
	cd tesslayers; bash tesslayers.sh; python plot.py output.txt 50 50

clean:
	@cd simple_tess; rm -f output.txt column*.png log.txt
	@cd custom_ratio; rm -f output.txt column*.png *.log
	@cd simple_prism; rm -f output.txt column*.png
	@cd tess2prism_flatten; rm -f output.txt column*.png prism-model.txt
	@cd tess2prism; rm -f output.txt column*.png prism-model.txt
	@cd tesslayers; rm -f output.txt column*.png tessmodel.txt depth.png \
	thickness.png topography.png
	@cd dem_brasil; rm -f dem-ggt.txt dem-dens.txt *.log dem.png dem-dens.png \
	dem-ggt.png dem-tess.txt
