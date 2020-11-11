cmake_minimum_required(VERSION 3.9)
project(xlslib C CXX)
add_definitions(-DHAVE_CONFIG_H )
#set(CMAKE_CXX_STANDARD 17)
include(CheckIncludeFileCXX)
include(CheckIncludeFile)
include(CheckIncludeFiles)
include(CheckFunctionExists)

CHECK_INCLUDE_FILE(crtdbg.h HAVE_CRTDBG_H)
CHECK_INCLUDE_FILE(crt_externs.h HAVE_CRT_EXTERNS_H)
CHECK_INCLUDE_FILE(ctype.h HAVE_CTYPE_H )
CHECK_INCLUDE_FILE(dlfcn.h HAVE_DLFCN_H )
CHECK_INCLUDE_FILE(inttypes.h HAVE_INTTYPES_H)
CHECK_INCLUDE_FILE(memory.h HAVE_MEMORY_H)
CHECK_INCLUDE_FILE(stdint.h HAVE_STDINT_H )
CHECK_INCLUDE_FILE(stdlib.h HAVE_STDLIB_H )
CHECK_INCLUDE_FILE(strings.h HAVE_STRINGS_H)
CHECK_INCLUDE_FILE(string.h HAVE_STRING_H )
CHECK_INCLUDE_FILE(sys/stat.h HAVE_SYS_STAT_H )
CHECK_INCLUDE_FILE(sys/types.h HAVE_SYS_TYPES_H)
CHECK_INCLUDE_FILE(unistd.h HAVE_UNISTD_H )
CHECK_INCLUDE_FILE(wchar.h HAVE_WCHAR_H )
CHECK_INCLUDE_FILE(windows.h HAVE_WINDOWS_H)

CHECK_FUNCTION_EXISTS(memchr HAVE_MEMCHR)
CHECK_FUNCTION_EXISTS(memmove HAVE_MEMMOVE)
CHECK_FUNCTION_EXISTS(memset HAVE_MEMSET )
CHECK_FUNCTION_EXISTS(snprintf HAVE_SNPRINTF)
CHECK_FUNCTION_EXISTS(strcasecmp HAVE_STRCASECMP)
CHECK_FUNCTION_EXISTS(strchr HAVE_STRCHR )
CHECK_FUNCTION_EXISTS(strcmp HAVE_STRCMP )
CHECK_FUNCTION_EXISTS(stricmp HAVE_STRICMP)
CHECK_FUNCTION_EXISTS(vsnprintf HAVE_VSNPRINTF)
CHECK_FUNCTION_EXISTS(_snprintf HAVE__SNPRINTF )
CHECK_FUNCTION_EXISTS(_vsnprintf HAVE__VSNPRINTF)

configure_file(src/common/xlconfig.h.cmake src/common/xlconfig.h)
include_directories(src ${CMAKE_BINARY_DIR}/src/common)
add_subdirectory(targets) 
add_library(xlslib STATIC
        src/common/overnew.cpp
        src/common/overnew.h
        src/common/stringtok.h
        src/common/systype.h
        src/common/timespan.h
        src/common/xls_poppack.h
        src/common/xls_pshpack1.h
        src/common/xls_pshpack2.h
        src/common/xls_pshpack4.h
        src/common/xlstypes.h
        src/common/xlsys.h
        src/oledoc/binfile.cpp
        src/oledoc/binfile.h
        src/oledoc/oledoc.cpp
        src/oledoc/oledoc.h
        src/oledoc/olefs.cpp
        src/oledoc/olefs.h
        src/oledoc/oleprop.cpp
        src/oledoc/oleprop.h
        src/xlslib/assert_assist.cpp
        src/xlslib/biffsection.h
        src/xlslib/blank.cpp
        src/xlslib/blank.h
        src/xlslib/boolean.cpp
        src/xlslib/boolean.h
        src/xlslib/cbridge.cpp
        src/xlslib/cbridge.h
        src/xlslib/cell.cpp
        src/xlslib/cell.h
        src/xlslib/colinfo.cpp
        src/xlslib/colinfo.h
        src/xlslib/colors.cpp
        src/xlslib/colors.h
        src/xlslib/common.h
        src/xlslib/continue.cpp
        src/xlslib/continue.h
        src/xlslib/datast.cpp
        src/xlslib/datast.h
        src/xlslib/docsumminfo.cpp
        src/xlslib/docsumminfo.h
        src/xlslib/err.cpp
        src/xlslib/err.h
        src/xlslib/extformat.cpp
        src/xlslib/extformat.h
        src/xlslib/font.cpp
        src/xlslib/font.h
        src/xlslib/format.cpp
        src/xlslib/format.h
        src/xlslib/formtags.h
        src/xlslib/formula_cell.cpp
        src/xlslib/formula_cell.h
        src/xlslib/formula_const.h
        src/xlslib/formula.cpp
        src/xlslib/formula_estimate.cpp
        src/xlslib/formula_estimate.h
        src/xlslib/formula_expr.cpp
        src/xlslib/formula_expr.h
        src/xlslib/formula.h
        src/xlslib/globalrec.cpp
        src/xlslib/globalrec.h
        src/xlslib.h
        src/xlslib/HPSF.cpp
        src/xlslib/HPSF.h
        src/xlslib/index.cpp
        src/xlslib/index.h
        src/xlslib/label.cpp
        src/xlslib/label.h
        src/xlslib/merged.cpp
        src/xlslib/merged.h
        src/xlslib/note.cpp
        src/xlslib/note.h
        src/xlslib/number.cpp
        src/xlslib/number.h
        src/xlslib/range.cpp
        src/xlslib/range.h
        src/xlslib/recdef.cpp
        src/xlslib/recdef.h
        src/xlslib/record.cpp
        src/xlslib/record.h
        src/xlslib/rectypes.h
        src/xlslib/row.cpp
        src/xlslib/row.h
        src/xlslib/sheetrec.cpp
        src/xlslib/sheetrec.h
        src/xlslib/summinfo.cpp
        src/xlslib/summinfo.h
        src/xlslib/tostr.h
        src/xlslib/unit.cpp
        src/xlslib/unit.h
        src/xlslib/workbook.cpp
        src/xlslib/workbook.cpp
        src/xlslib/workbook.h
        )