## Process this file with automake to produce Makefile.in

EXTRA_DIST = reference.cfg mainpage.dxt intro.dxt prog_rules.dxt event.dxt \
	map.dxt dialog.dxt python.dxt mapcharschedules.dxt save.dxt \
	mapviewschedules.dxt newmap.dxt

if BUILD_DOC
all:
	$(P_DOXYGEN) reference.cfg

CLEANFILES = -R html latex
endif
