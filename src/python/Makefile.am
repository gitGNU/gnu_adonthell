EXTRA_DIST = py_python.i

noinst_LIBRARIES = libpython.a

libpython_a_SOURCES = base.cc object.cc callback.cc \
	base.h object.h callback.h

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

py_python_wrap.cc:
	touch py_python_wrap.cc
