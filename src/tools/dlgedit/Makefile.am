bin_PROGRAMS = dlgedit

dlgedit_SOURCES = callbacks.cc \
circle.cc dlgcompile.cc crcle_callbacks.cc \
crcle_interface.cc dlgnode.cc error_dlg.cc \
err_callbacks.cc err_interface.cc events.cc  \
geometrie.cc graph.cc  \
lex.loadlg.cc function.cc \
main.cc pjt_interface.cc pjt_callbacks.cc preset.cc project.cc \
ps_callbacks.cc ps_dlg.cc ps_interface.cc pset_callbacks.cc \
pset_interface.cc run.cc run_callbacks.cc run_interface.cc \
interface.cc 

# Evil, but it works...
CPPFLAGS += -D_DLGEDIT_ -D_DEBUG_ -D_VERSION_=\"0.6\" `gtk-config --cflags`

LDADD = ../../inventory.o ../../py_mapengine_wrap.o \
../../animation.o ../../py_player_wrap.o \
../../audio.o ../../lex.prefs.o ../../screen.o \
../../audio_thread.o ../../event.o \
../../character.o ../../map.o ../../win_base.o \
../../mapcharacter.o ../../win_border.o \
../../cutscene.o ../../mapengine.o ../../win_container.o \
../../mapevent.o ../../win_font.o \
../../fileops.o ../../mapitem.o \
../../game.o ../../mappattern.o ../../win_image.o \
../../mapsquare.o ../../win_label.o \
../../image.o ../../pnm.o ../../win_select.o \
../../init.o ../../prefs.o ../../win_write.o \
../../input.o ../../yarg.o ../../win_anim.o \
../../py_inc.o ../../dialog.o ../../storage.o

LDFLAGS=-lm -lstdc++ `gtk-config --libs`

lex.loadlg.cc: loadlg.l
	flex -olex.loadlg.cc loadlg.l
