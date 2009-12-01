#ifndef __XLSLIB_PUSH_PACK_2_H__
#define __XLSLIB_PUSH_PACK_2_H__

#if defined(HAVE_PSHPACK2_H)

#include <pshpack2.h>

#else

#if defined(HAVE_PRAGMA_PACK_PUSH_POP)

#pragma pack(push, 2)

#elif defined(HAVE_PRAGMA_PACK)

#pragma pack(2)

#else

#error "must have some sort of packing #pragma for this tool to build"

#endif
#endif

#endif

