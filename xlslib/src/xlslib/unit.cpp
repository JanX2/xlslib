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
 * $Source: /cvsroot/xlslib/xlslib/src/xlslib/unit.cpp,v $
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

#include <unit.h>

using namespace std;
using namespace xlslib_core;

/* 
*********************************************************************************
*********************************************************************************
CUnit class implementation
*********************************************************************************
*********************************************************************************
*/

// Default constructor
CUnit::CUnit() :
	m_nSize(0),
	m_nDataSize(0),
	m_pData(NULL)
{

}
CUnit::CUnit(const CUnit& orig) :
	m_nSize(orig.m_nSize),
	m_nDataSize(orig.m_nDataSize),
	m_pData(orig.m_pData ? (unsigned8_t *)malloc(m_nSize) : NULL)
{
	if(m_pData) {
		memcpy(m_pData, orig.m_pData, m_nSize);
	}
}
CUnit& CUnit::operator=(const CUnit& right)
{
	if(this == &right) {
		return *this;
	}
	m_nSize			= right.m_nSize;
	m_nDataSize		= right.m_nDataSize;
	if(right.m_pData) {
		m_pData = (unsigned8_t *)malloc(m_nSize);
		memcpy(m_pData, right.m_pData, m_nSize);
	} else {
		m_pData = NULL;
	}
	return *this;
}

// Default destructor
CUnit::~CUnit (  )
{
   if(m_pData /*&& !m_ShadowUnit*/)
   {
      delete[] m_pData;
   }

}
/************************************************
 ************************************************/

const unsigned8_t CUnit::DefaultInflateSize = 10;

/************************************************
 ************************************************/

signed8_t CUnit::SetValueAt(unsigned8_t newval, unsigned32_t index)
{
   signed8_t errcode = NO_ERRORS;

   if(m_pData != NULL)
   {
      if (index < m_nDataSize)
         m_pData[index] = newval;
      else
         errcode =  ERR_INVALID_INDEX;
   } else {
      errcode =  ERR_DATASTORAGE_EMPTY;
   }
  
   return errcode;
}

/************************************************
 ************************************************/

signed8_t CUnit::AddValue16(unsigned16_t newval)
{
   signed8_t errcode = NO_ERRORS;

   if(AddValue8(BYTE_0(newval))) errcode = GENERAL_ERROR;
   if(AddValue8(BYTE_1(newval))) errcode = GENERAL_ERROR;
  
   return errcode;

}

/************************************************
 ************************************************/

signed8_t CUnit::AddValue32(unsigned32_t newval)
{
   signed8_t errcode = NO_ERRORS;
   
   if(AddValue8(BYTE_0(newval))) errcode = GENERAL_ERROR;
   if(AddValue8(BYTE_1(newval))) errcode = GENERAL_ERROR;
   if(AddValue8(BYTE_2(newval))) errcode = GENERAL_ERROR;
   if(AddValue8(BYTE_3(newval))) errcode = GENERAL_ERROR;
  
   return errcode;

}

/************************************************
 ************************************************/

signed8_t CUnit::AddValue64(unsigned64_t newval)
{
   signed8_t errcode = NO_ERRORS;

   if(AddValue8(BYTE_0(newval))) errcode = GENERAL_ERROR;
   if(AddValue8(BYTE_1(newval))) errcode = GENERAL_ERROR;
   if(AddValue8(BYTE_2(newval))) errcode = GENERAL_ERROR;
   if(AddValue8(BYTE_3(newval))) errcode = GENERAL_ERROR;

   if(AddValue8(BYTE_4(newval))) errcode = GENERAL_ERROR;
   if(AddValue8(BYTE_5(newval))) errcode = GENERAL_ERROR;
   if(AddValue8(BYTE_6(newval))) errcode = GENERAL_ERROR;
   if(AddValue8(BYTE_7(newval))) errcode = GENERAL_ERROR;

   return errcode;
}
signed8_t CUnit::AddValue64(unsigned64_t* newvalP)
{
   return CUnit::AddValue64(*newvalP);
}


/************************************************
 ************************************************/

signed8_t CUnit::SetValueAt(unsigned16_t newval, unsigned32_t index)
{
   signed8_t errcode = NO_ERRORS;

   if(SetValueAt(BYTE_0(newval), index  )) errcode = GENERAL_ERROR;
   if(SetValueAt(BYTE_1(newval), index+1)) errcode = GENERAL_ERROR;
  
   return errcode;
}

/************************************************
 ************************************************/

signed8_t CUnit::SetValueAt(unsigned32_t newval, unsigned32_t index)
{
   signed8_t errcode = NO_ERRORS;

   if(SetValueAt(BYTE_0(newval), index  )) errcode = GENERAL_ERROR;
   if(SetValueAt(BYTE_1(newval), index+1)) errcode = GENERAL_ERROR;
   if(SetValueAt(BYTE_2(newval), index+2)) errcode = GENERAL_ERROR;
   if(SetValueAt(BYTE_3(newval), index+3)) errcode = GENERAL_ERROR;

   return errcode;
}


/************************************************
 ************************************************/

signed8_t CUnit::GetValue16From(signed16_t* val, unsigned32_t index) const
{
   signed8_t errcode = NO_ERRORS;

   *val = (signed16_t)(operator[](index) + 
                       operator[](index+1)*0x0100);

   return errcode;
}

/************************************************
 ************************************************/

signed8_t CUnit::GetValue32From(signed32_t* val, unsigned32_t index) const
{
   signed8_t errcode = NO_ERRORS;
   // Yikes! this was signed16_t - DFH
   *val = (signed32_t)(operator[](index)  *0x00000001 + 
                       operator[](index+1)*0x00000100 +
                       operator[](index+2)*0x00010000 +
                       operator[](index+3)*0x01000000  );	// Yikes again, it was
   return errcode;
}

/************************************************
 ************************************************/

signed8_t CUnit::GetValue8From(signed8_t* data, unsigned32_t  index) const
{
   signed8_t errcode = NO_ERRORS;

   if(m_pData != NULL)
   {
      if (index < m_nDataSize)
      {
         *data = m_pData[index];
      } else {
         errcode =  ERR_INVALID_INDEX;
      }
   } else {
      errcode =  ERR_DATASTORAGE_EMPTY;
   }
  
   return errcode;

}
/************************************************
 ************************************************/
/*
  signed8_t CUnit::GetData(unsigned8_t** ppdata, unsigned32_t from, unsigned32_t to )
  {
  signed8_t errcode = NO_ERRORS;

  // Simply make the external pointer point to the local storage data
  *ppdata = m_pData;

  return errcode;
  }
*/
/************************************************
 ************************************************/
signed8_t CUnit::AddDataArray (const unsigned8_t* newdata, size_t size)
{

   signed8_t errcode = NO_ERRORS;
   size_t spaceleft = m_nSize - m_nDataSize;
  
   if(spaceleft < size) // allocate more space if new tobeadded array won't fit
   {
      Inflate(size-spaceleft+1);
   }

   if(newdata != NULL)
   {
      for(unsigned32_t i=0; i<size; i++)
         m_pData[m_nDataSize++] = newdata[i];
   } else {
      //No data to add. Do nothing
   }

   return errcode;
}

signed8_t CUnit::AddFixedDataArray (const unsigned8_t value, size_t size)
{

   signed8_t errcode = NO_ERRORS;
   size_t spaceleft = m_nSize - m_nDataSize;
  
   if(spaceleft < size) // allocate more space if new tobeadded array won't fit
   {
      Inflate(size-spaceleft+1);
   }

   // The following can be a memset
   for(unsigned32_t i=0; i<size; i++)
      m_pData[m_nDataSize++] = value;


   return errcode;
}
//    signed8_t AddDataArray (const unsigned16_t* newdata, size_t size);
//    signed8_t AddFixedDataArray (const unsigned16_t value, size_t size);

/************************************************
 ************************************************/
signed8_t CUnit::RemoveTrailData (unsigned32_t remove_size)
{
   /*
     total_to_remove = (m_nSize - m_nDataSize) - remove_size;
     size of temp_data = m_nSize - total_to_remove = m_nDataSize + remove_size
   */
   unsigned32_t temp_size = m_nDataSize + remove_size;
   unsigned8_t* temp_data = new unsigned8_t[temp_size];
  
   if(temp_data != NULL)
   {
      for(unsigned32_t i=0; i<temp_size; i++)
         temp_data[i] = m_pData[i];
   } else {
      return GENERAL_ERROR;
   }
  
   m_nDataSize = temp_size;
   m_nSize = m_nDataSize;
   delete[] m_pData;
   m_pData = temp_data;

   return NO_ERRORS;
}

/************************************************
 ************************************************/

signed8_t CUnit::SetArrayAt(const unsigned8_t* newdata, size_t size, unsigned32_t index)
{

   signed8_t errcode = NO_ERRORS;
   size_t spaceleft = m_nSize - index;

   if(spaceleft < size) // allocate more space if new tobeadded array won't fit
   {
      Inflate(size-spaceleft);
   }

   if(newdata != NULL)
   {
      for(unsigned32_t i=0; i<size; i++)
      {
         /*
         // The following code adds needed space to the whole allocated array
         if (index > m_nDataSize) m_nDataSize = index+1;
         else if(index==m_nDataSize) m_nDataSize++;
         m_pData[index++] = newdata[i];
         */
         // The following code truncates the array if it exceeds DataSize
         if(index==m_nDataSize) break;
		 
         m_pData[index++] = newdata[i];
      }
   } else {
      //No data to add. Do nothing
   }

   return errcode;
}

/************************************************
 ************************************************/

signed8_t CUnit::AddValue8(unsigned8_t newdata)
{
  
   if(m_nDataSize >= m_nSize) 
   {
      Inflate();
   }

   m_pData[m_nDataSize++] = newdata;

   return NO_ERRORS;
  
}

signed8_t CUnit::AddUnicodeString (const string* str, size_t size)
{
	string::const_iterator	cBegin, cEnd;
	signed8_t				errcode = NO_ERRORS;
	unsigned16_t			strSize, strLen;
	size_t					spaceleft;

	strLen = static_cast<unsigned16_t>(str->length());
	
	strSize = size == sizeof(unsigned8_t) ? 1 : 2;
	strSize += 1;	// flags byte
	strSize += strLen;

	spaceleft = m_nSize - m_nDataSize;
	if(spaceleft < strSize) // allocate more space if new tobeadded array won't fit
	{
	  Inflate((size_t)(strSize-spaceleft+1));
	}

	if(size == sizeof(unsigned8_t)) {
		m_pData[m_nDataSize++] = (unsigned8_t)strLen;
	} else {
		m_pData[m_nDataSize++] = strLen & 0xFF;
		m_pData[m_nDataSize++] = (strLen >> 8) & 0xFF;
	}
	m_pData[m_nDataSize++] = 0x00;	// ASCII
	
	cBegin	= str->begin();
	cEnd	= str->end();
	
	while(cBegin != cEnd) {
		m_pData[m_nDataSize++] = *cBegin++;
	}
	return errcode;
}
signed8_t CUnit::AddUnicodeString (const u16string* str16, size_t size, bool is_ascii)
{
	u16string::const_iterator	cBegin, cEnd;
	signed8_t					errcode = NO_ERRORS;
	unsigned16_t				strSize, strLen;
	size_t						spaceleft;

	strLen = static_cast<unsigned16_t>(str16->length());
	
	strSize = size == sizeof(unsigned8_t) ? 1 : 2;
	strSize += 1;	// flags byte
	strSize += is_ascii ? strLen : (strLen * 2);

	spaceleft = m_nSize - m_nDataSize;
	if(spaceleft < strSize) // allocate more space if new tobeadded array won't fit
	{
	  Inflate((size_t)(strSize-spaceleft+1));
	}

	if(size == sizeof(unsigned8_t)) {
		m_pData[m_nDataSize++] = (unsigned8_t)strLen;
	} else {
		m_pData[m_nDataSize++] = strLen & 0xFF;
		m_pData[m_nDataSize++] = (strLen >> 8) & 0xFF;
	}
	m_pData[m_nDataSize++] = is_ascii ? 0x00 : 0x01;	// ASCII or UTF-16
	
	cBegin	= str16->begin();
	cEnd	= str16->end();
	
	while(cBegin != cEnd) {
		unsigned16_t	c;
		
		c = *cBegin++;
		
		if(is_ascii) {
			m_pData[m_nDataSize++] = static_cast<unsigned8_t>(c);
		} else {
			m_pData[m_nDataSize++] = c & 0xFF;
			m_pData[m_nDataSize++] = (c >> 8) & 0xFF;
		}
	}
	return errcode;
}

/************************************************
 ************************************************/

signed8_t CUnit::Inflate(size_t increase)
{
   signed8_t errcode = NO_ERRORS;

   if (increase == 0)
      increase = CUnit::DefaultInflateSize;
  
   // Create the new storage with increased size
   // and initialize it to 0.
   unsigned8_t* temp_storage = new unsigned8_t[m_nSize + increase];

   if(temp_storage != NULL)
   {

      memset(temp_storage, 0, (m_nSize+increase)*(sizeof(unsigned8_t)));
      // Copy data to the new storage
      memcpy(temp_storage, m_pData, m_nSize*sizeof(unsigned8_t));
  
      // Update the size
      m_nSize += static_cast<unsigned32_t>(increase);
  
      if (m_pData != NULL)
         delete []m_pData;

      m_pData = temp_storage;
     
      errcode = ERR_UNNABLE_TOALLOCATE_MEMORY;
   } else {
      // No errors... errcode already clean
   }
  
   return errcode;
}

/************************************************
 ************************************************/

unsigned8_t& CUnit::operator[] ( const unsigned32_t index ) const
{

#if 1
   assert(index < m_nSize);	// DFH: need to read ahead when setting bits in 32bit words
   //if(index >= m_nDataSize) printf("ERROR: Short read!! \n");
#else
   // this old code really bad - get bad data and never know it!
   if(index >= m_nDataSize)
      return m_pData[m_nDataSize];
#endif
   return m_pData[index];

}

/************************************************
 ************************************************/

CUnit& CUnit::operator+= ( CUnit& from )
{
  
   if(&from != this)
      Append(from);
   else
   {
      CUnit shadow;
      shadow  = from;
      Append(shadow);
   }
   return *this;
}

/************************************************
 ************************************************/

CUnit& CUnit::operator+= ( unsigned8_t from )
{
  
   AddValue8(from);

   return *this;
}


/************************************************
 ************************************************/

signed8_t CUnit::Init (unsigned8_t* data, const size_t size, const unsigned32_t datasz)
{

   m_nSize		= static_cast<unsigned32_t>(size);
   m_nDataSize	= datasz;

   m_pData = new unsigned8_t[m_nSize];
  
   if(data)
   {
      memset(m_pData, 0, m_nSize*sizeof(unsigned8_t));
      // Copy data to the new storage
      memcpy(m_pData, data, m_nSize*sizeof(unsigned8_t));
   }

   return NO_ERRORS;
}


/************************************************
 ************************************************/

signed8_t CUnit::Append (CUnit& newunit)
{

   if(AddDataArray(newunit.GetBuffer(), newunit.GetDataSize()) == NO_ERRORS)
      return NO_ERRORS;
   else
      return GENERAL_ERROR;
}

/************************************************
 ************************************************/
signed8_t CUnit::InitFill (unsigned8_t data, unsigned32_t size)
{
   if(m_pData)
      delete[] m_pData;

   m_pData = new unsigned8_t[size];

   if(m_pData)
   {
      memset(m_pData, data, size*sizeof(unsigned8_t));
      m_nSize = m_nDataSize = size;

      return NO_ERRORS;
   } else {
      return GENERAL_ERROR;
   }

}

/************************************************
 ************************************************/

size_t CUnit::GetSize (void)
{
   return m_nSize;
}

/************************************************
 ************************************************/

unsigned32_t CUnit::GetDataSize (void)
{
   return m_nDataSize;
}

/************************************************
 ************************************************/

unsigned8_t* CUnit::GetBuffer (void)
{
   return m_pData;
}


/************************************************
 ************************************************/
/*
  void CUnit::SetShadow(bool shadowval)
  {
  m_ShadowUnit = shadowval;


  }
*/

/************************************************
 ************************************************/
/*
  void CUnit::CopyShadowUnit(unsigned8_t* data, unsigned32_t size)
  {
  m_Size = m_DataSize = size;
  m_pData = data;

  SetShadow(true);



  }
*/

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: unit.cpp,v $
 * Revision 1.8  2009/03/02 04:08:43  dhoerl
 * Code is now compliant to gcc  -Weffc++
 *
 * Revision 1.7  2009/01/23 16:09:55  dhoerl
 * General cleanup: headers and includes. Fixed issues building mainC and mainCPP
 *
 * Revision 1.6  2009/01/08 22:16:06  dhoerl
 * January Rework
 *
 * Revision 1.5  2009/01/08 02:52:47  dhoerl
 * December Rework
 *
 * Revision 1.4  2008/12/20 15:48:34  dhoerl
 * 1.2.5 fixes
 *
 * Revision 1.3  2008/10/25 18:39:54  dhoerl
 * 2008
 *
 * Revision 1.2  2004/09/01 00:47:21  darioglz
 * + Modified to gain independence of target
 *
 * Revision 1.1.1.1  2004/08/27 16:31:53  darioglz
 * Initial Import.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

