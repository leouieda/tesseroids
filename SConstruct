# Build the Tesseroids programs

# get the mode flag from the command line
# default to 'release' if the user didn't specify
mode = ARGUMENTS.get('mode', 'release')
if not (mode in ['debug', 'release']):
   print "Error: expected 'debug' or 'release', found: " + mode
   Exit(1)
print '**** Compiling in ' + mode + ' mode...'

if mode == 'debug':
    env = Environment(
        CFLAGS='-ansi -pedantic-errors -Wall -g',
        LIBS=['m'],
        CPPPATH='src/lib')
else:
    env = Environment(
        CFLAGS='-O3',
        LIBS=['m'],
        CPPPATH='src/lib')

# Build the tessg* programs
tesssrc = Split("""
    src/lib/logger.c
    src/lib/version.c
    src/lib/grav_tess.c
    src/lib/glq.c
    src/lib/constants.c
    src/lib/geometry.c
    src/lib/parsers.c
    src/lib/tessg_main.c
    """)
fields = ['pot', 'gx', 'gy', 'gz', 'gxx', 'gxy', 'gxz', 'gyy', 'gyz', 'gzz']
for f in fields:
    sources = ['src/tess%s.c' % (f)] + tesssrc
    env.Program('bin/tess%s' % (f), source=sources)

# Build the prismg* programs
tesssrc = Split("""
    src/lib/logger.c
    src/lib/version.c
    src/lib/constants.c
    src/lib/geometry.c
    src/lib/parsers.c
    src/lib/prismg_main.c
    src/lib/grav_prism.c
    """)
fields = ['pot', 'gx', 'gy', 'gz', 'gxx', 'gxy', 'gxz', 'gyy', 'gyz', 'gzz']
for f in fields:
    sources = ['src/prism%s.c' % (f)] + tesssrc
    env.Program('bin/prism%s' % (f), source=sources)

# Build prismpots, prismgs, and prismggts
env.Program('bin/prismpots', source=Split("""
    src/prismpots.c
    src/lib/grav_prism_sph.c
    src/lib/grav_prism.c
    src/lib/logger.c
    src/lib/version.c
    src/lib/constants.c
    src/lib/geometry.c
    src/lib/parsers.c
    """))
env.Program('bin/prismgs', source=Split("""
    src/prismgs.c
    src/lib/grav_prism_sph.c
    src/lib/grav_prism.c
    src/lib/logger.c
    src/lib/version.c
    src/lib/constants.c
    src/lib/geometry.c
    src/lib/parsers.c
    """))
env.Program('bin/prismggts', source=Split("""
    src/prismggts.c
    src/lib/grav_prism_sph.c
    src/lib/grav_prism.c
    src/lib/logger.c
    src/lib/version.c
    src/lib/constants.c
    src/lib/geometry.c
    src/lib/parsers.c
    """))

# Build tess2prism
env.Program('bin/tess2prism', source=Split("""
    src/tess2prism.c
    src/lib/logger.c
    src/lib/version.c
    src/lib/constants.c
    src/lib/geometry.c
    src/lib/parsers.c
    """))
# Build tessdefaults
env.Program('bin/tessdefaults', source=Split("""
    src/tessdefaults.c
    src/lib/logger.c
    src/lib/version.c
    src/lib/constants.c
    src/lib/glq.c
    src/lib/geometry.c
    """))
# Build tessgrd
env.Program('bin/tessgrd', source=Split("""
    src/tessgrd.c
    src/lib/logger.c
    src/lib/version.c
    src/lib/parsers.c
    src/lib/constants.c
    """))
# Build tessmass
env.Program('bin/tessmass', source=Split("""
    src/tessmass.c
    src/lib/logger.c
    src/lib/version.c
    src/lib/parsers.c
    src/lib/geometry.c
    src/lib/constants.c
    """))
# Build tessmodgen
env.Program('bin/tessmodgen', source=Split("""
    src/tessmodgen.c
    src/lib/logger.c
    src/lib/version.c
    src/lib/parsers.c
    src/lib/geometry.c
    src/lib/constants.c
    """))
# Build tesslayers
env.Program('bin/tesslayers', source=Split("""
    src/tesslayers.c
    src/lib/logger.c
    src/lib/version.c
    src/lib/parsers.c
    src/lib/geometry.c
    src/lib/constants.c
    """))

# Build the test runner
sources = ['test/test_all.c']
sources.extend(Glob("src/lib/*.c"))
tesstest = env.Program('tesstest', source=sources)

# When cleaning the build, also clean the doc build
#Clean('.', 'doc/_build/')
