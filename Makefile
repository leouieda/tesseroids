VERSION:=$(shell python print_version.py)
DIST:=dist
PKGNAME:=tesseroids-$(VERSION)
PKG:=$(DIST)/$(PKGNAME)

help:
	@echo "Commands:"
	@echo "  build        Compile the source"
	@echo "  check        Compile in debug mode and check for ANSI compatibility"
	@echo "  test         Run the tests"
	@echo "  benchmark    Run the benchmarks"
	@echo "  doc          Build the HTML documentation"
	@echo "  clean        Clean the directory"
	@echo "  package      Make binary packages for distribution"
	@echo "  pkg-bin32    Make a 32bit linux binary distribution"
	@echo "  pkg-bin64    Make a 64bit linux binary distribution"
	@echo "  pkg-win32    Cross-compile 32bit Windows binary distribution"
	@echo "  pkg-src      Make a source distribution"

build:
	scons

check:
	scons mode=check

.PHONY: test
test: build
	./tesstest

benchmark: build
	cd bench; make

.PHONY: doc
doc:
	cd doc; make html

package: pkg-bin32 pkg-bin64 pkg-win32

pkg-src: clean
	@echo "Creating source package..."
	mkdir -p $(PKG)
	cp -r src $(PKG)
	cp -r test $(PKG)
	cp -r cookbook $(PKG)
	cp print_version.py $(PKG)
	cp versioneer.py $(PKG)
	cp README.md $(PKG)/README.md
	cp LICENSE.txt $(PKG)
	cp DEPENDENCIES.txt $(PKG)
	cp CITATION.txt $(PKG)
	cp SConstruct $(PKG)
	cd $(DIST); zip -r $(PKGNAME)-src.zip $(PKGNAME); cd ..
	cd $(DIST); tar -zcvf $(PKGNAME)-src.tar.gz $(PKGNAME); cd ..
	rm -rf $(PKG)

pkg-bin32: clean
	@echo "Creating 32bit Linux package..."
	scons mode=bin32
	./tesstest
	mkdir -p $(PKG)
	cp -r cookbook $(PKG)
	cp -r bin $(PKG)
	cp README.md $(PKG)/README.md
	cp LICENSE.txt $(PKG)
	cp DEPENDENCIES.txt $(PKG)
	cp CITATION.txt $(PKG)
	cd $(DIST); tar -zcvf $(PKGNAME)-bin32.tar.gz $(PKGNAME); cd ..
	rm -r $(PKG)

pkg-bin64: clean
	@echo "Creating 64bit Linux package..."
	scons
	./tesstest
	mkdir -p $(PKG)
	cp -r cookbook $(PKG)
	cp -r bin $(PKG)
	cp README.md $(PKG)/README.md
	cp LICENSE.txt $(PKG)
	cp DEPENDENCIES.txt $(PKG)
	cp CITATION.txt $(PKG)
	cd $(DIST); tar -zcvf $(PKGNAME)-bin64.tar.gz $(PKGNAME); cd ..
	rm -r $(PKG)

pkg-win32: clean
	@echo "Creating 32bit Windows package..."
	scons mode=win32
	wine tesstest.exe
	mkdir $(PKG)
	cp -r cookbook $(PKG)
	cp -r bin $(PKG)
	cp README.md $(PKG)/README.md
	cp LICENSE.txt $(PKG)
	cp DEPENDENCIES.txt $(PKG)
	cp CITATION.txt $(PKG)
	cd $(DIST); zip -r $(PKGNAME)-win32.zip $(PKGNAME); cd ..
	rm -r $(PKG)

clean:
	scons -c
	cd doc; make clean

