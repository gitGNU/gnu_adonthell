## Process this file with automake to produce Makefile.in

MODULES_SUBDIRS = gfx input audio python lmap gui

if BUILD_TOOLS
SUBDIRS = $(MODULES_SUBDIRS) . tools
else
SUBDIRS = $(MODULES_SUBDIRS) .
endif


moddatadir = $(gamedatadir)/modules

#moddata_DATA = adonthell.pyc

bin_PROGRAMS = alextest schedtest joltest pytest

EXTRA_DIST = .indent.pro prefs.l # py_adonthell.i adonthell.py

CPPFLAGS = $(SDL_DEFS) $(OGG_DEFS) $(SDL_CFLAGS) $(OGG_CFLAGS) $(PY_CFLAGS) $(FT2_CFLAGS)

noinst_LIBRARIES = libbase.a

CLEANFILES = $(moddata_SCRIPTS)

libbase_a_SOURCES = \
	callback_sig.cc \
        character_base.cc \
        character.cc \
        event.cc \
        event_handler.cc \
        event_list.cc \
        fileops.cc \
        game.cc \
        gamedata.cc \
        gamedate.cc \
        gametime.cc \
        py_base_wrap.cc \
        quest.cc \
        schedule.cc \
        storage.cc \
        time_event.cc \
        time_event_handler.cc \
        yarg.cc \
        callback.h \
	callback_sig.h \
        character.h \
        character_base.h \
        event.h \
        event_handler.h \
        event_handler_base.h \
        event_list.h \
        fileops.h \
        game.h \
        gamedata.h \
        gamedate.h \
        gametime.h \
        quest.h \
        schedule.h \
        schedule_data.h \
        storage.h \
        str_hash.h \
        time_event.h \
        time_event_handler.h \
        types.h \
        yarg.h

libbase_LDADD = $(SDL_LIBS)

#libaudio_LDADD = $(OGG_LIBS)

libpython_LDADD = $(PY_LIBS)

#libgui_a_SOURCES = atk_box.cc atk_misc.cc atk_container.cc atk_label.cc \
#	atk_font.cc atk_widget.cc atk_fixed.cc  \
#	atk_bin.cc atk_window.cc atk_manager.cc atk_border_template.cc \
#	atk_border_ui.cc atk_theme.cc atk_background_template.cc \
#	atk_button_template.cc atk_button.cc atk_button_ui.cc \
#	atk_background_ui.cc \
#	callback_sig.cc \
#	atk_box.h atk_misc.h atk_container.h atk_label.h \
#	atk_font.h atk_widget.h atk_fixed.h \
#	atk_bin.h atk_window.h atk_manager.h atk_border_template.h  \
#	atk_border_ui.h atk_theme.h atk_background_template.h \
#	atk_button_template.h atk_button.h atk_button_ui.h \
#	atk_background_ui.h \
#	callback_sig.h callback_slot.h 

libgui_LDADD = $(FT2_LIBS)

##liboldwin_SOURCES = win_background.cc win_base.cc win_border.cc \
#	win_container.cc win_font.cc win_keys.cc \
#	win_manager.cc win_scrollbar.cc win_scroll.cc win_select.cc win_theme.cc \
#	label.cc label_input.cc win_event.cc \
#	win_font.h win_image.h win_label.h label.h label_input.h \
#	win_scrollbar.h win_scroll.h win_select.h win_theme.h win_keys.h win_event.h win_types.h \
#	win_write.h win_manager.h win_object.h window.h  
#liboldwin_LDADD = $(FT2_LIBS)

#libadonthell_a_SOURCES = input.cc \
#	input.h 

alextest_SOURCES = main.cc

joltest_SOURCES = joltest.cc input.cc input.h

pytest_SOURCES = pytest.cc

schedtest_SOURCES = schedtest.cc


joltest_LDADD =  gui/libgui.a python/libpython.a gfx/libgfx.a libbase.a $(libbase_LDADD) $(libgui_LDADD) $(libpython_LDADD) -lstdc++

alextest_LDADD = lmap/libmap.a gfx/libgfx.a input/libinput.a python/libpython.a libbase.a $(libbase_LDADD) $(libpython_LDADD)

pytest_LDADD = gfx/libgfx.a libbase.a python/libpython.a input/libinput.a $(libbase_LDADD) $(libpython_LDADD) $(libpython_LDADD)

schedtest_LDADD = libbase.a python/libpython.a gfx/libgfx.a python/libpython.a input/libinput.a $(libbase_LDADD) $(libpython_LDADD)

# Note: .py files are also built by this target.
py_%_wrap.cc : py_%.i
	@if test "${P_SWIG}"; then \
	   ${P_SWIG} -python -shadow ${SDL_CFLAGS} -I${srcdir} -c++ -make_default -o py_$*_wrap.cc $<; \
	else \
	   echo "You need swig >= $SWIG_MINVER in order to re-build this file."; \
	   exit 1; \
	fi;

# adonthell.pyc : adonthell.py adonthell
# 	./adonthell -c
