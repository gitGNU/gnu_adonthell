## Process this file with automake to produce Makefile.in

if BUILD_TOOLS
SUBDIRS = . tools
endif

moddatadir = $(gamedatadir)/modules

#moddata_DATA = adonthell.pyc

bin_PROGRAMS = alextest joltest

EXTRA_DIST = .indent.pro prefs.l py_adonthell.i adonthell.py

CPPFLAGS += $(SDL_DEFS) $(OGG_DEFS) $(SDL_CFLAGS) $(OGG_CFLAGS) $(PY_CFLAGS) $(FTT2_CFLAGS)

noinst_LIBRARIES = libadonthell.a

CLEANFILES = $(moddata_SCRIPTS)

#libadonthell_a_SOURCES = adonthell.cc animation.cc audio.cc \
#character_base.cc character.cc gamedata.cc data_screen.cc dialog.cc \
#dialog_screen.cc drawable.cc drawing_area.cc event.cc fileops.cc game.cc \
#gametime.cc image.cc input.cc landmap.cc lex.prefs.cc \
#mapsquare_walkable.cc mapcharacter.cc \
#mapsquare.cc mapobject.cc mapview.cc path.cc pnm.cc prefs.cc \
#py_callback.cc python_class.cc py_adonthell_wrap.cc py_object.cc quest.cc \
#screen.cc surface.cc storage.cc text_bubble.cc win_background.cc win_base.cc win_border.cc \
#win_container.cc win_font.cc win_keys.cc\
#win_manager.cc win_scrollbar.cc win_scroll.cc win_select.cc win_theme.cc \
#label.cc  label_input.cc win_event.cc yarg.cc \
#adonthell.h animation.h audio.h callback.h py_callback.h \
#character_base.h character.h gamedata.h data_screen.h dialog.h dialog_screen.h \
#drawable.h drawing_area.h event.h fileops.h game.h gametime.h image.h input.h inventory.h \
#item.h landmap.h mapsquare.h mapsquare_walkable.h mapcharacter.h mapobject.h mapview.h \
#path.h pnm.h prefs.h python_class.h py_object.h quest.h screen.h surface.h storage.h \
#types.h text_bubble.h win_background.h win_base.h win_border.h win_container.h \
#str_hash.h win_font.h win_image.h win_label.h win_mapview.h label.h label_input.h \
#win_scrollbar.h win_scroll.h win_select.h win_theme.h win_keys.h win_event.h win_types.h \
#win_write.h win_manager.h win_object.h window.h win_wrappers.h yarg.h \
#mixer.c music.c music_cmd.c music_cmd.h music_ogg.c music_ogg.h wave.h \
#wavestream.c wavestream.h SDL_mixer.h

#libadonthell_LDADD = $(SDL_LIBS) $(OGG_LIBS) $(PY_LIBS)

#adonthell_LDADD = libadonthell.a $(libadonthell_LDADD)

#adonthell_SOURCES = main.cc

libadonthell_a_SOURCES = animation.cc drawable.cc drawing_area.cc fileops.cc \
	image.cc input.cc gametime.cc map_coordinates.cc mapsquare_obj_area.cc \
	mapsquare_obj_area_gfx.cc map_placeable.cc map_placeable_gfx.cc \
	map_moving.cc map_moving_gfx.cc map_character.cc map_character_gfx.cc \
	map_character_with_gfx.cc map_object.cc map_object_gfx.cc game.cc \
	map_object_with_gfx.cc landmap.cc pnm.cc surface.cc screen.cc \
	py_callback.cc python_class.cc py_object.cc \
	win_background.cc win_base.cc win_border.cc \
	win_container.cc win_font.cc win_keys.cc\
	win_manager.cc win_scrollbar.cc win_scroll.cc win_select.cc win_theme.cc \
	label.cc  label_input.cc win_event.cc yarg.cc \
	input_manager.cc input_manager_SDL.cc input_event.cc input_listener.cc keyboard_event.cc \
	joystick_event.cc mouse_event.cc control_event.cc \
	types.h animation.h drawable.h drawing_area.h fileops.h image.h \
	input.h gametime.h map_coordinates.h mapsquare_obj_area.h \
	mapsquare_obj_area_gfx.h map_placeable.h map_placeable_gfx.h \
	map_moving.h map_moving_gfx.h map_character.h map_character_gfx.h \
	map_character_with_gfx.h map_object.h map_object_gfx.h game.h \
	map_object_with_gfx.h landmap.h pnm.h surface.h screen.h \
	callback.h py_callback.h python_class.h py_object.h \
	str_hash.h win_font.h win_image.h win_label.h win_mapview.h label.h label_input.h \
	win_scrollbar.h win_scroll.h win_select.h win_theme.h win_keys.h win_event.h win_types.h \
	win_write.h win_manager.h win_object.h window.h callback_sig.cc callback_sig.h\
	callback_slot.h atk_font.h atk_font.cc atk_widget.cc atk_widget.h \
	atk_container.h atk_container.cc atk_label.h atk_label.cc atk_box.h atk_box.cc atk_misc.cc atk_misc.h \
	input_manager.h input_event.h input_listener.h keyboard_event.h joystick_event.h mouse_event.h \
	control_event.h atk_fixed.h atk_fixed.cc


alextest_SOURCES = main.cc

joltest_SOURCES = joltest.cc

joltest_LDADD = libadonthell.a $(SDL_LIBS) $(FTT2_LIBS)

alextest_LDADD = libadonthell.a $(SDL_LIBS)

# Note: adonthell.py is also built by this target. 
py_adonthell_wrap.cc : py_adonthell.i *.h
	@if test "${P_SWIG}"; then \
	   ${P_SWIG} -python -shadow ${SDL_CFLAGS} -I./ -I../ -c++ -make_default -o $*.cc py_adonthell.i; \
	else \
	   echo "You need swig >= 1.3.10 in order to re-build this file."; \
	   exit 1; \
	fi;

lex.prefs.cc : prefs.l
	flex -olex.prefs.cc $<

adonthell.pyc : adonthell.py adonthell
	./adonthell -c
