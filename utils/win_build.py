targetSuffix = '.exe'

base ='#/src'

libOther = ['m']

libpaths = ['#/lib/']

cppflags = []

sourcepaths = [base]

linkflags = ['--static',
             '-Wl,--no-undefined',
             '-static-libgcc',
             '-static-libstdc++']
