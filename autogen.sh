#!/bin/sh
# First of all clean up the generated crud
rm -f configure config.log config.guess config.sub config.cache
rm -f config.status aclocal.m4
rm -f `find . -name 'Makefile.in'`
rm -f `find . -name 'Makefile'`

# Regenerate everything
aclocal
automake --add-missing --gnu
autoconf 
