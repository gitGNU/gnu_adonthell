## Process this file with automake to produce Makefile.in

if HAVE_GTK
noinst_PROGRAMS = dlgedit
endif

SUBDIRS = examples

# EXTRA_DIST = loadlg.l ins_dlgedit_modules.py py_character.i py_quest.i character.py quest.py

dlgedit_SOURCES = \
    dlg_arrow.cc \
    dlg_circle.cc \
    dlg_circle_entry.cc \
    dlg_compiler.cc \
    dlg_module.cc \
    dlg_mover.cc \
    dlg_node.cc \
    dlg_point.cc \
    dlg_rect.cc \
    gui_circle.cc \
    gui_circle_events.cc \
    gui_dlgedit.cc \
    gui_dlgedit_events.cc \
    gui_edit.cc \
    gui_file.cc \
    gui_graph.cc \
    gui_graph_events.cc \
    gui_list.cc \
    gui_list_events.cc \
    gui_messages.cc \
    gui_modal_dialog.cc \
    gui_tooltip.cc \
    lex.loadlg.cc \
    main.cc \
    dlg_arrow.h \
    dlg_circle.h \
    dlg_circle_entry.h \
    dlg_compiler.h \
    dlg_module.h \
    dlg_mover.h \
    dlg_node.h \
    dlg_node_gfx.h \
    dlg_point.h \
    dlg_rect.h \
    dlg_types.h \
    gui_circle.h \
    gui_circle_events.h \
    gui_dlgedit.h \
    gui_dlgedit_events.h \
    gui_edit.h \
    gui_file.h \
    gui_graph.h \
    gui_graph_events.h \
    gui_list.h \
    gui_list_events.h \
    gui_messages.h \
    gui_modal_dialog.h \
    gui_tooltip.h

# Evil, but it works...
CPPFLAGS +=-I../.. -D_VERSION_=\"0.8pre1\" $(PY_CFLAGS) $(GTK_CFLAGS) $(SDL_CFLAGS)

SWIGFLAGS = -D_DLGEDIT_ -D_DEBUG_

# dlgedit_LDADD = ../../libadonthell.a \
# $(SDL_LIBS) $(PY_LIBS) $(GTK_LIBS)

dlgedit_LDADD = $(GTK_LIBS)
#dlgedit_LDFLAGS = -Wl,-E

lex.loadlg.cc: loadlg.l
	flex -olex.loadlg.cc loadlg.l

py_character_wrap.cc : py_character.i ../../character.h ../../character_base.h
	swig $(SWIGFLAGS) -python -shadow -c++ -make_default -o py_character_wrap.cc $<

py_quest_wrap.cc : py_quest.i ../../quest.h
	swig $(SWIGFLAGS) -python -shadow -c++ -make_default -o py_quest_wrap.cc $<
