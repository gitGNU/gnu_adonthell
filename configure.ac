dnl **************************************************************
dnl Process this file with autoconf to produce a configure script.
dnl **************************************************************

AC_PREREQ(2.59)
AC_INIT([Adonthell], [0.3.6], [adonthell-general@nongnu.org], [adonthell], [http://adonthell.nongnu.org])
AC_CONFIG_SRCDIR([README])
AC_CONFIG_AUX_DIR(.)

dnl Keep the correct libtool macros in-tree
AC_CONFIG_MACRO_DIR([m4])

dnl Detect the canonical host and target build environment
AC_CANONICAL_HOST
AC_CANONICAL_TARGET

AM_INIT_AUTOMAKE
AM_CONFIG_HEADER([config.h])

dnl ********************
dnl Checks for programs.
dnl ********************

AC_PROG_LN_S
AC_PROG_CXX
AC_PROG_CXXCPP
AC_PROG_MAKE_SET
AC_PROG_RANLIB

dnl ****
dnl i18n
dnl ****

AM_GNU_GETTEXT([external])

dnl ********************
dnl Additional arguments
dnl ********************

AC_ARG_ENABLE([unix-install],
    [AS_HELP_STRING([--disable-unix-install],[disable a Unix-like install (enabled by default)])],
    [unixinst=$enableval], [unixinst=yes])
AC_ARG_ENABLE([dlgedit],
    [AS_HELP_STRING([--enable-dlgedit], [Build the Adonthell dialogue editior (disabled by default)])],
    [dlgedit=$enableval], [dlgedit=no])
AC_ARG_ENABLE([memleaks],
    [AS_HELP_STRING([--enable-memleaks], [enable check for memory leaks (disabled by default)])],
    [memleaks=$enableval], [memleaks=no])
AC_ARG_ENABLE([doc],
    [AS_HELP_STRING([--enable-doc], [build the Adonthell developer documentation (disabled by default)])],
    [builddoc=$enableval], [builddoc=no])
AC_ARG_ENABLE([py-debug],
    [AS_HELP_STRING([--disable-py-debug], [disable debugging of Python scripts (enabled by default)])],
    [pydebug=$enableval], [pydebug=yes])
AC_ARG_ENABLE([pyc],
    [AS_HELP_STRING([--disable-pyc], [Do not compile Python scripts])])
AC_ARG_WITH([ogg],
    [AS_HELP_STRING([--with-ogg=DIR], [set where the Ogg library is located])],
    [ogg=none])
AC_ARG_WITH([vorbis],
    [AS_HELP_STRING([--with-vorbis=DIR], [set where the Vorbis library is located])],
    [vorbis=none])
AC_ARG_WITH([data-dir],
    [AS_HELP_STRING([--with-data-dir=DIR], [directory where data will be installed])],
    [gamedatadir="$withval"], [gamedatadir=none])
AC_ARG_WITH([python],
    [AS_HELP_STRING([--with-python=CMD], [name of python executable])],,
    [with_python=no])
AC_ARG_WITH([py-libs],
    [AS_HELP_STRING([--with-py-libs=LIBS], [override Python libraries auto-detection])],
    [pylibs="$withval"], [pylibs=none])
AC_ARG_WITH([py-cflags],
    [AS_HELP_STRING([--with-py-cflags=FLAGS], [override Python cflags auto-detection])],
    [pycflags="$withval"], [pycflags=none])


AM_CONDITIONAL([COMPILE_SCRIPTS], [test "x$enable_pyc" != "xno"])

dnl **********************
dnl Set the data directory
dnl **********************

if test x$gamedatadir = xnone; then
  if test x$unixinst = xno; then
    DEFS="$DEFS -DSINGLE_DIR_INST"
    
    case "$target" in
      *-apple-darwin*)
        osx="yes"
        gamedatadir="${datadir}/Resources"
        DEFS="$DEFS -DDATA_DIR=\"\\\"../Resources\\\"\""
        LDFLAGS="$LDFLAGS -headerpad_max_install_names"
      ;;
      *-*-cygwin* | *-*-mingw32 | *-*-mingw64 )
        gamedatadir="${datadir}"
        DEFS="$DEFS -DDATA_DIR=\"\\\"../\\\"\""
      ;;
      *)
        gamedatadir="${datadir}/adonthell"
        DEFS="$DEFS -DDATA_DIR=\"\\\"../share/adonthell\\\"\""
      ;;
    esac        
  else
    gamedatadir="${datadir}/adonthell"
    DEFS="$DEFS -DDATA_DIR=\"\\\"$gamedatadir\\\"\""

    case "$target" in
      *-apple-darwin*)
        osx="yes"
      ;;
    esac
  fi
else
  DEFS="$DEFS -DDATA_DIR=\"\\\"$gamedatadir\\\"\""
fi

dnl **********************
dnl Check for memory leaks
dnl **********************

if test x$memleaks = xyes; then
    DEFS="$DEFS -DMEMORY_LEAKS"
fi

dnl ********************
dnl Check for libraries.
dnl ********************

AC_CHECK_LIB(pthread, main,
    PTHREADS=yes,
    PTHREADS=no)

AC_CHECK_LIB(z, main,,echo "Adonthell requires Zlib. Exitting...";exit 1)

dnl *******************
dnl Check for FreeType2
dnl *******************

AC_CHECK_FT2(6.0.0,
    [LIBS="$LIBS $FT2_LIBS"],
    AC_MSG_ERROR([*** Freetype version >= 6.0.0 not found!])
)

dnl *******************
dnl Check for OggVorbis
dnl *******************

OGG_VORBIS=no
XIPH_PATH_VORBIS(
        OGG_VORBIS="yes"
        ogg_music="enabled"
        OGG_LIBS="$VORBISFILE_LIBS $VORBIS_LIBS"
        OGG_DEFS="-DOGG_MUSIC"
        OGG_CFLAGS="$VORBIS_CFLAGS",
        AC_MSG_RESULT(Disabling OggVorbis support)
        ogg_music="disabled"
)

dnl *************
dnl Check for SDL
dnl *************

SDL_VERSION=2.0.0
AM_PATH_SDL2($SDL_VERSION,
    :,
	AC_MSG_ERROR([*** SDL version $SDL_VERSION not found!])
)
AC_CHECK_LIB([SDL2], [SDL_Init], [LIBS="$LIBS -lSDL2"])
AC_CHECK_FUNCS([SDL_GetDisplayUsableBounds])
AC_CHECK_DECLS([SDL_WINDOW_ALLOW_HIGHDPI], [], [], [[#include <SDL2/SDL_video.h>]])

AC_CHECK_LIB([SDL2_mixer], 
	[Mix_OpenAudio], 
	[LIBS="$LIBS -lSDL2_mixer"], 
	[AC_MSG_ERROR([*** SDL_mixer is required])]
)

AC_CHECK_LIB([SDL2_ttf], 
	[TTF_Init], 
	[LIBS="$LIBS -lSDL2_ttf"], 
	[AC_MSG_ERROR([*** SDL_ttf is required])]
)
AC_CHECK_FUNCS([TTF_GetFontKerningSizeGlyphs])

dnl on a mingw build, we do not want SDL to redefine 'main'
case "$target" in
  *-*-cygwin* | *-*-mingw32 | *-*-mingw64 )
     SDL_CFLAGS=`echo $SDL_CFLAGS | sed 's/ -Dmain=SDL_main//'`
     SDL_DEFS="-DSDL_MAIN_HANDLED"
  ;;
esac

dnl ****************
dnl Check for Python
dnl ****************

dnl Find Python executable
if test "x$with_python" != "xno"; then
    AC_PATH_PROG([PYTHON], [$with_python], no)
else
    AC_PATH_PROGS([PYTHON], [python3 python], no)
fi

if test x$PYTHON == xno ; then
    echo "Adonthell requires Python. Exiting..."
    exit 1
fi
  
dnl Extract the version using Python, check against 2.2+

changequote(<<, >>)
PY_HEX_VERSION=`$PYTHON -c 'import sys ; print (int(sys.hexversion))'`
PY_VERSION=`$PYTHON -c 'import sys ; print (sys.version.split(" ")[0])'`
changequote([, ])

if test $PY_HEX_VERSION -lt `printf %d 0x020201F0`; then
		echo "Sorry, you need to have Python 2.2.1 or greater installed - update your version!"
		AC_MSG_ERROR([*** Python 2.2.1 or better required])
fi

dnl Get the Path to the Python standard library

PY_PREFIX=`$PYTHON -c 'import sys; print (sys.prefix)'`
AC_SUBST(PY_PREFIX)

dnl Find the Python.h header file

if test "x$pycflags" = xnone; then
   AC_MSG_CHECKING(for Python header files)
   changequote(<<, >>)
   PYINCLUDE=`$PYTHON -c 'import distutils.sysconfig; print(distutils.sysconfig.get_config_var("INCLUDEPY"))'`
   changequote([, ])

   dnl convert windows paths to cygwin paths
   
    case "$target" in
    *-*-cygwin* | *-*-mingw32*)
        PYINCLUDE=`cd $PYINCLUDE && pwd`
        ;;
    esac
   
    if test -r "$PYINCLUDE/Python.h"; then
        PY_CFLAGS="-I$PYINCLUDE"
        AC_SUBST(PY_CFLAGS)
    else
        AC_MSG_ERROR([Could not find Python.h in $PYINCLUDE])
    fi
    AC_MSG_RESULT(found)
else
    PY_CFLAGS=${pycflags}
    AC_MSG_CHECKING(for Python cflags)
    AC_MSG_RESULT($PY_CFLAGS)
    AC_SUBST(PY_CFLAGS)
fi

   dnl Find the Python library

if test "x$pylibs" = xnone; then
   AC_MSG_CHECKING(for Python library)
   PYLIB=""
   changequote(<<, >>)
   PYLIBVER=`$PYTHON -c 'import sys; print (sys.version[:3])'`
   changequote([, ])

   dnl look for a framework build of python first
   fw=`echo "$PY_PREFIX" | sed 's/.*\(Python\.framework\).*/\1/;'`
   if test "x$fw" != x && test "x$osx" = "xyes"; then
       PY_LIBS="$PY_PREFIX/Python"
       AC_MSG_RESULT(found)
   else
       dnl Try for specific version first, then the generic version, then panic
       PYLIB=`$PYTHON -c 'import distutils.sysconfig; print(distutils.sysconfig.get_config_var("BLDLIBRARY"))'`
       if test "x$PYLIB" != x ; then
           PY_LIBS="$PYLIB $PY_LIBS"
           AC_MSG_RESULT($PYLIB)
       else
           AC_MSG_ERROR([*** Python library not found])
       fi
       
       AC_CHECK_LIB([${PYLIB:2}], main, FOUND_PYLIB=yes, FOUND_PYLIB=no)       
       if test "$FOUND_PYLIB" == "no" ; then
       	   AC_MSG_CHECKING(for Python library location)      	   
       	   PYLIBDIR=`$PYTHON -c 'import distutils.sysconfig; print(distutils.sysconfig.get_config_var("LIBPL"))'`
	       if test "x$PYLIBDIR" != x ; then
	           PY_LIBS="-L$PYLIBDIR $PY_LIBS"
	           AC_MSG_RESULT($PYLIBDIR)
	       else
	           AC_MSG_ERROR([*** Python library location not found])
	       fi
       fi
   fi

   dnl Get the libraries that a static python library depends on
   AC_MSG_CHECKING(for Python's dependencies)

   py_extradeps=`$PYTHON -c 'import distutils.sysconfig; print (distutils.sysconfig.get_config_var("LIBS"))'`
   py_sysextradeps=`$PYTHON -c 'import distutils.sysconfig; print (distutils.sysconfig.get_config_var("SYSLIBS"))'`

   PY_DEPS="$py_extradeps $py_sysextradeps"

   AC_MSG_RESULT($PY_DEPS)

    dnl only GNU ld seems to know -E flag
    
    if $LD -v 2>&1 </dev/null | egrep '(GNU|with BFD)' 1>&5; then
        PY_LIBS="-Wl,-E $PY_LIBS $PY_DEPS"
    else
        PY_LIBS="$PY_LIBS $PY_DEPS"
    fi
    AC_SUBST(PY_LIBS)
else
    PY_LIBS=${pylibs}
    AC_MSG_CHECKING(for Python ld flags)
    AC_MSG_RESULT($PY_LIBS)
    AC_SUBST(PY_LIBS)
fi

if test "$pydebug" == "yes" ; then
	DEFS="$DEFS -DPY_DEBUG"
fi

dnl **************
dnl Check for SWIG
dnl **************

dnl minimum SWIG version depends on version of the Python interpreter

if test $PY_HEX_VERSION -ge `printf %d 0x03050000` ; then
	swig_req_major=3
	swig_req_minor=0
	swig_req_micro=8
elif test $PY_HEX_VERSION -ge `printf %d 0x03030000` ; then
	swig_req_major=3
	swig_req_minor=0
	swig_req_micro=0
elif test $PY_HEX_VERSION -ge `printf %d 0x03020000` ; then
	swig_req_major=2
	swig_req_minor=0
	swig_req_micro=4
elif test $PY_HEX_VERSION -ge `printf %d 0x03000000` ; then
	swig_req_major=1
	swig_req_minor=3
	swig_req_micro=37
elif test $PY_HEX_VERSION -ge `printf %d 0x02050000` ; then
	swig_req_major=1
	swig_req_minor=3
	swig_req_micro=30
else
dnl that is the absolute minimum version for Python < 2.5
	swig_req_major=1
	swig_req_minor=3
	swig_req_micro=24
fi

SWIG_MINVER=$swig_req_major.$swig_req_minor.$swig_req_micro
haveswig=no


AC_PATH_PROGS([SWIG], [swig3.0 swig2.0 swig], no)
if test "$SWIG" != "no" ; then
    $SWIG -version &> swig.ver

    changequote(<<, >>)
    swig_major_ver=`sed 's/.* \([0-9]*\)\.[0-9]*\.[0-9]*.*/\1/p; d' swig.ver`
    swig_minor_ver=`sed 's/.* [0-9]*\.\([0-9]*\)\.[0-9]*.*/\1/p; d' swig.ver`
    swig_micro_ver=`sed 's/.* [0-9]*\.[0-9]*\.\([0-9]*\).*/\1/p; d' swig.ver`
    changequote([, ])

    rm -f swig.ver

    SWIG_VERSION="$swig_major_ver.$swig_minor_ver.$swig_micro_ver"

    if test $swig_major_ver -gt $swig_req_major -o \
            $swig_major_ver -eq $swig_req_major -a $swig_minor_ver -gt $swig_req_minor -o \
            $swig_major_ver -eq $swig_req_major -a $swig_minor_ver -eq $swig_req_minor -a $swig_micro_ver -ge $swig_req_micro;
    then
        haveswig=yes
    fi
fi

dnl SWIG is needed to compile the code from GIT 
dnl or after making changes to the distributed code

if test "x$haveswig" = "xno" ; then
    if test -d ".git"; then
        echo "*** To compile Adonthell from GIT, you will need SWIG ${SWIG_MINVER} or higher."
        echo "*** You can obtain the most recent version from http://www.swig.org."
        echo "***"
        echo "*** As an alternative, you can try using an older version of Python"
        echo "*** by specifying the --with-python=<python-exe> configure option"
        exit 1;
    else
        echo "*** NOTE: If you plan to make changes to the code you will need"
        echo "*** SWIG ${SWIG_MINVER} or higher. SWIG can be found at http://www.swig.org."
    fi
fi

dnl ******************************
dnl Check whether to build dlgedit
dnl ******************************

AC_MSG_CHECKING(whether to build the various editors)
if test "x$dlgedit" = "xno" ; then
	AC_MSG_RESULT(no)
else
	AC_MSG_RESULT(yes) 	 
fi

AM_CONDITIONAL(BUILD_DLGEDIT, test x$dlgedit = xyes)

AS_IF([test "x$dlgedit" = "xyes"], [

dnl *****************
dnl Cairo
dnl *****************

	PKG_CHECK_MODULES([CAIRO], [cairo >= 1.2.6])
	AC_SUBST(CAIRO_CFLAGS)
	AC_SUBST(CAIRO_LIBS)

dnl *****************
dnl GTK+
dnl *****************

	PKG_CHECK_MODULES(GTK, [gtk+-2.0 >= 2.18.0])
	AC_SUBST(GTK_CFLAGS)
	AC_SUBST(GTK_LIBS)

dnl *****************
dnl OSX Integration
dnl *****************

	_gdk_tgt=`$PKG_CONFIG --variable=target gdk-2.0`
	if test "x$_gdk_tgt" = xquartz; then 
	   PKG_CHECK_MODULES(IGE_MAC, ige-mac-integration)
	   AC_SUBST(IGE_MAC_LIBS)
	   AC_SUBST(IGE_MAC_CFLAGS)
	fi
])

dnl ****************************************
dnl Check whether to build the documentation
dnl ****************************************

AC_MSG_CHECKING(whether to build the developer documentation)
if test x$builddoc = xno; then
	AC_MSG_RESULT(no)
else
	AC_MSG_RESULT(yes)
fi

AM_CONDITIONAL(BUILD_DOC, test x$builddoc = xyes)

dnl *****************************************************
dnl Doxygen and dot are needed to build the documentation
dnl *****************************************************

if test x$builddoc = xyes; then
        AC_PATH_PROG(P_DOXYGEN,doxygen,no)
        if test x$P_DOXYGEN == xno ; then
                echo "Adonthell documentation requires doxygen. Exiting..."
                exit 1
        fi
        AC_PATH_PROG(P_DOT,dot,no)
        if test x$P_DOT == xno ; then
                echo "Adonthell documentation requires dot. Exiting..."
                exit 1
        fi
fi

dnl  **************************
dnl  Check for required headers
dnl  **************************

AC_CHECK_HEADER([SDL2/SDL.h])
AC_CHECK_HEADER([SDL2/SDL_mixer.h])
AC_CHECK_HEADER([SDL2/SDL_ttf.h])
AC_CHECK_HEADER([zlib.h])

if test x$PTHREADS = xyes; then
   AC_CHECK_HEADER([pthread.h])
fi

if test x$OGG_VORBIS = xyes; then
   AC_CHECK_HEADER([vorbis/vorbisfile.h]) 
fi 

AM_CONDITIONAL(HAVE_OGG_VORBIS, test x$OGG_VORBIS = xyes)

dnl ********************************
dnl Generate our compiler arguements
dnl ********************************

CXXFLAGS="$CXXFLAGS $DEFS $INCLUDES"

AC_SUBST(CXX) 
AC_SUBST(CXXFLAGS) 
AC_SUBST(CPPFLAGS)
AC_SUBST(LIBS) 
AC_SUBST(SDL_CFLAGS)
AC_SUBST(SDL_LIBS)
AC_SUBST(SDL_DEFS)
AC_SUBST(OGG_CFLAGS)
AC_SUBST(OGG_LIBS)
AC_SUBST(OGG_DEFS)
AC_SUBST(PY_CFLAGS)
AC_SUBST(PY_LIBS)
AC_SUBST(SWIG)
AC_SUBST(gamedatadir)

AC_CONFIG_FILES([ 
Makefile
adonthell.spec
doc/Makefile
doc/devel/Makefile
doc/dlgedit/Makefile
doc/dlgedit-api/Makefile
doc/items/Makefile
m4/Makefile
po/Makefile.in 
src/Makefile
src/modules/Makefile
src/tools/Makefile
src/tools/dlgedit/Makefile
])
AC_OUTPUT

dnl ****************
dnl Configure Status
dnl ****************

echo
echo "Adonthell v$VERSION"
echo
echo "SDL ....................... : `sdl2-config --version`"
echo "Python .................... : $PY_VERSION"
echo "Python debugging .......... : $pydebug"
echo "Freetype .................. : `freetype-config --version`"
if test "x$haveswig" = "xyes"; then
echo "SWIG ...................... : $SWIG_VERSION"
fi
if test "x$dlgedit" = "xyes"; then 	 
echo "Cairo ..................... : `pkg-config --modversion cairo`"
echo "GTK+ ...................... : `pkg-config --modversion gtk+-2.0`" 	 
fi
echo "Ogg music ................. : $ogg_music"
echo "Building dlgedit .......... : $dlgedit"
echo "Building API doc .......... : $builddoc"
if test "x$memleaks" = "xyes"; then
echo "Test for Memory Leaks ..... : enabled"
fi
if test x$unixinst = xno; then
    echo "Data directory ............ : Single Directory install"
else
    echo "Data directory ............ : ${gamedatadir}"
fi

echo
echo "Now type 'make' to build Adonthell"
echo "Afterwards, 'make install' will install Adonthell in $prefix/bin"
echo


