test: unittest doctest

unittest: test_abscissas test_weights test_tesseroid
		
test_abscissas:
	echo "\nTESTING ABSCISSAS CLASS:\n"
	python test_abscissas.py -v

test_weights:
	echo "\nTESTING WEIGHTS CLASS:\n"
	python test_weights.py -v

test_tesseroid:
	echo "\nTESTING TESSEROID CLASS:\n"
	python test_tesseroid.py -v

doctest:
	echo "\nDOCTEST FOR GLQ.PY:\n"
	python glq.py

	echo "\nDOCTEST FOR TESSEROID.PY:\n"
	python tesseroid.py