#!/bin/sh

cwd=`pwd`
osversion=`sw_vers -productVersion | sed 's/\(.*\)\..*$/\1/'`

if [ -z $MACOSX_DEPLOYMENT_TARGET ]; then
  export MACOSX_DEPLOYMENT_TARGET=$osversion
fi

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

# -- strip path from application name
appname=`echo "$APP" | sed 's/-.*//'`

# -- bundle name starts with capital
first=`echo ${appname:0:1} | tr 'a-z' 'A-Z'`
bundle=$first${appname:1}.app
fwdir=$bundle/Contents/Frameworks
plugindir=$bundle/Contents/PlugIns
pythondir=$bundle/Contents/Resources/modules

echo "Creating $bundle"

# -- cleanup existing bundle
rm -rf $bundle

# -- create bundle structure
mkdir -p $bundle
mkdir -p $fwdir
mkdir -p $pythondir

# -- populate bundle
echo "APPL????" > $bundle/Contents/PkgInfo
cat > $bundle/Contents/Info.plist <<EOF
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple Computer//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
        <key>MinimumOSVersion</key>
        <string>${MACOSX_DEPLOYMENT_TARGET}</string>
        <key>CFBundleDevelopmentRegion</key>
        <string>en_GB</string>
        <key>CFBundleExecutable</key>
        <string>$APP</string>
        <key>CFBundleGetInfoString</key>
        <string>Adonthell - Waste’s Edge</string>
        <key>CFBundleIconFile</key>
        <string>$appname</string>
        <key>CFBundleIdentifier</key>
        <string>org.nongnu.adonthell</string>
        <key>CFBundleInfoDictionaryVersion</key>
        <string>6.0</string>
        <key>CFBundleName</key>
        <string>$first${appname:1}</string>
        <key>CFBundlePackageType</key>
        <string>APPL</string>
        <key>CFBundleShortVersionString</key>
        <string>0.3.6</string>
        <key>CFBundleSignature</key>
        <string>????</string>
        <key>CFBundleVersion</key>
        <string>0.3.6</string>
        <key>NSMainNibFile</key>
        <string>MainMenu</string>
        <key>NSPrincipalClass</key>
        <string>NSApplication</string>
        <key>NSSupportsSuddenTermination</key>
        <string>YES</string>
        <key>NSHumanReadableCopyright</key>
        <string>Copyright ©2016, Adonthell Team</string>
        <key>NSHighResolutionCapable</key>
        <string>YES</string>
</dict>
</plist>
EOF

# -- prepare application
prefix=${cwd}/${bundle}/Contents

configure_args="--disable-unix-install --disable-pyc --prefix=$prefix/MacOS --bindir=$prefix/MacOS --datadir=$prefix"
if test $MACOSX_DEPLOYMENT_TARGET = "10.6" ; then
    configure_args="$configure_args --with-python=python2.6"
fi

cd build
echo "Configuring $appname. This may take a while ..."

# -- Python distutils used in confgure script are picky about the deployment target; make sure they are happy
MACOSX_DEPLOYMENT_TARGET=$osversion ../configure $configure_args > /dev/null
if [ $? -ne 0 ]; then
   exit 1
fi

# -- compile application
echo "Building for OSX version $MACOSX_DEPLOYMENT_TARGET and higher"
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
    for i in `otool -L $1 | awk '{ print $1 }' | grep usr/local` ; do

        # -- strip path from library name
        libname=`echo "$i" | sed 's/.*\///'`

        # -- copy them to the bundle framework directory, if not there yet
        if [ ! -f $fwdir/$libname ] ; then
            echo "Adding $libname to $fwdir"
            cp $i $fwdir
            chmod 644 $fwdir/$libname
            copyLibs $fwdir/$libname
        fi
    done
}

# -- copy shared libraries used by application
copyLibs $bundle/Contents/MacOS/$APP

function relocate
{
    local relpath="@executable_path/../Frameworks"
    local basename=`echo "$1" | sed 's/.*\///'`
    echo "Relocating $basename"

    # -- update location of referenced libraries
    for i in `otool -L $1 | awk '{ print $1 }' | grep usr/local` ; do

        libname=`echo "$i" | sed 's/.*\///'`

        if [ -f $fwdir/$libname ] ; then
            install_name_tool -change $i $relpath/$libname $1
        fi
    done

    # -- update location of shared libraries
    case $basename in
        *.dylib | *.so) 
            install_name_tool -id $relpath/$basename $1 

            # -- remove debugging information
            strip -S $1
        ;;
        *)
            strip -u -r $1
        ;;
    esac
}

# -- fix rpath
relocate $bundle/Contents/MacOS/$APP
for i in `find $fwdir -name *.dylib` ; do 
    relocate "$i"
done
