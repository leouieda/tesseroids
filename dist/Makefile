VERSION = 1.1
DIRBASE = tesseroids-${VERSION}
DOCS=../doc/_build/latex/Tesseroids_v1.1.pdf


all:
	@echo "Commands:"
	@echo "    make src for a source distribution"
	@echo "    make bin32 for a linux binary distribution"
	@echo "    make bin64 for a linux binary distribution"
	@echo "    make win32 for a windows binary distribution"

${DOCS}:
	cd ../doc; make clean
	cd ../doc; make latex

src: ${DOCS}
	scons -C ../ -c
	mkdir ${DIRBASE}
	mkdir ${DIRBASE}/doc
	cp -r ../src ${DIRBASE}
	cp -r ../test ${DIRBASE}
	cp -r ../cookbook ${DIRBASE}
	cp ${DOCS} ${DIRBASE}/doc
	cp ../README.rst ${DIRBASE}
	cp ../LICENSE.txt ${DIRBASE}
	cp ../SConstruct ${DIRBASE}
	zip -r ${DIRBASE}-src.zip ${DIRBASE}
	tar -zcvf ${DIRBASE}-src.tar.gz ${DIRBASE}
	rm -r ${DIRBASE}

bin32: ${DOCS}
	scons -C ..
	mkdir ${DIRBASE}
	mkdir ${DIRBASE}/doc
	cp -r ../cookbook ${DIRBASE}
	cp -r ../bin ${DIRBASE}
	cp ${DOCS} ${DIRBASE}/doc
	cp ../README.rst ${DIRBASE}
	cp ../LICENSE.txt ${DIRBASE}
	tar -zcvf ${DIRBASE}-bin32.tar.gz ${DIRBASE}
	rm -r ${DIRBASE}

bin64: ${DOCS}
	scons -C ..
	mkdir ${DIRBASE}
	mkdir ${DIRBASE}/doc
	cp -r ../cookbook ${DIRBASE}
	cp -r ../bin ${DIRBASE}
	cp ${DOCS} ${DIRBASE}/doc
	cp ../README.rst ${DIRBASE}
	cp ../LICENSE.txt ${DIRBASE}
	tar -zcvf ${DIRBASE}-bin64.tar.gz ${DIRBASE}
	rm -r ${DIRBASE}

win32: ${DOCS}
	scons -C ..
	mkdir ${DIRBASE}-win32
	mkdir ${DIRBASE}-win32\doc
	mkdir ${DIRBASE}-win32\cookbook
	mkdir ${DIRBASE}-win32\cookbook\dem_brasil
	copy ..\cookbook\dem_brasil ${DIRBASE}-win32\cookbook\dem_brasil
	mkdir ${DIRBASE}-win32\bin
	copy ..\bin ${DIRBASE}-win32\bin
	copy ..\README.rst ${DIRBASE}-win32
	copy ..\LICENSE.txt ${DIRBASE}-win32

clean:
	rm -rf ${DIRBASE} *.zip *.tar.gz
