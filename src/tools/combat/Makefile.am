## Process this file with automake to produce Makefile.in

bin_PROGRAMS = combat-test itemtest

CPPFLAGS = $(SDL_DEFS) $(SDL_CFLAGS) $(PY_CFLAGS)

EXTRA_DIST = py_itemtest_wrap.cc py_itemtest.i

combat_test_SOURCES = combat-test.cc melee.cc yarg.cc combat_classes.h \
	items.h

combat_test_LDADD = ${SDL_LIBS}

itemtest_SOURCES = character_base.cc item_base.cc storage.cc python_class.cc \
	py_object.cc itemtest.cc py_itemtest_wrap.cc fileops.cc \
	character_base.h item_base.h storage.h python_class.h py_object.h \
	fileops.h

itemtest_LDADD = ${PY_LIBS}

py_itemtest_wrap.cc : py_itemtest.i *.h
	@if test "${P_SWIG}"; then \
	   ${P_SWIG} -python -shadow -I./ -I../ -c++ -make_default -o $*.cc py_itemtest.i; \
	else \
	   echo "You need swig >= 1.3.10 in order to re-build this file."; \
	   exit 1; \
	fi;
