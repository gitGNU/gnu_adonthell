CPP = g++ 
CPPFLAGS = -Wall -g -D_DEBUG_ `gtk-config --cflags` 
LIBS = -lm -lstdc++ `gtk-config --libs` 

OBJECTS = main.o interface.o callbacks.o dirbrowser.o
EXEC = questedit 

MERGEOBJS = mergequests.o ../../quest.o ../../storage.o ../../fileops.o

all: $(OBJECTS) $(MERGEOBJS)
	$(CPP) $(STDOBJECTS) $(OBJECTS) $(LIBS) -o $(EXEC)
	$(CPP) $(MERGEOBJS) `glib-config --libs` -lz -o mergequests

clean:
	rm -rf $(OBJECTS) mergequests.o
