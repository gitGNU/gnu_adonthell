SUBDIRS = . tools

bin_PROGRAMS = adonthell

adonthell_SOURCES = animation.cpp audio.cpp audio_thread.cpp \
character.cpp cutscene.cpp data.cpp dialog.cpp dialog_engine.cpp \
drawing_area.cpp event.cpp fileops.cpp game.cpp gametime.cpp \
image.cpp input.cpp lex.prefs.cpp main.cpp \
mapselect.cpp maptpl.cpp mapobject.cpp mapcharacter.cpp landmap.cpp mapview.cpp \
pnm.cpp prefs.cpp py_character_wrap.cpp\
py_dialog_engine_wrap.cpp py_inc.cpp py_modules.cpp py_quest_wrap.cpp \
quest.cpp screen.cpp storage.cpp \
win_base.cpp win_border.cpp win_container.cpp win_font.cpp win_theme.cpp win_scrolled.cpp win_background.cpp win_scrollbar.cpp \
win_file.cpp win_image.cpp win_label.cpp win_select.cpp win_write.cpp yarg.cc

py_dialog_engine_wrap.cpp : py_dialog_engine.i
	swig -python -shadow -c++ -make_default -o $*.cpp $< 
	@mv dialog_engine.py ../scripts

py_character_wrap.cpp : py_character.i
	swig -python -shadow -c++ -make_default -o $*.cpp $< 
	@mv character.py ../scripts

py_quest_wrap.cpp : py_quest.i
	swig -python -shadow -c++ -make_default -o $*.cpp $< 
	@mv quest.py ../scripts

py_baseitem_wrap.cpp : py_baseitem.i
	swig -python -shadow -c++ -o $*.cpp $< 
	@mv baseitem.py ../scripts

lex.prefs.cpp : prefs.l
	flex -olex.prefs.cpp $<

