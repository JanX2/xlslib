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
 * $Source: /cvsroot/xlslib/xlslib/src/xlslib/datast.h,v $
 * $Revision: 1.4 $
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



#ifndef DATAST_H
#define DATAST_H 

#include <config.h>
#include <common.h>
#include <unit.h>
#include <row.h>
#include <HPSF.h>


/* 
******************************
COleFile class declaration
******************************
*/


#include <xls_pshpack2.h>

namespace xlslib_core
{
#if defined(LEIGHTWEIGHT_UNIT_FEATURE)

#define FIXEDWIDTH_STORAGEUNIT_SIZE  18 // the most frequent BIFF fixed-width records are BLANK, RK and NUMBER, which top out at 18 bytes a head

  /*
  This structure is constructed to give the least amount of storage overhead per [BIFF] data unit:
  
  4 bytes for simple types (fixed width, up to 18 bytes of storage per unit: BLANK/RK/NUMBER
  
  4+18+A / 4+18+A bytes per variabel/larger sized units on 32-/64-bit architectures, where 'A' is
      the number of bytes overhead per malloc(), which will very probably be >= 4

  Note: bitfields are used to allow the compiler to roll multiple variables into one integer, thus
        further reducing overhead. Given the limitations of Excel (including Office 2007), an upper
		bound of 2^28 ~ 500M bytes for each storage unit's size is quite acceptable.

  Note #2: the whole fixed vs. varying(large) width business was changed at the last minute
        to use the inverse of the flag (!m_varying_width --renamed--> m_varying_width) so that the
		copy constructor ASSERTs don't trigger when this stupid M$ vector<> calls the copy
		constructor for a memset(0,size)-all-zeroes 'object instance' (which isn't: the constructor
		wasn't called on the original!). Yes, the M$ MSVC2008 STL vector.end() only works out
		when your default class init is all zeroes everywhere. Now, with m_varying_width==0,
		the copy constructor will take the harmless 'local storage copy' route for this bogus
		class instance vector<>.end().
  */
	class CUnitStore
	{
	friend class CUnit;

	private:
		// 32-bits:
		unsigned m_varying_width: 1;		
		unsigned m_is_in_use: 1;		// used to flag a 'deleted' unit store
		unsigned m_is_sticky: 1;		// used to flag a 'sticky' unit store: that's a store which will remain after the accompanying CUnit is deleted. It will only be discarded by the time the CDataStorage container is deleted.
		unsigned m_nDataSize: 29;		// Actual use

		union
		{
			struct
			{
				unsigned8_t	m_pData[FIXEDWIDTH_STORAGEUNIT_SIZE];	// Data storage
			} fixed;
			struct
			{
				size_t m_nSize;		// Size of data store (maybe larger than actual used data)
				unsigned8_t*	m_pData;		// Data storage
			} vary;
		} s;

	public:
		CUnitStore();
		~CUnitStore();
		CUnitStore(const CUnitStore &src); // must provide a copy constructor or otherwise STL vector will nuke us

		signed8_t Prepare(size_t minimum_size);
		void Reset(void);
		signed8_t Resize(size_t newlen);
		signed8_t Init(const unsigned8_t *data, size_t size, size_t datasize);
		signed8_t InitWithValue(unsigned8_t value, size_t size);

		inline unsigned8_t *GetBuffer(void)
		{
			assert(m_is_in_use);
			if (!m_varying_width)
			{
				return s.fixed.m_pData;
			}
			else
			{
				return s.vary.m_pData;
			}
		}
		inline const unsigned8_t *GetBuffer(void) const
		{
			assert(m_is_in_use);
			if (!m_varying_width)
			{
				return s.fixed.m_pData;
			}
			else
			{
				return s.vary.m_pData;
			}
		}
		inline void SetDataSize(size_t len)
		{
			assert(m_is_in_use);
			assert(len <= GetSize());
			m_nDataSize = len;
		}
		inline size_t GetSize(void) const
		{
			assert(m_is_in_use);
			if (!m_varying_width)
			{
				return FIXEDWIDTH_STORAGEUNIT_SIZE;
			}
			else
			{
				return s.vary.m_nSize;
			}
		}
		inline size_t GetDataSize(void) const
		{
			assert(m_is_in_use);
			return m_nDataSize;
		}
		inline bool IsSticky(void) const
		{
			assert(m_is_in_use);
			return m_is_sticky;
		}
		inline void MakeSticky(void)
		{
			assert(m_is_in_use);
			m_is_sticky = 1;
		}
	};

	/*
	To reduce storage overhead and cut down on malloc()/free() library abuse (which is also hit by
	each new()/delete()), this vector stores the units as is, NOT pointers to new() allocated units!
	*/
  typedef std::vector<xlslib_core::CUnitStore XLSLIB_DFLT_ALLOCATOR> StoreList_t;
  typedef StoreList_t::iterator StoreList_Itor_t;

  typedef std::vector<xlslib_core::CUnit* XLSLIB_DFLT_ALLOCATOR> UnitList_t;
  typedef UnitList_t::iterator UnitList_Itor_t;

#else

  typedef std::vector<xlslib_core::CUnit* XLSLIB_DFLT_ALLOCATOR> DataList_t;
  typedef DataList_t::iterator DataList_Itor_t;

#endif

	class CRecord;
	class CRow;
	class CBof;
	class CEof;
	class CDimension;
	class CWindow1;
	class CWindow2;
	class CDateMode;
	class CStyle;
	class CBSheet;
	class CFormat;
	class CFont;
	class CNumber;
	class CMergedCells;
	class CLabel;
	class CIndex;
	class CExtFormat;
	class CContinue;
	class CPalette;
	class CColInfo;
	class CBlank;
	class CCodePage;
	class HPSFdoc; 

	class xf_t;
	class window1;
	struct style_t;
	class boundsheet_t;
	class format_t;
	class font_t;
	class number_t;
	class label_t;
	struct color_entry_t;
	struct colinfo_t;
	class blank_t;

	class CDataStorage
    {
    private:

#if defined(LEIGHTWEIGHT_UNIT_FEATURE)
		StoreList_t store;
#else
		DataList_t data;
#endif

		size_t m_DataSize;
   
    public:
		CDataStorage ();
		CDataStorage (size_t blobs);
		virtual ~CDataStorage ();

#if defined(LEIGHTWEIGHT_UNIT_FEATURE)

		StoreList_Itor_t begin();
		StoreList_Itor_t end();

#else

		DataList_Itor_t begin();
		DataList_Itor_t end();

#endif
	
	private:
		CDataStorage(const CDataStorage& that);
		CDataStorage& operator=(const CDataStorage& right);

    public:
		
		void operator+=(CUnit* from);
		size_t GetDataSize() const;

#if defined(LEIGHTWEIGHT_UNIT_FEATURE)

		signed32_t RequestIndex(size_t minimum_size);
		CUnitStore& operator[](signed32_t index);

	private:
		/*
		collect all CUnit instances here so we can discard them at 
		the appropriate time: some of them require late backpatching, 
		so lifetimes differ for certain Cunit instances

		To speed up the 'FlushEm' process a bit over multiple invocations, 
		we recall the position the previous flush stopped - which is a 
		perfect starting point for the next flush when the levels are
		identical.
		*/
		UnitList_t m_FlushStack; 
		size_t /* UnitList_Itor_t */ m_FlushLastEndPos; // default: .begin()
		/*
		^^^^^ WARNING LEST I FORGET [Ger Hobbelt / i_a] ^^^^^

		It's not safe, at least not with MSVC to keep a vector iterator around
		while code calls push_back() to add items to the vector: at some point in
		time the vector will be re-allocated and since the iterator is a pointer
		(not an index!) that one suddenly points straight to Hell.

		Another case of 'this almost never happens...' :-(
		*/
		unsigned16_t m_FlushLastEndLevel;

	public:
		void Push(CUnit* unit);
		void FlushEm(unsigned16_t backpatch_level = 0);
		void FlushLowerLevelUnits(const CUnit *unit);

		// factory:
	public:
	    CUnit* MakeCUnit();
        CRecord* MakeCRecord();
        CRow* MakeCRow(unsigned32_t rownum, unsigned32_t firstcol, unsigned32_t lastcol,
           unsigned16_t rowheight = ROW_DFLT_HEIGHT,
		   const xf_t* xformat = NULL);
        CBof* MakeCBof(unsigned16_t boftype);
        CEof* MakeCEof();
		CDimension* MakeCDimension(unsigned32_t minRow, unsigned32_t maxRow, unsigned32_t minCol, unsigned32_t maxCol);
		CWindow1* MakeCWindow1(const window1& wind1);
		CWindow2* MakeCWindow2(bool isActive);
		CDateMode* MakeCDateMode();
		CStyle* MakeCStyle(const style_t* styledef);
		CBSheet* MakeCBSheet(const boundsheet_t* bsheetdef);
		CFormat* MakeCFormat(const format_t* formatdef);
		CFont* MakeCFont(const font_t* fontdef);
		CNumber* MakeCNumber(const number_t& blankdef);
		CMergedCells* MakeCMergedCells();
		CLabel* MakeCLabel(const label_t& labeldef);
		CIndex* MakeCIndex(unsigned32_t firstrow, unsigned32_t lastrow);
		CExtFormat* MakeCExtFormat(const xf_t* xfdef);
		CContinue* MakeCContinue(const unsigned8_t* data, size_t size);
		CPalette* MakeCPalette(const color_entry_t *colors);
		CColInfo* MakeCColInfo(const colinfo_t* newci);
		CBlank* MakeCBlank(const blank_t& blankdef);
		CCodePage* MakeCCodePage(unsigned16_t boftype);
		CDBCell* MakeCDBCell(size_t startblock = DBC_DFLT_STARTBLOCK);
		HPSFdoc* MakeHPSFdoc(docType_t dt);

#endif
	};
}

#include <xls_poppack.h>

#endif //DATAST_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: datast.h,v $
 * Revision 1.4  2009/03/02 04:08:43  dhoerl
 * Code is now compliant to gcc  -Weffc++
 *
 * Revision 1.3  2009/01/08 02:53:15  dhoerl
 * December Rework
 *
 * Revision 1.2  2008/10/25 18:39:54  dhoerl
 * 2008
 *
 * Revision 1.1.1.1  2004/08/27 16:31:54  darioglz
 * Initial Import.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

