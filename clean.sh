#!/bin/bash
# Clean all generated files from the repository.
# Needs to be in a separate script for "make package" to clean before building
# each package. Just using a dependency on the "clean" target doesn't work
# because it's only called once.
scons -c
make clean -C doc
make clean -C cookbook
