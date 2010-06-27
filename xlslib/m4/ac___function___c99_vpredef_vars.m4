##### http://autoconf-archive.cryp.to/ac_func_snprintf.html
#
# SYNOPSIS
#
#   AX_C_PREDEF___FUNCTION__
#
# DESCRIPTION
#
#   Checks for a fully C99 compliant __func__ and additionally checks 
#   for the existence and proper functioning of __FUNCTION__.
#
# LAST MODIFICATION
#
#   2007-08-15
#
# COPYLEFT
#
#   Copyright (c) 2007-2009 Ger Hobbelt <ger@hobbelt.com>
#
#   Code has been largely derived from the work by Kris Katterjohn for NMAP.
#
#   Copying and distribution of this file, with or without
#   modification, are permitted in any medium without royalty provided
#   the copyright notice and this notice are preserved.

AC_DEFUN([AX_C_PREDEF___FUNCTION__],
[AC_CHECK_FUNCS(strcmp)
AC_MSG_CHECKING(for working C99 __func__)
AC_CACHE_VAL(ac_cv_have_working___func__,
[AC_RUN_IFELSE(
  [AC_LANG_SOURCE([[#include <stdio.h>
#include <stdlib.h>

const char *testfunc(void)
{
  return __func__;
}

int main(void)
{
    exit(!!strcmp(testfunc(), "testfunc"));
}]])], 
  [ac_cv_have_working___func__=yes],
  [ac_cv_have_working___func__=no], 
  [ac_cv_have_working___func__=cross])])
AC_MSG_RESULT([$ac_cv_have_working___func__])
AC_MSG_CHECKING(for working __FUNCTION__)
AC_CACHE_VAL(ac_cv_have_working___function__,
[AC_RUN_IFELSE(
  [AC_LANG_SOURCE([[#include <stdio.h>
#include <stdlib.h>

const char *testfunc(void)
{
  return __FUNCTION__;
}

int main(void)
{
    exit(!!strcmp(testfunc(), "testfunc"));
}]])], 
  [ac_cv_have_working___function__=yes], 
  [ac_cv_have_working___function__=no], 
  [ac_cv_have_working___function__=cross])])
AC_MSG_RESULT([$ac_cv_have_working___function__])
if test x$ac_cv_have_working___func__ == "xyes"; then
  AC_DEFINE(HAVE___FUNC__, 1, [Define if compiler implements __func__.])
fi
if test x$ac_cv_have_working___function__ == "xyes"; then
  AC_DEFINE(HAVE___FUNCTION__, 1, [Define if compiler implements __FUNCTION__.])
fi
])

