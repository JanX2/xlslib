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
 * $Source: /cvsroot/xlslib/xlslib/src/xlslib/unit.h,v $
 * $Revision: 1.8 $
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

#ifndef UNIT_H
#define UNIT_H

#include <config.h>
#include <common.h>

#include <xlstypes.h>


namespace xlslib_core
{

#define UNIT_MAX_SIZE (0xFFFF)

  // Error codes
#define ERR_DATASTORAGE_EMPTY (-2)
#define ERR_INVALID_INDEX     (-3)
#define ERR_UNNABLE_TOALLOCATE_MEMORY     (-3)

  //Block definitions
#define BIG_BLOCK_SIZE     (0x200)
#define SMALL_BLOCK_SIZE   (0x040)
#define PROP_BLOCK_SIZE    (0x080)

  class CUnit {
    // Attributes
  protected:
	unsigned32_t	m_nSize;		// Size of data store (maybe larger than actual used data)
	unsigned32_t	m_nDataSize;	// Actual use (next empty space)
	unsigned8_t*	m_pData;		// Data storage
	//  bool m_ShadowUnit;

	// Static attributes
	static const unsigned8_t DefaultInflateSize;

    // Operations
  public:
    CUnit();
    CUnit(const CUnit& orig);
    CUnit& operator=(const CUnit& right);
    virtual ~CUnit();

    unsigned8_t& operator[](const unsigned32_t index) const;
    CUnit&   operator+=(CUnit& from);
    CUnit&   operator+= ( unsigned8_t from );

    size_t GetSize (void);
    unsigned32_t GetDataSize (void);
    unsigned8_t* GetBuffer (void);
    signed8_t Init (unsigned8_t* data, 
                    const size_t size, 
                    const unsigned32_t datasz);

    signed8_t AddDataArray (const unsigned8_t* newdata, size_t size);
    signed8_t AddFixedDataArray (const unsigned8_t value, size_t size);
    signed8_t AddUnicodeString (const std::string* str, size_t size);
    signed8_t AddUnicodeString (const u16string* newdata, size_t size, bool is_ascii);	// size is the string length, 1 or 2
    /*
      void SetShadow(bool shadowval);
      void CopyShadowUnit(unsigned8_t* data, unsigned32_t size);
    */
    signed8_t GetValue16From(signed16_t* val, unsigned32_t index) const;
    signed8_t GetValue32From(signed32_t* val, unsigned32_t index) const;
    signed8_t GetValue8From(signed8_t* data, unsigned32_t  index) const;
	
  protected:
    signed8_t Append (CUnit& newunit);

    signed8_t AddValue8(unsigned8_t newdata);
    signed8_t AddValue16(unsigned16_t newval);
    signed8_t AddValue32(unsigned32_t newval);
    signed8_t AddValue64(unsigned64_t newval);
    signed8_t AddValue64(unsigned64_t* newvalP);

    signed8_t SetValueAt(unsigned8_t newval, unsigned32_t index);							// Modify specific position
    signed8_t SetValueAt(unsigned16_t newval, unsigned32_t index);
    signed8_t SetValueAt(unsigned32_t newval, unsigned32_t index);

    signed8_t SetArrayAt (const unsigned8_t* newdata, size_t size, unsigned32_t index);
    //  signed8_t GetData(unsigned8_t** data, unsigned32_t from, unsigned32_t to );

    signed8_t RemoveTrailData (unsigned32_t remove_size);
    signed8_t InitFill (unsigned8_t data, unsigned32_t size);
    signed8_t Inflate(size_t increase = 0);
  };
}
#endif // UNIT_H

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

