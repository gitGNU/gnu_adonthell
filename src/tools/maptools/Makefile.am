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

animation.o : ../../animation.cpp
	$(CXXCOMPILE) -c $< -o $@

animation_off.o : ../../animation_off.cpp
	$(CXXCOMPILE) -c $< -o $@

audio.o : ../../audio.cpp
	$(CXXCOMPILE) -c $< -o $@

data.o : ../../data.cpp
	$(CXXCOMPILE) -c $< -o $@

event.o : ../../event.cpp
	$(CXXCOMPILE) -c $< -o $@

drawing_area.o : ../../drawing_area.cpp
	$(CXXCOMPILE) -c $< -o $@

fileops.o : ../../fileops.cpp
	$(CXXCOMPILE) -c $< -o $@

game.o : ../../game.cpp
	$(CXXCOMPILE) -c $< -o $@

gametime.o : ../../gametime.cpp
	$(CXXCOMPILE) -c $< -o $@

image.o : ../../image.cpp
	$(CXXCOMPILE) -c $< -o $@

input.o : ../../input.cpp
	$(CXXCOMPILE) -c $< -o $@

lex.prefs.o : ../../lex.prefs.cpp
	$(CXXCOMPILE) -c $< -o $@

../../lex.prefs.o : ../../prefs.l
	flex -o../../lex.prefs.cpp $<

mapselect.o : ../../mapselect.cpp
	$(CXXCOMPILE) -c $< -o $@

maptpl.o : ../../maptpl.cpp
	$(CXXCOMPILE) -c $< -o $@

mapobject.o : ../../mapobject.cpp
	$(CXXCOMPILE) -c $< -o $@

mapcharacter.o : ../../mapcharacter.cpp
	$(CXXCOMPILE) -c $< -o $@

character.o : ../../character.cpp
	$(CXXCOMPILE) -c $< -o $@

landmap.o : ../../landmap.cpp
	$(CXXCOMPILE) -c $< -o $@

mapview.o : ../../mapview.cpp
	$(CXXCOMPILE) -c $< -o $@

pnm.o : ../../pnm.cpp
	$(CXXCOMPILE) -c $< -o $@

prefs.o : ../../prefs.cpp
	$(CXXCOMPILE) -c $< -o $@

screen.o : ../../screen.cpp
	$(CXXCOMPILE) -c $< -o $@

storage.o : ../../storage.cpp
	$(CXXCOMPILE) -c $< -o $@

py_inc.o : ../../py_inc.cpp
	$(CXXCOMPILE) -c $< -o $@

quest.o : ../../quest.cpp
	$(CXXCOMPILE) -c $< -o $@

win_base.o : ../../win_base.cpp
	$(CXXCOMPILE) -c $< -o $@

win_border.o : ../../win_border.cpp
	$(CXXCOMPILE) -c $< -o $@

win_container.o : ../../win_container.cpp
	$(CXXCOMPILE) -c $< -o $@

win_font.o : ../../win_font.cpp
	$(CXXCOMPILE) -c $< -o $@

win_theme.o : ../../win_theme.cpp
	$(CXXCOMPILE) -c $< -o $@

win_scrolled.o : ../../win_scrolled.cpp
	$(CXXCOMPILE) -c $< -o $@

win_background.o : ../../win_background.cpp
	$(CXXCOMPILE) -c $< -o $@

win_scrollbar.o : ../../win_scrollbar.cpp
	$(CXXCOMPILE) -c $< -o $@

win_file.o : ../../win_file.cpp
	$(CXXCOMPILE) -c $< -o $@

win_image.o : ../../win_image.cpp
	$(CXXCOMPILE) -c $< -o $@

win_label.o : ../../win_label.cpp
	$(CXXCOMPILE) -c $< -o $@

win_select.o : ../../win_select.cpp
	$(CXXCOMPILE) -c $< -o $@

win_write.o : ../../win_write.cpp
	$(CXXCOMPILE) -c $< -o $@

win_query.o : ../../win_query.cpp
	$(CXXCOMPILE) -c $< -o $@

win_info.o : ../../win_info.cpp
	$(CXXCOMPILE) -c $< -o $@

win_file_select.o : ../../win_file_select.cpp
	$(CXXCOMPILE) -c $< -o $@

yarg.o : ../../yarg.cc
	$(CXXCOMPILE) -c $< -o $@
