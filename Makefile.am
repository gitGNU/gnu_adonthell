SUBDIRS = src doc
EXTRA_DIST = NEWBIE debian/README.Debian debian/changelog debian/control debian/copyright \
	debian/dirs debian/docs debian/adonthell.1 debian/manpage.sgml debian/menu debian/postinst \
	debian/postrm debian/rules autogen.sh README.BeOS README.FreeBSD

install-data-local:
	if test ! -e $(gamedatadir)/games ; then \
	mkdir $(gamedatadir)/games; \
	fi
