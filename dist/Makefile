VERSION = 1.0
DIRBASE = tesseroids-${VERSION}
DOCS=../doc/_build/latex/Tesseroids.pdf

all:
	@echo "Commands:"
	@echo "    make src for a source distribution"
	@echo "    make bin32 for a linux binary distribution"
	@echo "    make bin64 for a linux binary distribution"
	@echo "    make win32 for a windows binary distribution"

${DOCS}:
	cd ../doc; make clean
	cd ../doc; make latex
	cd ../doc/_build/latex; make

src: ${DOCS}
	scons -C ../ -c
	mkdir ${DIRBASE}
	mkdir ${DIRBASE}/doc
	cp -r ../src ${DIRBASE}
	cp -r ../test ${DIRBASE}
	cp -r ../examples ${DIRBASE}
	cp ${DOCS} ${DIRBASE}/doc
	cp ../README.rst ${DIRBASE}
	cp ../LICENSE ${DIRBASE}
	cp ../SConstruct ${DIRBASE}
	zip -r ${DIRBASE}-src.zip ${DIRBASE}
	tar -zcvf ${DIRBASE}-src.tar.gz ${DIRBASE}
	rm -r ${DIRBASE}

bin32: ${DOCS}
	scons -C ..
	mkdir ${DIRBASE}
	mkdir ${DIRBASE}/doc
	cp -r ../examples ${DIRBASE}
	cp -r ../bin ${DIRBASE}
	cp -r ../doc/build/html ${DIRBASE}/doc/html
	cp ../doc/build/user-manual-v1_0.html ${DIRBASE}/doc
	cp ../README ${DIRBASE}
	cp ../LICENSE ${DIRBASE}
	tar -zcvf ${DIRBASE}-bin32.tar.gz ${DIRBASE}
	rm -r ${DIRBASE}

bin64: ${DOCS}
	scons -C ..
	mkdir ${DIRBASE}
	mkdir ${DIRBASE}/doc
	cp -r ../examples ${DIRBASE}
	cp -r ../bin ${DIRBASE}
	cp -r ../doc/build/html ${DIRBASE}/doc/html
	cp ../doc/build/user-manual-v1_0.html ${DIRBASE}/doc
	cp ../README ${DIRBASE}
	cp ../LICENSE ${DIRBASE}
	tar -zcvf ${DIRBASE}-bin64.tar.gz ${DIRBASE}
	rm -r ${DIRBASE}

win32: ${DOCS}
	scons -C ..
	mkdir ${DIRBASE}-win32
	mkdir ${DIRBASE}-win32\doc
	mkdir ${DIRBASE}-win32\doc\html
	copy ..\doc\build\html ${DIRBASE}-win32\doc\html
	copy ..\doc\build\user-manual-v1_0.html ${DIRBASE}-win32\doc
	mkdir ${DIRBASE}-win32\examples
	mkdir ${DIRBASE}-win32\examples\dem_brasil
	mkdir ${DIRBASE}-win32\examples\prism
	mkdir ${DIRBASE}-win32\examples\tess
	copy ..\examples\dem_brasil ${DIRBASE}-win32\examples\dem_brasil
	copy ..\examples\prism ${DIRBASE}-win32\examples\prism
	copy ..\examples\tess ${DIRBASE}-win32\examples\tess
	mkdir ${DIRBASE}-win32\bin
	copy ..\bin ${DIRBASE}-win32\bin
	copy ..\README ${DIRBASE}-win32
	copy ..\LICENSE ${DIRBASE}-win32

clean:
	rm -r ${DIRBASE} *.zip *.tar.gz
