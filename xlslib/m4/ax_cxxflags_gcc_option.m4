##### derived from http://autoconf-archive.cryp.to/ax_cflags_gcc_option.html
#
# SYNOPSIS
#
#   AX_CXXFLAGS_GCC_OPTION (optionflag [,[shellvar][,[A][,[NA]]])
#
# DESCRIPTION
#
#   AX_CXXFLAGS_GCC_OPTION(-fvomit-frame) would show a message as like
#   "checking CXXFLAGS for gcc -fvomit-frame ... yes" and adds the
#   optionflag to CXXFLAGS if it is understood. You can override the
#   shellvar-default of CXXFLAGS of course. The order of arguments stems
#   from the explicit macros like AX_CFLAGS_WARN_ALL.
#
#   The cousin AX_CFLAGS_GCC_OPTION would check for an option to add
#   to CFLAGS - and it uses the autoconf setup for C++ instead of C
#   (since it is possible to use different compilers for C and C++).
#
#   The macro is a lot simpler than any special AX_CFLAGS_* macro (or
#   ac_cxx_rtti.m4 macro) but allows to check for arbitrary options.
#   However, if you use this macro in a few places, it would be great
#   if you would make up a new function-macro and submit it to the
#   ac-archive.
#
#     - $1 option-to-check-for : required ("-option" as non-value)
#     - $2 shell-variable-to-add-to : CFLAGS (or CXXFLAGS in the other case)
#     - $3 action-if-found : add value to shellvariable
#     - $4 action-if-not-found : nothing
#
#   Note: in earlier versions, $1-$2 were swapped. We try to detect the
#   situation and accept a $2=~/-/ as being the old
#   option-to-check-for.
#
#   There are other variants that emerged from the original macro
#   variant which did just test an option to be possibly added.
#   However, some compilers accept an option silently, or possibly for
#   just another option that was not intended. Therefore, we have to do
#   a generic test for a compiler family. For gcc we check "-pedantic"
#   being accepted which is also understood by compilers who just want
#   to be compatible with gcc even when not being made from gcc
#   sources.
#
#   See also: AX_CFLAGS_SUN_OPTION, AX_CFLAGS_HPUX_OPTION,
#   AX_CFLAGS_AIX_OPTION, and AX_CFLAGS_IRIX_OPTION.
#
# LAST MODIFICATION
#
#   2007-07-27
#
# COPYLEFT
#
#   Copyright (c) 2007 Guido U. Draheim <guidod@gmx.de>
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


AC_DEFUN([AX_CXXFLAGS_GCC_OPTION],
[
  AC_REQUIRE([AC_USE_SYSTEM_EXTENSIONS])
  AS_VAR_PUSHDEF([opt], [ac_cv_cflags_gcc_option_[]$1])
  AC_CACHE_CHECK([m4_default([$2], [CXXFLAGS]) for g++ $1 command line option],
  [opt],
  [
    AC_LANG_PUSH([C++])
    ax_cflags_gcc_option_save_CXXFLAGS="${CXXFLAGS}"
    AS_VAR_SET([opt], ["no, unknown"])
    for ac_arg in                         dnl
      "-pass-exit-codes -pedantic -Werror % m4_ifval([$1],[$1],[-bogus_option])"  dnl   GCC
      "-pass-exit-codes -pedantic % m4_ifval([$1],[$1],[-bogus_option]) %% no, obsolete"  dnl new GCC
    #
    do
      CXXFLAGS="${ax_cflags_gcc_option_save_CXXFLAGS} "`echo $ac_arg | sed -e 's,%%.*,,' -e 's,.*%,,'`
      AC_COMPILE_IFELSE([AC_LANG_PROGRAM([])],
      [
        AS_VAR_SET([opt], [`echo $ac_arg | sed -e 's,.* % *,,'`])
        # result successful!
        break
      ])
    done
    CXXFLAGS=$ax_cflags_gcc_option_save_CXXFLAGS
    # reset language
    AC_LANG_POP
  ])

  # cache check done

  AS_CASE([AS_VAR_GET([opt])],
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




