## Process this file with automake to produce Makefile.in

SUBDIRS = . tools

bin_PROGRAMS = adonthell

EXTRA_DIST = py_audio.i py_character.i py_dialog_engine.i py_image.i \
py_input.i py_landmap.i py_mapcharacter.i py_mapview.i py_mapengine.i \
py_quest.i py_screen.i py_win_base.i py_win_container.i py_win_font.i \
py_win_image.i py_win_label.i py_win_theme.i prefs.l win_query.cpp \
py_event.i win_info.cpp win_file_select.cpp .indent.pro

adonthell_SOURCES = animation.cpp animation_off.cpp audio.cpp \
audio_loop.cpp \
character.cpp data.cpp data_screen.cpp dialog.cpp dialog_engine.cpp \
drawing_area.cpp event.cpp fileops.cpp game.cpp gametime.cpp \
image.cpp input.cpp lex.prefs.cpp main.cpp \
mapselect.cpp maptpl.cpp mapobject.cpp mapcharacter.cpp landmap.cpp \
mapengine.cpp \
mapview.cpp pnm.cpp prefs.cpp py_character_wrap.cpp py_landmap_wrap.cpp \
py_input_wrap.cpp py_mapcharacter_wrap.cpp py_mapview_wrap.cpp \
py_mapengine_wrap.cpp py_dialog_engine_wrap.cpp py_inc.cpp py_event_wrap.cpp \
py_modules.cpp py_quest_wrap.cpp py_screen_wrap.cpp py_image_wrap.cpp \
quest.cpp screen.cpp storage.cpp win_base.cpp win_border.cpp \
win_container.cpp win_font.cpp win_theme.cpp win_scrolled.cpp \
win_background.cpp win_scrollbar.cpp win_file.cpp win_image.cpp \
win_label.cpp win_select.cpp win_write.cpp \
yarg.cc py_win_font_wrap.cpp \
py_win_theme_wrap.cpp py_win_base_wrap.cpp py_win_label_wrap.cpp \
py_win_container_wrap.cpp py_audio_wrap.cpp py_win_image_wrap.cpp \
animation.h animation_off.h audio.h audio_loop.h callback.h character.h data.h \
data_screen.h dialog.h dialog_engine.h drawing_area.h event.h fileops.h \
game.h gametime.h image.h input.h inventory.h item.h landmap.h mapcharacter.h \
mapobject.h mapselect.h maptpl.h mapview.h mapengine.h pnm.h prefs.h py_inc.h \
py_modules.h quest.h screen.h storage.h types.h win_anim.h win_background.h \
win_base.h win_border.h win_container.h win_file.h win_file_select.h \
win_font.h win_image.h win_info.h win_label.h win_query.h win_scrollbar.h \
win_scrolled.h win_select.h win_theme.h win_types.h win_write.h window.h \
yarg.h

py_input_wrap.cpp : py_input.i input.h
	swig -python -shadow -c++ -make_default -o $*.cpp $< 
	@mv input.py ../scripts

py_audio_wrap.cpp : py_audio.i audio.h
	swig -python -shadow -c++ -make_default -o $*.cpp $< 
	@mv audio.py ../scripts

py_screen_wrap.cpp : py_screen.i screen.h
	swig -python -shadow -c++ -make_default -o $*.cpp $< 
	@mv screen.py ../scripts

py_image_wrap.cpp : py_image.i image.h
	swig -python -shadow -c++ -make_default -o $*.cpp $< 
	@mv image.py ../scripts

py_landmap_wrap.cpp : py_landmap.i landmap.h
	swig -python -shadow -c++ -make_default -o $*.cpp $< 
	@mv landmap.py ../scripts

py_mapcharacter_wrap.cpp : py_mapcharacter.i mapcharacter.h
	swig -python -shadow -c++ -make_default -o $*.cpp $< 
	@mv mapcharacter.py ../scripts

py_mapview_wrap.cpp : py_mapview.i mapview.h
	swig -python -shadow -c++ -make_default -o $*.cpp $< 
	@mv mapview.py ../scripts

py_mapengine_wrap.cpp : py_mapengine.i mapengine.h
	swig -python -shadow -c++ -make_default -o $*.cpp $< 
	@mv mapengine.py ../scripts

py_event_wrap.cpp : py_event.i event.h
	swig -python -shadow -c++ -make_default -o $*.cpp $< 
	@mv event.py ../scripts

py_win_font_wrap.cpp : py_win_font.i
	swig -python -shadow -c++ -make_default -o $*.cpp $< 
	@mv win_font.py ../scripts

py_win_theme_wrap.cpp : py_win_theme.i
	swig -python -shadow -c++ -make_default -o $*.cpp $< 
	@mv win_theme.py ../scripts

py_win_base_wrap.cpp : py_win_base.i
	swig -python -shadow -c++ -make_default -o $*.cpp $< 
	@mv win_base.py ../scripts

py_win_label_wrap.cpp : py_win_label.i
	swig -python -shadow -c++ -make_default -o $*.cpp $< 
	@mv win_label.py ../scripts

py_win_image_wrap.cpp : py_win_image.i
	swig -python -shadow -c++ -make_default -o $*.cpp $< 
	@mv win_image.py ../scripts

py_win_container_wrap.cpp : py_win_container.i
	swig -python -shadow -c++ -make_default -o $*.cpp $< 
	@mv win_container.py ../scripts

py_dialog_engine_wrap.cpp : py_dialog_engine.i
	swig -python -shadow -c++ -make_default -o $*.cpp $< 
	@mv dialog_engine.py ../scripts

py_character_wrap.cpp : py_character.i
	swig -python -shadow -c++ -make_default -o $*.cpp $< 
	@mv character.py ../scripts

py_quest_wrap.cpp : py_quest.i
	swig -python -shadow -c++ -make_default -o $*.cpp $< 
	@mv quest.py ../scripts

lex.prefs.cpp : prefs.l
	flex -olex.prefs.cpp $<
