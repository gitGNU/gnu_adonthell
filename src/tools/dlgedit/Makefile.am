bin_PROGRAMS = dlgedit

dlgedit_SOURCES = callbacks.cc circle.cc crcle_callbacks.cc \
crcle_interface.cc dbg_callbacks.cc dbg_interface.cc debug.cc \
dlgcompile.cc dlgnode.cc events.cc function.cc geometrie.cc \
graph.cc lex.loadlg.cc interface.cc main.cc preset.cc \
ps_callbacks.cc ps_dlg.cc ps_interface.cc pset_callbacks.cc \
pset_interface.cc py_character_wrap.cpp run.cc run_callbacks.cc \
run_interface.cc

# Evil, but it works...
CPPFLAGS +=-I../.. -D_DLGEDIT_ -D_DEBUG_ -D_VERSION_=\"0.7\" `gtk-config --cflags`

LDADD = ../../character.o ../../data.o ../../dialog.o \
../../event.o ../../fileops.o ../../lex.prefs.o ../../prefs.o ../../py_inc.o \
 ../../py_quest_wrap.o  ../../quest.o  ../../storage.o ../../yarg.o
 

LDFLAGS=-lm -lstdc++ `gtk-config --libs`

lex.loadlg.cc: loadlg.l
	flex -olex.loadlg.cc loadlg.l

py_character_wrap.cpp : py_character.i
	swig -python -shadow -c++ -make_default -o $*.cpp $<
	@rm character.py
 
