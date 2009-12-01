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
 * $Source: /cvsroot/xlslib/xlslib/src/xlslib/record.cpp,v $
 * $Revision: 1.5 $
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


#include <config.h>

#include <record.h>

using namespace std;
using namespace xlslib_core;

/* 
******************************
CRecord class implementation
******************************
*/
CRecord::CRecord(CDataStorage &datastore) :
	CUnit(datastore),
   m_Num(RECTYPE_NULL)
{
   static const unsigned8_t array[] = {0,0,0,0} ; 
   // Initialize (and create) the space for record type
   // and record length
   AddDataArray(array, 4);
}

CRecord::~CRecord()
{
}

/* 
******************************
******************************
*/
void CRecord::SetRecordType(unsigned16_t rtype)
{
   SetValueAt16((unsigned16_t)rtype, 0);
}


/* 
******************************
******************************
*/
unsigned16_t CRecord::GetRecordType()
{
   signed16_t value;
   GetValue16From(&value, 0);

   return value;
}

/* 
******************************
******************************
*/
void CRecord::SetRecordLength(size_t rlength)
{
   SetValueAt16((unsigned16_t)rlength, 2);
}


/* 
******************************
******************************
*/

size_t CRecord::GetRecordLength()
{
   unsigned16_t value;
   GetValue16From((signed16_t*)&value, 2);

   return value;
}


/* 
******************************
******************************
*/
unsigned8_t* CRecord::GetRecordDataBuffer()
{
   return GetBuffer() + 4;
}


/* 
******************************
******************************
*/
size_t CRecord::GetRecordDataSize()
{
   size_t len = GetDataSize() - 4;
   return len;
}

/* 
******************************
******************************
*/

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: record.cpp,v $
 * Revision 1.5  2009/03/02 04:08:43  dhoerl
 * Code is now compliant to gcc  -Weffc++
 *
 * Revision 1.4  2009/01/08 02:52:47  dhoerl
 * December Rework
 *
 * Revision 1.3  2008/12/11 21:12:24  dhoerl
 * Cleanup
 *
 * Revision 1.2  2008/10/25 18:39:54  dhoerl
 * 2008
 *
 * Revision 1.1.1.1  2004/08/27 16:31:46  darioglz
 * Initial Import.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

