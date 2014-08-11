build:
	scons

check:
	scons mode=check

.PHONY: test
test: build
	./tesstest

benchmark: build
	cd bench; make

clean:
	scons -c
