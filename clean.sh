#!/bin/sh
# First of all clean up the generated crud
rm -f configure config.log config.cache libtool config.guess
rm -f config.status aclocal.m4 ltmain.sh config.h stamp-h stamp-h.in config.sub
rm -f doc/reference.tag
rm -f `find . -name 'Makefile.in'`
rm -f `find . -name 'Makefile'`
rm -Rf depcomp stamp-h1 a.exe autom4te.cache config.h.in
rm -Rf `find . -name '.deps'`
rm -Rf `find . -name '.libs'`
rm -f `find . -name '*~'`
rm -f `find . -name '#*'`
 


