## Process this file with automake to produce Makefile.in

if HAVE_OGG_VORBIS
bin_PROGRAMS = oggloop 
endif

oggloop_LDADD = $(OGG_LIBS)
oggloop_SOURCES = oggloop.c vcedit.c \
	vcedit.h

install:
