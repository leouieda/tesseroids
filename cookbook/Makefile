# Run all the examples and clean up after them

.PHONY: simple_tess simple_prism tess2prism_flatten

all: simple_tess simple_prism prism_flat_tess

tess2prism_flatten:
	@cd tess2prism_flatten; bash tess2prism_flatten.sh; python plot.py output.txt 50 50

simple_tess:
	@cd simple_tess; bash simple_tess.sh; python plot.py output.txt 50 50
	
simple_prism:
	@cd simple_prism; bash simple_prism.sh; python plot.py output.txt 50 50

clean: clean_simple_tess clean_simple_prism clean_tess2prism_flatten

clean_simple_tess:
	cd simple_tess; rm -f output.txt column*.png
	
clean_simple_prism:
	cd simple_prism; rm -f output.txt column*.png
	
clean_tess2prism_flatten:
	cd tess2prism_flatten; rm -f output.txt column*.png prism-model.txt
