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
 * Copyright 2009 David Hoerl
 *  
 * $Source: /cvsroot/xlslib/xlslib/src/common/xlstypes.h,v $
 * $Revision: 1.1 $
 * $Author: dhoerl $
 * $Date: 2009/03/02 04:21:46 $
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * File description:
 *
 *	type declaration
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef XLSTYPES_H
#define XLSTYPES_H

// Setup our typedefs now - would like to get them from from systype.h
#ifdef uint8_t				// if we have this one we will have all the others too

typedef uint8_t				unsigned8_t;
typedef uint16_t			unsigned16_t;
typedef uint32_t			unsigned32_t;

typedef int8_t				signed8_t;
typedef int16_t				signed16_t;
typedef int32_t				signed32_t;

#else						// no systype.h

typedef unsigned char      unsigned8_t;
typedef unsigned short int unsigned16_t;
typedef unsigned int       unsigned32_t;

typedef char               signed8_t;
typedef short int          signed16_t;
typedef int                signed32_t;

#endif // uint8_t


#if defined(_MSC_VER) && defined(WIN32)		// Windows

typedef __int64 unsigned64_t;

#else										// not windows

#ifdef _UINT64_T

typedef uint64_t unsigned64_t;

#else

typedef unsigned long long unsigned64_t;

#endif // _UINT64_T

#endif // defined(_MSC_VER) && defined(WIN32)


#if defined(__cplusplus)					// ALL C++ users

#if defined(_MSC_VER) && defined(WIN32)		// Windows

typedef wchar_t uchar_t;
#define ustring wstring
//typedef wstring ustring;
typedef std::basic_string<unsigned16_t> u16string;

#elif defined(__FRAMEWORK__)				// MAC Framework

#undef HAVE_ICONV

//typedef unichar uchar_t;
#define uchar_t unsigned16_t

//typedef std::basic_string<unsigned16_t> ustring;
#define ustring basic_string<unsigned16_t>

typedef std::basic_string<unsigned16_t> u16string;

#else										// All other C++

typedef wchar_t uchar_t;
#define ustring wstring
typedef std::basic_string<unsigned16_t> u16string;

#endif	// defined(_MSC_VER) && defined(WIN32)

#endif	// C++

#endif   //XLSTYPES_H