## Process this file with automake to produce Makefile.in

if BUILD_TOOLS
SUBDIRS = . tools
endif

bin_PROGRAMS = adonthell

EXTRA_DIST = win_info.cpp win_file_select.cpp .indent.pro prefs.l win_query.cpp

CPPFLAGS +=-DUSE_PYTHON -DUSE_MAP $(SDL_DEFS) $(OGG_DEFS) $(SDL_CFLAGS) $(OGG_CFLAGS) $(PY_CFLAGS)
adonthell_LDADD = $(SDL_LIBS) $(OGG_LIBS) $(PY_LIBS)

adonthell_SOURCES = animation.cpp audio.cpp audio_loop.cpp callback_wrap.cpp \
character_base.cpp character.cpp data.cpp data_screen.cpp dialog.cpp \
dialog_engine.cpp drawing_area.cpp event.cpp fileops.cpp game.cpp gametime.cpp \
image.cpp input.cpp landmap.cpp lex.prefs.cpp main.cpp mapcharacter.cpp \
mapengine.cpp mapobject.cpp mapselect.cpp maptpl.cpp mapview.cpp pnm.cpp \
prefs.cpp py_inc.cpp py_modules.cpp py_adonthell_wrap.cpp quest.cpp \
screen.cpp storage.cpp win_background.cpp win_base.cpp win_border.cpp \
win_container.cpp win_file.cpp win_font.cpp win_image.cpp win_label.cpp \
win_manager.cpp win_scrollbar.cpp win_scrolled.cpp win_select.cpp win_theme.cpp \
win_write.cpp yarg.cc \
animation.h animation_off.h audio.h audio_loop.h callback.h callback_wrap.h \
character_base.h character.h data.h data_screen.h dialog.h dialog_engine.h \
drawing_area.h event.h fileops.h game.h gametime.h image.h input.h inventory.h \
item.h landmap.h mapcharacter.h mapobject.h mapselect.h maptpl.h mapview.h \
mapengine.h pnm.h prefs.h py_inc.h py_modules.h quest.h screen.h storage.h \
types.h win_anim.h win_background.h win_base.h win_border.h win_container.h \
win_file.h win_file_select.h win_font.h win_image.h win_info.h win_label.h \
win_query.h win_scrollbar.h win_scrolled.h win_select.h win_theme.h win_types.h \
win_write.h win_manager.h window.h yarg.h

py_adonthell_wrap.cpp : py_adonthell.i *.h
	swig -python -shadow -I/tmp/gnu/include/SDL -I/usr/local/include/SDL -I/usr/include/SDL -I./ -I../ -c++ -make_default -o $*.cpp $< 
	@datapath=`grep '^ *Data' ~/.adonthell/adonthellrc | sed 's/^ *Data *\[\(.*\)\] *$$/\1/'` ;\
	for subdir in $$datapath ; do \
	  if test -d "$$subdir" ; then \
	    echo "Copying adonthell.py to $$subdir/scripts/modules" ; \
	    cp adonthell.py $$subdir/scripts/modules ; \
	  else \
	    echo "$$subdir/scripts/modules isn't a valid directory. Check your config file!" ; \
	  fi ; \
	done

lex.prefs.cpp : prefs.l
	flex -olex.prefs.cpp $<
