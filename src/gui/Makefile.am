noinst_LIBRARIES = libgui.a

libgui_a_SOURCES = background.cc base.cc border.cc \
	container.cc font.cc keys.cc \
	manager.cc scrollbar.cc scroll.cc select_list.cc theme.cc \
	event.cc label.cc label_input.cc ttf.cc \
	font.h image.h label.h label_input.h\
	scrollbar.h scroll.h select_list.h theme.h keys.h event.h types.h \
	manager.h object.h gui.h ttf.h

CPPFLAGS = -I$(srcdir)/.. $(SDL_DEFS) $(SDL_CFLAGS) $(FT2_CFLAGS) $(PY_CFLAGS)
