
AM_CPPFLAGS		=  \
-I$(top_srcdir)/src/common \
-I$(top_srcdir)/src/xlslib \
-I$(top_srcdir)/src/oledoc 


noinst_PROGRAMS	= testCPP testC PR2859188
testCPP_SOURCES	= mainCPP.cpp md5.c
testCPP_LDADD	= $(top_builddir)/src/libxls.la 
testC_SOURCES	= mainC.c md5.c
testC_LDADD	    = $(top_builddir)/src/libxls.la 
PR2859188_SOURCES = PR2859188.cpp md5.c
PR2859188_LDADD = $(top_builddir)/src/libxls.la 

check_PROGRAMS = testCPP testC PR2859188

TESTS = testCPP testC PR2859188

clean-local:
	-rm -rf *.xls


