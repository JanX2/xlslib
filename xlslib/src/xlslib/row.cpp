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
 * $Source: /cvsroot/xlslib/xlslib/src/xlslib/row.cpp,v $
 * $Revision: 1.3 $
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


#include <config.h>

#include <row.h>
#include <datast.h>

using namespace std;
using namespace xlslib_core;

/*
******************************
CRow class implementation
******************************
*/

CRow::CRow(CDataStorage &datastore, 
		   unsigned32_t rownum,  
           unsigned32_t firstcol,
           unsigned32_t lastcol, 
           unsigned16_t rowheight,
		   const xf_t* xformat):
		CRecord(datastore)
{
	SetRecordType(RECTYPE_ROW);
	AddValue16((unsigned16_t)rownum);
	AddValue16((unsigned16_t)firstcol);
	AddValue16((unsigned16_t)(lastcol+1));
	AddValue16(rowheight);

	// A field used by MS for "optimizing" (?) the loading of a file.
	// Doc says it shall be set to 0 if I'm creating a BIFF...
	AddValue16(0);
	// A reserved value:
	AddValue16(0);

	// TODO: The following flag-word can be used for outline cells.
	// As a default the GhostDirty flag is set, so the row has a default
	// format (set by the index of byte 18).
	if(rowheight == ROW_DFLT_HEIGHT)
	{
		AddValue16(ROW_DFLT_GRBIT/*|0x100*/); // [i_a] Excel2003 also sets bit 8: 0x100
	} else {
		AddValue16(ROW_DFLT_GRBIT|ROW_GRBIT_UNSYNC/*|0x100*/);   
	}
	if(xformat == NULL) {
		AddValue16(ROW_DFLT_IXFE);
	} else {
		AddValue16(xformat->GetIndex());
	}

	SetRecordLength(GetDataSize()-4);
}

#if 0
// NOTE: row_t has not a height field
CRow::CRow(row_t& rowdef)
{
	SetRecordType(RECTYPE_ROW);
	AddValue16(rowdef.rownum);
	AddValue16(rowdef.firstcol);
	AddValue16(rowdef.lastcol+1);
	AddValue16(ROW_DFLT_HEIGHT);

	// A field used by MS for "optimizing" (?) the loading of a file.
	// Doc says it shall be set to 0 if I'm creating a BIFF...
	AddValue16((unsigned16_t)0);
	// A reserved value:
	AddValue16((unsigned16_t)0);

	// TODO: The following flag-word can be used for outline cells.
	// As a default the GhostDirty flag is set, so the row has a default
	// format (set by the index of byte 18).
	AddValue16(ROW_DFLT_GRBIT|ROW_GRBIT_UNSYNC);
	if(rowdef.xformat == NULL) {
		AddValue16((unsigned16_t)ROW_DFLT_IXFE);
	} else {
		AddValue16((unsigned16_t)rowdef.xformat->GetIndex());
	}

	SetRecordLength(GetDataSize()-4);
}
#endif

CRow::~CRow()
{
}

#if 0
/*
******************************
******************************
*/
void CRow::SetFirstCol(unsigned32_t firstcol)
{
   SetValueAt16((unsigned16_t)firstcol, ROW_OFFSET_FIRSTCOL);
}

/*
******************************
******************************
*/
void CRow::SetLastCol(unsigned32_t lastcol)
{
   SetValueAt16((unsigned16_t)(lastcol+1), ROW_OFFSET_LASTCOL);
}
  
/*
******************************
******************************
*/
unsigned32_t CRow::GetFirstCol(void)
{

   signed16_t firstcol;
   GetValue16From(&firstcol,ROW_OFFSET_FIRSTCOL);
   return (unsigned16_t)firstcol;

}

/*
******************************
******************************
*/
unsigned32_t CRow::GetLastCol(void)
{
   signed16_t lastcol;
   GetValue16From(&lastcol,ROW_OFFSET_LASTCOL);
   return (unsigned16_t)lastcol-1;
}

#endif

/*
******************************
CDBCell class implementation
******************************
*/
CDBCell::CDBCell(CDataStorage &datastore, size_t startblock):
		CRecord(datastore)
{
#if defined(LEIGHTWEIGHT_UNIT_FEATURE)
   m_Backpatching_Level = 1;
#else
#endif

   // The new initializated DBCell record points to nowhere and has no 
   // extra rows (the array of stream offsets is empty);
   SetRecordType(RECTYPE_DBCELL);
   AddValue32((unsigned32_t) startblock);

   SetRecordLength(GetDataSize()-4);
}

CDBCell::~CDBCell()
{
}


void CDBCell::AddRowOffset(size_t rowoffset)
{    
   AddValue16((unsigned16_t) rowoffset);
   SetRecordLength(GetDataSize()-4);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: row.cpp,v $
 * Revision 1.3  2009/01/08 02:52:47  dhoerl
 * December Rework
 *
 * Revision 1.2  2008/10/25 18:39:54  dhoerl
 * 2008
 *
 * Revision 1.1.1.1  2004/08/27 16:31:53  darioglz
 * Initial Import.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

