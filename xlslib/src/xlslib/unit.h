/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * This file is part of xlslib -- A multiplatform, C/C++ library
 * for dynamic generation of Excel(TM) files.
 *
 * Copyright 2004 Yeico S. A. de C. V. All Rights Reserved.
 * Copyright 2008-2011 David Hoerl All Rights Reserved.
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

#ifndef UNIT_H
#define UNIT_H


#include "common/xlsys.h"
#include "common/systype.h"
// #include "common/xls_pshpack2.h"

#include "xlslib/common.h"

namespace xlslib_core
{

#define UNIT_MAX_SIZE  0xFFFF

  // Error codes
#define ERR_DATASTORAGE_EMPTY            (-2)
#define ERR_INVALID_INDEX                (-3)
#define ERR_UNABLE_TOALLOCATE_MEMORY     (-4) // [i_a]

  //Block definitions
#define BIG_BLOCK_SIZE     0x200
#define SMALL_BLOCK_SIZE   0x040
#define PROP_BLOCK_SIZE    0x080

	// forward ref
	class CDataStorage;
	class CGlobalRecords;

  class CUnit 
  {
    // Attributes
  protected:

#define INVALID_STORE_INDEX			((signed32_t)0x80000000) // marks a 'not yet set up' unit store
	  
	friend class CDataStorage;

	CDataStorage &m_Store;
	signed32_t m_Index; // positive numbers index space in the associated storage; negative numbers are that index, but the data has been marked as 'sticky' in the associated storage

	unsigned16_t m_Backpatching_Level; // 0: requires no backpatching, 1: innermost backpatching (DBCELL), ...

	// Static attributes
	static const size_t DefaultInflateSize;
	
	// needed for SST labels (maybe others in the future)
	void SetAlreadyContinued(bool val) { m_AlreadyContinued = val; }

    // Operations
  protected: // deny these operations to others...
    CUnit(CDataStorage &datastore);

  private: // deny these operations to others...
	bool m_AlreadyContinued;
    CUnit(const CUnit& orig);
    CUnit& operator=(const CUnit& right);

  public:
	// primarily for Continue Records
	CDataStorage & DataStore() { return m_Store; }
	unsigned16_t BackPatchingLevel() const { return m_Backpatching_Level; };

	virtual ~CUnit();

    unsigned8_t& operator[](const size_t index) const;
	CUnit&   operator+=(const CUnit& from);
    CUnit&   operator+=(unsigned8_t from);

  protected:
	void ResetDataStorage(void);

  public:
    size_t GetSize(void) const;
    size_t GetDataSize(void) const;
    const unsigned8_t* GetBuffer(void) const;
	bool AlreadyContinued() const { return m_AlreadyContinued; };
//  protected:
//    unsigned8_t* GetBuffer(void);

  protected:
    signed8_t Init(const unsigned8_t* data, 
                   const size_t size, 
                   const unsigned32_t datasz);

  public:
    signed8_t AddDataArray(const unsigned8_t* newdata, size_t size);
    signed8_t AddFixedDataArray(const unsigned8_t value, size_t size);

	enum XlsUnicodeStringFormat_t
	{
		LEN2_FLAGS_UNICODE=1, // RECTYPE_FORMAT, RECTYPE_LABEL
		LEN2_NOFLAGS_PADDING_UNICODE, // RECTYPE_NOTE (RECTYPE_TXO)
		LEN1_FLAGS_UNICODE, // RECTYPE_BOUNDSHEET
		NOLEN_FLAGS_UNICODE, // RECTYPE_NAME
	};

	signed8_t AddUnicodeString(CGlobalRecords& gRecords, const std::string& str, XlsUnicodeStringFormat_t fmt /* = LEN2_FLAGS_UNICODE */ );
    signed8_t AddUnicodeString(const u16string& newdata, XlsUnicodeStringFormat_t fmt /* = LEN2_FLAGS_UNICODE */ );
    size_t UnicodeStringLength(const u16string& str16, size_t& strLen, bool& isAscii, XlsUnicodeStringFormat_t fmt /* = LEN2_FLAGS_UNICODE */ );

    signed8_t GetValue16From(unsigned16_t* val, unsigned32_t index) const;
    signed8_t GetValue32From(unsigned32_t* val, unsigned32_t index) const;
    signed8_t GetValue8From(unsigned8_t* data, unsigned32_t  index) const;
	
    signed8_t Append(const CUnit& newunit);

    signed8_t AddValue8(unsigned8_t newdata);
    signed8_t AddValue16(unsigned16_t newval);
    signed8_t AddValue32(unsigned32_t newval);
    signed8_t AddValue64(unsigned64_t newval);
	signed8_t AddValue64FP(double newval);

    signed8_t SetValueAt8(unsigned8_t newval, unsigned32_t index);							// Modify specific position
    signed8_t SetValueAt16(unsigned16_t newval, unsigned32_t index);
    signed8_t SetValueAt32(unsigned32_t newval, unsigned32_t index);

    signed8_t SetArrayAt(const unsigned8_t* newdata, size_t size, unsigned32_t index);
    //  signed8_t GetData(unsigned8_t** data, unsigned32_t from, unsigned32_t to );

  public:
    signed8_t Inflate(size_t newsize);
	signed32_t GetIndex() const { return m_Index; }

  protected:
    signed8_t InitFill(unsigned8_t data, size_t size);
  };
}

// #include "common/xls_poppack.h"

#endif 
// UNIT_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: unit.h,v $
 * Revision 1.8  2009/03/02 04:08:43  dhoerl
 * Code is now compliant to gcc  -Weffc++
 *
 * Revision 1.7  2009/01/23 16:09:55  dhoerl
 * General cleanup: headers and includes. Fixed issues building mainC and mainCPP
 *
 * Revision 1.6  2009/01/09 03:23:12  dhoerl
 * GlobalRec references tuning
 *
 * Revision 1.5  2009/01/08 22:16:06  dhoerl
 * January Rework
 *
 * Revision 1.4  2009/01/08 02:52:47  dhoerl
 * December Rework
 *
 * Revision 1.3  2008/12/20 15:48:23  dhoerl
 * 1.2.5 fixes
 *
 * Revision 1.2  2008/10/25 18:39:54  dhoerl
 * 2008
 *
 * Revision 1.1.1.1  2004/08/27 16:31:46  darioglz
 * Initial Import.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

