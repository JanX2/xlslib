auxdir					= @ac_aux_dir@
AUX_DIST				= $(auxdir)/install-sh $(auxdir)/missing \
							$(auxdir)/mkinstalldirs
AUX_DIST_EXTRA			= $(auxdir)/readline.m4 $(auxdir)/sys_errlist.m4 \
							$(auxdir)/sys_siglist.m4
EXTRA_DIST				= bootstrap

AUTOMAKE_OPTIONS 		= foreign
SUBDIRS 				= src targets
MAINTAINERCLEANFILES 	= Makefile.in aclocal.m4 configure sic/config-h.in \
							sic/stamp-h.in $(AUX_DIST)

ACLOCAL					= aclocal -I $(auxdir)
ACLOCAL_AMFLAGS			= -I m4 


##dist-hook:
##	(cd $(distdir) && mkdir $(auxdir))
##	for file in $(AUX_DIST) $(AUX_DIST_EXTRA); do \
##	  cp $$file $(distdir)/$$file; \
##	done
