## Process this file with automake to produce Makefile.in
 
SUBDIRS = . modules

bin_PROGRAMS = adonthell

EXTRA_DIST = .indent.pro prefs.l py_adonthell.i

AM_CPPFLAGS =$(SDL_DEFS) $(OGG_DEFS) $(SDL_CFLAGS) $(OGG_CFLAGS) $(PY_CFLAGS) $(FT2_CFLAGS)

noinst_LIBRARIES = libadonthell.a

CLEANFILES = $(moddata_SCRIPTS)

headers = \
adonthell.h animation.h audio.h callback.h py_callback.h character_base.h \
character.h gamedata.h data_screen.h dialog.h dialog_screen.h drawable.h \
drawing_area.h event.h event_handler.h event_handler_base.h event_list.h \
fileops.h game.h gamedate.h gametime.h gettext.h image.h input.h \
landmap.h map_event.h map_event_handler.h mapsquare.h \
mapsquare_walkable.h nls.h mapcharacter.h mapobject.h mapview.h path.h \
pnm.h prefs.h python_class.h py_object.h quest.h screen.h surface.h storage.h \
types.h text_bubble.h time_event.h time_event_handler.h win_background.h \
win_base.h win_border.h win_container.h str_hash.h win_font.h win_image.h \
win_label.h win_mapview.h label.h label_input.h win_scrollbar.h win_scroll.h \
win_select.h win_theme.h win_keys.h win_event.h win_types.h win_write.h \
win_ttf.h win_manager.h win_object.h window.h win_wrappers.h yarg.h SDL_ttf.h

libadonthell_a_SOURCES = adonthell.cc animation.cc audio.cc \
character_base.cc character.cc data_screen.cc dialog.cc dialog_screen.cc \
drawable.cc drawing_area.cc event.cc event_handler.cc event_list.cc \
fileops.cc game.cc gamedata.cc gamedate.cc gametime.cc image.cc input.cc \
label.cc label_input.cc landmap.cc \
lex.prefs.cc map_event.cc \
map_event_handler.cc mapsquare_walkable.cc mapcharacter.cc mapsquare.cc \
mapobject.cc mapview.cc nls.cc path.cc pnm.cc prefs.cc py_callback.cc \
python_class.cc py_adonthell_wrap.cc py_object.cc quest.cc screen.cc \
surface.cc storage.cc text_bubble.cc time_event.cc time_event_handler.cc \
win_background.cc win_base.cc win_border.cc win_container.cc win_font.cc \
win_ttf.cc win_keys.cc win_manager.cc win_scrollbar.cc win_scroll.cc \
win_select.cc \
win_theme.cc win_event.cc yarg.cc mixer.c music.c music_cmd.c music_cmd.h \
music_ogg.c music_ogg.h wave.h wavestream.c wavestream.h SDL_mixer.h \
SDL_ttf.c $(headers)

libadonthell_LDADD = $(SDL_LIBS) $(OGG_LIBS) $(PY_LIBS) $(INTLLIBS) $(FT2_LIBS)

adonthell_LDADD = libadonthell.a $(libadonthell_LDADD)

adonthell_SOURCES = main.cc

# Note: adonthell.py is also built by this target. 
py_adonthell_wrap.cc : py_adonthell.i $(headers)
	@if test "${P_SWIG}"; then \
	   echo ${P_SWIG} -python -modern -shadow ${SDL_CFLAGS} -I$(srcdir) -I$(srcdir)/.. -c++ -makedefault -o $(srcdir)/$*.cc $(srcdir)/py_adonthell.i; \
	   ${P_SWIG} -python -modern -shadow ${SDL_CFLAGS} -I$(srcdir) -I$(srcdir)/.. -c++ -makedefault -o $(srcdir)/$*.cc $(srcdir)/py_adonthell.i; \
           mv $(srcdir)/adonthell.py modules/adonthell.py; \
	else \
	   echo "You need swig >= ${SWIG_MINVER} in order to re-build this file."; \
	   exit 1; \
	fi;

lex.prefs.cc : prefs.l
	flex -olex.prefs.cc $<

# -- MacOS X App Bundle
if MAKE_BUNDLE
bundle_contents = Adonthell.app/Contents

Adonthell.app: Adonthell $(srcdir)/modules/*.py $(srcdir)/modules/*.pyc
	mkdir -p $(bundle_contents)/MacOS/modules
	mkdir -p $(bundle_contents)/Resources
	echo "APPL????" > $(bundle_contents)/PkgInfo
	$(INSTALL_PROGRAM) $< $(bundle_contents)/MacOS/
	$(INSTALL_DATA) modules/*.py $(bundle_contents)/MacOS/modules/
	$(INSTALL_DATA) modules/*.pyc $(bundle_contents)/MacOS/modules/
	
install-exec-hook: Adonthell.app
	rm -rf $(DESTDIR)/Applications/Adonthell.app
	mkdir -p $(DESTDIR)/Applications
	cp -r $< $(DESTDIR)/Applications/
endif
