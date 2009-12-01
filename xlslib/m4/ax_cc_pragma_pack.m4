#
# SYNOPSIS
#
#   AX_CC_PRAGMA_PACK
#
# DESCRIPTION
#
#   Checks for the #pragma pack(push/pop/N) feature set and sets several
#   variables accordingly.
#
# LAST MODIFICATION
#
#   2008-05-29
#
# COPYLEFT
#
#   Copyright (c) 2008 Ger Hobbelt <ger@hobbelt.com>
#
#   Copying and distribution of this file, with or without
#   modification, are permitted in any medium without royalty provided
#   the copyright notice and this notice are preserved.

AC_DEFUN([AX_CC_PRAGMA_PACK],
[
  AC_CHECK_FUNCS([memset memchr])
  AC_MSG_CHECKING([for working @%:@pragma pack])
  AC_CACHE_VAL([ac_cv_have_pragma_pack_N],
  [
    AC_LANG_PUSH([C])
    AC_RUN_IFELSE([
      AC_LANG_PROGRAM(
[AC_INCLUDES_DEFAULT([])[

@%:@pragma pack(2)
struct
{
    char a;
    int  b;
    char c;
    int  d;
    int  e;
} s1;
@%:@pragma pack()

@%:@pragma pack(16)
struct
{
    char a;
    int  b;
    char c;
    int  d;
    int  e;
} s2;
@%:@pragma pack()

@%:@pragma pack(4)
struct
{
    char a;
    int  b;
    char c;
    int  d;
    int  e;
} s3;
@%:@pragma pack()

@%:@undef CFG_TRACE    /* @%:@define this one if you need to debug this M4 macro */

]],[[
    char *p;
    char *q;

@%:@if defined(CFG_TRACE)
    printf("sizes: %d %d %d\n",     (int)sizeof(s1) , (int)sizeof(s2), (int)sizeof(s3));
@%:@endif

    if (sizeof(s1) >= sizeof(s2))
        return 2;
    if (sizeof(s2) < sizeof(s3))  /* tolerate overly smart packing compilers - for now... */
        return 3;
    if (sizeof(s1) >= sizeof(s3))
        return 4;

    memset(&s1, 0, sizeof(s1));
    memset(&s2, 0, sizeof(s2));
    memset(&s3, 0, sizeof(s3));

    s1.a = -1;
    s1.b = -1;
    s1.c = -1;
    s1.e = -1;

    s2.a = -1;
    s2.b = -1;
    s2.c = -1;
    s2.e = -1;

    // find 'a' */
    p = memchr(&s1, 0xFF, sizeof(s1));
    if (!p)
        return 5;

    /* skip 'a' and find 'b' */
    p++;
    q = memchr(p, 0xFF, sizeof(s1));

    if (p == q)
    {
        /* this is allowed; but then there should be a one-byte gap between b and c field! */
        /* find gap between 'b' and 'c': */
        q = memchr(q, 0, sizeof(s1) / 2);
        if (!q)
            return 6;
        /* find 'c' */
        p = memchr(q, 0xFF, sizeof(s1) / 2);
        if (!p)
            return 7;
@%:@if defined(CFG_TRACE)
        printf("gap: %d\n", (int)(p - q));
@%:@endif
        if (p != q + 1)
        {
            /* gap is bigger than 1 byte! pack doesn't work as advertized here! */
            return 8;
        }
    }
    else
    {
        /* gap between 'a' and 'b': not allowed to be bigger than 1 byte! */
@%:@if defined(CFG_TRACE)
        printf("gap: %d\n", (int)(q - p));
@%:@endif
        if (q != p + 1)
        {
            /* gap is bigger than 1 byte! pack doesn't work as advertized here! */
            return 9;
        }
    }
    return 0;
]]
    )],
    [ac_cv_have_pragma_pack_N=yes],
    [ac_cv_have_pragma_pack_N=no])
    # reset language
    AC_LANG_POP
  ])
  AC_MSG_RESULT([$ac_cv_have_pragma_pack_N])

  AC_MSG_CHECKING([for working @%:@pragma pack(push / pop)])
  AC_CACHE_VAL([ac_cv_have_pragma_pack_push_pop],
  [
    AC_LANG_PUSH([C])
    AC_RUN_IFELSE([
      AC_LANG_PROGRAM(
[AC_INCLUDES_DEFAULT([])[

@%:@pragma pack(push, 4)
@%:@pragma pack(push, 16)
@%:@pragma pack(push, 2)
struct
{
    char a;
    int  b;
    char c;
    int  d;
    int  e;
} s1;
@%:@pragma pack(pop)  /* --> 16 */

struct
{
    char a;
    int  b;
    char c;
    int  d;
    int  e;
} s2;

@%:@pragma pack(pop)  /* --> 4 */
struct
{
    char a;
    int  b;
    char c;
    int  d;
    int  e;
} s3;
@%:@pragma pack(pop)

@%:@undef CFG_TRACE    /* @%:@define this one if you need to debug this M4 macro */

]],[[
    char *p;
    char *q;

@%:@if defined(CFG_TRACE)
    printf("sizes: %d %d %d\n",     (int)sizeof(s1) , (int)sizeof(s2), (int)sizeof(s3));
@%:@endif

    if (sizeof(s1) >= sizeof(s2))
        return 2;
    if (sizeof(s2) < sizeof(s3))  /* tolerate overly smart packing compilers - for now... */
        return 3;
    if (sizeof(s1) >= sizeof(s3))
        return 4;

    memset(&s1, 0, sizeof(s1));
    memset(&s2, 0, sizeof(s2));
    memset(&s3, 0, sizeof(s3));

    s1.a = -1;
    s1.b = -1;
    s1.c = -1;
    s1.e = -1;

    s2.a = -1;
    s2.b = -1;
    s2.c = -1;
    s2.e = -1;

    // find 'a' */
    p = memchr(&s1, 0xFF, sizeof(s1));
    if (!p)
        return 5;

    /* skip 'a' and find 'b' */
    p++;
    q = memchr(p, 0xFF, sizeof(s1));

    if (p == q)
    {
        /* this is allowed; but then there should be a one-byte gap between b and c field! */
        /* find gap between 'b' and 'c': */
        q = memchr(q, 0, sizeof(s1) / 2);
        if (!q)
            return 6;
        /* find 'c' */
        p = memchr(q, 0xFF, sizeof(s1) / 2);
        if (!p)
            return 7;
@%:@if defined(CFG_TRACE)
        printf("gap: %d\n", (int)(p - q));
@%:@endif
        if (p != q + 1)
        {
            /* gap is bigger than 1 byte! pack doesn't work as advertized here! */
            return 8;
        }
    }
    else
    {
        /* gap between 'a' and 'b': not allowed to be bigger than 1 byte! */
@%:@if defined(CFG_TRACE)
        printf("gap: %d\n", (int)(q - p));
@%:@endif
        if (q != p + 1)
        {
            /* gap is bigger than 1 byte! pack doesn't work as advertized here! */
            return 9;
        }
    }
    return 0;
]]
    )],
    [ac_cv_have_pragma_pack_push_pop=yes],
    [ac_cv_have_pragma_pack_push_pop=no])
    # reset language
    AC_LANG_POP
  ])
  AC_MSG_RESULT([$ac_cv_have_pragma_pack_push_pop])

  AC_MSG_CHECKING([for overly smart compiler using @%:@pragma pack])
  AC_CACHE_VAL([ac_cv_have_pragma_pack_cc_too_smart],
  [
    AC_RUN_IFELSE([
      AC_LANG_PROGRAM(
[AC_INCLUDES_DEFAULT([])[

@%:@pragma pack(2)
struct
{
    char a;
    int  b;
    char c;
    int  d;
    int  e;
} s1;
@%:@pragma pack()

@%:@pragma pack(16)
struct
{
    char a;
    int  b;
    char c;
    int  d;
    int  e;
} s2;
@%:@pragma pack()

@%:@pragma pack(4)
struct
{
    char a;
    int  b;
    char c;
    int  d;
    int  e;
} s3;
@%:@pragma pack()

@%:@undef CFG_TRACE    /* @%:@define this one if you need to debug this M4 macro */

]],[[
    char *p;
    char *q;

@%:@if defined(CFG_TRACE)
    printf("sizes: %d %d %d\n",     (int)sizeof(s1) , (int)sizeof(s2), (int)sizeof(s3));
@%:@endif

    if (sizeof(s1) >= sizeof(s2))
        return 2;
    if (sizeof(s2) <= sizeof(s3))  /* do NOT tolerate overly smart packing compilers!!! */
        return 0;
    if (sizeof(s1) >= sizeof(s3))
        return 4;
    return 5;
]]
    )],
    [ac_cv_have_pragma_pack_cc_too_smart=yes],
    [ac_cv_have_pragma_pack_cc_too_smart=no])
  ])
  AC_MSG_RESULT([$ac_cv_have_pragma_pack_cc_too_smart])

  if test "x$ac_cv_have_pragma_pack_N" == "xyes"; then
    AC_DEFINE(HAVE_PRAGMA_PACK, 1, [Define if compiler supports @%:@pragma pack(<size>).])
    if test "x$ac_cv_have_pragma_pack_push_pop" == "xyes"; then
      AC_DEFINE(HAVE_PRAGMA_PACK_PUSH_POP, 1, [Define if compiler supports @%:@pragma pack(push) / pack(pop) and pack(<size>).])
    fi
    if test "x$ac_cv_have_pragma_pack_cc_too_smart" == "xyes"; then
      AC_DEFINE(HAVE_PRAGMA_PACK_OVERSMART_COMPILER, 1, [Define if compiler does not listen strictly to large <size>s in @%:@pragma pack(<size>) but applies further member packing instead when none of the (basic) members are <size> bytes or more.])
    fi
  fi
])

