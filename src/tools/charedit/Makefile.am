## Process this file with automake to produce Makefile.in

EXTRA_DIST = Enter.txt Leave.txt attributes.txt

bin_PROGRAMS = charedit mergechars

charedit_SOURCES = main.cc interface.cc callbacks.cc dirbrowser.cc \
callbacks.h dirbrowser.h interface.h main.h

mergechars_SOURCES = mergechars.cc

mergechars_LDADD = character_base.o storage.o fileops.o

# Evil, but it works...
CPPFLAGS +=-I../.. `gtk-config --cflags`

LDFLAGS=-lm -lstdc++ `gtk-config --libs`

%.o : ../../%.cpp
	$(CXXCOMPILE) -c $< -o $@
