EXTRA_DIST = py_lmap.i

noinst_LIBRARIES = libmap.a

libmap_a_SOURCES = coordinates.cc placeable_area.cc \
	placeable_area_gfx.cc placeable_model.cc \
	placeable_model_gfx.cc placeable.cc placeable_gfx.cc \
	moving.cc object.cc object_with_gfx.cc character.cc \
	character_with_gfx.cc landmap.cc \
	coordinates.h placeable_area.h \
	placeable_area_gfx.h placeable_model.h \
	placeable_model_gfx.h placeable.h placeable_gfx.h \
	moving.h object.h object_with_gfx.h character.h \
	character_with_gfx.h landmap.h

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

py_lmap_wrap.cc:
	touch py_lmap_wrap.cc
