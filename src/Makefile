CC = g++

# These two lines are for SDL and GGI
CPPFLAGS = -Wall -DSDL -DSDL_MIXER -O6 -D_REENTRANT -I/usr/local/include/SDL
LIBS = -lpthread -lSDL -lSDL_mixer

# These two lines are for only GGI
#CPPFLAGS = -Wall -O6
#LIBS = -lggi
 
OBJECTS = interpreter.o commands.o dialog_cmds.o main.o init.o fileops.o \
	keyboard.o gfx.o pnm.o window.o array_tmpl.o dlg_io.o mappattern.o \
	mapsquare.o mapevent.o mapcharacter.o map.o mapengine.o \
	audio.o audio_thread.o instruction_class.o instruction.o

main:$(OBJECTS)
	$(CC) $(OBJECTS) $(LIBS) -o ../mapengine

clean:
	rm -f $(OBJECTS) test *~
