VERSION = 1.1.1
DIRBASE = tesseroids-${VERSION}

help:
	@echo "Commands:"
	@echo "    all      create all below"
	@echo "    src      source distribution"
	@echo "    bin32    32bit linux binary distribution"
	@echo "    bin64    64bit linux binary distribution"
	@echo "    win32    cross-compile 32bit Windows binary distribution"

all: src bin32 bin64 win32

src:
	scons -C ../ -c
	mkdir ${DIRBASE}
	cp -r ../src ${DIRBASE}
	cp -r ../test ${DIRBASE}
	cp -r ../cookbook ${DIRBASE}
	cp ../README.rst ${DIRBASE}/README.txt
	cp ../LICENSE.txt ${DIRBASE}
	cp ../DEPENDENCIES.txt ${DIRBASE}
	cp ../CITATION.txt ${DIRBASE}
	cp ../SConstruct ${DIRBASE}
	zip -r ${DIRBASE}-src.zip ${DIRBASE}
	tar -zcvf ${DIRBASE}-src.tar.gz ${DIRBASE}
	rm -r ${DIRBASE}

bin32:
	scons -C ../ -c
	scons -C .. mode=bin32
	../tesstest
	mkdir ${DIRBASE}
	cp -r ../cookbook ${DIRBASE}
	cp -r ../bin ${DIRBASE}
	cp ../README.rst ${DIRBASE}/README.txt
	cp ../LICENSE.txt ${DIRBASE}
	cp ../DEPENDENCIES.txt ${DIRBASE}
	cp ../CITATION.txt ${DIRBASE}
	tar -zcvf ${DIRBASE}-bin32.tar.gz ${DIRBASE}
	rm -r ${DIRBASE}

bin64:
	scons -C ../ -c
	scons -C ..
	../tesstest
	mkdir ${DIRBASE}
	cp -r ../cookbook ${DIRBASE}
	cp -r ../bin ${DIRBASE}
	cp ../README.rst ${DIRBASE}/README.txt
	cp ../LICENSE.txt ${DIRBASE}
	cp ../DEPENDENCIES.txt ${DIRBASE}
	cp ../CITATION.txt ${DIRBASE}
	tar -zcvf ${DIRBASE}-bin64.tar.gz ${DIRBASE}
	rm -r ${DIRBASE}

win32:
	scons -C ../ -c
	scons -C .. mode=win32
	wine ../tesstest.exe
	mkdir ${DIRBASE}
	cp -r ../cookbook ${DIRBASE}
	cp -r ../bin ${DIRBASE}
	cp ../README.rst ${DIRBASE}/README.txt
	cp ../LICENSE.txt ${DIRBASE}
	cp ../DEPENDENCIES.txt ${DIRBASE}
	cp ../CITATION.txt ${DIRBASE}
	zip -r ${DIRBASE}-win32.zip ${DIRBASE}
	rm -r ${DIRBASE}

clean:
	rm -rf ${DIRBASE} *.zip *.tar.gz
