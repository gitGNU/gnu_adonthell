## Process this file with automake to produce Makefile.in

bin_PROGRAMS = pydonthell

pydonthell_SOURCES = pydonthell.cpp

LDADD = ../../animation.o ../../animation_off.o ../../audio.o \
../../character.o ../../data.o ../../data_screen.o ../../dialog.o ../../dialog_engine.o \
../../drawing_area.o ../../event.o ../../fileops.o ../../game.o ../../gametime.o \
../../image.o ../../input.o ../../lex.prefs.o \
../../mapselect.o ../../maptpl.o ../../mapobject.o ../../mapcharacter.o ../../landmap.o ../../mapview.o \
../../pnm.o ../../prefs.o ../../py_character_wrap.o ../../py_landmap_wrap.o ../../py_input_wrap.o ../../py_mapcharacter_wrap.o \
../../py_mapview_wrap.o ../../py_dialog_engine_wrap.o \
../../py_inc.o ../../py_modules.o ../../py_quest_wrap.o \
../../py_screen_wrap.o ../../py_image_wrap.o ../../quest.o ../../screen.o ../../storage.o \
../../win_base.o ../../win_border.o ../../win_container.o ../../win_font.o ../../win_theme.o ../../win_scrolled.o ../../win_background.o ../../win_scrollbar.o \
../../win_file.o ../../win_image.o ../../win_label.o ../../win_select.o ../../win_write.o ../../yarg.cc \
../../py_win_font_wrap.o ../../py_win_theme_wrap.o ../../py_win_base_wrap.o ../../py_win_label_wrap.o ../../py_win_container_wrap.o ../../py_audio_wrap.o ../../py_win_image_wrap.o

# Evil, but it works...
CPPFLAGS +=-I../..

py_audio_wrap.cpp : py_audio.i
	swig -python -shadow -c++ -make_default -o $*.cpp $< 
	@mv audio.py ../scripts

py_input_wrap.cpp : py_input.i
	swig -python -shadow -c++ -make_default -o $*.cpp $< 
	@mv input.py ../scripts

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

py_baseitem_wrap.cpp : py_baseitem.i
	swig -python -shadow -c++ -o $*.cpp $< 
	@mv baseitem.py ../scripts

lex.prefs.cpp : prefs.l
	flex -olex.prefs.cpp $<

%.o: %.cpp
	$(CXXCOMPILE) -c $< -o $@
