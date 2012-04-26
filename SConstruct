# Build the Tesseroids programs
env = Environment(
    CFLAGS='-ansi -pedantic-errors -Wall -O3',
    LIBS=['m'],
    CPPPATH='src/lib')

# Build the tessg* programs
tesssrc = Split("""
    src/lib/logger.c
    src/lib/version.c
    src/lib/grav_tess.c
    src/lib/glq.c
    src/lib/constants.c
    src/lib/utils.c
    src/lib/cmd.c
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
    src/lib/utils.c
    src/lib/cmd.c
    src/lib/prismg_main.c
    src/lib/grav_prism.c
    """)
fields = ['pot', 'gx', 'gy', 'gz', 'gxx', 'gxy', 'gxz', 'gyy', 'gyz', 'gzz']
for f in fields:
    sources = ['src/prism%s.c' % (f)] + tesssrc
    env.Program('bin/prism%s' % (f), source=sources)
    
# Build tess2prism
env.Program('bin/tess2prism', source=Split("""
    src/tess2prism.c
    src/lib/logger.c
    src/lib/version.c
    src/lib/constants.c
    src/lib/utils.c
    src/lib/cmd.c
    """))    
# Build tessdefaults
env.Program('bin/tessdefaults', source=Split("""
    src/tessdefaults.c
    src/lib/logger.c
    src/lib/version.c
    src/lib/constants.c
    src/lib/glq.c
    src/lib/utils.c
    """))
# Build tessgrd
env.Program('bin/tessgrd', source=Split("""
    src/tessgrd.c
    src/lib/logger.c
    src/lib/version.c
    src/lib/cmd.c
    """))
# Build tessmass
env.Program('bin/tessmass', source=Split("""
    src/tessmass.c
    src/lib/logger.c
    src/lib/version.c
    src/lib/cmd.c
    src/lib/utils.c
    src/lib/constants.c
    """))
# Build tessmodgen
env.Program('bin/tessmodgen', source=Split("""
    src/tessmodgen.c
    src/lib/logger.c
    src/lib/version.c
    src/lib/cmd.c
    src/lib/utils.c
    src/lib/constants.c
    """))

# Build the test runner
sources = ['test/test_all.c']
sources.extend(Glob("src/lib/*.c"))
tesstest = env.Program('bin/tesstest', source=sources)
