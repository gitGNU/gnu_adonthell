EXTRA_DIST = py_input.i

noinst_LIBRARIES = libinput.a

libinput_a_SOURCES = event.cc joystick_event.cc mouse_event.cc \
	keyboard_event.cc control_event.cc listener.cc manager.cc \
	manager_SDL.cc py_input_wrap.cc \
	event.h joystick_event.h mouse_event.h keyboard_event.h \
	control_event.h listener.h manager.h

CPPFLAGS += -I$(srcdir)/.. $(SDL_DEFS) $(SDL_CFLAGS) $(PY_CFLAGS)

SWIGFLAGS = -I$(srcdir)/.. $(SDL_CFLAGS)

# Note: .py files are also built by this target.
#py_%_wrap.cc: py_%.i
#	@if test "${P_SWIG}"; then \
#	   ${P_SWIG} -python -shadow ${SWIGFLAGS} -c++ -make_default -o py_$*_wrap.cc $<; \
#	else \
#	   echo "You need swig >= $SWIG_MINVER in order to re-build this file."; \
#	   exit 1; \
#	fi;

py_input_wrap.cc:
	touch py_input_wrap.cc
