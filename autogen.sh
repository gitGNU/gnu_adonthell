#!/bin/sh
# First of all clean up the generated crud
rm -f configure config.log config.guess config.sub config.cache
rm -f config.status aclocal.m4
rm -f `find . -name 'Makefile.in'`
rm -f `find . -name 'Makefile'`

# Regenerate everything
aclocal -I .
automake --add-missing --gnu
autoconf 

echo
echo "Now type './configure' to prepare Adonthell for compilation."
echo "Afterwards, 'make' will build Adonthell."
echo
