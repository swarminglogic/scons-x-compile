
#  General setup
##############################
cross_prefix = "/usr/local/cross-tools/x86_64-w64-mingw32"
sdl_libs = ['mingw32', 'SDL2main', 'SDL2',
            'SDL2_image', 'SDL2_mixer', 'SDL2_ttf']

 # freetype is required by SDL_ttf
 # vorbisfile, vorbis, and ogg, are required by SDL_mixer
 # jpeg, png, and z, are required by SDL_image
other_libs = ['freetype',
              'vorbisfile', 'vorbis', 'ogg',
              'jpeg', 'png', 'z',
              'm', 'dinput8', 'dxguid', 'dxerr8', 'user32',
              'gdi32', 'winmm', 'imm32', 'ole32', 'oleaut32',
              'shell32', 'version', 'uuid']


#  Required output variables
##############################
targetSuffix = '.exe'

base ='#/src'

libs = sdl_libs + other_libs

libpaths = ['#/lib/',
            cross_prefix + '/lib']

cppflags = ['-Dmain=SDL_main']

sourcepaths = [base,
               cross_prefix + '/include']

linkflags = ['--static',
             '-Wl,--no-undefined',
             '-static-libgcc',
             '-static-libstdc++']
