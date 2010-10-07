#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.


dnl Initialize autoconf/automake
AC_INIT([xlslib], [2.0.0], [dhoerl@mac.com])

AC_CONFIG_AUX_DIR([config])
# Minimum Autoconf version required.
AC_PREREQ(2.65)
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([foreign color-tests 1.11 check-news dist-bzip2 dist-zip dist-tarZ])
AC_CONFIG_SRCDIR([src/common/systype.h])
AC_CONFIG_HEADERS([src/common/xlconfig.h])


# Checks for programs.
AC_PROG_SED

AC_PROG_CXX
AC_PROG_CC
AC_PROG_CC_STDC   # check for ISO/C99 options for the compilers, if available. Be hip, don't be square.
AC_PROG_CPP
AC_PROG_CPP_WERROR
AC_USE_SYSTEM_EXTENSIONS

# AC_PROG_RANLIB  -- rendered obsolete by LT_INIT

# see also: GNU libtool docs: AC_PROG_LIBTOOL et al:
# AC_DISABLE_SHARED
# AC_DISABLE_STATIC
AC_LIBTOOL_WIN32_DLL
AC_LIBTOOL_DLOPEN
# AC_PROG_LIBTOOL -- obsoleted version of LT_INIT
LT_INIT
AC_SUBST(LIBTOOL_DEPS)

AC_PROG_INSTALL

# Checks for libraries.

# Checks for header files.

# Taken from libxml - DFH
AC_CHECK_HEADERS([wchar.h stdint.h stdbool.h])
# added to make syncing config for Win32/64 easier
AC_CHECK_HEADERS([ctype.h crtdbg.h crt_externs.h windows.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_INT8_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_LONG_LONG_INT
AC_CHECK_SIZEOF([long int])
AC_CHECK_SIZEOF([int])
if test $ac_cv_type_long_long_int = yes; then
  AC_CHECK_SIZEOF([long long int])
fi
AC_C_STRINGIZE
AX_C_PREDEF___FUNCTION__

AX_CC_PRAGMA_PACK


# Checks for library functions.
AC_HEADER_STDC
AC_CHECK_FUNCS([memset strchr strcasecmp stricmp])
AC_CHECK_FUNCS([memmove])
AC_CHECK_FUNCS([snprintf vsnprintf])
AC_CHECK_FUNCS([_snprintf _vsnprintf])


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
#include <iconv.h>

#ifdef __GLIBC__
#if (__GLIBC__ == 2 && __GLIBC_MINOR__ <= 1 )
#error "Too old glibc. This version's iconv() implementation cannot be trusted."
#endif
#endif
]], [[
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
#include <iconv.h>

#ifdef __GLIBC__
#if (__GLIBC__ == 2 && __GLIBC_MINOR__ <= 1 )
#error "Too old glibc. This version's iconv() implementation cannot be trusted."
#endif
#endif
],[
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







#
# debug or release build of the binary?
#
AC_MSG_CHECKING([if we want a debug or release build])
AC_ARG_ENABLE([debug], AC_HELP_STRING([--disable-debug],
  [Specify to create a so-called 'production' or 'release build'.
You can explicitly specify '--enable-debug' to create a standard
XLSLIB (debuggable) build. (default: debugging turned OFF)]),
  [
   #
   # since AC_PROG_CC sets '-O2' in CFLAGS by default, while _we_'d like
   # to have '-O3', we override the operation (for gcc only) right here.
   # Of course, this is rather hackish, so if anyone knows about a cleaner
   # solution, I'm all for it!
   #                                               Ger Hobbelt
   if test ! -z "$SED"; then
     CFLAGS=`$ECHO " $CFLAGS " | $SED -e 's:-O[[0-3]] : :' -e 's:-g : :' -e 's:  : :g'`
     CXXFLAGS=`$ECHO " $CXXFLAGS " | $SED -e 's:-O[[0-3]] : :' -e 's:-g : :' -e 's:  : :g'`
   fi
   if eval "test x$enable_debug = xyes"; then
     AC_MSG_RESULT([debug])
     AX_CFLAGS_GCC_OPTION([-O0])
     AX_CFLAGS_GCC_OPTION([-g3],
       [],
       [],
       [AX_CFLAGS_GCC_OPTION([-g])]) # symbolic debugging
     AX_CXXFLAGS_GCC_OPTION([-O0])
     AX_CXXFLAGS_GCC_OPTION([-g3],
       [],
       [],
       [AX_CXXFLAGS_GCC_OPTION([-g])]) # symbolic debugging
     WITH_BUILD="debug"
     WITH_DEBUGGING=yes
   else
     AC_MSG_RESULT([release])
     AX_CFLAGS_GCC_OPTION([-O3],
       [],
       [],
       [AX_CFLAGS_GCC_OPTION([-O2])])
     AX_CXXFLAGS_GCC_OPTION([-O3],
       [],
       [],
       [AX_CXXFLAGS_GCC_OPTION([-O2])])
     WITH_BUILD="release (maximum optimizations)"
     WITH_DEBUGGING=no
   fi
  ],
  [
   AC_MSG_RESULT([default])
   if test ! -z "$SED"; then
     if test -z `$ECHO " $CFLAGS " | $SED -e 's:.* \(-O[[0-3]]\) .*:\1:'`; then
       # only add '-O2' if there's no '-O expression custom defined yet (by the user before [s]he started ./configure)
       AX_CFLAGS_GCC_OPTION([-O2])
     fi
     if test -z `$ECHO " $CXXFLAGS " | $SED -e 's:.* \(-O[[0-3]]\) .*:\1:'`; then
       # only add '-O2' if there's no '-O expression custom defined yet (by the user before [s]he started ./configure)
       AX_CXXFLAGS_GCC_OPTION([-O2])
     fi
   fi
   WITH_BUILD="release (default optimizations)"
   WITH_DEBUGGING=no
  ])



#
# include run-time profiling support (requires the use of specific development tools: prof/gprof)
#
WITH_GPROFILING="no"
WITH_PROFILING="no"

AC_MSG_CHECKING([if we want to instrument the build for profiling])
AC_ARG_ENABLE([profiling], AC_HELP_STRING([--enable-profiling],
  [You can explicitly specify '--enable-profiling' to
create a XLSLIB build which can be profiled with, for 
example, gprof. (default: profiling turned OFF)]),
  [
   AC_MSG_RESULT([$enable_profiling])
   ac_profiling_cc_flags=""
   AX_CFLAGS_GCC_OPTION([-pg], [ac_profiling_cc_flags],
    	[
         WITH_GPROFILING="yes"
         AC_MSG_RESULT([$enable_profiling (gprof)])
        ],
   	[
   	 AX_CFLAGS_GCC_OPTION([-p], [ac_profiling_cc_flags],
   		[
   		WITH_PROFILING="yes"
   		AC_MSG_RESULT([$enable_profiling (prof)])
   		])
       ])
   AX_CXXFLAGS_GCC_OPTION([-pg], [ac_profiling_cc_flags],
    	[
         WITH_GPROFILING="yes"
         AC_MSG_RESULT([$enable_profiling (gprof)])
        ],
   	[
   	 AX_CXXFLAGS_GCC_OPTION([-p], [ac_profiling_cc_flags],
   		[
   		WITH_PROFILING="yes"
   		AC_MSG_RESULT([$enable_profiling (prof)])
   		])
       ])
   # delay copying into CFLAGS/CXXFLAGS/LDFLAGS; we don't want to influence the cc used to test stuff in this configure script
  ],
  [AC_MSG_RESULT([no])])

#
# maximum warning level for neurotic developers? (default = yes)
#
AC_MSG_CHECKING([if we want maximum compiler warnings])
AC_ARG_ENABLE([extended-compile-warnings],
  AC_HELP_STRING([--disable-extended-compile-warnings],
    [turn maximum compiler/linker warnings level off
(default: turn on all warnings)]),
  [
   AC_MSG_RESULT([$enable_extended_compile_warnings])
   if eval "test x$enable_extended_compile_warnings = xyes"; then
     AX_CFLAGS_WARN_ALL([])
     AX_CXXFLAGS_WARN_ALL([])
   fi
  ],
  [
   AC_MSG_RESULT([yes])
   AX_CFLAGS_WARN_ALL([])
   AX_CXXFLAGS_WARN_ALL([])
  ])

#
# track test coverage
#
WITH_GCOV="no"
AC_MSG_CHECKING([if we want test coverage tracking])
AC_ARG_ENABLE([gcov],
    AC_HELP_STRING([--enable-gcov],
        [Build with test coverage information (gcov)]),
  [
   ac_coverage_cc_flags=""
   AX_CFLAGS_GCC_OPTION([-fprofile-arcs -ftest-coverage], [ac_coverage_cc_flags],
    	[
         WITH_GCOV="yes"
         AC_MSG_RESULT([$enable_gcov (gcov)])
        ],
   	[
         AC_MSG_RESULT([no])
       ])
   AX_CXXFLAGS_GCC_OPTION([-fprofile-arcs -ftest-coverage], [ac_coverage_cc_flags],
    	[
         WITH_GCOV="yes"
         AC_MSG_RESULT([$enable_gcov (gcov)])
        ],
   	[
         AC_MSG_RESULT([no])
       ])
   # delay copying into CFLAGS/CXXFLAGS/LDFLAGS; we don't want to influence the cc used to test stuff in this configure script
  ],
  [AC_MSG_RESULT([no])])


#
# Do you want those XL_ASSERT() calls active in your run-time or removed by the compiler?
#
AC_MSG_CHECKING([if we want run-time assertions])
AC_ARG_ENABLE([assertions],
  AC_HELP_STRING([--disable-assertions],
    [turn assertion checks off.
This will produce a faster running executable but such at the cost
of lacking a specific type of 'developer' checks. These SHOULD not
get triggered EVER, so you MAY assume specifying this option is
a 'free speedup'. (default: assertions are ENABLED)]),
  [
   AC_MSG_RESULT($enable_assertions)
   if eval "test x$enable_assertions = xno"; then
     AC_DEFINE_UNQUOTED(XLSLIB_DONT_ASSERT, 1, [DISable ASSERT/VERIFY checks])
   fi
  ],
  [
   AC_MSG_RESULT([yes])
  ])





AC_SUBST(WITH_ICONV)

XLSLIB_LIBS="$ICONV_LIBS $LIBS"

AC_SUBST(XLSLIB_LIBS)


# write delayed options into CFLAGS/LDFLAGS:
AX_ADD_OPTION([CFLAGS], [$ac_profiling_cc_flags])
AX_ADD_OPTION([CXXFLAGS], [$ac_profiling_cc_flags])
AX_ADD_OPTION([LDFLAGS], [$ac_profiling_cc_flags])


###########################################################

AC_CONFIG_FILES([Makefile
		 src/Makefile
		 targets/Makefile
		 targets/test/Makefile
	         build/msvc2005/ac-config.win32.h
        	 build/msvc2008/ac-config.win32.h
	         ])
AC_OUTPUT




dnl Print configuration summary

my_installpath=`eval echo $exec_prefix`

cat <<EOF

Configuration summary
=====================

$PACKAGE_NAME (version $PACKAGE_VERSION) is now configured as follows:

* Compilation environment

  CC           = $CC
  CFLAGS       = $CFLAGS
  CPP          = $CPP
  CPPFLAGS     = $CPPFLAGS
  CXX          = $CXX
  CXXFLAGS     = $CXXFLAGS
  LD           = $LD
  LDFLAGS      = $LDFLAGS
  LIBS         = $LIBS
  INSTALLFLAGS = $INSTALLFLAGS
  BINDIR       = $BIN_PATH

* $PACKAGE_NAME options

  Development-time debugging:               $WITH_DEBUGGING
  Allow profiling using gprof:              $WITH_GPROFILING
  Allow profiling using prof:               $WITH_PROFILING
  Allow test coverage analysis using gcov:  $WITH_GCOV
  Build:                                    $WITH_BUILD

  Settings:                                 $settings

-----------------------------------------------------------------------

Congratulations, your $PACKAGE_NAME $PACKAGE_VERSION is configured now.
You can now build and install $PACKAGE_NAME using the Makefile, by calling

  \$ make

Before running 

  \$ make install

you may wish to test the newly built binary by running

  \$ make check

Any unacceptable mistakes will be reported and cause the 'check' run to abort.

EOF

