bin_PROGRAMS = dlgedit

dlgedit_SOURCES = callbacks.cc \
circle.cc compile.cc cond.tab.cc crcle_callbacks.cc \
crcle_interface.cc create_code.cc dlgnode.cc error_dlg.cc \
err_callbacks.cc err_interface.cc events.cc  \
geometrie.cc graph.cc  \
lex.cond.cc lex.loadlg.cc lex.vars.cc \
main.cc pjt_interface.cc pjt_callbacks.cc preset.cc project.cc \
ps_callbacks.cc ps_dlg.cc ps_interface.cc pset_callbacks.cc \
pset_interface.cc run.cc run_callbacks.cc run_interface.cc \
vars.tab.cc interface.cc

# Evil, but it works...
CPPFLAGS += -Wall -Werror -D_DLGEDIT_ `gtk-config --cflags`

LDADD = ../../character.o ../../interpreter.o ../../yarg.o ../../storage.o ../../dialog_cmds.o \
../../generic_cmds.o 

LDFLAGS=-lm -lstdc++ `gtk-config --libs`

lex.loadlg.cc: loadlg.l
	flex -olex.loadlg.cc loadlg.l

lex.vars.cc: variable.l
	flex -olex.vars.cc variable.l

vars.tab.cc: variable.y
	bison -d -p vars -o vars.tab.cc variable.y

lex.cond.cc: condition.l
	flex -olex.cond.cc condition.l

cond.tab.cc: condition.y
	bison -d -p cond -o cond.tab.cc condition.y




