## Process this file with automake to produce Makefile.in

EXTRA_DIST = Enter.txt Leave.txt attributes.txt

bin_PROGRAMS = charedit mergechars

charedit_SOURCES = main.cc interface.cc callbacks.cc dirbrowser.cc \
callbacks.h dirbrowser.h interface.h main.h

mergechars_SOURCES = mergechars.cc

mergechars_LDADD = ../../character.o ../../storage.o ../../event.o \
 ../../fileops.o

# Evil, but it works...
CPPFLAGS +=-I../.. -D_DLGEDIT_ -D_DEBUG_ -D_VERSION_=\"0.7\" `gtk-config --cflags`

LDFLAGS=-lm -lstdc++ `gtk-config --libs`

../../fileops.o : ../../fileops.cpp
	$(CXXCOMPILE) -c $< -o $@

../../character.o : ../../character.cpp
	$(CXXCOMPILE) -c $< -o $@

../../event.o : ../../event.cpp
	$(CXXCOMPILE) -c $< -o $@

../../storage.o : ../../storage.cpp
	$(CXXCOMPILE) -c $< -o $@
