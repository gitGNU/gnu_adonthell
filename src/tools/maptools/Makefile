CPP = g++
CPPFLAGS = -D_EDIT_ -DREVERSE_ALPHA -g -Wall -I../../ -I/usr/local/include/SDL -I../mapcharedit `sdl-config --cflags`
LIBS = -lz -g `sdl-config --cflags` `sdl-config --libs`
OBJECTS = pnm.o drawing_area.o image.o screen.o animation.o \
	prefs.o lex.prefs.o input.o \
	win_label.o win_write.o win_query.o win_info.o \
	win_container.o win_select.o win_file.o \
	win_border.o win_base.o win_font.o win_image.o\
	win_background.o win_scrolled.o win_scrollbar.o win_theme.o mapselect.o

MOBJEDOBJ=mapobject.o mapobjedit.o
MEDOBJ=mapobject.o landmap.o mapview.o mapedit.o
ANIMEDOBJ=animedit.o
MOBJEDEXEC=mapobjedit
MEDEXEC=mapedit
ANIMEDEXEC=animedit

all:animedit mapobjedit mapedit

mapedit:$(OBJECTS) $(MEDOBJ)
	$(CPP) $(STDOBJECTS) $(OBJECTS) $(MEDOBJ) $(LIBS) -o $(MEDEXEC)

mapobjedit:$(OBJECTS) $(MOBJEDOBJ)
	$(CPP) $(STDOBJECTS) $(OBJECTS) $(MOBJEDOBJ) $(LIBS) -o $(MOBJEDEXEC)

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

prefs.o: ../../prefs.cpp
	$(CPP) -c $(CPPFLAGS) ../../prefs.cpp -o prefs.o

lex.prefs.o: ../../lex.prefs.cpp
	$(CPP) -c $(CPPFLAGS) ../../lex.prefs.cpp -o lex.prefs.o

input.o: ../../input.cpp
	$(CPP) -c $(CPPFLAGS) ../../input.cpp -o input.o

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

clean:
	rm -rf *.o
