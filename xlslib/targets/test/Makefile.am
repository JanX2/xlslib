
AM_CPPFLAGS		=  \
-I$(top_srcdir)/src/common \
-I$(top_srcdir)/src/xlslib \
-I$(top_srcdir)/src/oledoc 


noinst_PROGRAMS	= testCPP testC
testCPP_SOURCES	= mainCPP.cpp
testCPP_LDADD	= $(top_builddir)/src/libxls.la 
testC_SOURCES	= mainC.c
testC_LDADD	= $(top_builddir)/src/libxls.la 

check_PROGRAMS = testCPP testC

TESTS = testCPP testC

clean-local:
	-rm -rf *.xls


