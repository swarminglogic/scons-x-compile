import subprocess

#  General setup
##############################
sdl_prefix = subprocess.check_output(["sdl2-config", "--prefix"]).strip()
sdl_libs = ['SDL2', 'SDL2_image', 'SDL2_mixer', 'pthread']
other_libs = ['m', 'dl', 'rt']

#  Required output variables
##############################
targetSuffix = ''

base ='#/src'

libs = sdl_libs + other_libs

libpaths = ['#/lib/',
            sdl_prefix + '/lib']

cppflags = ['-D_REENTRANT']

sourcepaths = [base,
               sdl_prefix + '/include']

linkflags = []
