scons-x-compile
==================================

Cross-Compilation (windows/linux) `SCons` build helper.

Serves as examples in an article on cross-compiling an SDL project for windows, from linux.

 * http://swarminglogic.com/article/2014_11_crosscompile
 * http://swarminglogic.com/article/2014_11_crosscompile2

The repository has many branches that can be checked out, each adding to the prerequisites.

Cross-compilation is done using `x86_64-w64-mingw32`. On debian/ubuntu, install using:

```bash
sudo apt-get install g++-mingw-w64 mingw-w64-{tools,x86-64-dev}
```

Build setup uses `SCons`. On debian/ubuntu, install with:
```bash
sudo apt-get install scons
```

## How to use:

 1. Cross-compile all the prequisites and SDL libraries and GLEW as described further down.
 1. Check out a desired branch, e.g. `git checkout hello-sdl`
 1. Build for linux with `scons` or windows `scons --win64`

## Branches and utilized libraries:

| Branch              | Libraries Used         | Target Executable   |
| :------------------ | :----------------------| :------------------ |
| `basic`             | None                   | `main`              |
| `hello-sdl`         | `SDL2`                 | `Hello_SDL`         |
| `hello-sdl-image`   | ` -"- ` + `SDL2_image` | `Hello_SDL_image`   |
| `hello-sdl-mixer`   | ` -"- ` + `SDL2_mixer` | `Hello_SDL_mixer`   |
| `hello-sdl-ttf`     | ` -"- ` + `SDL2_ttf`   | `Hello_SDL_ttf`     |
| `hello-opengl`      | ` -"- ` + `GLEW`       | `Hello_OpenGL`      |


## Cross-compiling libraries using helper script

| Library             | `crosscompile_libs.sh` arguments        |
| :-----------------  | :--------------------------------       |
| `zlib`              | --x-comp-zlib                           |
| `libpng`            | --x-comp-libpng                         |
| `libjpeg`           | --x-comp-libjpeg                        |
| `libogg`            | --x-comp-libogg                         |
| `libvorbis`         | --x-comp-libvorbis                      |
| `libfreetype`       | --x-comp-libfreetype                    |
| `glew`              | --x-comp-glew                           |
| `SDL2`              | --x-comp-sdl  [SDL2_SRC_DIR]            |
| `SDL2_image`        | --x-comp-sdl-image [SDL2_IMAGE_SRC_DIR] |
| `SDL2_mixer`        | --x-comp-sdl-mixer [SDL2_MIXER_SRC_DIR] |
| `SDL2_ttf`          | --x-comp-sdl-ttf" [SDL2_TTF_SRC_DIR]    |

Example `libogg` compile:
```bash
./crosscompile_libs.sh --x-comp-libogg
```

Example `SDL2` compile:
```bash
./crosscompile_libs.sh --x-comp-sdl /opt/sdl2/SDL
```

If environment variables `SDL2_SRC_DIR`, `SDL2_IMAGE_SRC_DIR`, `SDL2_MIXER_SRC_DIR`, and
`SDL2_TTF_SRC_DIR` are set, you can cross-compile all libraries, including prerequisites with
```bash
./crosscompile_libs.sh --x-comp-all
```



## Cross-compile `zlib` (prereq)
```bash
# We do this in /tmp
cd /tmp

# Download zlib 1.2.8
wget http://zlib.net/zlib128.zip
unzip zlib128.zip
cd zlib-1.2.8

# Specify our toolset in the makefile
perl -i -pe 's,(PREFIX =)$,$1 x86_64-w64-mingw32-,' win32/Makefile.gcc

# Call make
make -f win32/Makefile.gcc

# Install it to our cross-tools directory
PREFIX=/usr/local/cross-tools/
TOOLSET=x86_64-w64-mingw32
sudo DESTDIR=$PREFIX/$TOOLSET/ \
    INCLUDE_PATH=include  LIBRARY_PATH=lib  BINARY_PATH=bin \
    make install -f win32/Makefile.gcc
```

## Cross-compile `libpng` (prereq for SDL2_image, SDL2_ttf)
```bash
# We do this in /tmp
cd /tmp

# Download libpng 1.2.50
wget http://download.sourceforge.net/libpng/libpng-1.2.50.tar.gz
tar xf libpng-1.2.50.tar.gz
cd libpng-1.2.50

# Configure
PREFIX=/usr/local/cross-tools/
TOOLSET=x86_64-w64-mingw32
CROSSPATH=$PREFIX/$TOOLSET
export CFLAGS="-I${CROSSPATH}/include"
export LDFLAGS="-L${CROSSPATH}/lib"
./configure --target=$TOOLSET --host=$TOOLSET \
             --build=x86_64-linux --prefix=$CROSSPATH

# make and install
make
sudo make install
```

## Cross-compile `libjpeg` (prereq for SDL2_image)
```bash
# We do this in /tmp
cd /tmp

# check if you have nasm assembler, if not, install it
if ! command -v nasm ; then
  sudo apt-get install nasm
fi

# Download libjpeg-turbo 1.3.0
wget "http://downloads.sourceforge.net/project/"\
"libjpeg-turbo/1.3.0/libjpeg-turbo-1.3.0.tar.gz"
tar xf libjpeg-turbo-1.3.0.tar.gz
cd libjpeg-turbo-1.3.0/

PREFIX=/usr/local/cross-tools/
TOOLSET=x86_64-w64-mingw32
CROSSPATH=$PREFIX/$TOOLSET
export CFLAGS="-I${CROSSPATH}/include"
export LDFLAGS="-L${CROSSPATH}/lib"
./configure --target=$TOOLSET --host=$TOOLSET \
             --build=x86_64-linux --prefix=$CROSSPATH

# Make and install
make
sudo make install
```


## Cross-compile `libogg` (prereq for SDL2_mixer)
```bash
# We do this in /tmp
cd /tmp

# Download libogg 1.3.1
wget http://downloads.xiph.org/releases/ogg/libogg-1.3.1.tar.gz
tar xf libogg-1.3.1.tar.gz
cd libogg-1.3.1/

PREFIX=/usr/local/cross-tools/
TOOLSET=x86_64-w64-mingw32
CROSSPATH=$PREFIX/$TOOLSET
./configure --target=$TOOLSET --host=$TOOLSET \
             --build=x86_64-linux --prefix=$CROSSPATH

# Make and install
make
sudo make install
```

## Cross-compile `libvorbis` (prereq for SDL2_mixer)
```bash
# We do this in /tmp
cd /tmp

# Download libvorbis 1.3.2
wget http://downloads.xiph.org/releases/vorbis/libvorbis-1.3.2.tar.gz
tar xf libvorbis-1.3.2.tar.gz
cd libvorbis-1.3.2/

PREFIX=/usr/local/cross-tools/
TOOLSET=x86_64-w64-mingw32
CROSSPATH=$PREFIX/$TOOLSET
export CFLAGS="-I${CROSSPATH}/include"
export LDFLAGS="-L${CROSSPATH}/lib"
./configure --target=$TOOLSET --host=$TOOLSET \
             --build=x86_64-linux --prefix=$CROSSPATH

# Make and install
make
sudo make install
```

## Cross-compile `libfreetype` (prereq for SDL2_ttf)
```bash
# We do this in /tmp
cd /tmp

# Download freetype-2.5.2
wget http://download.savannah.gnu.org/releases/freetype/freetype-2.5.2.tar.gz
tar xf freetype-2.5.2.tar.gz
cd freetype-2.5.2/

PREFIX=/usr/local/cross-tools/
TOOLSET=x86_64-w64-mingw32
CROSSPATH=$PREFIX/$TOOLSET
PATH=${CROSSPATH}/bin:$PATH
export CFLAGS="-I${CROSSPATH}/include"
export LDFLAGS="-L${CROSSPATH}/lib"
export LIBPNG="`libpng-config --libs`"
export LIBPNG_CFLAGS="`libpng-config --cflags`"
export LIBPNG_LDFLAGS="`libpng-config --ldflags`"

./configure --target=$TOOLSET --host=$TOOLSET \
             --build=x86_64-linux --prefix=$CROSSPATH

# Make and install
make
sudo make install
```


## Cross-compiling `SDL2`:
```bash
# From SDL source directory
# create build-win64 directory and cd to it
mkdir build-win64 && cd $_

# Set up some common variables
PREFIX=/usr/local/cross-tools/
TOOLSET=x86_64-w64-mingw32
# We make CC variablee available to child processes
export CC="$TOOLSET-gcc -static-libgcc"

# Configure build
../configure --target=$TOOLSET --host=$TOOLSET \
             --build=x86_64-linux --prefix=$PREFIX/$TOOLSET
# building and installing
make
sudo make install
```

## Cross-compiling `SDL2_image`:
```bash
# From SDL_image source directory
# create build-win64 directory and cd to it
mkdir build-win64 && cd $_

# Set up some common variables
PREFIX=/usr/local/cross-tools/
TOOLSET=x86_64-w64-mingw32
CROSSPATH=$PREFIX/$TOOLSET

export CC="$TOOLSET-gcc -static-libgcc"
export PKG_CONFIG_PATH=${CROSSPATH}/lib/pkgconfig
export PATH=${CROSSPATH}/bin:$PATH
export CFLAGS="-I${CROSSPATH}/include"
export LDFLAGS="-L${CROSSPATH}/lib"

# Configure build (it should find both libjpeg and libpng now)
../configure --target=$TOOLSET --host=$TOOLSET \
             --build=x86_64-linux --prefix=$CROSSPATH \
             --disable-webp

# Build and install
make
sudo make install
```

## Cross-compiling `SDL2_mixer`:
```bash
# From SDL_mixer source directory
# create build-win64 directory and cd to it
mkdir build-win64 && cd $_

# Set up some common variables
PREFIX=/usr/local/cross-tools/
TOOLSET=x86_64-w64-mingw32
CROSSPATH=$PREFIX/$TOOLSET

export CC="$TOOLSET-gcc -static-libgcc"
export PKG_CONFIG_PATH=${CROSSPATH}/lib/pkgconfig
export PATH=${CROSSPATH}/bin:$PATH
export CFLAGS="-I${CROSSPATH}/include"
export LDFLAGS="-L${CROSSPATH}/lib"

# Configure build (it should find ogg and vorbis)
../configure --target=$TOOLSET --host=$TOOLSET \
             --build=x86_64-linux --prefix=$CROSSPATH \
             --disable-music-mp3-smpeg

# Build and install
make
sudo make install
```


## Cross-compiling `SDL2_ttf`:
```bash
# From SDL_ttf source directory
# create build-win64 directory and cd to it
mkdir build-win64 && cd $_

# Set up some common variables
PREFIX=/usr/local/cross-tools/
TOOLSET=x86_64-w64-mingw32
CROSSPATH=$PREFIX/$TOOLSET

export CC="$TOOLSET-gcc -static-libgcc"
export PKG_CONFIG_PATH=${CROSSPATH}/lib/pkgconfig
export PATH=${CROSSPATH}/bin:$PATH
export CFLAGS="-I${CROSSPATH}/include"
export LDFLAGS="-L${CROSSPATH}/lib"

# Configure build (it should find libfreetype now)
../configure --target=$TOOLSET --host=$TOOLSET \
             --build=x86_64-linux --prefix=$CROSSPATH

# Build and install
make
sudo make install
```
