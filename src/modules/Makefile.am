moddatadir = $(gamedatadir)/modules

pyc_files = dialogue.pyc adonthell.pyc
py_files = dialogue.py adonthell.py

moddata_DATA = dialogue.pyc adonthell.pyc

EXTRA_DIST = $(py_files)

all: $(py_files)
	$(top_builddir)/src/adonthell -c

dialogue.pyc: dialogue.py
adonthell.pyc: adonthell.py

dialogue.py:
	cp $(top_srcdir)/src/modules/dialogue.py .

adonthell.py:
	cp $(top_srcdir)/src/modules/adonthell.py .

CLEANFILES = $(pyc_files)
