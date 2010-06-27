#
# SYNOPSIS
#
#   AX_ADD_OPTION(<var>, <option>)
#
# DESCRIPTION
#
#   Only add option $2 to var $1 if it's not already in there.
#
#   Note that the new option-to-be-added is added AT THE FRONT.
#   Sometimes it doesn't matter, but it MIGHT make a difference
#   for particular items, such as -I include paths and such...
#
# NOTES
#
#   local use only (no caching)
#
# LAST MODIFICATION
#
#   2010-06-23
#
# COPYLEFT
#
#   Copyright (c) 2007-2010 Ger Hobbelt <ger@hobbelt.com>
#
#   Copying and distribution of this file, with or without
#   modification, are permitted in any medium without royalty provided
#   the copyright notice and this notice are preserved.
#


AC_DEFUN([AX_ADD_OPTION],
  [
    # split option list up into separate part; then check each item
    # once done, append AT FRONT of $1, but the order within $2 is
    # kept intact!
    ax_ao_list="$2"
    ax_ao_accepted_set=""
    for ax_ao_e in ${ax_ao_list}
    do
      case " [$]{$1} " in
        *" ${ax_ao_e} "* )
                   ;;
        *        )
                   if test -z "${ax_ao_accepted_set}" ; then
                     ax_ao_accepted_set="${ax_ao_e}"
                   else
                     ax_ao_accepted_set="${ax_ao_accepted_set} ${ax_ao_e}"
                   fi
                   ;;
      esac
    done
    $1="${ax_ao_accepted_set} [$]{$1}"
  ])

