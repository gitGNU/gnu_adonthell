EXTRA_DIST = py_gfx.i

noinst_LIBRARIES = libgfx.a

headers = drawable.h drawing_area.h screen.h surface.h pnm.h image.h \
	animation.h

libgfx_a_SOURCES = drawable.cc drawing_area.cc screen.cc surface.cc pnm.cc \
	image.cc animation.cc py_gfx_wrap.cc \
	$(headers)

CPPFLAGS = -I$(srcdir)/.. $(SDL_DEFS) $(SDL_CFLAGS) $(PY_CFLAGS)

SWIGFLAGS = -I$(srcdir)/.. $(SDL_CFLAGS)

# Note: .py files are also built by this target.
py_%_wrap.cc: py_%.i $(headers)
	@if test "${P_SWIG}"; then \
	   ${P_SWIG} -python -shadow ${SWIGFLAGS} -c++ -make_default -o py_$*_wrap.cc $<; \
	else \
	   echo "You need swig >= $SWIG_MINVER in order to re-build this file."; \
	   exit 1; \
	fi;
