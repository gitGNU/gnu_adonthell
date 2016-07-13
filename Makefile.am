SUBDIRS = m4 po src doc
EXTRA_DIST = config.rpath ABOUT-NLS NEWBIE autogen.sh \
        autogen.sh adonthell.spec.in adonthell.spec README.MacOSX \
        README.Win32 make_bundle.sh make_windows_pkg.sh

dist_man_MANS = adonthell.6

install-data-local:
	if test ! -d $(DESTDIR)$(gamedatadir)/games ; then \
	mkdir $(DESTDIR)$(gamedatadir)/games; \
	fi

ACLOCAL_AMFLAGS = -I m4
