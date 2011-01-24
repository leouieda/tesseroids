# Build the Tesseroids programs

import os
from os.path import join

c_dir = join('src', 'c')
build_dir = 'bin'
test_dir = 'test'

libs = ['m']

grav_prism = Object(join(c_dir, 'grav_prism.c'))
constants = Object(join(c_dir, 'constants.c'))
glq = Object(join(c_dir, 'glq.c'))

utests = [('test_glq.c', glq)]

for testfile, dep in utests:

    test = Object(join(test_dir, testfile))
    Program(test + dep)


