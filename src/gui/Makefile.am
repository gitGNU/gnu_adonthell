noinst_LIBRARIES = libgui.a

libgui_a_SOURCES = box.cc misc.cc container.cc label.cc \
	font.cc widget.cc fixed.cc  \
	bin.cc window.cc manager.cc border_template.cc \
	border_ui.cc theme.cc background_template.cc \
	button_template.cc button.cc button_ui.cc \
	background_ui.cc \
	box.h misc.h container.h label.h \
	font.h widget.h fixed.h \
	bin.h window.h manager.h border_template.h  \
	border_ui.h theme.h background_template.h \
	button_template.h button.h button_ui.h \
	background_ui.h 


CPPFLAGS = -I$(srcdir)/.. $(SDL_DEFS) $(SDL_CFLAGS) $(FT2_CFLAGS) $(PY_CFLAGS)
