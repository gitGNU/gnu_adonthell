#!/bin/sh

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

APP=adonthell-0.3.exe

# -- strip version from application name
appname=`echo "$APP" | sed 's/-.*//'`

# -- application folder name starts with capital
first=`echo ${appname:0:1} | tr 'a-z' 'A-Z'`
bundle=$first${appname:1}
bindir=$bundle/bin
moduledir=$bundle/modules
pythondir=$bundle/lib/python3.5

echo "Creating $bundle"

# -- cleanup existing bundle
rm -rf $bundle

# -- create bundle structure
mkdir -p $bundle
mkdir -p $bindir
mkdir -p $moduledir
mkdir -p $pythondir

cd build

# -- create resource file
cat > adonthell.rc << EOF
1 VERSIONINFO
FILEVERSION     0,3,6,0
PRODUCTVERSION  0,3,6,0
BEGIN
  BLOCK "StringFileInfo"
  BEGIN
    BLOCK "080904E4"
    BEGIN
      VALUE "CompanyName", "The Adonthell Team"
      VALUE "FileDescription", "Adonthell RPG Engine"
      VALUE "FileVersion", "0.3.6"
      VALUE "InternalName", "adonthell"
      VALUE "LegalCopyright", "© 2016 The Adonthell Team"
      VALUE "OriginalFilename", "adonthell-0.3.exe"
      VALUE "ProductName", "Adonthell"
      VALUE "ProductVersion", "0.3.6"
    END
  END

  BLOCK "VarFileInfo"
  BEGIN
    VALUE "Translation", 0x809, 1252
  END
END
EOF
windres adonthell.rc -O coff -o adonthell.res

# -- prepare application
prefix=${cwd}/${bundle}

configure_args="--disable-unix-install --disable-pyc --prefix=$prefix --datadir=$prefix --mandir=/tmp"
linker_args="-L$MINGW_PREFIX/lib $cwd/build/adonthell.res -static-libgcc -static-libstdc++"

echo "Configuring $appname. This may take a while ..."
CXXFLAGS="-I$MINGW_PREFIX/usr/local/include" LDFLAGS=$linker_args ../configure $configure_args > /dev/null
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

function copyLibs
{
    # -- find all non-standard shared libraries used by app
    for i in `ldd $1 | awk '/=>/ { print $3 }' | grep -i -v -e system32 -e '?'` ; do
  
        # -- strip path from library name
        libname=`echo "$i" | sed 's/.*\///'`

        # -- copy them to the bundle framework directory, if not there yet
        if [ ! -f $bindir/$libname ] ; then
            echo "Adding $i to $bindir"
            cp "$i" $bindir
            copyLibs $bindir/$libname
        fi
    done
}

# -- copy shared libraries used by application
copyLibs "$bindir/$APP"

function removeDbgSyms
{
    local relpath="@executable_path/../Frameworks"
    echo "Stripping debug symbols from $1"

    # -- remove debugging information
    strip -S $1
}

# -- clean
removeDbgSyms $bindir/$APP
for i in `find $bindir -name *.dll` ; do 
    removeDbgSyms "$i"
done

# -- copy required python modules from the standard library
#
# Note that this list is for Python 3.5.2 as provided by
# mingw64 and will have to be adjusted for other versions
# of Python. 
#
# Instead of hardcoding the required files, perhaps a similar
# approach as with the make_linux_appimg.sh could be used.
py3_modules="
__future__.py
_bootlocale.py
_collections_abc.py
_sitebuiltins.py
_sysconfigdata.py
_weakrefset.py
abc.py
codecs.py
collections/__init__.py
collections/abc.py
contextlib.py
copyreg.py
encodings/__init__.py
encodings/aliases.py
encodings/cp*.py
encodings/latin_1.py
encodings/mbcs.py
encodings/utf_8.py
functools.py
genericpath.py
hashlib.py
heapq.py
importlib/__init__.py
importlib/abc.py
importlib/machinery.py
importlib/util.py
imp.py
io.py
keyword.py
lib-dynload/_codecs_*-cpython-35m.dll
lib-dynload/_md5-cpython-35m.dll
lib-dynload/_random-cpython-35m.dll
lib-dynload/_sha1-cpython-35m.dll
lib-dynload/_sha256-cpython-35m.dll
lib-dynload/_sha512-cpython-35m.dll
lib-dynload/math-cpython-35m.dll
linecache.py
ntpath.py
operator.py
os.py
random.py
reprlib.py
re.py
site.py
sre_compile.py
sre_constants.py
sre_parse.py
stat.py
sysconfig.py
traceback.py
token.py
tokenize.py
types.py
warnings.py
weakref.py"

IFS='
'

for i in $py3_modules ; do
  target=`dirname $i`
  if [ ! -d $pythondir/$target ] ; then
    mkdir $pythondir/$target
  fi
  cp $MINGW_PREFIX/lib/python3.5/$i $pythondir/$target 
done
