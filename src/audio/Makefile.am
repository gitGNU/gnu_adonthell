EXTRA_DIST = py_audio.i

noinst_LIBRARIES = libaudio.a

libaudio_a_SOURCES =

CPPFLAGS = -I$(srcdir)/.. $(SDL_DEFS) $(SDL_CFLAGS) $(PY_CFLAGS)

SWIGFLAGS = -I$(srcdir)/.. $(SDL_CFLAGS)

# Note: .py files are also built by this target.
#py_%_wrap.cc: py_%.i
#	@if test "${P_SWIG}"; then \
#	   ${P_SWIG} -python -shadow ${SWIGFLAGS} -c++ -make_default -o py_$*_wrap.cc $<; \
#	else \
#	   echo "You need swig >= $SWIG_MINVER in order to re-build this file."; \
#	   exit 1; \
#	fi;

py_audio_wrap.cc:
	touch py_audio_wrap.cc
