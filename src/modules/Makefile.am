moddatadir = $(gamedatadir)/modules

moddata_DATA = *.pyc

EXTRA_DIST = dialogue.py adonthell.py

all:
	../adonthell -c

*.pyc: *.py

CLEANFILES = *.pyc
