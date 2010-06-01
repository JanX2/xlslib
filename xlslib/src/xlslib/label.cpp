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
 * $Source: /cvsroot/xlslib/xlslib/src/xlslib/label.cpp,v $
 * $Revision: 1.7 $
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

#include <label.h>
#include <datast.h>


using namespace std;
using namespace xlslib_core;

/*
******************************
label_t class implementation
******************************
*/
xlslib_core::label_t::label_t(CGlobalRecords& gRecords, 
		unsigned32_t rowval, unsigned32_t colval, const u16string& labelstrval, xf_t* pxfval) :
	cell_t(gRecords, rowval, colval),
	strLabel(),
	isASCII(true)
{
	u16string::const_iterator cBegin, cEnd;
	size_t	len;
	
	SetXF(pxfval);

	len = labelstrval.length();
	strLabel.reserve(len);

	cBegin = labelstrval.begin();
	cEnd = labelstrval.end();
	
	while(cBegin != cEnd) 
	{
		unsigned16_t	c;
		
		c = *cBegin++;
		if(c > 0x7F) isASCII = false;

		strLabel.push_back(c);		
	}
}


xlslib_core::label_t::~label_t()
{
}

/*
******************************
******************************
*/
size_t xlslib_core::label_t::GetSize(void) const
{
	size_t size = 0;

	size = 10;		// empty Unicode string has a flags byte
	size += 
      (sizeof(unsigned16_t) + 1 + strLabel.size() * (isASCII ? sizeof(unsigned8_t) : sizeof(unsigned16_t)));
	  
	return size;
}
/*
******************************
******************************
*/
CUnit* xlslib_core::label_t::GetData(CDataStorage &datastore) const
{
#if defined(LEIGHTWEIGHT_UNIT_FEATURE)
	return datastore.MakeCLabel(*this);	// NOTE: this pointer HAS to be deleted elsewhere.
#else
	return (CUnit*)( new CLabel(datastore, *this));	// NOTE: this pointer HAS to be deleted elsewhere.
#endif
}

/*
******************************
CLabel class implementation
******************************
*/
#if 0
CLabel::CLabel(CDataStorage &datastore, 
		   unsigned32_t row,	  
	       unsigned32_t col,	  
	       const u16string& strlabel,
		   bool isASCII,
		   const xf_t* pxfval):
		CRecord(datastore)
{
	unsigned16_t xfindex;

	SetRecordType(RECTYPE_LABEL);
	AddValue16((unsigned16_t)row);
	AddValue16((unsigned16_t)col);

	if(pxfval != NULL)
		xfindex = pxfval->GetIndex();
	else
		xfindex = XF_PROP_XF_DEFAULT_CELL;

	AddValue16(xfindex);	  

	AddUnicodeString(&strlabel, sizeof(unsigned16_t), isASCII);

	SetRecordLength(GetDataSize()-4);
}
#endif

CLabel::CLabel(CDataStorage &datastore, const label_t& labeldef):
		CRecord(datastore)
{
	SetRecordType(RECTYPE_LABEL);
	AddValue16((unsigned16_t)labeldef.GetRow());
	AddValue16((unsigned16_t)labeldef.GetCol());
	AddValue16(labeldef.GetXFIndex());

	AddUnicodeString(labeldef.GetStrLabel(), sizeof(unsigned16_t), labeldef.GetIsASCII());

	SetRecordLength(GetDataSize()-4);
}

CLabel::~CLabel()
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: label.cpp,v $
 * Revision 1.7  2009/03/02 04:08:43  dhoerl
 * Code is now compliant to gcc  -Weffc++
 *
 * Revision 1.6  2009/01/10 21:10:50  dhoerl
 * More tweaks
 *
 * Revision 1.5  2009/01/08 02:52:47  dhoerl
 * December Rework
 *
 * Revision 1.4  2008/12/20 15:49:05  dhoerl
 * 1.2.5 fixes
 *
 * Revision 1.3  2008/12/06 01:42:57  dhoerl
 * John Peterson changes along with lots of tweaks. Many bugs that causes Excel crashes fixed.
 *
 * Revision 1.2  2008/10/25 18:39:54  dhoerl
 * 2008
 *
 * Revision 1.1.1.1  2004/08/27 16:31:53  darioglz
 * Initial Import.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

