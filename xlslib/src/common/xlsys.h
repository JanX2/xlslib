/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * This file is part of xlslib -- A multiplatform, C/C++ library
 * for dynamic generation of Excel(TM) files.
 *
 * Copyright 2004 Yeico S. A. de C. V. All Rights Reserved.
 * Copyright 2008 David Hoerl All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are
 * permitted provided that the following conditions are met:
 * 
 *    1. Redistributions of source code must retain the above copyright notice, this list of
 *       conditions and the following disclaimer.
 * 
 *    2. Redistributions in binary form must reproduce the above copyright notice, this list
 *       of conditions and the following disclaimer in the documentation and/or other materials
 *       provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY David Hoerl ''AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL David Hoerl OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * File description:
 *
 *
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef XLSLIB_CONFIG_H
#define XLSLIB_CONFIG_H

#ifdef HAVE_CONFIG_H
#include <xlconfig.h>


#define CFG_TRUE          1
#define CFG_FALSE         0

#define ALLOCATOR_STL     0
#define ALLOCATOR_BTI     1


#if defined(USE_BTI_ALLOC)
#   define ALLOCATOR_DFLT ALLOCATOR_BTI
#elif defined(ALLOC_BTI)
#   define ALLOCATOR_DFLT ALLOCATOR_STL
#else
#   define ALLOCATOR_DFLT ALLOCATOR_STL
#endif


#else // HAVE_CONFIG_H

#if defined(_MSC_VER) && defined(WIN32)

#if (_MSC_VER >= 1400)

#if defined(WANT_MICROSOFT_MEMDBG)

// Studio 2008 (and maybe earlier versions too?)
#define _CRTDBG_MAP_ALLOC

#include "crtdbg.h"

#if defined(_DEBUG) && defined(__cplusplus)

#include <xdebug>

#include <xlocale>
#include <xiosbase>
#include <xlocnum>
#include <xtree>

#define new_dbg		new(_CLIENT_BLOCK, __FILE__, __LINE__)
#define new			new_dbg

#endif

#endif

#else

// Older versions of MSVC++
// Fakes the inclusion of crtdbg.h to avoid duplicated symbols
// of (inlined) new() and delete() operators
#define _INC_CRTDBG

#endif // _MSC_VER >= 1400

#include "ac-config.win32.h"

#endif // defined(_MSC_VER) && defined(WIN32)

#define CFG_TRUE          1
#define CFG_FALSE         0

#define ALLOCATOR_STL     0
#define ALLOCATOR_BTI     1


#if defined(ALLOC_STL)
#   define ALLOCATOR_DFLT ALLOCATOR_STL
#elif defined(ALLOC_BTI)
#   define ALLOCATOR_DFLT ALLOCATOR_BTI
#else
#   define ALLOCATOR_DFLT ALLOCATOR_STL
#endif


#endif // HAVE_CONFIG_H

#endif //XLSLIB_CONFIG_H
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: config.h,v $
 * Revision 1.4  2008/12/20 15:49:05  dhoerl
 * 1.2.5 fixes
 *
 * Revision 1.3  2008/10/25 18:39:53  dhoerl
 * 2008
 *
 * Revision 1.2  2004/09/01 00:47:04  darioglz
 * + Modified to gain independence of target
 *
 * Revision 1.1.1.1  2004/08/27 16:32:05  darioglz
 * Initial Import.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

