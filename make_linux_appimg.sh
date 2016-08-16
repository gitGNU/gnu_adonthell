#!/bin/sh

# -- check for virtualenv 
if [ ! -x "$(command -v virtualenv)" ]; then
  echo "This script requires virtualenv"
  echo "See https://pypi.python.org/pypi/virtualenv"
  exit 1
fi

cwd=`pwd`

# -- build adonthell
if [ ! -f "configure" ]; then
  if [ ! -f "autogen.sh" ]; then
    echo "This script must be run in the adonthell-0.3.x directory"
    exit 1
  fi
  ./autogen.sh
fi

if [ -f "Makefile" ]; then
   make distclean
fi

if [ -d "build" ]; then
  rm -rf build
fi

mkdir build

APP=adonthell-0.3

# -- strip version from application name
appname=`echo "$APP" | sed 's/-.*//'`

# -- application folder name starts with capital| awk '{ print $3 }'
uppercase=`echo $appname | sed 's/./\U&/'`
appdir="$uppercase.AppDir"
bundle="$appdir/usr"
bindir=$bundle/bin
libdir=$bundle/lib

# -- cleanup existing bundle
rm -rf $appdir

# -- TODO: need a better way to copy all dependencies on the standard
#          python library in one fell swoop

# -- prepare python
virtualenv -p python --always-copy --no-setuptools --no-pip $bundle
#virtualenv --relocatable $bundle
rm -rf $bundle/bin
rm -rf $bundle/include
rm -rf $bundle/local

# -- we also require random.py and its dependencies
pylibdir=`python -c 'import distutils.sysconfig; print(distutils.sysconfig.get_config_var("LIBP"))'`
for i in `python -m modulefinder -x unittest $pylibdir/random.py | grep ^m | awk '{ print $3 }'` ; do
    if test "x$i" = "x" ; then
        continue
    fi
    cp -n "$i" $appdir$i
done
cp $pylibdir/random.py $appdir$pylibdir

cd build

# -- prepare application
prefix=${cwd}/${bundle}

configure_args="--disable-unix-install --disable-pyc --with-python=python --prefix=$prefix --mandir=/tmp"

echo "Configuring $appname. This may take a while ..."
../configure $configure_args > /dev/null
if [ $? -ne 0 ]; then
   exit 1
fi

# -- compile application
echo "Building $appname ..."
make V=0 -j 2
if [ $? -ne 0 ]; then
   exit 1
fi


# -- install application
make V=0 install
if [ $? -ne 0 ]; then
   exit 1
fi

cd ..

copyLibs()
{
    # -- find all non-standard shared libraries used by app
    for i in `ldd $1 | awk '{ print $3 }' | grep -i -e /usr -e libpng` ; do
  
        # -- strip debug symbols
        strip -S "$1" 

        # -- strip path from library name
        libname=`echo "$i" | sed 's/.*\///'`

        # -- skip some system libs under /usr
        if test $libname = "libstdc++.so.6" ; then
            continue
        fi

        # -- copy them to the bundle framework directory, if not there yet
        if [ ! -f $libdir/$libname ] ; then
            echo "Adding $i to $libdir"
            cp "$i" $libdir
            copyLibs "$libdir/$libname"
        fi
    done
}

# -- copy shared libraries used by application
copyLibs "$bindir/$APP"

# -- copy libvorbis and dependencies
libvorbispath=`pkg-config vorbis --variable=libdir`
if test "x$libvorbispath" != "x" ; then
  cp "$libvorbispath/libvorbis.so" "$libdir"
  copyLibs "$libdir/libvorbis.so"
else
  echo "*** Cannot find libvorbis. Background music will not work ..."
fi

