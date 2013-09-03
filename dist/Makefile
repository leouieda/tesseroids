VERSION = 1.1
DIRBASE = tesseroids-${VERSION}
DOCS=../doc/_build/latex/Tesseroids_v${VERSION}.pdf
DOCSWIN=..\doc\_build\latex\Tesseroids_v${VERSION}.pdf

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
	cp ../README.rst ${DIRBASE}/README.txt
	cp ../LICENSE.txt ${DIRBASE}
	cp ../DEPENDENCIES.txt ${DIRBASE}
	cp ../CITATION.txt ${DIRBASE}
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
	cp ../README.rst ${DIRBASE}/README.txt
	cp ../LICENSE.txt ${DIRBASE}
	cp ../DEPENDENCIES.txt ${DIRBASE}
	cp ../CITATION.txt ${DIRBASE}
	tar -zcvf ${DIRBASE}-bin32.tar.gz ${DIRBASE}
	rm -r ${DIRBASE}

bin64: ${DOCS}
	scons -C ..
	mkdir ${DIRBASE}
	mkdir ${DIRBASE}/doc
	cp -r ../cookbook ${DIRBASE}
	cp -r ../bin ${DIRBASE}
	cp ${DOCS} ${DIRBASE}/doc
	cp ../README.rst ${DIRBASE}/README.txt
	cp ../LICENSE.txt ${DIRBASE}
	cp ../DEPENDENCIES.txt ${DIRBASE}
	cp ../CITATION.txt ${DIRBASE}
	tar -zcvf ${DIRBASE}-bin64.tar.gz ${DIRBASE}
	rm -r ${DIRBASE}

win32:
	scons -C ..
	mkdir ${DIRBASE}-win32
	mkdir ${DIRBASE}-win32\doc
	xcopy ${DOCSWIN} ${DIRBASE}-win32\doc
	mkdir ${DIRBASE}-win32\cookbook
	xcopy /E ..\cookbook ${DIRBASE}-win32\cookbook
	mkdir ${DIRBASE}-win32\bin
	xcopy ..\bin ${DIRBASE}-win32\bin
	xcopy ..\README.rst ${DIRBASE}-win32
	rename ${DIRBASE}-win32\README.rst README.txt
	xcopy ..\LICENSE.txt ${DIRBASE}-win32
	xcopy ..\DEPENDENCIES.txt ${DIRBASE}-win32
	xcopy ..\CITATION.txt ${DIRBASE}-win32

clean:
	rm -rf ${DIRBASE} *.zip *.tar.gz
