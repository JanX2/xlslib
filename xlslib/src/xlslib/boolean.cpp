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
 * Copyright 2010 Ger Hobbelt
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * File description:
 *
 *
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <xlsys.h>

#include <boolean.h>
#include <datast.h>


using namespace xlslib_core;

/*
*********************************
boolean_t class implementation
*********************************
*/
boolean_t::boolean_t(CGlobalRecords& gRecords, unsigned32_t rowval, unsigned32_t colval, bool value, xf_t* pxfval) :
	cell_t(gRecords, rowval, colval, pxfval)
{
   num = value;
}

CUnit* boolean_t::GetData(CDataStorage &datastore) const {
#if defined(LEIGHTWEIGHT_UNIT_FEATURE)
   return datastore.MakeCBoolean(*this);	// NOTE: this pointer HAS to be deleted elsewhere.
#else
   return (CUnit*)(new CBoolean(datastore, *this));	// NOTE: this pointer HAS to be deleted elsewhere.
#endif
}

/*
*********************************
CBoolean class implementation
*********************************
*/

CBoolean::CBoolean(CDataStorage &datastore, const boolean_t& booldef):
		CRecord(datastore)
{
   SetRecordType(RECTYPE_BOOLERR);	
   AddValue16(booldef.GetRow());
   AddValue16(booldef.GetCol());
   AddValue16(booldef.GetXFIndex());
   AddValue8(booldef.GetBoolean());
   AddValue8(0);

   SetRecordLength(GetDataSize()-4);
}

CBoolean::~CBoolean()
{
}

