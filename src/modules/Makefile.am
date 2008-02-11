moddatadir = $(gamedatadir)/modules

pyc_files = dialogue.pyc adonthell.pyc
py_files = dialogue.py adonthell.py

moddata_DATA = $(pyc_files)

EXTRA_DIST = $(py_files)

all: $(py_files) 
	$(top_builddir)/src/adonthell-0.3 -c

%.pyc : %.py
	@if test x$(top_srcdir) != x$(top_builddir) ; then \
		cp $< . ; \
	fi ;

CLEANFILES = $(pyc_files)
