VERSION = 1.0
DIRBASE = tesseroids-${VERSION}

all:
	@echo "Commands:"
	@echo "    make src for a source distribution"
	@echo "    make bin for a linux binary distribution"
	@echo "    make win32 for a windows binary distribution"

src:
	scons -C ../ -c
	make -C ../doc
	mkdir ${DIRBASE}
	mkdir ${DIRBASE}/doc
	cp -r ../src ${DIRBASE}
	cp -r ../test ${DIRBASE}
	cp -r ../examples ${DIRBASE}
	cp -r ../doc/build/html ${DIRBASE}/doc/html
	cp ../doc/build/user-manual-v1_0.html ${DIRBASE}/doc
	cp ../README ${DIRBASE}
	cp ../LICENSE ${DIRBASE}
	cp ../SConstruct ${DIRBASE}
	zip -r ${DIRBASE}-src.zip ${DIRBASE}
	tar -zcvf ${DIRBASE}-src.tar.gz ${DIRBASE}
	rm -r ${DIRBASE}

bin:
	scons -C ..
	make -C ../doc
	mkdir ${DIRBASE}
	mkdir ${DIRBASE}/doc
	cp -r ../examples ${DIRBASE}
	cp -r ../bin ${DIRBASE}
	cp -r ../doc/build/html ${DIRBASE}/doc/html
	cp ../doc/build/user-manual-v1_0.html ${DIRBASE}/doc
	cp ../README ${DIRBASE}
	cp ../LICENSE ${DIRBASE}
	tar -zcvf ${DIRBASE}-bin.tar.gz ${DIRBASE}
	rm -r ${DIRBASE}

win32:
	scons -C ..
	mkdir ${DIRBASE}
	mkdir ${DIRBASE}\doc
	mkdir ${DIRBASE}\examples
	mkdir ${DIRBASE}\examples\dem_brasil
	mkdir ${DIRBASE}\examples\prism
	mkdir ${DIRBASE}\examples\tess
	copy ..\examples\dem_brasil ${DIRBASE}\examples\dem_brasil
	copy ..\examples\prism ${DIRBASE}\examples\prism
	copy ..\examples\tess ${DIRBASE}\examples\tess
	mkdir ${DIRBASE}\bin
	copy ..\bin ${DIRBASE}\bin
	copy ..\README ${DIRBASE}
	copy ..\LICENSE ${DIRBASE}

clean:
	rm ${DIRBASE}-*.zip
	rm ${DIRBASE}-*.tar.gz