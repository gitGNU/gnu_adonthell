## Process this file with automake to produce Makefile.in

bin_PROGRAMS = animedit mapobjedit mapcharedit mapedit

animedit_SOURCES = animedit.cpp

animedit_LDADD = animation.o animation_off.o \
drawing_area.o fileops.o game.o image.o \
input.o lex.prefs.o pnm.o prefs.o \
screen.o storage.o quest.o win_manager.o win_base.o win_border.o \
win_container.o win_font.o win_theme.o win_scrolled.o \
win_background.o win_scrollbar.o win_file.o win_image.o \
win_label.o win_select.o win_write.o win_query.o win_info.o \
win_file_select.o yarg.o $(SDL_LIBS)

mapobjedit_SOURCES = mapobjedit.cpp

mapobjedit_LDADD =  animation.o animation_off.o \
drawing_area.o fileops.o game.o image.o \
input.o lex.prefs.o pnm.o prefs.o mapselect.o maptpl.o \
mapobject.o character_base.o screen.o storage.o quest.o \
win_manager.o win_base.o win_border.o win_container.o win_font.o win_theme.o \
win_scrolled.o win_background.o win_scrollbar.o win_file.o \
win_image.o win_label.o win_select.o win_write.o win_query.o \
win_info.o win_file_select.o yarg.o $(SDL_LIBS)

mapcharedit_SOURCES = mapcharedit.cpp

mapcharedit_LDADD =  animation.o animation_off.o \
drawing_area.o fileops.o game.o image.o \
input.o lex.prefs.o pnm.o prefs.o mapselect.o maptpl.o \
screen.o storage.o quest.o win_manager.o win_base.o win_border.o \
win_container.o win_font.o win_theme.o win_scrolled.o \
mapcharacter.o character_base.o mapobject.o landmap.o mapview.o \
win_background.o win_scrollbar.o win_file.o win_image.o \
win_label.o win_select.o win_write.o win_query.o win_info.o \
win_file_select.o yarg.o $(SDL_LIBS)

mapedit_SOURCES = mapedit.cpp

mapedit_LDADD =  animation.o animation_off.o \
drawing_area.o fileops.o game.o image.o \
input.o lex.prefs.o mapselect.o maptpl.o mapobject.o \
mapcharacter.o landmap.o character_base.o mapview.o pnm.o prefs.o \
screen.o storage.o quest.o win_manager.o win_base.o win_border.o \
win_container.o win_font.o win_theme.o win_scrolled.o \
win_background.o win_scrollbar.o win_file.o win_image.o \
win_label.o win_select.o win_write.o win_query.o win_info.o \
win_file_select.o yarg.o $(SDL_LIBS)

CPPFLAGS +=-I../.. -D_EDIT_ -DUSE_MAP $(SDL_CFLAGS)

%.o : ../../%.cpp
	$(CXXCOMPILE) -c $< -o $@

yarg.o: ../../yarg.cc
	$(CXXCOMPILE) -c $< -o $@

../../lex.prefs.o : ../../prefs.l
	flex -o../../lex.prefs.cpp $<
