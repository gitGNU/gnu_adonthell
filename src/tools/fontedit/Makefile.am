## Process this file with automake to produce Makefile.in

noinst_PROGRAMS = fontedit

fontedit_SOURCES = font_editor.cc win_font_editor.cc \
	win_font_editor.h

fontedit_LDADD = ../../libadonthell.a $(SDL_LIBS) $(PY_LIBS) $(GTK_LIBS)

CPPFLAGS +=-I../.. -D_EDIT_ -DUSE_MAP $(SDL_CFLAGS)

