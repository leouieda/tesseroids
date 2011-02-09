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
tessg_main = Object(join(c_dir, 'tessg_main.c'))
prismg_main = Object(join(c_dir, 'prismg_main.c'))
version = Object(join(c_dir, 'version.c'))
glq = Object(join(c_dir, 'glq.c'))
grav_prism = Object(join(c_dir, 'grav_prism.c'))
grav_sphere = Object(join(c_dir, 'grav_sphere.c'))
grav_tess = Object(join(c_dir, 'grav_tess.c'))

# Compile the test suite and link it to the tested moules
test = Object(join(test_dir, 'test_all.c'))
Program(join(bin_dir, 'tesstest'),
        test + constants + glq + utils + grav_prism + grav_sphere + grav_tess +
        logger,
        LIBS=['m'])

# Clean up the docs build as well
#Clean(os.path.curdir, join('doc', 'build'))

# Compile the main programs
tessdefaults = Object(join(c_dir, 'tessdefaults.c'))
Program(join(bin_dir, 'tessdefaults'),
        tessdefaults + logger + version + glq + constants, LIBS=['m'])

tessgrd = Object(join(c_dir, 'tessgrd.c'))
Program(join(bin_dir, 'tessgrd'), tessgrd + logger + cmd + version)

tess2prism = Object(join(c_dir, 'tess2prism.c'))
Program(join(bin_dir, 'tess2prism'),
        tess2prism + logger + cmd + version + utils + constants, LIBS=['m'])

tessmass = Object(join(c_dir, 'tessmass.c'))
Program(join(bin_dir, 'tessmass'),
        tessmass + logger + cmd + version + utils + constants, LIBS=['m'])

tessmodgen = Object(join(c_dir, 'tessmodgen.c'))
Program(join(bin_dir, 'tessmodgen'),
        tessmodgen + logger + cmd + version + utils + constants, LIBS=['m'])

# The tessg* programs
tessgx = Object(join(c_dir, 'tessgx.c'))
Program(join(bin_dir, 'tessgx'),
        tessgx + logger + cmd + version + glq + grav_tess + utils + constants +
        tessg_main,
        LIBS=['m'])

tessgy = Object(join(c_dir, 'tessgy.c'))
Program(join(bin_dir, 'tessgy'),
        tessgy + logger + cmd + version + glq + grav_tess + utils + constants +
        tessg_main,
        LIBS=['m'])

tessgz = Object(join(c_dir, 'tessgz.c'))
Program(join(bin_dir, 'tessgz'),
        tessgz + logger + cmd + version + glq + grav_tess + utils + constants +
        tessg_main,
        LIBS=['m'])

tessgxx = Object(join(c_dir, 'tessgxx.c'))
Program(join(bin_dir, 'tessgxx'),
        tessgxx + logger + cmd + version + glq + grav_tess + utils + constants +
        tessg_main,
        LIBS=['m'])

tessgxy = Object(join(c_dir, 'tessgxy.c'))
Program(join(bin_dir, 'tessgxy'),
        tessgxy + logger + cmd + version + glq + grav_tess + utils + constants +
        tessg_main,
        LIBS=['m'])

tessgxz = Object(join(c_dir, 'tessgxz.c'))
Program(join(bin_dir, 'tessgxz'),
        tessgxz + logger + cmd + version + glq + grav_tess + utils + constants +
        tessg_main,
        LIBS=['m'])

tessgyy = Object(join(c_dir, 'tessgyy.c'))
Program(join(bin_dir, 'tessgyy'),
        tessgyy + logger + cmd + version + glq + grav_tess + utils + constants +
        tessg_main,
        LIBS=['m'])

tessgyz = Object(join(c_dir, 'tessgyz.c'))
Program(join(bin_dir, 'tessgyz'),
        tessgyz + logger + cmd + version + glq + grav_tess + utils + constants +
        tessg_main,
        LIBS=['m'])

tessgzz = Object(join(c_dir, 'tessgzz.c'))
Program(join(bin_dir, 'tessgzz'),
        tessgzz + logger + cmd + version + glq + grav_tess + utils + constants +
        tessg_main,
        LIBS=['m'])

# The prismg* programs
prismgx = Object(join(c_dir, 'prismgx.c'))
Program(join(bin_dir, 'prismgx'),
        prismgx + logger + cmd + version + grav_prism + utils + constants +
        prismg_main,
        LIBS=['m'])
        
prismgy = Object(join(c_dir, 'prismgy.c'))
Program(join(bin_dir, 'prismgy'),
        prismgy + logger + cmd + version + grav_prism + utils + constants +
        prismg_main,
        LIBS=['m'])

prismgz = Object(join(c_dir, 'prismgz.c'))
Program(join(bin_dir, 'prismgz'),
        prismgz + logger + cmd + version + grav_prism + utils + constants +
        prismg_main,
        LIBS=['m'])

prismgxx = Object(join(c_dir, 'prismgxx.c'))
Program(join(bin_dir, 'prismgxx'),
        prismgxx + logger + cmd + version + grav_prism + utils + constants +
        prismg_main,
        LIBS=['m'])

prismgxy = Object(join(c_dir, 'prismgxy.c'))
Program(join(bin_dir, 'prismgxy'),
        prismgxy + logger + cmd + version + grav_prism + utils + constants +
        prismg_main,
        LIBS=['m'])

prismgxz = Object(join(c_dir, 'prismgxz.c'))
Program(join(bin_dir, 'prismgxz'),
        prismgxz + logger + cmd + version + grav_prism + utils + constants +
        prismg_main,
        LIBS=['m'])

prismgyy = Object(join(c_dir, 'prismgyy.c'))
Program(join(bin_dir, 'prismgyy'),
        prismgyy + logger + cmd + version + grav_prism + utils + constants +
        prismg_main,
        LIBS=['m'])

prismgyz = Object(join(c_dir, 'prismgyz.c'))
Program(join(bin_dir, 'prismgyz'),
        prismgyz + logger + cmd + version + grav_prism + utils + constants +
        prismg_main,
        LIBS=['m'])

prismgzz = Object(join(c_dir, 'prismgzz.c'))
Program(join(bin_dir, 'prismgzz'),
        prismgzz + logger + cmd + version + grav_prism + utils + constants +
        prismg_main,
        LIBS=['m'])

# Temp testing
#mapper = Object(join(test_dir, 'mapper.c'))
#Program(mapper + glq + constants + utils + grav_tess + logger, LIBS=['m'])

#logtest = Object(join(test_dir, 'logtest.c'))
#Program(logtest + logger)