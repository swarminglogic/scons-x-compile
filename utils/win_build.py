import os

targetSuffix = '.exe'

base ='#/src'

libOther = ['m']

libpaths = ['#/lib/']

cppflags = []

linkflags = ['--static', '-Wl,--no-undefined', '-static-libgcc', '-static-libstdc++']

sourcepaths = [base]
