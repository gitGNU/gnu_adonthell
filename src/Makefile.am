SUBDIRS = . tools

bin_PROGRAMS = mapengine joltest

mapengine_SOURCES = animation.cpp audio.cpp audio_thread.cpp \
character.cpp cutscene.cpp dialog.cpp drawing_area.cpp event.cpp fileops.cpp game.cpp \
image.cpp init.cpp input.cpp lex.prefs.cpp main.cpp \
map.cpp mapcharacter.cpp mapengine.cpp mapevent.cpp mapitem.cpp \
mappattern.cpp mapsquare.cpp pnm.cpp prefs.cpp py_inc.cpp py_character_wrap.cpp\
py_dialog_engine_wrap.cpp py_mapengine_wrap.cpp py_quest_wrap.cpp quest.cpp screen.cpp storage.cpp \
win_anim.cpp win_base.cpp win_border.cpp win_container.cpp win_font.cpp \
win_file.cpp win_image.cpp win_label.cpp win_select.cpp win_write.cpp yarg.cc

joltest_SOURCES = audio.cpp audio_thread.cpp \
drawing_area.cpp image.cpp animation.cpp init.cpp input.cpp \
lex.prefs.cpp pnm.cpp prefs.cpp screen.cpp win_label.cpp \
win_write.cpp win_container.cpp win_select.cpp win_border.cpp \
win_base.cpp win_font.cpp win_image.cpp win_anim.cpp joel.cpp yarg.cc win_file.cpp

py_dialog_engine_wrap.cpp : py_dialog_engine.i
	swig -python -shadow -c++ -make_default -o $*.cpp $< 
	@mv dialog_engine.py ../scripts

py_character_wrap.cpp : py_character.i
	swig -python -shadow -c++ -make_default -o $*.cpp $< 
	@mv character.py ../scripts

py_quest_wrap.cpp : py_quest.i
	swig -python -shadow -c++ -make_default -o $*.cpp $< 
	@mv quest.py ../scripts

py_mapengine_wrap.cpp : py_mapengine.i
	swig -python -shadow -c++ -o $*.cpp $< 
	@mv mapengine.py ../scripts

py_baseitem_wrap.cpp : py_baseitem.i
	swig -python -shadow -c++ -o $*.cpp $< 
	@mv baseitem.py ../scripts

lex.prefs.cpp : prefs.l
	flex -olex.prefs.cpp $<

