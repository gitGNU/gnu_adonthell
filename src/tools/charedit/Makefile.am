## Process this file with automake to produce Makefile.in

if HAVE_GTK
noinst_PROGRAMS = charedit mergechars
endif

EXTRA_DIST = Enter.txt Leave.txt attributes.txt

charedit_SOURCES = main.cc interface.cc callbacks.cc dirbrowser.cc \
callbacks.h dirbrowser.h interface.h main.h
charedit_LDADD = -lm -lstdc++ $(GTK_LIBS) 

mergechars_SOURCES = mergechars.cc
mergechars_LDADD = ../../libadonthell.a $(GTK_LIBS)

# Evil, but it works...
AM_CPPFLAGS =-I../.. $(SDL_CFLAGS) $(GTK_CFLAGS) $(PY_CFLAGS)
