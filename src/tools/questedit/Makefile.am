## Process this file with automake to produce Makefile.in

if HAVE_GTK
noinst_PROGRAMS = questedit mergequests
endif

EXTRA_DIST = attributes.txt

questedit_SOURCES = main.cc interface.cc callbacks.cc dirbrowser.cc \
callbacks.h dirbrowser.h interface.h main.h

questedit_LDADD = -lm -lstdc++ $(GTK_LIBS)

mergequests_SOURCES = mergequests.cc

mergequests_LDADD = ../../libadonthell.a $(GTK_LIBS)

# Evil, but it works...
CPPFLAGS +=-I../.. $(SDL_CFLAGS) $(GTK_CFLAGS)

