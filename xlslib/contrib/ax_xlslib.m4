# ===========================================================================
#          http://www.nongnu.org/autoconf-archive/ax_lib_xls.html</a>
# ===========================================================================
#
# SYNOPSIS
#
#   AX_LIB_XLS()
#
# DESCRIPTION
#
#   This macro provides tests of availability of XLS library
#
#   The --with-xlslib option takes one of two possible values:
#
#   no - do not check for XLS library (same as using --without-xlslib)
#
#   path - root directory for xlslib installation, use this option if xlslib
#   isn't installed in /usr/local or /usr.
#
#   This macro calls:
#
#     AC_SUBST(XLS_CPPFLAGS)
#     AC_SUBST(XLS_LDFLAGS)
#
#   And sets:
#
#     HAVE_XLSLIB
#
# LICENSE
#
#   Copyright (c) 2010 Dee Holtsclaw <dee@pcds.biz>
#
#   Copying and distribution of this file, with or without modification, are
#   permitted in any medium without royalty provided the copyright notice
#   and this notice are preserved. This file is offered as-is, without any
#   warranty.

#serial 1

AC_DEFUN([AX_LIB_XLS],
[
    AC_ARG_WITH(xlslib,
	[  --with-xlslib=DIR root directory path of zlib installation
	    [defaults to /usr/local or /usr if not found in /usr/local]
	    --without-xlslib to disable xlslib usage completely],
	[if test "$withval" != no ; then
	    if test -d "$withval"
	    then
		XLSLIB_HOME="$withval"
	    else
		AC_MSG_WARN([Sorry, $withval does not exist, checking usual places])
	    fi
	fi])

    if test "x$with_xlslib" != "xno" ;
    then
	XLSLIB_HOME=/usr/local
	if test ! -f "${XLSLIB_HOME}/include/xlslib/xlslib.h"
	then
	    XLSLIB_HOME=/usr
	fi

	#
	# Locate xlslib, if wanted
	#
	if test -n "${XLSLIB_HOME}"
	then
	    XLSLIB_OLD_LDFLAGS="$LDFLAGS"
	    XLSLIB_OLD_CPPFLAGS="$CPPFLAGS"
	    LDFLAGS="$LDFLAGS -L${XLSLIB_HOME}/lib"
	    CPPFLAGS="$CPPFLAGS -I${XLSLIB_HOME}/include/xlslib"
	    AC_LANG_SAVE
	    AC_LANG_CPLUSPLUS
	    AC_CHECK_LIB(xls, xlsNewWorkbook, [xlslib_cv_libxls=yes], [xlslib_cv_libxls=no])
	    AC_CHECK_HEADER(xlslib.h, [xlslib_cv_xlslib_h=yes], [xlslib_cv_xlslib_h=no])
	    AC_LANG_RESTORE
	    if test "$xlslib_cv_libxls" = "yes" -a "$xlslib_cv_xlslib_h" = "yes"
	    then
		#
		# If both library and header were found, use them
		#
		AC_MSG_CHECKING(xlslib in ${XLSLIB_HOME})
		AC_MSG_RESULT(ok)
		XLS_CPPFLAGS="-I${XLSLIB_HOME}/include/xlslib"
		XLS_LDFLAGS="-L${XLSLIB_HOME} -lxls"
		AC_DEFINE([HAVE_XLSLIB], [1],
		    [Define to 1 if the XLS library is available])
		AC_SUBST([XLS_CPPFLAGS])
		AC_SUBST([XLS_LDFLAGS])
	    else
		#
		# If either header or library was not found, revert and bomb
		#
		AC_MSG_CHECKING(xlslib in ${XLSLIB_HOME})
		AC_MSG_RESULT(failed)
		AC_MSG_ERROR([either specify a valid xlslib installation with --with-xlslib=DIR or disable xlslib usage with --without-xlslib])
	    fi
	    # Restore compiler & library flags
	    LDFLAGS="$XLSLIB_OLD_LDFLAGS"
	    CPPFLAGS="$XLSLIB_OLD_CPPFLAGS"
	fi
    fi
])
