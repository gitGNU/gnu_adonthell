## Process this file with automake to produce Makefile.in

bin_PROGRAMS = pydonthell

pydonthell_SOURCES = pydonthell.cpp

CPPFLAGS +=-DUSE_PYTHON -DUSE_MAP

LDADD = ../../animation.o ../../animation_off.o ../../audio.o \
../../audio_loop.o \
../../character.o ../../character_base.o ../../data.o ../../data_screen.o ../../dialog.o ../../dialog_engine.o \
../../drawing_area.o ../../event.o ../../fileops.o ../../game.o ../../gametime.o ../../py_inc.o ../../py_modules.o ../../py_adonthell_wrap.o \
../../image.o ../../input.o ../../lex.prefs.o \
../../mapselect.o ../../maptpl.o ../../mapobject.o ../../mapcharacter.o ../../landmap.o ../../mapview.o ../../mapengine.o \
../../pnm.o ../../prefs.o ../../quest.o ../../screen.o ../../storage.o \
../../win_base.o ../../win_border.o ../../win_container.o ../../win_font.o ../../win_theme.o ../../win_scrolled.o ../../win_background.o ../../win_scrollbar.o \
../../win_file.o ../../win_image.o ../../win_label.o ../../win_select.o ../../win_write.o ../../yarg.cc

# Evil, but it works...
CPPFLAGS +=-I../..

lex.prefs.cpp : prefs.l
	flex -olex.prefs.cpp $<

%.o: %.cpp
	$(CXXCOMPILE) -c $< -o $@
