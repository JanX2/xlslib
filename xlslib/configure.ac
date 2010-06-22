#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.


dnl Initialize autoconf/automake
AC_INIT([xlslib], [1.6.0], [dhoerl@mac.com])

AC_CONFIG_AUX_DIR([config])
# Minimum Autoconf version required.
AC_PREREQ(2.65)
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([foreign color-tests 1.11])
AC_CONFIG_SRCDIR([src/common/systype.h])
AC_CONFIG_HEADERS([src/common/xlconfig.h])


# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
# AC_PROG_RANLIB  -- rendered obsolete by LT_INIT
LT_INIT

# see also: GNU libtool docs: AC_PROG_LIBTOOL et al:
AC_LIBTOOL_WIN32_DLL
AC_LIBTOOL_DLOPEN
AC_PROG_LIBTOOL
AC_SUBST(LIBTOOL_DEPS)

AC_PROG_INSTALL

# Checks for libraries.

# Checks for header files.

# Taken from libxml - DFH
AC_CHECK_HEADERS([wchar.h stdint.h stdbool.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T

AX_CC_PRAGMA_PACK

# Checks for library functions.
AC_HEADER_STDC
AC_CHECK_FUNCS([memset strchr strcasecmp stricmp])

# Taken from libxml - DFH
WITH_ICONV=0
AC_ARG_WITH(iconv, [  --with-iconv[=DIR]        add ICONV support (on)])
if test "$with_iconv" = "no" ; then
    echo Disabling ICONV support
else
    if test "$with_iconv" != "yes" -a "$with_iconv" != "" ; then
	CPPFLAGS="${CPPFLAGS} -I$with_iconv/include"
	# Export this since our headers include iconv.h
	XML_INCLUDEDIR="${XML_INCLUDEDIR} -I$with_iconv/include"
	ICONV_LIBS="-L$with_iconv/lib"
    fi

    AC_CHECK_HEADER(iconv.h,
	AC_MSG_CHECKING(for iconv)
	AC_LINK_IFELSE([AC_LANG_PROGRAM([[#include <stdlib.h>
#include <iconv.h>]], [[
iconv_t cd = iconv_open ("","");
iconv (cd, NULL, NULL, NULL, NULL);]])],[
	    AC_MSG_RESULT(yes)
	    WITH_ICONV=1],[
	    AC_MSG_RESULT(no)
	    AC_MSG_CHECKING(for iconv in -liconv)

	    _ldflags="${LDFLAGS}"
	    _libs="${LIBS}"
	    LDFLAGS="${LDFLAGS} ${ICONV_LIBS}"
	    LIBS="${LIBS} -liconv"

	    AC_TRY_LINK([#include <stdlib.h>
#include <iconv.h>],[
iconv_t cd = iconv_open ("","");
iconv (cd, NULL, NULL, NULL, NULL);],[
		AC_MSG_RESULT(yes)
		WITH_ICONV=1
		ICONV_LIBS="${ICONV_LIBS} -liconv"
		LIBS="${_libs}"
		LDFLAGS="${_ldflags}"],[
		AC_MSG_RESULT(no)
		LIBS="${_libs}"
		LDFLAGS="${_ldflags}"])]))
fi

if test "$WITH_ICONV" = "1"; then
    AC_DEFINE(HAVE_ICONV, 1, [Define if you have the iconv() function.])
fi

AC_SUBST(WITH_ICONV)

XLSLIB_LIBS="$ICONV_LIBS $LIBS"

AC_SUBST(XLSLIB_LIBS)

###########################################################

AC_CONFIG_FILES([Makefile
		 src/Makefile
		 targets/Makefile
		 targets/test/Makefile
	         build/msvc2005/ac-config.win32.h
        	 build/msvc2008/ac-config.win32.h
	         ])
AC_OUTPUT

