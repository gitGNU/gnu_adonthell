## Process this file with automake to produce Makefile.in

SUBDIRS = examples

EXTRA_DIST = loadlg.l ins_dlgedit_modules.py py_character.i

bin_PROGRAMS = dlgedit

dlgedit_SOURCES = callbacks.cc circle.cc crcle_callbacks.cc \
crcle_interface.cc dbg_callbacks.cc dbg_interface.cc debug.cc \
dlgcompile.cc dlgnode.cc events.cc function.cc geometrie.cc \
graph.cc lex.loadlg.cc interface.cc main.cc preset.cc py_character_wrap.cpp \
ps_callbacks.cc ps_dlg.cc ps_interface.cc pset_callbacks.cc \
pset_interface.cc run.cc run_callbacks.cc run_interface.cc \
callbacks.h circle.h crcle_callbacks.h crcle_interface.h \
dbg_callbacks.h dbg_interface.h debug.h dlgcompile.h dlgnode.h \
events.h function.h geometrie.h graph.h interface.h main.h \
preset.h ps_callbacks.h ps_dlg.h ps_interface.h pset_callbacks.h \
pset_interface.h run.h run_callbacks.h run_interface.h

# Evil, but it works...
CPPFLAGS +=-I../.. -D_DLGEDIT_ -D_VERSION_=\"0.7\" `gtk-config --cflags`

SWIGFLAGS = -D_DLGEDIT_ -D_DEBUG_

LDADD = character.o data.o dialog.o \
event.o fileops.o lex.prefs.o prefs.o py_inc.o \
py_quest_wrap.o quest.o storage.o yarg.o
 

LDFLAGS=-lm -lstdc++ `gtk-config --libs`

lex.loadlg.cc: loadlg.l
	flex -olex.loadlg.cc loadlg.l

py_character_wrap.cpp : py_character.i
	swig $(SWIGFLAGS) -python -shadow -c++ -make_default -o $*.cpp $<
	@rm character.py
 
character.o : ../../character.cpp
	$(CXXCOMPILE) -c $< -o $@

data.o : ../../data.cpp
	$(CXXCOMPILE) -c $< -o $@

dialog.o : ../../dialog.cpp
	$(CXXCOMPILE) -c $< -o $@

event.o : ../../event.cpp
	$(CXXCOMPILE) -c $< -o $@

fileops.o : ../../fileops.cpp
	$(CXXCOMPILE) -c $< -o $@

lex.prefs.o : ../../lex.prefs.cpp
	$(CXXCOMPILE) -c $< -o $@

prefs.o : ../../prefs.cpp
	$(CXXCOMPILE) -c $< -o $@

py_inc.o : ../../py_inc.cpp
	$(CXXCOMPILE) -c $< -o $@

py_quest_wrap.o : ../../py_quest_wrap.cpp
	$(CXXCOMPILE) -c $< -o $@

quest.o : ../../quest.cpp
	$(CXXCOMPILE) -c $< -o $@

storage.o : ../../storage.cpp
	$(CXXCOMPILE) -c $< -o $@

yarg.o : ../../yarg.cc
	$(CXXCOMPILE) -c $< -o $@
