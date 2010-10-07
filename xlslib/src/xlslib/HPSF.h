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
 * $Source: /cvsroot/xlslib/xlslib/src/xlslib/HPSF.h,v $
 * $Revision: 1.2 $
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

#ifndef HPSF_H
#define HPSF_H

#include <time.h>
#include <xlsys.h>
#include <common.h>
#include <unit.h>

#define SUMMARY_SIZE		4096		// one big blocks

#define FILETIME2UNIX_NS	11644473600000000ll		// from the web


// #include <xls_pshpack2.h>

namespace xlslib_core
{
	extern const unsigned32_t summaryFormat[4], docSummaryFormat[4], hpsfValues[];

	typedef enum _docType_t 
	{
		HPSF_SUMMARY,
		HPSF_DOCSUMMARY
	} docType_t;
	
	typedef enum 
	{
		HPSF_STRING=0,
		HPSF_BOOL,
		HPSF_INT16,
		HPSF_INT32,
		HPSF_INT64
	} hpsf_t;
	typedef union 
	{
		std::string		*str;
		bool			isOn;
		unsigned16_t	val16;
		unsigned32_t	val32;
		unsigned64_t	val64;
	} hValue;
	
	class insertsort2;
	class HPSFitem 
	{
	friend class insertsort2;

	private:
		unsigned16_t		propID;
		hpsf_t				variant;
		hValue				value;
		size_t offset;

	public:
		HPSFitem(unsigned16_t type, const std::string& str);
		HPSFitem(unsigned16_t type, bool val);
		HPSFitem(unsigned16_t type, unsigned16_t val);
		HPSFitem(unsigned16_t type, unsigned32_t val);
		HPSFitem(unsigned16_t type, unsigned64_t val);
		~HPSFitem();
		
		hValue				GetValue() const {return value;};
		unsigned16_t		GetPropID() const {return propID;};
		unsigned16_t		GetVariant() const {return (unsigned16_t)variant;};
		
		void SetOffset(size_t of) { offset=of;};
		size_t GetOffset() const { return offset;};
		
		size_t GetSize();	// actual length rounded up to 4 bytes

		bool operator< (const HPSFitem& rhs) const { return variant < rhs.variant; };
	};

  class insertsort2
	{
    public:
		bool operator()(HPSFitem* a, HPSFitem* b) const { return a->propID < b->propID;};
	};

	typedef std::set<xlslib_core::HPSFitem*, insertsort2 XLSLIB_DFLT_ALLOCATOR> HPSF_Set_t;
	typedef HPSF_Set_t::iterator HPSF_Set_Itor_t;
	typedef HPSF_Set_t::const_iterator HPSF_Set_ConstItor_t;



	class CHPSFdoc;

	class hpsf_doc_t
	{
		friend class CHPSFdoc;

	private:
		void insert(HPSFitem *item);

	public:
		hpsf_doc_t(docType_t dt);
		virtual ~hpsf_doc_t();
		
	public:
		void addItem(unsigned16_t key, unsigned16_t val) {insert(new HPSFitem(key, val));};
		void addItem(unsigned16_t key, const std::string& str) {insert(new HPSFitem(key, str));};

		void addItem(unsigned16_t key, bool val) {insert(new HPSFitem(key, val));};
		void addItem(unsigned16_t key, unsigned32_t val) {insert(new HPSFitem(key, val));};
		void addItem(unsigned16_t key, unsigned64_t val) {insert(new HPSFitem(key, val));};
		
		//unsigned32_t	NumProperties() const {return numProperties;};
		unsigned64_t unix2mstime(time_t unixTime);
	  
	public:
	  virtual size_t GetSize(void) const;
	  virtual CUnit* GetData(CDataStorage &datastore) const;

	private:
		docType_t		docType;
		HPSF_Set_t		itemList;
		//unsigned32_t	numProperties;
	};


	// forward ref
	class CDataStorage;

	class CHPSFdoc : public CUnit
	{
		friend class CSummaryInfo;
		friend class CDocSummaryInfo;
#if defined(LEIGHTWEIGHT_UNIT_FEATURE)
		friend class CDataStorage;
#endif

    protected:
		CHPSFdoc(CDataStorage &datastore, const hpsf_doc_t& docdef);
	private:
		virtual ~CHPSFdoc();
	};
}


// #include <xls_poppack.h>

#endif
