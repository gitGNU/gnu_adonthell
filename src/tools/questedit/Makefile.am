## Process this file with automake to produce Makefile.in

if HAVE_GTK
bin_PROGRAMS = questedit mergequests
endif

EXTRA_DIST = attributes.txt

questedit_SOURCES = main.cc interface.cc callbacks.cc dirbrowser.cc \
callbacks.h dirbrowser.h interface.h main.h

questedit_LDADD = -lm -lstdc++ $(GTK_LIBS)

mergequests_SOURCES = mergequests.cc

mergequests_LDADD = quest.o storage.o fileops.o $(GTK_LIBS)

# Evil, but it works...
CPPFLAGS +=-I../.. $(GTK_CFLAGS)

LDFLAGS=

fileops.o : ../../fileops.cc
	$(CXXCOMPILE) -c $< -o $@

quest.o : ../../quest.cc
	$(CXXCOMPILE) -c $< -o $@

storage.o : ../../storage.cc
	$(CXXCOMPILE) -c $< -o $@

