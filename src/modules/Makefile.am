moddatadir = $(gamedatadir)/modules

moddata_DATA = *.pyc

EXTRA_DIST = dialogue.py adonthell.py

all: *.py
	../adonthell -c

*.pyc: *.py

CLEANFILES = *.pyc
