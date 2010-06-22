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
 * $Source: /cvsroot/xlslib/xlslib/src/common/systype.h,v $
 * $Revision: 1.9 $
 * $Author: dhoerl $
 * $Date: 2009/01/23 16:09:55 $
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * File description:
 *
 *
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

//#define __DEBUG__

#ifndef SYSTYPE_H
#define SYSTYPE_H

#if defined(_MSC_VER) && defined(WIN32) && 0
#pragma warning(disable: 4786) // Long debug information related to STL
#pragma warning(disable: 4290) // C++ Exception Specification ignored
#endif


#include <xlsys.h>


//#   define PHPEXT_TSRMLS_C  tsrm_ls
//#   define PHPEXT_TSRMLS_D  void ***tsrm_ls

// Global includes
#include <stdio.h>
#ifdef HAVE_STDINT_H	// DONT_HAVE_STDINT_H
#include <stdint.h>
#endif
#ifdef HAVE_INTTYPES_H
#include <inttypes.h>
#endif
#if defined(__cplusplus)					// ALL C++ users
#include <cstring>
#include <string>
#endif
#ifdef HAVE_STRING_H
#include <string.h>	// DFH 10-2-08
#endif
#if defined(__cplusplus)					// ALL C++ users
#include <iostream>
#include <fstream>  // [i_a] MSVC needs this for cerr (XTRACE() et al)
#include <list>
#include <set>
#include <algorithm>
#endif
#include <assert.h>

//#define INLINE inline

//#define NULL ((void*)(0))

#define NO_ERRORS         0
#define FILE_ERROR        (-1)
#define GENERAL_ERROR     (-100)

#define PTYPE_DIRECTORY   (0x01)
#define PTYPE_FILE        (0x02)
#define PTYPE_ROOT        (0x05)

#define PCOLOR_BLACK      (0x01)
#define PCOLOR_RED		  (0x00)

#define PLINK_EMPTY       (-1)
#define PBLOCK_END        (-2)


#if USE_ALLOCATOR==ALLOCATOR_STL
#define XLSLIB_DFLT_ALLOCATOR

#elif USE_ALLOCATOR==ALLOCATOR_BTI
#include "bti_thread_alloc.h"
#define XLSLIB_DFLT_ALLOCATOR ,bti::thr_alloc

#else
#   error ERROR: Invalid allocator option
#endif



#include <xlstypes.h>  /* [i_a]   better to get the definition from a single place */





#if defined(__cplusplus)					// ALL C++ users

typedef std::list<std::string* XLSLIB_DFLT_ALLOCATOR> StringList_t;
typedef StringList_t::const_iterator StringListItor_t;

#endif

// Macros
# define MASK_BYTE0 0x00000000000000ff
# define MASK_BYTE1 0x000000000000ff00
# define MASK_BYTE2 0x0000000000ff0000
# define MASK_BYTE3 0x00000000ff000000

#if defined(_MSC_VER) && defined(WIN32) && 0 // [i_a]

# define MASK_BYTE4 0x000000ff00000000
# define MASK_BYTE5 0x0000ff0000000000
# define MASK_BYTE6 0x00ff000000000000
# define MASK_BYTE7 0xff00000000000000

#else

# define MASK_BYTE4 (unsigned64_t)0x000000ff00000000LL
# define MASK_BYTE5 (unsigned64_t)0x0000ff0000000000LL
# define MASK_BYTE6 (unsigned64_t)0x00ff000000000000LL
# define MASK_BYTE7 (unsigned64_t)0xff00000000000000ULL

#endif

#define BYTE_0(num) (unsigned8_t) (num & MASK_BYTE0)
#define BYTE_1(num) (unsigned8_t)((num & MASK_BYTE1)>>0x08)
#define BYTE_2(num) (unsigned8_t)((num & MASK_BYTE2)>>0x10)
#define BYTE_3(num) (unsigned8_t)((num & MASK_BYTE3)>>0x18)

#define BYTE_4(num) (unsigned8_t)((num & MASK_BYTE4)>>0x20)
#define BYTE_5(num) (unsigned8_t)((num & MASK_BYTE5)>>0x28)
#define BYTE_6(num) (unsigned8_t)((num & MASK_BYTE6)>>0x30)
#define BYTE_7(num) (unsigned8_t)((num & MASK_BYTE7)>>0x38)



#define PROPERTY_DFLT_NOTUSED		(0x00)
#define PROPERTY_MAX_NAME_LENGTH	(32)

#define PROPERTY_TYPE_DIR			(0x01)
#define PROPERTY_TYPE_FILE			(0x02)
#define PROPERTY_TYPE_ROOT			(0x05)

#define PROPERTY_COLOR_NODE_RED		(0x00)
#define PROPERTY_COLOR_NODE_BLACK	(0x01)
#define PROPERTY_LINK_EMPTY			(-1)

#define PPTPOS_NAME					(0x00)
#define PPTPOS_NAMELENGTH			(0x40)
#define PPTPOS_TYPE					(0x42)
#define PPTPOS_NODECOLOR			(0x43)
#define PPTPOS_PREVIOUS				(0x44)
#define PPTPOS_NEXT					(0x48)
#define PPTPOS_CHILD				(0x4C)
#define PPTPOS_UNUSED_EMPTY0		(0x50)


#define PPTPOS_SECS1				(0x64)
#define PPTPOS_SECS2				(0x68)
#define PPTPOS_DAYS1				(0x6c)
#define PPTPOS_DAYS2				(0x70)

#define PPTPOS_STARTBLOCK			(0x74)
#define PPTPOS_SIZE					(0x78)


#define PPT_DFLT_SECS1				(0)
#define PPT_DFLT_SECS2				(0)
#define PPT_DFLT_DAYS1				(0)
#define PPT_DFLT_DAYS2				(0)


// More macros

/*  
****************************************
****************************************
*/
#define Delete_Pointer(pt) {                    \
  if(pt != NULL)                                \
    {                                           \
      delete pt;                                \
      pt = NULL;                                \
    }                                           \
}


/*  
****************************************
****************************************
*/
#ifdef __DEBUG__

#define XTRACE(str) {                           \
  std::cerr<<"DEBUG: "<<str<<std::endl;         \
}
#define XTRACE2(str, arg) {                     \
  std::cerr<<"DEBUG: "<<str<<arg<<std::endl;    \
}

#else

#define XTRACE(str)
#define XTRACE2(str, arg)

#endif


#ifdef __PHP_DEBUG__

#define PHPTRACE(str) {                         \
	std::cerr<<"PHPDEBUG: "<<str<<std::endl;    \
}

#else

#define PHPTRACE(str)

#endif


#endif //SYSTYPE_H


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: systype.h,v $
 * Revision 1.9  2009/01/23 16:09:55  dhoerl
 * General cleanup: headers and includes. Fixed issues building mainC and mainCPP
 *
 * Revision 1.8  2009/01/09 03:23:11  dhoerl
 * GlobalRec references tuning
 *
 * Revision 1.7  2009/01/08 02:53:45  dhoerl
 * December Rework
 *
 * Revision 1.6  2008/12/20 15:49:05  dhoerl
 * 1.2.5 fixes
 *
 * Revision 1.5  2008/12/10 03:33:40  dhoerl
 * m_usage was 16bit and wrapped
 *
 * Revision 1.4  2008/12/06 01:42:57  dhoerl
 * John Peterson changes along with lots of tweaks. Many bugs that causes Excel crashes fixed.
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

