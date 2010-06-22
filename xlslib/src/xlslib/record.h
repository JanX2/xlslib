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
 * $Source: /cvsroot/xlslib/xlslib/src/xlslib/record.h,v $
 * $Revision: 1.4 $
 * $Author: dhoerl $
 * $Date: 2009/01/08 02:52:47 $
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * File description:
 *
 *
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef RECORD_H
#define RECORD_H

#include <xlsys.h>
#include <common.h>
#include <rectypes.h>
#include <unit.h>


#include <xls_pshpack2.h>

namespace xlslib_core
{
	// forward ref
	class CDataStorage;

  /* 
******************************
CRecord class declaration
******************************
*/
  class CRecord: public CUnit
    {
#if defined(LEIGHTWEIGHT_UNIT_FEATURE)
	friend class CDataStorage;
#endif

    protected:
      unsigned16_t m_Num;
      //unsigned32_t streamPos;

    protected:
      CRecord(CDataStorage &datastore);
      virtual ~CRecord();
  
    public:
      void SetRecordType(unsigned16_t rtype);
      unsigned16_t GetRecordType() const;

      void SetRecordLength(size_t);
      size_t GetRecordLength() const;

      const unsigned8_t* GetRecordDataBuffer() const;
      size_t GetRecordDataSize() const;

      //void SetStreamPos(size_t pos) { streamPos = pos; };
      //size_t GetStreamPos() { return streamPos; };
    };
}

#include <xls_poppack.h>

#endif //RECORD_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: record.h,v $
 * Revision 1.4  2009/01/08 02:52:47  dhoerl
 * December Rework
 *
 * Revision 1.3  2008/12/11 21:12:16  dhoerl
 * Cleanup
 *
 * Revision 1.2  2008/10/25 18:39:54  dhoerl
 * 2008
 *
 * Revision 1.1.1.1  2004/08/27 16:31:54  darioglz
 * Initial Import.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

