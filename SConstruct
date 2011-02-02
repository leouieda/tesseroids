# Build the Tesseroids programs

import os
from os.path import join

# List the directories used
c_dir = join('src', 'c')
bin_dir = 'bin'
test_dir = 'test'

# Compile the various modules to objects
constants = Object(join(c_dir, 'constants.c'))
utils = Object(join(c_dir, 'utils.c'))
logger = Object(join(c_dir, 'logger.c'))
cmd = Object(join(c_dir, 'cmd.c'))
version = Object(join(c_dir, 'version.c'))
glq = Object(join(c_dir, 'glq.c'))
grav_prism = Object(join(c_dir, 'grav_prism.c'))
grav_sphere = Object(join(c_dir, 'grav_sphere.c'))
grav_tess = Object(join(c_dir, 'grav_tess.c'))

# Compile the test suite and link it to the tested moules
test = Object(join(test_dir, 'test.c'))
Program(test + constants + glq + utils + grav_prism + grav_sphere + grav_tess +
        logger,
        LIBS=['m'])

# Clean up the docs build as well
Clean(os.path.curdir, join('doc', 'build'))

# Compile the main programs
tessmkgrd = Object(join(c_dir, 'tessmkgrd.c'))
Program(join(bin_dir, 'tessmkgrd'), tessmkgrd + logger + cmd + version)

tessgz = Object(join(c_dir, 'tessgz.c'))
Program(join(bin_dir, 'tessgz'),
        tessgz + logger + cmd + version + glq + grav_tess + utils + constants,
        LIBS=['m'])

# Temp testing
mapper = Object(join(test_dir, 'mapper.c'))
Program(mapper + glq + constants + utils + grav_tess + logger, LIBS=['m'])

logtest = Object(join(test_dir, 'logtest.c'))
Program(logtest + logger)