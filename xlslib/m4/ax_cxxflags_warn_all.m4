##### derived from http://autoconf-archive.cryp.to/ax_cflags_warn_all.html
#
# SYNOPSIS
#
#   AX_CXXFLAGS_WARN_ALL [(shellvar [,default, [A][, [NA]]])]
#
# DESCRIPTION
#
#   Try to find a compiler option that enables most reasonable
#   warnings. This macro is directly derived from VL_PROG_CC_WARNINGS
#   which is split up into two AX_CFLAGS_WARN_ALL and
#   AX_CFLAGS_WARN_ALL_ANSI
#
#   For the GNU CC compiler it will be -Wall (and -ansi -pedantic) The
#   result is added to the shellvar being CFLAGS by default.
#
#   Currently this macro knows about GCC, Solaris C compiler, Digital
#   Unix C compiler, C for AIX Compiler, HP-UX C compiler, IRIX C
#   compiler, NEC SX-5 (Super-UX 10) C compiler, and Cray J90 (Unicos
#   10.0.0.8) C compiler.
#
#    - $1 shell-variable-to-add-to : CFLAGS
#    - $2 add-value-if-not-found : nothing
#    - $3 action-if-found : add value to shellvariable
#    - $4 action-if-not-found : nothing
#
# LAST MODIFICATION
#
#   2008-05-24
#
# COPYLEFT
#
#   Copyright (c) 2006 Guido U. Draheim <guidod@gmx.de>
#   Copyright (c) 2008 Ger Hobbelt <ger@hobbelt.com>
#
#   This program is free software; you can redistribute it and/or
#   modify it under the terms of the GNU General Public License as
#   published by the Free Software Foundation; either version 2 of the
#   License, or (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful, but
#   WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
#   General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with this program; if not, write to the Free Software
#   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA
#   02111-1307, USA.
#
#   As a special exception, the respective Autoconf Macro's copyright
#   owner gives unlimited permission to copy, distribute and modify the
#   configure scripts that are the output of Autoconf when processing
#   the Macro. You need not follow the terms of the GNU General Public
#   License when using or distributing such scripts, even though
#   portions of the text of the Macro appear in them. The GNU General
#   Public License (GPL) does govern all other use of the material that
#   constitutes the Autoconf Macro.
#
#   This special exception to the GPL applies to versions of the
#   Autoconf Macro released by the Autoconf Macro Archive. When you
#   make and distribute a modified version of the Autoconf Macro, you
#   may extend this special exception to the GPL to apply to your
#   modified version as well.


dnl  implementation tactics:
dnl   the for-argument contains a list of options. The first part of
dnl   these does only exist to detect the compiler - usually it is
dnl   a global option to enable -ansi or -extrawarnings. All other
dnl   compilers will fail about it. That was needed since a lot of
dnl   compilers will give false positives for some option-syntax
dnl   like -Woption or -Xoption as they think of it is a pass-through
dnl   to later compile stages or something. The "%" is used as a
dnl   delimimiter. A non-option comment can be given after "%%" marks
dnl   which will be shown but not added to the respective C/CXXFLAGS.



dnl #####################################################################
dnl the only difference - the LANG selection... and the default FLAGS
dnl #####################################################################


AC_DEFUN([AX_CXXFLAGS_WARN_ALL],
[
  AC_REQUIRE([AC_USE_SYSTEM_EXTENSIONS])
AS_VAR_PUSHDEF([opt],[ac_cv_cflags_warn_all_cxx])
AC_CACHE_CHECK([m4_default([$1],[CXXFLAGS]) for maximum warnings],
  [opt],
  [
 AC_LANG_PUSH([C++])
 ax_cflags_warn_all_save_CXXFLAGS="$CXXFLAGS"
    AS_VAR_SET([opt], ["no, unknown"])
    #
    # construction of those strings:
    #   a % b
    # where 'a' is used to 'recognize' the compiler we're talking about, which means
    # 'b' is the set of options that actually turn on those warnings.
    #
    for ac_arg in                         dnl
      "-pass-exit-codes -pedantic -Werror -Wno-unused-parameter -Wno-missing-field-initializers % -Wall -Wextra -Wno-unused-parameter -Wno-missing-field-initializers"    dnl   GCC 3/4 - NOTE that I explicitly DISABLE two 'extra' warnings here as I find those annoying most of the time
      "-pass-exit-codes -pedantic -Werror % -Wall"             dnl   GCC
      "-xstrconst % -v"                   dnl Solaris C
      "-std1      % -verbose -w0 -warnprotos" dnl Digital Unix
      "-qlanglvl=ansi % -qsrcmsg -qinfo=all:noppt:noppc:noobs:nocnd" dnl AIX
      "-ansi -ansiE % -fullwarn"          dnl IRIX
      "+ESlit     % +w1"                  dnl HP-UX C
      "-Xc        % -pvctl[,]fullmsg"     dnl NEC SX-5 (Super-UX 10)
      "-h conform % -h msglevel 2"        dnl Cray C (Unicos)
    #
    do
      CXXFLAGS="$ax_cflags_warn_all_save_CXXFLAGS "`echo $ac_arg | sed -e 's,%%.*,,' -e 's,%,,'`
      AC_COMPILE_IFELSE([AC_LANG_PROGRAM([])],
      [
        AS_VAR_SET([opt], [`echo $ac_arg | sed -e 's,.*% *,,'`])
        break
      ])
    done
    CXXFLAGS=$ax_cflags_warn_all_save_CXXFLAGS
    AC_LANG_POP
  ])

  # cache check done

  AS_CASE(AS_VAR_GET([opt]),
  [no,*], [
            m4_ifval([$2], [AS_VAR_SET([opt], [$2])])
            m4_ifval([$4], [$4],
            [
              m4_ifval([$2],
              [
                CXXFLAGS=`echo "$CXXFLAGS AS_VAR_GET([opt])" | sed -e 's,%%.*,,' -e 's, *$,,'`
              ])
            ])
  ],
  [
            m4_ifval([$3], [$3],
            [
              CXXFLAGS=`echo "$CXXFLAGS AS_VAR_GET([opt])" | sed -e 's,%%.*,,' -e 's, *$,,'`
            ])
  ])
  AS_VAR_POPDEF([opt])
])









