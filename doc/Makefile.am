## Process this file with automake to produce Makefile.in

EXTRA_DIST = charedit.lyx dlgedit.lyx dlgedit_debug.ps dlgedit_edit.ps dlgedit_main.ps dlgedit_run.ps \
event.lyx interpreter.lyx items.txt \
step1.ps step2.ps step3.ps step4.ps reference.cfg \
mainpage.dxt intro.dxt prog_rules.dxt event.dxt map.dxt dialog.dxt python.dxt

if BUILD_DOC
all:
	$(P_DOXYGEN) reference.cfg

clean:
	rm -Rf html latex
endif
