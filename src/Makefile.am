SUBDIRS = . tools

bin_PROGRAMS = mapengine #alextest joltest

mapengine_SOURCES = animation.cpp audio.cpp audio_thread.cpp \
character.cpp cutscene.cpp dialog.cpp fileops.cpp game.cpp \
image.cpp init.cpp input.cpp inventory.cpp \
lex.prefs.cpp main.cpp map.cpp mapcharacter.cpp mapengine.cpp mapevent.cpp \
mappattern.cpp mapsquare.cpp pnm.cpp prefs.cpp screen.cpp storage.cpp win_label.cpp \
win_write.cpp win_container.cpp win_select.cpp win_friend.cpp win_border.cpp \
win_image.cpp win_base.cpp win_font.cpp mapitem.cpp py_inc.cpp py_player_wrap.cpp \
py_mapengine_wrap.cpp yarg.cc py_dialogue.cpp

py_player_wrap.cpp : py_player.i
	swig -python -shadow -c++ -make_default -o $*.cpp $< 
	@mv player.py ../scripts

py_mapengine_wrap.cpp : py_mapengine.i
	swig -python -shadow -c++ -o $*.cpp $< 
	@mv mapengine.py ../scripts

py_baseitem_wrap.cpp : py_baseitem.i
	swig -python -shadow -c++ -o $*.cpp $< 
	@mv baseitem.py ../scripts

#joltest_SOURCES = animation.cpp audio.cpp audio_thread.cpp \
#character.cpp commands.cpp cutscene.cpp dialog_cmds.cpp fileops.cpp game.cpp \
#generic_cmds.cpp image.cpp init.cpp interpreter.cpp input.cpp inventory.cpp \
#lex.prefs.cpp pnm.cpp prefs.cpp screen.cpp storage.cpp win_label.cpp \
#win_write.cpp win_container.cpp win_select.cpp win_friend.cpp win_border.cpp \
#win_base.cpp win_font.cpp win_image.cpp joel.cpp yarg.cc

#alextest_SOURCES = fileops.cpp pnm.cpp screen.cpp image.cpp animation.cpp \
#lex.prefs.cpp prefs.cpp alextest.cpp input.cpp win_label.cpp \
#win_write.cpp win_container.cpp win_select.cpp win_friend.cpp win_border.cpp \
#win_base.cpp win_font.cpp win_image.cpp



