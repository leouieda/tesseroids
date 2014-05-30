# Benchmark for calculating the effect of a tesseroid model

run: result.txt

model.txt:
	@echo "Generate the model"
	@../bin/tessgrd -r-10/10/-10/10 -b101/101 -z0 | \
	../bin/tessmodgen -s0.2/0.2 -d1000 -z-40000 > model.txt

result.txt: model.txt
	@echo "Computer specs" > result.txt
	@cat /proc/cpuinfo | grep -m 1 -i -E 'model name' >> result.txt
	@cat /proc/meminfo | grep -m 1 -i -E 'MemTotal' >> result.txt
	@echo ""
	@echo "Run the tesspot program"
	@echo "" >> result.txt
	@echo "Time spent on potential:" >> result.txt
	@../bin/tessgrd -r-10/10/-10/10 -b50/50 -z5000 > grid.txt
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
	rm model.txt tmp.txt grid.txt
