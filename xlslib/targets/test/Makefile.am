
INCLUDES		=    \
-I$(top_builddir)/src/common \
-I$(top_builddir)/src/xlslib \
-I$(top_builddir)/src/oledoc 


noinst_PROGRAMS	= testCPP testC
testCPP_SOURCES	= mainCPP.cpp
testCPP_LDADD	= $(top_builddir)/src/libxls.la 
testC_SOURCES	= mainC.c
testC_LDADD		= $(top_builddir)/src/libxls.la 

clean-local:
	-rm -rf .deps

