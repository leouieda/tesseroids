# Build the Tesseroids programs

import os
from os.path import join

# List the directories used
c_dir = join('src', 'c')
build_dir = 'bin'
test_dir = 'test'

# Compile the various modules to objects
constants = Object(join(c_dir, 'constants.c'))
utils = Object(join(c_dir, 'utils.c'))
glq = Object(join(c_dir, 'glq.c'))
grav_prism = Object(join(c_dir, 'grav_prism.c'))
grav_sphere = Object(join(c_dir, 'grav_sphere.c'))

# Compile the test suite and link it to the tested moules
test = Object(join(test_dir, 'test.c'))
Program(test + constants + glq + utils, LIBS=['m'])

testgrav = Object(join(test_dir, 'test_grav.c'))
Program(testgrav + constants + grav_sphere, LIBS=['m'])

# Clean up the docs build as well
Clean(os.path.curdir, join('doc', 'build'))