#auxdir					= m4 
#@AC_AUX_DIR@
AUX_DIST				= $(auxdir)/install-sh $(auxdir)/missing \
							$(auxdir)/mkinstalldirs
AUX_DIST_EXTRA			= $(auxdir)/readline.m4 $(auxdir)/sys_errlist.m4 \
							$(auxdir)/sys_siglist.m4
EXTRA_DIST				= bootstrap

#AUTOMAKE_OPTIONS 		= foreign color-tests
SUBDIRS 				= src targets
MAINTAINERCLEANFILES 	= Makefile.in aclocal.m4 acinclude.m4 configure sic/config-h.in \
							sic/stamp-h.in $(AUX_DIST)

#ACLOCAL				= aclocal
ACLOCAL_AMFLAGS			= -I m4 


##dist-hook:
##	(cd $(distdir) && mkdir $(auxdir))
##	for file in $(AUX_DIST) $(AUX_DIST_EXTRA); do \
##	  cp $$file $(distdir)/$$file; \
##	done

# see also the GNU libtool docs for AC_PROG_LIBTOOL et al:
LIBTOOL_DEPS = @LIBTOOL_DEPS@
libtool: $(LIBTOOL_DEPS)
	$(SHELL) ./config.status --recheck

