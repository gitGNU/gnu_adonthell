CPP = g++ 
CPPFLAGS = -g -D_DEBUG_ `sdl-config --cflags` -I/usr/local/include/python1.6 `gtk-config --cflags` 
LIBS = -lm -lstdc++ `gtk-config --libs` 

OBJECTS = main.o interface.o callbacks.o dirbrowser.o
EXEC = charedit 

MERGEOBJS = mergechars.o ../../character.o ../../storage.o ../../event.o ../../prefs.o ../../lex.prefs.o

all: $(OBJECTS) $(MERGEOBJS)
	$(CPP) $(STDOBJECTS) $(OBJECTS) $(LIBS) -o $(EXEC)
	$(CPP) $(MERGEOBJS) -L/usr/local/lib/python1.6/config `glib-config --libs` -lpython1.6 -ldl -o mergechars

clean:
	rm -rf $(OBJECTS) mergechars.o
