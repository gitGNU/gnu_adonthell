## Process this file with automake to produce Makefile.in

bin_PROGRAMS = pydonthell

pydonthell_SOURCES = pydonthell.cc

CPPFLAGS +=-I../.. -DUSE_PYTHON -DUSE_MAP $(SDL_CFLAGS) $(OGG_CFLAGS) $(PY_CFLAGS) 

pydonthell_LDADD = ../../animation.o ../../audio.o ../../drawable.o \
../../audio_loop.o ../../py_callback.o \
../../character.o ../../character_base.o ../../data.o ../../data_screen.o ../../dialog.o ../../dialog_engine.o \
../../drawing_area.o ../../event.o ../../fileops.o ../../game.o ../../gametime.o ../../py_inc.o ../../py_modules.o ../../py_adonthell_wrap.o \
../../image.o ../../input.o ../../lex.prefs.o ../../mapsquares.o \
../../mapselect.o ../../maptpl.o ../../mapobject.o ../../mapcharacter.o ../../landmap.o ../../mapview.o ../../mapengine.o \
../../pnm.o ../../prefs.o ../../quest.o ../../screen.o ../../storage.o \
../../win_base.o ../../win_border.o ../../win_container.o ../../win_font.o ../../win_theme.o ../../win_scrolled.o ../../win_background.o ../../win_scrollbar.o ../../surface.o \
../../win_file.o ../../win_image.o ../../win_label.o ../../win_select.o ../../win_write.o ../../win_manager.o ../../yarg.o $(SDL_LIBS) $(OGG_LIBS) $(PY_LIBS)


lex.prefs.cc : prefs.l
	flex -olex.prefs.cc $<

%.o: %.cc
	$(CXXCOMPILE) -c $< -o $@
