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
 * $Source: /cvsroot/xlslib/xlslib/src/xlslib/number.cpp,v $
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

#include <number.h>
#include <datast.h>


using namespace std;
using namespace xlslib_core;

/*
*********************************
number_t class implementation
*********************************
*/
number_t::number_t(CGlobalRecords& gRecords, 
		unsigned32_t rowval, 
		unsigned32_t colval, 
		double numval, 
		xf_t* pxfval) :
	cell_t(gRecords, rowval, colval),
	isDouble(true),
	num()
{
	num.dblNum = numval;

	SetXF(pxfval);
}
number_t::number_t(CGlobalRecords& gRecords, 
		unsigned32_t rowval, 
		unsigned32_t colval, 
		signed32_t numval, 
		xf_t* pxfval) :
	cell_t(gRecords, rowval, colval),
	isDouble(false),
	num()
{
	// 536870911 >= numval >= -536870912
	if(numval <= 0x1FFFFFFF && numval >= 0xE0000000) {
		num.intNum = numval;
	} else {
		isDouble	= true;
		num.dblNum	= (double)numval;	// original value
	}

	SetXF(pxfval);
}

CUnit* number_t::GetData(CDataStorage &datastore) const
{
#if defined(LEIGHTWEIGHT_UNIT_FEATURE)
	return datastore.MakeCNumber(*this);
#else
	return (CUnit*)(new CNumber(datastore, *this));
#endif
}

/*
*********************************
number_t class implementation
*********************************
*/
CNumber::CNumber(CDataStorage &datastore, const number_t& numdef):
		CRecord(datastore)
{
	unsigned16_t type;

	type = numdef.isDouble ? RECTYPE_NUMBER : RECTYPE_RK;

	SetRecordType(type);

	AddValue16((unsigned16_t)numdef.row);
	AddValue16((unsigned16_t)numdef.col);
	AddValue16(numdef.GetXFIndex());

	if(type == RECTYPE_RK) {
		unsigned32_t val;

		val = (unsigned32_t)numdef.num.intNum << 2;		// lower two bits for flags
		val |= 0x2;										// Integral type
		AddValue32(val);
		//cerr << "RK: " <<  numdef.num.intNum << " (" << hex << val << ") " << dec << endl << flush;
	} else {
		AddValue64((unsigned64_t*)&numdef.num.dblNum);
		//cerr << "DBL: val=" << numdef.num.dblNum << endl << flush;
	}

	SetRecordLength(GetDataSize()-4);
}
CNumber::~CNumber()
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: number.cpp,v $
 * Revision 1.5  2009/03/02 04:08:43  dhoerl
 * Code is now compliant to gcc  -Weffc++
 *
 * Revision 1.4  2009/01/23 16:09:55  dhoerl
 * General cleanup: headers and includes. Fixed issues building mainC and mainCPP
 *
 * Revision 1.3  2009/01/08 02:52:47  dhoerl
 * December Rework
 *
 * Revision 1.2  2008/10/25 18:39:54  dhoerl
 * 2008
 *
 * Revision 1.1.1.1  2004/08/27 16:31:55  darioglz
 * Initial Import.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

