## Process this file with automake to produce Makefile.in

SUBDIRS = . tools

bin_PROGRAMS = adonthell

EXTRA_DIST = win_info.cpp win_file_select.cpp .indent.pro prefs.l win_query.cpp

CPPFLAGS +=-DUSE_PYTHON -DUSE_MAP

adonthell_SOURCES = animation.cpp animation_off.cpp audio.cpp \
audio_loop.cpp \
character_base.cpp character.cpp data.cpp data_screen.cpp dialog.cpp dialog_engine.cpp \
drawing_area.cpp event.cpp fileops.cpp game.cpp gametime.cpp \
image.cpp input.cpp lex.prefs.cpp main.cpp \
mapselect.cpp maptpl.cpp mapobject.cpp mapcharacter.cpp landmap.cpp \
mapengine.cpp py_inc.cpp py_modules.cpp \
mapview.cpp pnm.cpp prefs.cpp  \
quest.cpp screen.cpp storage.cpp win_base.cpp win_border.cpp \
win_container.cpp win_font.cpp win_theme.cpp win_scrolled.cpp \
win_background.cpp win_scrollbar.cpp win_file.cpp win_image.cpp \
win_label.cpp win_select.cpp win_write.cpp win_manager.cpp \
yarg.cc py_adonthell_wrap.cpp \
animation.h animation_off.h audio.h audio_loop.h callback.h character_base.h character.h data.h \
data_screen.h dialog.h dialog_engine.h drawing_area.h event.h fileops.h \
game.h gametime.h image.h input.h inventory.h item.h landmap.h mapcharacter.h \
mapobject.h mapselect.h maptpl.h mapview.h mapengine.h pnm.h prefs.h py_inc.h \
py_modules.h quest.h screen.h storage.h types.h win_anim.h win_background.h \
win_base.h win_border.h win_container.h win_file.h win_file_select.h \
win_font.h win_image.h win_info.h win_label.h win_query.h win_scrollbar.h \
win_scrolled.h win_select.h win_theme.h win_types.h win_write.h \
win_manager.h window.h \
yarg.h

py_adonthell_wrap.cpp : py_adonthell.i
	swig -python -shadow -I/tmp/gnu/include/SDL -I/usr/local/include/SDL -I/usr/include/SDL -c++ -make_default -o $*.cpp $< 
	@mv adonthell.py ../scripts

lex.prefs.cpp : prefs.l
	flex -olex.prefs.cpp $<
