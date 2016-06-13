moddatadir = $(gamedatadir)/modules

pyc_files = dialogue.pyc adonthell.pyc
py_files = dialogue.py adonthell.py

if COMPILE_SCRIPTS
nodist_moddata_DATA = $(pyc_files)
all: $(py_files) 
	@if test x$(top_srcdir) != x$(top_builddir) ; then \
	    for i in $(py_files); do \
	        cp $(srcdir)/$$i $(builddir)/$$i; \
	    done \
	fi ;

	$(top_builddir)/src/adonthell-0.3 -c
else
nodist_moddata_DATA = $(py_files)
endif

EXTRA_DIST = $(py_files)

$(pyc_files) : $(py_files)

CLEANFILES = $(pyc_files)
