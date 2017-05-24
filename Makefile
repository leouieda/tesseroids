VERSION := $(shell python print_version.py)
DIST := dist
PKGNAME := tesseroids-$(VERSION)
PKG := $(DIST)/$(PKGNAME)

.PHONY: help build check test package pkg-bin32 pkg-bin64 pkg-win32 pkg-win64 clean

help:
	@echo "Commands:"
	@echo "  build        Compile the source"
	@echo "  check        Compile in debug mode and check for ANSI compatibility"
	@echo "  test         Run the tests"
	@echo "  clean        Clean the directory"
	@echo "  package      Make binary packages for all supported platforms"
	@echo "  pkg-bin32    Make a 32bit linux binary distribution"
	@echo "  pkg-bin64    Make a 64bit linux binary distribution"
	@echo "  pkg-win32    Cross-compile 32bit Windows binary distribution"
	@echo "  pkg-win64    Cross-compile 64bit Windows binary distribution"
	@echo "  pkg-src      Make a source distribution"

build:
	scons

check:
	scons mode=check

test: build
	./tesstest

package: pkg-bin32 pkg-bin64 pkg-win32 pkg-win64

pkg-src:
	@echo "\nCreating source package\n"
	@echo "#######################\n"
	bash clean.sh
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

pkg-bin32:
	@echo "\nCreating 32bit Linux package"
	@echo "############################\n"
	bash clean.sh
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

pkg-bin64:
	@echo "\nCreating 64bit Linux package"
	@echo "############################\n"
	bash clean.sh
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

pkg-win32:
	@echo "\nCreating 32bit Windows package"
	@echo "##############################\n"
	bash clean.sh
	scons mode=win32
	# Need to clear the wine install or it will complain when running both
	# 32bit and 64bit
	rm -r ~/.wine
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

pkg-win64:
	@echo "\nCreating 64bit Windows package"
	@echo "##############################\n"
	bash clean.sh
	scons mode=win64
	# Need to clear the wine install or it will complain when running both
	# 32bit and 64bit
	rm -r ~/.wine
	wine64 tesstest.exe
	mkdir $(PKG)
	cp -r cookbook $(PKG)
	cp -r bin $(PKG)
	cp README.md $(PKG)/README.md
	cp LICENSE.txt $(PKG)
	cp DEPENDENCIES.txt $(PKG)
	cp CITATION.txt $(PKG)
	cd $(DIST); zip -r $(PKGNAME)-win64.zip $(PKGNAME); cd ..
	rm -r $(PKG)

clean:
	bash clean.sh
