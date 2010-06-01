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
 * $Source: /cvsroot/xlslib/xlslib/src/xlslib/format.cpp,v $
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


#include <xlsys.h>

#include <format.h>

using namespace std;
using namespace xlslib_core;

const unsigned16_t xlslib_core::format2index[] =
{
   FMTCODE_GENERAL,
   FMTCODE_NUMBER1,
   FMTCODE_NUMBER2,
   FMTCODE_NUMBER3,
   FMTCODE_NUMBER4,
   FMTCODE_CURRENCY1,
   FMTCODE_CURRENCY2,
   FMTCODE_CURRENCY3,
   FMTCODE_CURRENCY4,
   FMTCODE_PERCENT1,
   FMTCODE_PERCENT2,
   FMTCODE_SCIENTIFIC1,
   FMTCODE_FRACTION1,
   FMTCODE_FRACTION2,
   FMTCODE_DATE1,
   FMTCODE_DATE2,
   FMTCODE_DATE3,
   FMTCODE_DATE4,
   FMTCODE_HOUR1,
   FMTCODE_HOUR2,
   FMTCODE_HOUR3,
   FMTCODE_HOUR4,
   FMTCODE_HOURDATE,
   FMTCODE_ACCOUNTING1,
   FMTCODE_ACCOUNTING2,
   FMTCODE_ACCOUNTING3,
   FMTCODE_ACCOUNTING4,
   FMTCODE_CURRENCY5,
   FMTCODE_CURRENCY6,
   FMTCODE_CURRENCY7,
   FMTCODE_CURRENCY8,
   FMTCODE_HOUR5,
   FMTCODE_HOUR6,
   FMTCODE_HOUR7,
   FMTCODE_SCIENTIFIC2,
   FMTCODE_TEXT
};

/* 
**********************************
CFormat class implementation
**********************************
*/
#if 0
CFormat::CFormat(CDataStorage &datastore, 
		   u16string&  formatstr, bool isASCII, unsigned16_t index):
		CRecord(datastore)
{
   SetRecordType(RECTYPE_FORMAT);

   AddValue16(index);
   AddUnicodeString(&formatstr, sizeof(unsigned16_t), isASCII);
    
   SetRecordLength(GetDataSize()-4);
}
#endif
CFormat::CFormat(CDataStorage &datastore, const format_t* formatdef):
		CRecord(datastore)
{
	SetRecordType(RECTYPE_FORMAT);

	AddValue16(formatdef->GetIndex());
	//cerr << "Format: index=" << formatdef->GetIndex() << endl << flush;

	AddUnicodeString(formatdef->GetFormatStr(), sizeof(unsigned16_t), formatdef->GetIsASCII());

	SetRecordLength(GetDataSize()-4);
}

CFormat::~CFormat()
{
}

#if 0	// if you ever need this, account for flag bit, also, is the string ascii or unicode
/* 
**********************************
**********************************
*/
int CFormat::GetFormatStr(u16string& formatstr) const
{
   int errcode = NO_ERRORS;

   signed16_t formatsize;
   GetValue16From(&formatsize, FORMAT_OFFSET_NAMELENGTH);

   formatstr.resize(0);
   formatstr.reserve(formatsize);

   for(int i=0; i<formatsize; i++)
      formatstr += operator[](FORMAT_OFFSET_NAME+i);

   return errcode;
}
/* 
**********************************
**********************************
*/
int CFormat::SetIndex(unsigned16_t index)
{
   return (SetValueAt16((unsigned16_t)index, FORMAT_OFFSET_INDEX));
}

/* 
**********************************
**********************************
*/
unsigned16_t CFormat::GetIndex(void) const
{
   unsigned16_t indexval;
   GetValue16From((signed16_t*)&indexval, FORMAT_OFFSET_INDEX);

   return(indexval);
}
#endif



format_t::format_t(u16string fmtstr) :
	index(0),
	formatstr(),
	isASCII(true)
{
	u16string::const_iterator	cBegin, cEnd;
	size_t	len;

	len = fmtstr.length();
	formatstr.reserve(len);

	cBegin	= fmtstr.begin();
	cEnd	= fmtstr.end();
	
	while(cBegin != cEnd) {
		unsigned16_t	c;
		
		c = *cBegin++;
		if(c > 0x7F) isASCII = false;

		formatstr.push_back(c);		
	}
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: format.cpp,v $
 * Revision 1.5  2009/03/02 04:08:43  dhoerl
 * Code is now compliant to gcc  -Weffc++
 *
 * Revision 1.4  2009/01/23 16:09:55  dhoerl
 * General cleanup: headers and includes. Fixed issues building mainC and mainCPP
 *
 * Revision 1.3  2009/01/08 02:53:15  dhoerl
 * December Rework
 *
 * Revision 1.2  2008/10/25 18:39:54  dhoerl
 * 2008
 *
 * Revision 1.1.1.1  2004/08/27 16:31:48  darioglz
 * Initial Import.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

