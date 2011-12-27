/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * This file is part of xlslib -- A multiplatform, C/C++ library
 * for dynamic generation of Excel(TM) files.
 *
 * Copyright 2004 Yeico S. A. de C. V. All Rights Reserved.
 * Copyright 2008-2011 David Hoerl All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are
 * permitted provided that the following conditions are met:
 * 
 *    1. Redistributions of source code must retain the above copyright notice, this list of
 *       conditions and the following disclaimer.
 * 
 *    2. Redistributions in binary form must reproduce the above copyright notice, this list
 *       of conditions and the following disclaimer in the documentation and/or other materials
 *       provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY David Hoerl ''AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL David Hoerl OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * File description:
 *
 *
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "common/xlsys.h"

#include "xlslib/label.h"
#include "xlslib/globalrec.h"
#include "xlslib/datast.h"


using namespace std;
using namespace xlslib_core;

/*
******************************
label_t class implementation
******************************
*/

xlslib_core::label_t::label_t(CGlobalRecords& gRecords, 
		unsigned32_t rowval, unsigned32_t colval, const u16string& labelstrval, xf_t* pxfval) :
	cell_t(gRecords, rowval, colval, pxfval),
	strLabel(labelstrval),
	inSST(false)
{
	setType();
}

xlslib_core::label_t::label_t(CGlobalRecords& gRecords, 
							  unsigned32_t rowval, unsigned32_t colval, const std::string& labelstrval, xf_t* pxfval) :
cell_t(gRecords, rowval, colval, pxfval),
	strLabel(),
	inSST(false)
{
	gRecords.char2str16(labelstrval, strLabel);
	setType();
}

#ifndef __FRAMEWORK__
xlslib_core::label_t::label_t(CGlobalRecords& gRecords, 
							  unsigned32_t rowval, unsigned32_t colval, const std::ustring& labelstrval, xf_t* pxfval) :
	cell_t(gRecords, rowval, colval, pxfval),
	strLabel(),
	inSST(false)
{
	gRecords.wide2str16(labelstrval, strLabel);
	setType();
}
#endif

void xlslib_core::label_t::setType()
{
	if(strLabel.length() > 255) {
		inSST = true;
		m_GlobalRecords.AddLabelSST(*this);
	}
}

xlslib_core::label_t::~label_t()
{
	// suppose someone creates a SST string, then overwrites it or overwrites cell with something else.
	// Unlikely, but this protects against that case.
	if(inSST) {
		m_GlobalRecords.DeleteLabelSST(*this);
	}
}

/*
******************************
******************************
*/
size_t xlslib_core::label_t::GetSize(void) const
{
	size_t size;

	if(inSST) {
		size = 4 + 8;	// =2 + 2 + 2 + 2 + 4
	} else {
		size = 4 + 6;	// 4:type/size 2:row 2:col 2:xtnded 
		// 2:size 1:flag len:(ascii or not)
		size += (sizeof(unsigned16_t) + 1 + strLabel.length() * (CGlobalRecords::IsASCII(strLabel) ? sizeof(unsigned8_t) : sizeof(unsigned16_t)));
	}
	return size;
}
/*
******************************
******************************
*/
CUnit* xlslib_core::label_t::GetData(CDataStorage &datastore) const
{
	return datastore.MakeCLabel(*this);	// NOTE: this pointer HAS to be deleted elsewhere.
}

/*
******************************
CLabel class implementation
******************************
*/
CLabel::CLabel(CDataStorage &datastore, const label_t& labeldef):
		CRecord(datastore)
{
	SetRecordType(labeldef.GetInSST() ? RECTYPE_LABELSST : RECTYPE_LABEL);
	AddValue16((unsigned16_t)labeldef.GetRow());
	AddValue16((unsigned16_t)labeldef.GetCol());
	AddValue16(labeldef.GetXFIndex());
	if(labeldef.GetInSST()) {
		size_t index = labeldef.GetGlobalRecords().GetLabelSSTIndex(labeldef);
		AddValue32(index);
	} else {
		AddUnicodeString(labeldef.GetStrLabel(), LEN2_FLAGS_UNICODE);
	}
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

