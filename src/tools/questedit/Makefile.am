## Process this file with automake to produce Makefile.in

EXTRA_DIST = attributes.txt

bin_PROGRAMS = questedit mergequests

questedit_SOURCES = main.cc interface.cc callbacks.cc dirbrowser.cc \
callbacks.h dirbrowser.h interface.h main.h

questedit_LDADD = -lm -lstdc++ $(GTK_LIBS)

mergequests_SOURCES = mergequests.cc

mergequests_LDADD = quest.o storage.o fileops.o $(GTK_LIBS)

# Evil, but it works...
CPPFLAGS +=-I../.. $(GTK_CFLAGS)

LDFLAGS=

fileops.o : ../../fileops.cpp
	$(CXXCOMPILE) -c $< -o $@

quest.o : ../../quest.cpp
	$(CXXCOMPILE) -c $< -o $@

storage.o : ../../storage.cpp
	$(CXXCOMPILE) -c $< -o $@
