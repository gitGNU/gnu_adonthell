SUBDIRS = intl po src doc
EXTRA_DIST = config.rpath ABOUT-NLS NEWBIE autogen.sh README.BeOS \
        README.FreeBSD autogen.sh debian/README.debian debian/adonthell.6 \
        debian/changelog debian/control debian/copyright debian/dirs \
        debian/rules adonthell.spec.in adonthell.spec README.MacOSX

install-data-local:
	if test ! -d $(gamedatadir)/games ; then \
	mkdir $(gamedatadir)/games; \
	fi

