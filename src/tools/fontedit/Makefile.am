## Process this file with automake to produce Makefile.in

bin_PROGRAMS = fontedit

fontedit_SOURCES = font_editor.cpp win_font_editor.cpp \
	win_font_editor.h

fontedit_LDADD = pnm.o fileops.o drawing_area.o image.o screen.o \
	drawable.o prefs.o lex.prefs.o input.o \
	win_label.o win_write.o win_query.o win_info.o \
	win_container.o win_select.o win_file.o win_file_select.o\
	win_background.o win_border.o win_base.o win_font.o win_image.o \
	win_scrolled.o win_scrollbar.o win_theme.o mapselect.o $(SDL_LIBS)

CPPFLAGS +=-I../.. -D_EDIT_ -DUSE_MAP $(SDL_CFLAGS)

%.o : ../../%.cpp
	$(CXXCOMPILE) -c $< -o $@

%.o : ../../%.cc
	$(CXXCOMPILE) -c $< -o $@
