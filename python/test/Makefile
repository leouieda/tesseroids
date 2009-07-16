OPTIONS=sample.mod -r -5 5 -5 5 -b 0.1 0.1 -e 250000
NLAT=101
TESTOUTPUT=tests-verbose.txt

all: unittest doctest sample

unittest:
	echo "" >> ${TESTOUTPUT}
	echo "UNIT TESTS:" >> ${TESTOUTPUT}
	echo "---------------------------------------" >> ${TESTOUTPUT}
	python regression.py -v 2>> ${TESTOUTPUT}

doctest:
	echo "" >> ${TESTOUTPUT}
	echo "DOC TESTS:" >> ${TESTOUTPUT}
	echo "---------------------------------------" >> ${TESTOUTPUT}
	python ../glq.py >> ${TESTOUTPUT}
	python ../tesseroids_lib/tesseroid.py >> ${TESTOUTPUT}

sample:
	echo "" >> ${TESTOUTPUT}
	echo "SAMPLES:" >> ${TESTOUTPUT}
	echo "---------------------------------------" >> ${TESTOUTPUT}
	tesspot ${OPTIONS} -l pot.log > pot.dat 2>> ${TESTOUTPUT}
	python plot.py pot.dat ${NLAT} >> ${TESTOUTPUT}
	
	tessgx ${OPTIONS} -l gx.log > gx.dat 2>> ${TESTOUTPUT}
	python plot.py gx.dat ${NLAT} >> ${TESTOUTPUT}
	
	tessgy ${OPTIONS} -l gy.log > gy.dat 2>> ${TESTOUTPUT}
	python plot.py gy.dat ${NLAT} >> ${TESTOUTPUT}
	
	tessgz ${OPTIONS} -l gz.log > gz.dat 2>> ${TESTOUTPUT}
	python plot.py gz.dat ${NLAT} >> ${TESTOUTPUT}
	
	tessgxx ${OPTIONS} -l gxx.log > gxx.dat 2>> ${TESTOUTPUT}
	python plot.py gxx.dat ${NLAT} >> ${TESTOUTPUT}
	
	tessgxy ${OPTIONS} -l gxy.log > gxy.dat 2>> ${TESTOUTPUT}
	python plot.py gxy.dat ${NLAT} >> ${TESTOUTPUT}
	
	tessgxz ${OPTIONS} -l gxz.log > gxz.dat 2>> ${TESTOUTPUT}
	python plot.py gxz.dat ${NLAT} >> ${TESTOUTPUT}
	
	tessgyy ${OPTIONS} -l gyy.log > gyy.dat 2>> ${TESTOUTPUT}
	python plot.py gyy.dat ${NLAT} >> ${TESTOUTPUT}
	
	tessgyz ${OPTIONS} -l gyz.log > gyz.dat 2>> ${TESTOUTPUT}
	python plot.py gyz.dat ${NLAT} >> ${TESTOUTPUT}
	
	tessgzz ${OPTIONS} -l gzz.log > gzz.dat 2>> ${TESTOUTPUT}
	python plot.py gzz.dat ${NLAT} >> ${TESTOUTPUT}

clean:
	rm ${TESTOUTPUT}
	rm *.png
	rm *.dat
	rm *.log
