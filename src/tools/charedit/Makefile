CPP = g++ 

# -- Python 1.6
# CPPFLAGS = -g -D_DEBUG_ `sdl-config --cflags` -I/usr/local/include/python1.6  `gtk-config --cflags` 
# MERGELIBS = -L/usr/local/lib/python1.6/config `glib-config --libs` -lpython1.6 -ldl

# -- Python 2.0
CPPFLAGS = -g -D_DEBUG_ `sdl-config --cflags` -I/usr/local/include/python2.0 -I/usr/local/include/python2.0 `gtk-config --cflags` 
LIBS = -lm -lstdc++ `gtk-config --libs` 
MERGELIBS = -L/usr/local/lib/python2.0/config `glib-config --libs` -lpython2.0 -ldb -lutil -lpthread -ldl

OBJECTS = main.o interface.o callbacks.o dirbrowser.o
EXEC = charedit 

MERGEOBJS = mergechars.o ../../character.o ../../storage.o ../../event.o

all: $(OBJECTS) $(MERGEOBJS)
	$(CPP) $(STDOBJECTS) $(OBJECTS) $(LIBS) -o $(EXEC)
	$(CPP) $(MERGEOBJS) $(MERGELIBS) -o mergechars

clean:
	rm -rf $(OBJECTS) mergechars.o


# -- Dependencies --
main.o: main.h interface.h
interface.o: main.h interface.h callbacks.h
callbacks.o: main.h interface.h callbacks.h dirbrowser.h
dirbrowser.o: main.h callbacks.h
mergechars.o: ../../character.h ../../event.h ../../game.h
