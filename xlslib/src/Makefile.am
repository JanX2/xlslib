#SUBDIRS 		= common oledoc xlslib

# WAS CXXFLAGS = -O2 DFH 
# SEE IF WE REALLY NEED IT THOUGH 
# AM_CXXFLAGS = -O2

INCLUDES		=    \
-I$(top_builddir)/src/data   \
-I$(top_builddir)/src/oledoc \
-I$(top_builddir)/src/common \
-I$(top_builddir)/src/xlslib

lib_LTLIBRARIES 		= libxls.la

# DFH addition
libxls_la_LDFLAGS = $(XLSLIB_LIBS)

libxls_la_SOURCES 	=         \
common/overnew.cpp            \
xlslib/blank.cpp		      \
xlslib/cbridge.cpp		      \
xlslib/cell.cpp			      \
xlslib/colinfo.cpp		      \
xlslib/continue.cpp		      \
xlslib/datast.cpp		      \
xlslib/docsumminfo.cpp		  \
xlslib/extformat.cpp		  \
xlslib/font.cpp			      \
xlslib/format.cpp		      \
xlslib/globalrec.cpp		  \
xlslib/HPSF.cpp			      \
xlslib/index.cpp		      \
xlslib/label.cpp		      \
xlslib/merged.cpp		      \
xlslib/number.cpp		      \
xlslib/range.cpp		      \
xlslib/recdef.cpp		      \
xlslib/record.cpp		      \
xlslib/row.cpp			      \
xlslib/sheetrec.cpp		      \
xlslib/summinfo.cpp		      \
xlslib/unit.cpp			      \
xlslib/workbook.cpp		      \
oledoc/binfile.cpp		      \
oledoc/oledoc.cpp		      \
oledoc/olefs.cpp		      \
oledoc/oleprop.cpp

pkginclude_HEADERS =          \
common/xlconfig.h             \
common/config.h			      \
common/overnew.h		      \
common/stringtok.h		      \
common/systype.h		      \
common/xlconfig.h		      \
common/timespan.h		      \
common/xlslib.h				  \
oledoc/binfile.h		      \
oledoc/oledoc.h			      \
oledoc/olefs.h			      \
oledoc/oleprop.h		      \
xlslib/biffsection.h		  \
xlslib/blank.h			      \
xlslib/cell.h			      \
xlslib/colinfo.h		      \
xlslib/colors.h			      \
xlslib/common.h			      \
xlslib/continue.h		      \
xlslib/datast.h			      \
xlslib/docsumminfo.h		  \
xlslib/extformat.h		      \
xlslib/font.h			      \
xlslib/format.h			      \
xlslib/formtags.h		      \
xlslib/globalrec.h		      \
xlslib/HPSF.h			      \
xlslib/index.h			      \
xlslib/label.h			      \
xlslib/merged.h			      \
xlslib/number.h			      \
xlslib/range.h			      \
xlslib/recdef.h			      \
xlslib/record.h			      \
xlslib/rectypes.h		      \
xlslib/row.h			      \
xlslib/sheetrec.h		      \
xlslib/summinfo.h		      \
xlslib/unit.h			      \
xlslib/workbook.h

clean-local:
	-rm -rf .deps

