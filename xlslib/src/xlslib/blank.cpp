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
 * $Source: /cvsroot/xlslib/xlslib/src/xlslib/blank.cpp,v $
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

#include <xlsys.h>

#include <blank.h>
#include <datast.h>


using namespace std;
using namespace xlslib_core;

/*
*********************************
blank_t class implementation
*********************************
*/
blank_t::blank_t(CGlobalRecords& gRecords, unsigned32_t rowval, unsigned32_t colval, xf_t* pxfval) :
	cell_t(gRecords, rowval, colval, pxfval)
{
}
CUnit* blank_t::GetData(CDataStorage &datastore) const {
#if defined(LEIGHTWEIGHT_UNIT_FEATURE)
   return datastore.MakeCBlank(*this);	// NOTE: this pointer HAS to be deleted elsewhere.
#else
   return (CUnit*)(new CBlank(datastore, *this));	// NOTE: this pointer HAS to be deleted elsewhere.
#endif
}
blank_t::~blank_t()
{
}

/*
*********************************
CBlank class implementation
*********************************
*/
CBlank::CBlank(CDataStorage &datastore, const blank_t& blankdef):
		CRecord(datastore)
{
   SetRecordType(RECTYPE_BLANK);
   AddValue16((unsigned16_t)blankdef.GetRow());
   AddValue16((unsigned16_t)blankdef.GetCol());
   AddValue16(blankdef.GetXFIndex());

   SetRecordLength(GetDataSize()-4);
}

CBlank::~CBlank()
{
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: blank.cpp,v $
 * Revision 1.4  2009/03/02 04:08:43  dhoerl
 * Code is now compliant to gcc  -Weffc++
 *
 * Revision 1.3  2009/01/08 02:53:15  dhoerl
 * December Rework
 *
 * Revision 1.2  2008/10/25 18:39:53  dhoerl
 * 2008
 *
 * Revision 1.1.1.1  2004/08/27 16:31:54  darioglz
 * Initial Import.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

