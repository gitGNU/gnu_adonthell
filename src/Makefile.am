## Process this file with automake to produce Makefile.in

if BUILD_TOOLS
SUBDIRS = . tools
endif

moddatadir = $(gamedatadir)/modules

moddata_DATA = adonthell.py

bin_PROGRAMS = adonthell

EXTRA_DIST = .indent.pro prefs.l py_adonthell.i adonthell.py

CPPFLAGS += $(SDL_DEFS) $(OGG_DEFS) $(SDL_CFLAGS) $(OGG_CFLAGS) $(PY_CFLAGS)

noinst_LIBRARIES = libadonthell.a

libadonthell_a_SOURCES = animation.cc audio.cc audio_loop.cc \
character_base.cc character.cc gamedata.cc data_screen.cc dialog.cc \
dialog_engine.cc drawable.cc drawing_area.cc event.cc fileops.cc game.cc \
gametime.cc image.cc input.cc landmap.cc lex.prefs.cc \
mapsquare_walkable.cc mapcharacter.cc mapengine.cc \
mapsquare.cc mapobject.cc mapview.cc path.cc pnm.cc prefs.cc \
py_callback.cc python.cc py_adonthell_wrap.cc py_script.cc quest.cc \
screen.cc surface.cc storage.cc text_bubble.cc win_background.cc win_base.cc win_border.cc \
win_container.cc win_font.cc win_keys.cc\
win_manager.cc win_scrollbar.cc win_scroll.cc win_select.cc win_theme.cc \
label.cc  label_input.cc win_event.cc yarg.cc \
animation.h audio.h audio_loop.h callback.h py_callback.h \
character_base.h character.h gamedata.h data_screen.h dialog.h dialog_engine.h \
drawable.h drawing_area.h event.h fileops.h game.h gametime.h image.h input.h inventory.h \
item.h landmap.h mapsquare.h mapsquare_walkable.h mapcharacter.h mapobject.h mapview.h \
mapengine.h path.h pnm.h prefs.h python.h py_script.h quest.h screen.h surface.h storage.h \
types.h text_bubble.h win_background.h win_base.h win_border.h win_container.h \
win_font.h win_image.h win_label.h win_mapview.h label.h label_input.h \
win_scrollbar.h win_scroll.h win_select.h win_theme.h win_keys.h win_event.h win_types.h \
win_write.h win_manager.h win_object.h window.h win_wrappers.h yarg.h \
mixer.c music.c music_cmd.c music_cmd.h music_ogg.c music_ogg.h wave.h \
wavestream.c wavestream.h SDL_mixer.h

libadonthell_LDADD = $(SDL_LIBS) $(OGG_LIBS) $(PY_LIBS)

adonthell_LDADD = libadonthell.a $(libadonthell_LDADD)

adonthell_SOURCES = main.cc


# Note: adonthell.py is also built by this target. 
py_adonthell_wrap.cc : py_adonthell.i *.h
	swig -python -shadow ${SDL_CFLAGS} -I./ -I../ -c++ -make_default -o $*.cc py_adonthell.i

lex.prefs.cc : prefs.l
	flex -olex.prefs.cc $<
