## Process this file with automake to produce Makefile.in

SUBDIRS = examples

EXTRA_DIST = loadlg.l ins_dlgedit_modules.py py_character.i py_quest.i

bin_PROGRAMS = dlgedit

dlgedit_SOURCES = callbacks.cc circle.cc crcle_callbacks.cc \
crcle_interface.cc dbg_callbacks.cc dbg_interface.cc debug.cc \
dlgcompile.cc dlgnode.cc events.cc function.cc geometrie.cc \
graph.cc lex.loadlg.cc interface.cc main.cc preset.cc py_character_wrap.cpp \
py_quest_wrap.cpp ps_callbacks.cc ps_dlg.cc ps_interface.cc pset_callbacks.cc \
pset_interface.cc run.cc run_callbacks.cc run_interface.cc \
callbacks.h circle.h crcle_callbacks.h crcle_interface.h \
dbg_callbacks.h dbg_interface.h debug.h dlgcompile.h dlgnode.h \
events.h function.h geometrie.h graph.h interface.h main.h \
preset.h ps_callbacks.h ps_dlg.h ps_interface.h pset_callbacks.h \
pset_interface.h run.h run_callbacks.h run_interface.h

# Evil, but it works...
CPPFLAGS +=-I../.. -DUSE_PYTHON -D_VERSION_=\"0.7\" $(PY_CFLAGS) $(GTK_CFLAGS) 

SWIGFLAGS = -D_DLGEDIT_ -D_DEBUG_

dlgedit_LDADD = character.o data.o dialog.o \
event.o fileops.o lex.prefs.o prefs.o py_inc.o \
quest.o storage.o yarg.o character_base.o $(PY_LIBS) $(GTK_LIBS) -lm -lstdc++

lex.loadlg.cc: loadlg.l
	flex -olex.loadlg.cc loadlg.l

py_%_wrap.cpp : py_%.i
	swig $(SWIGFLAGS) -python -shadow -c++ -make_default -o py_%_wrap.cpp $<

py_quest_wrap.cpp : py_quest.i
	swig $(SWIGFLAGS) -python -shadow -c++ -make_default -o py_quest_wrap.cpp $<
 
%.o : ../../%.cpp
	$(CXXCOMPILE) -c $< -o $@

%.o : ../../%.cc
	$(CXXCOMPILE) -c $< -o $@
