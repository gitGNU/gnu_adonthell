## Process this file with automake to produce Makefile.in

bin_PROGRAMS = animedit mapobjedit mapcharedit mapedit

animedit_SOURCES = animedit.cpp

animedit_LDADD = animation.o animation_off.o data.o \
event.o drawing_area.o fileops.o game.o gametime.o image.o \
input.o lex.prefs.o pnm.o prefs.o character.o \
screen.o storage.o py_inc.o quest.o win_base.o win_border.o \
win_container.o win_font.o win_theme.o win_scrolled.o \
win_background.o win_scrollbar.o win_file.o win_image.o \
win_label.o win_select.o win_write.o win_query.o win_info.o \
win_file_select.o yarg.o

mapobjedit_SOURCES = mapobjedit.cpp

mapobjedit_LDADD =  animation.o animation_off.o data.o \
event.o drawing_area.o fileops.o game.o gametime.o image.o \
input.o lex.prefs.o pnm.o prefs.o mapselect.o maptpl.o \
mapobject.o character.o screen.o storage.o py_inc.o quest.o \
win_base.o win_border.o win_container.o win_font.o win_theme.o \
win_scrolled.o win_background.o win_scrollbar.o win_file.o \
win_image.o win_label.o win_select.o win_write.o win_query.o \
win_info.o win_file_select.o yarg.o

mapcharedit_SOURCES = mapcharedit.cpp

mapcharedit_LDADD =  animation.o animation_off.o data.o \
event.o drawing_area.o fileops.o game.o gametime.o image.o \
input.o lex.prefs.o pnm.o prefs.o mapselect.o maptpl.o \
screen.o storage.o py_inc.o quest.o win_base.o win_border.o \
win_container.o win_font.o win_theme.o win_scrolled.o \
mapcharacter.o character.o mapobject.o landmap.o mapview.o \
win_background.o win_scrollbar.o win_file.o win_image.o \
win_label.o win_select.o win_write.o win_query.o win_info.o \
win_file_select.o yarg.o

mapedit_SOURCES = mapedit.cpp

mapedit_LDADD =  animation.o animation_off.o data.o \
event.o drawing_area.o fileops.o game.o gametime.o image.o \
input.o lex.prefs.o mapselect.o maptpl.o mapobject.o \
mapcharacter.o landmap.o character.o mapview.o pnm.o prefs.o \
screen.o storage.o py_inc.o quest.o win_base.o win_border.o \
win_container.o win_font.o win_theme.o win_scrolled.o \
win_background.o win_scrollbar.o win_file.o win_image.o \
win_label.o win_select.o win_write.o win_query.o win_info.o \
win_file_select.o yarg.o

CPPFLAGS +=-I../.. -D_EDIT_

%.o : ../../%.cpp
	$(CXXCOMPILE) -c $< -o $@

yarg.o: ../../yarg.cc
	$(CXXCOMPILE) -c $< -o $@

../../lex.prefs.o : ../../prefs.l
	flex -o../../lex.prefs.cpp $<
