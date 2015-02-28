# Benchmark for calculating the effect of a tesseroid model

run: result.txt

model.txt:
	@echo "Generate the model"
	@../bin/tessgrd -r-10/10/-10/10 -b41/41 -z1000 | \
	../bin/tessmodgen -s0.5/0.5 -d2670 -z0 > model.txt

result.txt: model.txt
	@echo "Computer specs" > result.txt
	@cat /proc/cpuinfo | grep -m 1 -i -E 'model name' >> result.txt
	@cat /proc/meminfo | grep -m 1 -i -E 'MemTotal' >> result.txt
	@echo "\nTesseroids version\n" >> result.txt
	@tessgz --version >> result.txt
	@echo ""
	@../bin/tessgrd -r-8/8/-8/8 -b51/51 -z2000 > grid.txt
	@echo "\nModel size: `wc -l model.txt`" >> result.txt
	@echo "Grid size: `wc -l grid.txt`" >> result.txt
	@echo "Run the tesspot program"
	@echo "" >> result.txt
	@echo "Time spent on potential:" >> result.txt
	@/usr/bin/time -o result.txt -a \
	../bin/tesspot model.txt -v < grid.txt > tmp.txt
	@echo ""
	@echo "Run the tessgz program"
	@echo "" >> result.txt
	@echo "Time spent on gz:" >> result.txt
	@/usr/bin/time -o result.txt -a \
	../bin/tessgz model.txt -v < grid.txt > tmp.txt
	@echo ""
	@echo "Run the tessgzz program"
	@echo "" >> result.txt
	@echo "Time spent on gzz:" >> result.txt
	@/usr/bin/time -o result.txt -a \
	../bin/tessgzz model.txt -v < grid.txt > tmp.txt
	@echo ""
	@echo "Results:"
	@cat result.txt

clean:
	rm -rf model.txt tmp.txt grid.txt
