## Process this file with automake to produce Makefile.in

EXTRA_DIST = dlgedit.tex dlgedit.cfg dlgedit.dxt dlgedit.sty \
	dlgedit_debug.eps dlgedit_debug.png dlgedit_edit.eps \
	dlgedit_edit.png dlgedit_main.eps dlgedit_main.png \
	dlgedit_run.eps dlgedit_run.png step1.eps step1.png \
	step2.eps step2.png step3.eps step3.png step4.eps \
	step4.png header.html footer.html

if BUILD_DOC
all:
	$(P_DOXYGEN) dlgedit.cfg
	@cp dlgedit.sty latex/dlgedit.sty
clean:
	rm -Rf html latex
endif
