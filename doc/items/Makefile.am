## Process this file with automake to produce Makefile.in

EXTRA_DIST = items.tex items.cfg items.dxt items.sty Makefile.latex

if BUILD_DOC
all:
	$(P_DOXYGEN) items.cfg
	@cp items.sty latex/items.sty
	@cp Makefile.latex latex/Makefile
	@mv latex/refman.tex latex/items.tex    

CLEANFILES = -R html latex
endif
