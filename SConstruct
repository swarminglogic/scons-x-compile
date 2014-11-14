import os

env = Environment(ENV = {'PATH' : os.environ['PATH'],
                         'HOME' : os.environ['HOME'],
                         'TERM' : 'xterm'})

# Defines the command-line option "--win64"
AddOption('--win64',
          action='store_true',
          help='cross-compiles for windows',
          default=False)

# If SCons was called with '--win64'
if GetOption('win64'):
    env['CXX']='x86_64-w64-mingw32-g++'
    vardir='build/win'
else:
    env['CXX']='g++'
    vardir='build/linux'

# The VariantDir makes so that all build files end up in
# build/linux/ or build/windows. This keeps the ./src clean
VariantDir(vardir, 'src')
Export('env')
SConscript(vardir + '/SConscript')
