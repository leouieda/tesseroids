# Benchmark for calculating the effect of 6.25 million tesseroids on 100 points

run: result.txt

model.txt:
	@echo "Generate the model"
	@../bin/tessgrd -r-10/10/-10/10 -b2501/2501 -z0 | \
	../bin/tessmodgen -s0.008/0.008 -d1000 -z-40000 > model.txt

result.txt: model.txt
	@echo "Print the computer specs to result.txt"
	@echo "Computer specs" > result.txt
	@cat /proc/cpuinfo | grep -m 1 -i -E 'model name' >> result.txt
	@cat /proc/meminfo | grep -m 1 -i -E 'MemTotal' >> result.txt
	@cat result.txt
	@echo ""
	@echo "Run the tessgzz program"
	@echo "" >> result.txt
	@echo "Time spent" >> result.txt
	@../bin/tessgrd -r-10/10/-10/10 -b10/10 -z250000 > grid.txt
	@/usr/bin/time -o result.txt -a \
	../bin/tessgzz model.txt -v < grid.txt > tmp.txt

clean:
	rm model.txt tmp.txt grid.txt
