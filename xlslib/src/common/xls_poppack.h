#ifndef __XLSLIB_POP_PACK_H__
#define __XLSLIB_POP_PACK_H__

#if defined(HAVE_POPPACK_H)

#include <poppack.h>

#else

#if defined(HAVE_PRAGMA_PACK_PUSH_POP)

#pragma pack(pop)

#elif defined(HAVE_PRAGMA_PACK)

#pragma pack()

#else

#error "must have some sort of packing #pragma for this tool to build"

#endif
#endif

#endif

