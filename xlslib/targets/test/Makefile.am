
AM_CPPFLAGS		=  \
-I$(top_srcdir)/src


noinst_PROGRAMS	  = testCPP testC PR2859188 PR3076678
testCPP_SOURCES	  = mainCPP.cpp md5cpp.cpp
testCPP_LDADD	  = $(top_builddir)/src/libxls.la 
testCPP_LDFLAGS   = -static -lstdc++
testC_SOURCES	  = mainC.c md5.c
testC_LDADD       = $(top_builddir)/src/libxls.la 
testC_LDFLAGS     = -static -lstdc++
PR2859188_SOURCES = PR2859188.cpp md5cpp.cpp
PR2859188_LDADD   = $(top_builddir)/src/libxls.la 
PR2859188_LDFLAGS = -static -lstdc++
PR3076678_SOURCES = PR3076678.cpp md5cpp.cpp
PR3076678_LDADD   = $(top_builddir)/src/libxls.la 
PR3076678_LDFLAGS = -static -lstdc++

check_PROGRAMS = testCPP testC PR2859188 PR3076678

TESTS = testCPP testC PR2859188 PR3076678

clean-local:
	-rm -rf *.xls


