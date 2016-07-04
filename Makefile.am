SUBDIRS = m4 po src doc
EXTRA_DIST = config.rpath ABOUT-NLS NEWBIE autogen.sh \
        autogen.sh debian/README.debian debian/adonthell.6 \
        debian/changelog debian/control debian/copyright debian/dirs \
        debian/rules adonthell.spec.in adonthell.spec README.MacOSX \
        debian/compat debian/docs make_bundle.sh make_windows_pkg.sh

install-data-local:
	if test ! -d $(DESTDIR)$(gamedatadir)/games ; then \
	mkdir $(DESTDIR)$(gamedatadir)/games; \
	fi

ACLOCAL_AMFLAGS = -I m4
