/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * This file is part of xlslib -- A multiplatform, C/C++ library
 * for dynamic generation of Excel(TM) files.
 *
 * xlslib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * xlslib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with xlslib.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * Copyright 2004 Yeico S. A. de C. V.
 * Copyright 2008 David Hoerl
 *  
 * $Source: /cvsroot/xlslib/xlslib/src/xlslib/common.h,v $
 * $Revision: 1.6 $
 * $Author: dhoerl $
 * $Date: 2009/03/02 04:08:43 $
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

#include <systype.h>

#include <list>
#include <vector>
#include <algorithm>
#include <string>

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

/*
#define when we want the leight weights factory code activated: it's a memory footprint reduction 
*/
#define LEIGHTWEIGHT_UNIT_FEATURE

#define RANGE_FEATURE

// #include <xls_pshpack2.h>

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
#endif // RANGE_FEATURE
#else  
  typedef std::vector<xlslib_core::range_t* XLSLIB_DFLT_ALLOCATOR> Range_Vect_t;
  typedef Range_Vect_t::iterator Range_Vect_Itor_t;
#ifdef RANGE_FEATURE
  class range;
  typedef std::vector<xlslib_core::range* XLSLIB_DFLT_ALLOCATOR> RangeObj_Vect_t;
  typedef RangeObj_Vect_t::iterator RangeObj_Vect_Itor_t;
#endif // RANGE_FEATURE
#endif // defined(_MSC_VER) && defined(WIN32)
}


// #include <xls_poppack.h>

#endif //XLSLIB_COMMON_H

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

