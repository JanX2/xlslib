#ifndef __XLSLIB_PUSH_PACK_4_H__
#define __XLSLIB_PUSH_PACK_4_H__

#if defined(HAVE_PSHPACK4_H)

#include <pshpack4.h>

#else

#if defined(HAVE_PRAGMA_PACK_PUSH_POP)

#pragma pack(push, 4)

#elif defined(HAVE_PRAGMA_PACK)

#pragma pack(4)

#else

#error "must have some sort of packing #pragma for this tool to build"

#endif
#endif

#endif

