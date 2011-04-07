VERSION = 1.0
DIRBASE = tesseroids-${VERSION}

all:
	@echo "Commands:"
	@echo "    make src for a source distribution"
	@echo "    make bin for a linux binary distribution"
	@echo "    make win for a windows binary distribution"

src:
	scons -C ../ -c
	mkdir ${DIRBASE}
	mkdir ${DIRBASE}/doc
	cp -r ../src ${DIRBASE}
	cp -r ../test ${DIRBASE}
	cp -r ../examples ${DIRBASE}
	cp -r ../doc/build/html ${DIRBASE}/doc/html
	cp ../README ${DIRBASE}
	cp ../LICENSE ${DIRBASE}
	cp ../SConstruct ${DIRBASE}
	zip -r ${DIRBASE}-src.zip ${DIRBASE}
	tar -zcvf ${DIRBASE}-src.tar.gz ${DIRBASE}
	rm -r ${DIRBASE}

bin:
	scons -C ..
	mkdir ${DIRBASE}
	mkdir ${DIRBASE}/doc
	cp -r ../examples ${DIRBASE}
	cp -r ../bin ${DIRBASE}
	cp -r ../doc/build/html ${DIRBASE}/doc/html
	cp ../README ${DIRBASE}
	cp ../LICENSE ${DIRBASE}
	tar -zcvf ${DIRBASE}-bin.tar.gz ${DIRBASE}
	rm -r ${DIRBASE}

win:
	scons -C ..
	mkdir ${DIRBASE}
	mkdir ${DIRBASE}\doc
	copy -r ..\examples ${DIRBASE}
	copy -r ..\bin ${DIRBASE}
	copy -r ..\doc\build\html ${DIRBASE}\doc\html
	copy ..\README ${DIRBASE}
	copy ..\LICENSE ${DIRBASE}

clean:
	rm ${DIRBASE}-*.zip
	rm ${DIRBASE}-*.tar.gz
