SUBDIRS = m4 po src doc
EXTRA_DIST = config.rpath ABOUT-NLS NEWBIE autogen.sh README.BeOS \
        README.FreeBSD autogen.sh debian/README.debian debian/adonthell.6 \
        debian/changelog debian/control debian/copyright debian/dirs \
        debian/rules adonthell.spec.in adonthell.spec README.MacOSX \
	FULLSCREEN.howto debian/compat debian/docs README.Solaris

install-data-local:
	if test ! -d $(DESTDIR)$(gamedatadir)/games ; then \
	mkdir $(DESTDIR)$(gamedatadir)/games; \
	fi

ACLOCAL_AMFLAGS = -I m4
