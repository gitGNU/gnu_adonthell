SUBDIRS = src doc
EXTRA_DIST = NEWBIE autogen.sh README.BeOS README.FreeBSD autogen.sh \
	debian/README.debian debian/adonthell.6 debian/changelog debian/control \
	debian/copyright debian/dirs debian/rules adonthell.spec.in

install-data-local:
	if test ! -e $(gamedatadir)/games ; then \
	mkdir $(gamedatadir)/games; \
	fi
