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
 * $Source: /cvsroot/xlslib/xlslib/src/xlslib/index.cpp,v $
 * $Revision: 1.3 $
 * $Author: dhoerl $
 * $Date: 2009/01/08 02:52:59 $
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * File description:
 *
 *
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


#include <xlsys.h>

#include <index.h>
#include <datast.h>

using namespace std;
using namespace xlslib_core;


/*
******************************
CIndex class implementation
******************************
*/
CIndex::CIndex(CDataStorage &datastore, 
		   unsigned32_t firstrow, 
               unsigned32_t lastrow):
		CRecord(datastore)
{
#if defined(LEIGHTWEIGHT_UNIT_FEATURE)
	m_Backpatching_Level = 2;
#else
#endif

	SetRecordType(RECTYPE_INDEX);

	AddValue32(INDEX_DFLT_RESERVED);

	AddValue32(firstrow);
	AddValue32(lastrow+1);

	AddValue32(INDEX_DFLT_RESERVED);
	// NOTE: This record is created with an empty array. It should work if the rest
	// of the record is not completed later, since the record's size reflects the
	// lack of such array
	SetRecordLength(GetDataSize()-4);
}

CIndex::~CIndex()
{
}


/* 
**********************************
**********************************
*/
void CIndex::AddDBCellOffset(size_t dboffset)
{
   AddValue32((unsigned32_t)dboffset);

   SetRecordLength(GetDataSize()-4); // Update record's length
}

/* 
**********************************
**********************************
*/
void CIndex::SetRows(unsigned32_t firstrow,
                     unsigned32_t lastrow)
{
	SetValueAt32((unsigned32_t)firstrow, INDEX_OFFSET_B8FIRSTROW); // [i_a]
	SetValueAt32((unsigned32_t)(lastrow+1), INDEX_OFFSET_B8LASTROW); // [i_a]
}

/* 
**********************************
**********************************
*/

unsigned32_t CIndex::GetFirstRow(void)
{
	unsigned32_t retval;

	signed32_t firstrow;
	GetValue32From(&firstrow, INDEX_OFFSET_B8FIRSTROW); // [i_a]
	retval = firstrow;
   
	return retval;
}

/* 
**********************************
**********************************
*/

unsigned32_t CIndex::GetLastRow(void)
{
	unsigned32_t retval;
	
	signed32_t lastrow;
	GetValue32From(&lastrow, INDEX_OFFSET_B8LASTROW); // [i_a]
	retval = lastrow;

	return retval;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: index.cpp,v $
 * Revision 1.3  2009/01/08 02:52:59  dhoerl
 * December Rework
 *
 * Revision 1.2  2008/10/25 18:39:54  dhoerl
 * 2008
 *
 * Revision 1.1.1.1  2004/08/27 16:31:53  darioglz
 * Initial Import.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

