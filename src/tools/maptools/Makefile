CPP = g++
CPPFLAGS = -D_EDIT_ -DREVERSE_ALPHA -g -Wall -I../../ -I/usr/local/include/SDL -I../mapcharedit `sdl-config --cflags` -I/home/maitrise/MAITRISE/include
LIBS = -lz -g `sdl-config --cflags` `sdl-config --libs` -L/home/maitrise/MAITRISE/lib
OBJECTS = pnm.o drawing_area.o image.o screen.o animation.o game.o \
	prefs.o lex.prefs.o input.o mapselect.o win_label.o win_write.o \
	win_query.o animation_off.o \
	win_info.o win_container.o win_select.o win_file.o win_file_select.o \
	win_border.o win_base.o win_font.o win_image.o \
	win_background.o win_scrolled.o win_scrollbar.o win_theme.o

MOBJEDOBJ=mapobject.o maptpl.o mapobjedit.o
MCAREDOBJ=mapcharacter.o maptpl.o landmap.o mapobject.o mapview.o mapcharedit.o
MEDOBJ=maptpl.o mapobject.o mapcharacter.o landmap.o mapview.o mapedit.o
ANIMEDOBJ=animedit.o
MOBJEDEXEC=mapobjedit
MCAREDEXEC=mapcharedit
MEDEXEC=mapedit
ANIMEDEXEC=animedit

all:animedit mapobjedit mapcharedit mapedit

mapedit:$(OBJECTS) $(MEDOBJ)
	$(CPP) $(STDOBJECTS) $(OBJECTS) $(MEDOBJ) $(LIBS) -o $(MEDEXEC)

mapobjedit:$(OBJECTS) $(MOBJEDOBJ)
	$(CPP) $(STDOBJECTS) $(OBJECTS) $(MOBJEDOBJ) $(LIBS) -o $(MOBJEDEXEC)

mapcharedit:$(OBJECTS) $(MCAREDOBJ)
	$(CPP) $(STDOBJECTS) $(OBJECTS) $(MCAREDOBJ) $(LIBS) -o $(MCAREDEXEC)

animedit:$(OBJECTS) $(ANIMEDOBJ)
	$(CPP) $(STDOBJECTS) $(OBJECTS) $(ANIMEDOBJ) $(LIBS) -o $(ANIMEDEXEC)

pnm.o: ../../pnm.cpp
	$(CPP) -c $(CPPFLAGS) ../../pnm.cpp -o pnm.o

drawing_area.o: ../../drawing_area.cpp
	$(CPP) -c $(CPPFLAGS) ../../drawing_area.cpp -o drawing_area.o

image.o: ../../image.cpp
	$(CPP) -c $(CPPFLAGS) ../../image.cpp -o image.o

screen.o: ../../screen.cpp
	$(CPP) -c $(CPPFLAGS) ../../screen.cpp -o screen.o

animation.o: ../../animation.cpp
	$(CPP) -c $(CPPFLAGS) ../../animation.cpp -o animation.o

animation_off.o: ../../animation_off.cpp
	$(CPP) -c $(CPPFLAGS) ../../animation_off.cpp -o animation_off.o

prefs.o: ../../prefs.cpp
	$(CPP) -c $(CPPFLAGS) ../../prefs.cpp -o prefs.o

lex.prefs.o: ../../lex.prefs.cpp
	$(CPP) -c $(CPPFLAGS) ../../lex.prefs.cpp -o lex.prefs.o

input.o: ../../input.cpp
	$(CPP) -c $(CPPFLAGS) ../../input.cpp -o input.o

game.o: ../../game.cpp
	$(CPP) -c $(CPPFLAGS) ../../game.cpp -o game.o

gametime.o: ../../gametime.cpp
	$(CPP) -c $(CPPFLAGS) ../../gametime.cpp -o gametime.o

event.o: ../../event.cpp
	$(CPP) -c $(CPPFLAGS) ../../event.cpp -o event.o

py_inc.o: ../../py_inc.cpp
	$(CPP) -c $(CPPFLAGS) ../../py_inc.cpp -o py_inc.o

py_character_wrap.o: ../../py_character_wrap.cpp
	$(CPP) -c $(CPPFLAGS) ../../py_character_wrap.cpp -o py_character_wrap.o

py_dialog_engine_wrap.o: ../../py_dialog_engine_wrap.cpp
	$(CPP) -c $(CPPFLAGS) ../../py_dialog_engine_wrap.cpp -o py_dialog_engine_wrap.o

py_mapengine_wrap.o: ../../py_mapengine_wrap.cpp
	$(CPP) -c $(CPPFLAGS) ../../py_mapengine_wrap.cpp -o py_mapengine_wrap.o

py_quest_wrap.o: ../../py_quest_wrap.cpp
	$(CPP) -c $(CPPFLAGS) ../../py_quest_wrap.cpp -o py_quest_wrap.o

quest.o: ../../quest.cpp
	$(CPP) -c $(CPPFLAGS) ../../quest.cpp -o quest.o

dialog.o: ../../dialog.cpp
	$(CPP) -c $(CPPFLAGS) ../../dialog.cpp -o dialog.o

mapengine.o: ../../mapengine.cpp
	$(CPP) -c $(CPPFLAGS) ../../mapengine.cpp -o mapengine.o

mapcharacter.o: ../../mapcharacter.cpp
	$(CPP) -c $(CPPFLAGS) ../../mapcharacter.cpp -o mapcharacter.o

mapsquare.o: ../../mapsquare.cpp
	$(CPP) -c $(CPPFLAGS) ../../mapsquare.cpp -o mapsquare.o

map.o: ../../map.cpp
	$(CPP) -c $(CPPFLAGS) ../../map.cpp -o map.o

mappattern.o: ../../mappattern.cpp
	$(CPP) -c $(CPPFLAGS) ../../mappattern.cpp -o mappattern.o

mapevent.o: ../../mapevent.cpp
	$(CPP) -c $(CPPFLAGS) ../../mapevent.cpp -o mapevent.o

character.o: ../../character.cpp
	$(CPP) -c $(CPPFLAGS) ../../character.cpp -o character.o

fileops.o: ../../fileops.cpp
	$(CPP) -c $(CPPFLAGS) ../../fileops.cpp -o fileops.o

yarg.o: ../../yarg.cc
	$(CPP) -c $(CPPFLAGS) ../../yarg.cc -o yarg.o

storage.o: ../../storage.cpp
	$(CPP) -c $(CPPFLAGS) ../../storage.cpp -o storage.o

win_label.o: ../../win_label.cpp
	$(CPP) -c $(CPPFLAGS) ../../win_label.cpp -o win_label.o

win_write.o: ../../win_write.cpp
	$(CPP) -c $(CPPFLAGS) ../../win_write.cpp -o win_write.o

win_query.o: ../../win_query.cpp
	$(CPP) -c $(CPPFLAGS) ../../win_query.cpp -o win_query.o

win_info.o: ../../win_info.cpp
	$(CPP) -c $(CPPFLAGS) ../../win_info.cpp -o win_info.o

win_container.o: ../../win_container.cpp
	$(CPP) -c $(CPPFLAGS) ../../win_container.cpp -o win_container.o

win_select.o: ../../win_select.cpp
	$(CPP) -c $(CPPFLAGS) ../../win_select.cpp -o win_select.o

win_file.o: ../../win_file.cpp
	$(CPP) -c $(CPPFLAGS) ../../win_file.cpp -o win_file.o

win_file_select.o: ../../win_file_select.cpp
	$(CPP) -c $(CPPFLAGS) ../../win_file_select.cpp -o win_file_select.o

win_border.o: ../../win_border.cpp
	$(CPP) -c $(CPPFLAGS) ../../win_border.cpp -o win_border.o

win_base.o: ../../win_base.cpp
	$(CPP) -c $(CPPFLAGS) ../../win_base.cpp -o win_base.o

win_font.o: ../../win_font.cpp
	$(CPP) -c $(CPPFLAGS) ../../win_font.cpp -o win_font.o

win_image.o: ../../win_image.cpp
	$(CPP) -c $(CPPFLAGS) ../../win_image.cpp -o win_image.o

win_background.o: ../../win_background.cpp
	$(CPP) -c $(CPPFLAGS) ../../win_background.cpp -o win_background.o

win_scrolled.o: ../../win_scrolled.cpp
	$(CPP) -c $(CPPFLAGS) ../../win_scrolled.cpp -o win_scrolled.o

win_scrollbar.o: ../../win_scrollbar.cpp
	$(CPP) -c $(CPPFLAGS) ../../win_scrollbar.cpp -o win_scrollbar.o

win_theme.o: ../../win_theme.cpp
	$(CPP) -c $(CPPFLAGS) ../../win_theme.cpp -o win_theme.o

mapselect.o: ../../mapselect.cpp
	$(CPP) -c $(CPPFLAGS) ../../mapselect.cpp -o mapselect.o

mapobject.o: ../../mapobject.cpp
	$(CPP) -c $(CPPFLAGS) ../../mapobject.cpp -o mapobject.o

landmap.o: ../../landmap.cpp
	$(CPP) -c $(CPPFLAGS) ../../landmap.cpp -o landmap.o

mapview.o: ../../mapview.cpp
	$(CPP) -c $(CPPFLAGS) ../../mapview.cpp -o mapview.o

maptpl.o: ../../maptpl.cpp
	$(CPP) -c $(CPPFLAGS) ../../maptpl.cpp -o maptpl.o

clean:
	rm -rf *.o
