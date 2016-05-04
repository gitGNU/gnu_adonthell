#!/bin/sh
# First of all clean up the generated crud
rm -f configure config.log config.cache
rm -f config.status aclocal.m4

# Regenerate everything

aclocal -I m4
autoheader
automake --add-missing --gnu --copy
autoconf 

echo
echo "Now type './configure' to prepare Adonthell for compilation."
echo "Afterwards, 'make' will build Adonthell."
echo
