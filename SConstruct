# Build the Tesseroids programs

import os
from os.path import join

c_dir = join('src', 'c')
build_dir = 'bin'

libs = ['m']

grav_prism = Object(join(c_dir, 'grav_prism.c'))
constants = Object(join(c_dir, 'constants.c'))
main = Object(join(c_dir, 'main.c'))

Program(join(build_dir, 'main'), constants + main + grav_prism, LIBS=libs)