#ifndef __XLSLIB_PUSH_PACK_1_H__
#define __XLSLIB_PUSH_PACK_1_H__

#if defined(HAVE_PSHPACK1_H)

#include <pshpack1.h>

#else

#if defined(HAVE_PRAGMA_PACK_PUSH_POP)

#pragma pack(push, 1)

#elif defined(HAVE_PRAGMA_PACK)

#pragma pack(1)

#else

#error "must have some sort of packing #pragma for this tool to build"

#endif
#endif

#endif

