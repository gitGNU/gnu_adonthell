SUBDIRS = . tools

bin_PROGRAMS = adonthell

adonthell_SOURCES = animation.cpp animation_off.cpp audio.cpp \
character.cpp data.cpp data_screen.cpp dialog.cpp dialog_engine.cpp \
drawing_area.cpp event.cpp fileops.cpp game.cpp gametime.cpp \
image.cpp input.cpp lex.prefs.cpp main.cpp \
mapselect.cpp maptpl.cpp mapobject.cpp mapcharacter.cpp landmap.cpp mapview.cpp \
pnm.cpp prefs.cpp py_character_wrap.cpp py_landmap_wrap.cpp py_input_wrap.cpp \
py_mapview_wrap.cpp \
py_dialog_engine_wrap.cpp py_inc.cpp py_modules.cpp py_quest_wrap.cpp \
py_screen_wrap.cpp py_image_wrap.cpp quest.cpp screen.cpp storage.cpp \
win_base.cpp win_border.cpp win_container.cpp win_font.cpp win_theme.cpp win_scrolled.cpp win_background.cpp win_scrollbar.cpp \
win_file.cpp win_image.cpp win_label.cpp win_select.cpp win_write.cpp yarg.cc \
py_win_font_wrap.cpp py_win_theme_wrap.cpp py_win_base_wrap.cpp py_win_label_wrap.cpp py_win_image_wrap.cpp py_win_container_wrap.cpp py_audio_wrap.cpp

py_input_wrap.cpp : py_input.i
	swig -python -shadow -c++ -make_default -o $*.cpp $< 
	@mv input.py ../scripts

py_audio_wrap.cpp : py_audio.i
	swig -python -shadow -c++ -make_default -o $*.cpp $< 
	@mv audio.py ../scripts

py_screen_wrap.cpp : py_screen.i
	swig -python -shadow -c++ -make_default -o $*.cpp $< 
	@mv screen.py ../scripts

py_image_wrap.cpp : py_image.i
	swig -python -shadow -c++ -make_default -o $*.cpp $< 
	@mv image.py ../scripts

py_landmap_wrap.cpp : py_landmap.i
	swig -python -shadow -c++ -make_default -o $*.cpp $< 
	@mv landmap.py ../scripts

py_mapview_wrap.cpp : py_mapview.i
	swig -python -shadow -c++ -make_default -o $*.cpp $< 
	@mv mapview.py ../scripts

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
