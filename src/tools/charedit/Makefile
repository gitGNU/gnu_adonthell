CPP = g++ 
CPPFLAGS = -g -D_DEBUG_ `gtk-config --cflags` 
LIBS = -lm -lstdc++ `gtk-config --libs` 

OBJECTS = main.o interface.o callbacks.o dirbrowser.o
EXEC = charedit 

all: $(OBJECTS)
	$(CPP) $(STDOBJECTS) $(OBJECTS) $(LIBS) -o $(EXEC)

clean:
	rm -rf $(OBJECTS)
