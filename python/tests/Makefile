OPTIONS=sample.mod -r -5 5 -5 5 -b 0.1 0.1 -e 250000
NLAT=101

test: unittest doctest sample

unittest:
	python test_all.py -v

doctest:
	python ../mods/glq.py
	python ../mods/tesseroid.py

sample:
	python ../tesspot.py ${OPTIONS} -l pot.log > pot.dat
	python plot.py pot.dat ${NLAT}
	python ../tessgx.py ${OPTIONS} -l gx.log > gx.dat
	python plot.py gx.dat ${NLAT}
	python ../tessgy.py ${OPTIONS} -l gy.log > gy.dat
	python plot.py gy.dat ${NLAT}
	python ../tessgz.py ${OPTIONS} -l gz.log > gz.dat
	python plot.py gz.dat ${NLAT}
	python ../tessgxx.py ${OPTIONS} -l gxx.log > gxx.dat
	python plot.py gxx.dat ${NLAT}
	python ../tessgxy.py ${OPTIONS} -l gxy.log > gxy.dat
	python plot.py gxy.dat ${NLAT}
	python ../tessgxz.py ${OPTIONS} -l gxz.log > gxz.dat
	python plot.py gxz.dat ${NLAT}
	python ../tessgyy.py ${OPTIONS} -l gyy.log > gyy.dat
	python plot.py gyy.dat ${NLAT}
	python ../tessgyz.py ${OPTIONS} -l gyz.log > gyz.dat
	python plot.py gyz.dat ${NLAT}
	python ../tessgzz.py ${OPTIONS} -l gzz.log > gzz.dat
	python plot.py gzz.dat ${NLAT}

clean:
	rm *.png
	rm *.dat
	rm *.log