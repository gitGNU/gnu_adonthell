## Process this file with automake to produce Makefile.in

noinst_PROGRAMS = pydonthell

pydonthell_SOURCES = pydonthell.cc

CPPFLAGS +=-I../.. -DUSE_MAP $(SDL_CFLAGS) $(OGG_CFLAGS) $(PY_CFLAGS) 

pydonthell_LDADD = ../../libadonthell.a $(SDL_LIBS) $(OGG_LIBS) $(PY_LIBS)

