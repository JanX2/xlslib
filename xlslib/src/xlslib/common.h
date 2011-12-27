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

#ifndef XLSLIB_COMMON_H
#define XLSLIB_COMMON_H

#ifdef HAVE_ICONV
#include <iconv.h>

/* part of fix for PR #3039001 */
#ifndef __GLIBC__
#define UCS_2_INTERNAL "UCS-2-INTERNAL"
#else
#if (__GLIBC__ == 2 && __GLIBC_MINOR__ <= 1 )
#error "Too old glibc. This version's iconv() implementation cannot be trusted."
#endif
#define UCS_2_INTERNAL "UCS-2"
#endif
#endif /* HAVE_ICONV */


#define RANGE_FEATURE

// #include "common/xls_pshpack2.h"

namespace xlslib_core
{
  // Some typedefs used only by xlslib core
  class range_t
  {
	public:
		range_t() : first_row(0), last_row(0), first_col(0), last_col(0) { }
		virtual ~range_t() { }

	public:
		unsigned32_t first_row;
		unsigned32_t last_row;
		unsigned32_t first_col;
		unsigned32_t last_col;
  };


#if defined(_MSC_VER) && defined(WIN32)
  typedef std::vector<range_t* XLSLIB_DFLT_ALLOCATOR> Range_Vect_t;
  typedef Range_Vect_t::iterator Range_Vect_Itor_t;

#ifdef RANGE_FEATURE
  class range;
  typedef std::vector<range* XLSLIB_DFLT_ALLOCATOR> RangeObj_Vect_t;
  typedef RangeObj_Vect_t::iterator RangeObj_Vect_Itor_t;
#endif 
// RANGE_FEATURE
#else
  typedef std::vector<xlslib_core::range_t* XLSLIB_DFLT_ALLOCATOR> Range_Vect_t;
  typedef Range_Vect_t::iterator Range_Vect_Itor_t;
#ifdef RANGE_FEATURE
  class range;
  typedef std::vector<xlslib_core::range* XLSLIB_DFLT_ALLOCATOR> RangeObj_Vect_t;
  typedef RangeObj_Vect_t::iterator RangeObj_Vect_Itor_t;
#endif 
// RANGE_FEATURE
#endif 
// defined(_MSC_VER) && defined(WIN32)
}

// #include "common/xls_poppack.h"

#endif 
//XLSLIB_COMMON_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: common.h,v $
 * Revision 1.6  2009/03/02 04:08:43  dhoerl
 * Code is now compliant to gcc  -Weffc++
 *
 * Revision 1.5  2009/01/23 16:09:55  dhoerl
 * General cleanup: headers and includes. Fixed issues building mainC and mainCPP
 *
 * Revision 1.4  2009/01/09 03:23:12  dhoerl
 * GlobalRec references tuning
 *
 * Revision 1.3  2009/01/08 02:53:15  dhoerl
 * December Rework
 *
 * Revision 1.2  2008/10/25 18:39:54  dhoerl
 * 2008
 *
 * Revision 1.1.1.1  2004/08/27 16:31:57  darioglz
 * Initial Import.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

