## Process this file with automake to produce Makefile.in

EXTRA_DIST = dlgedit-api.cfg

if BUILD_DOC
all:
	$(P_DOXYGEN) dlgedit-api.cfg

CLEANFILES = -R html latex
endif
