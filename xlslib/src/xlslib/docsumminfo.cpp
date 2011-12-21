/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * This file is part of xlslib -- A multiplatform, C/C++ library
 * for dynamic generation of Excel(TM) files.
 *
 * Copyright 2004 Yeico S. A. de C. V. All Rights Reserved.
 * Copyright 2008 David Hoerl All Rights Reserved.
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

#include "xlslib/summinfo.h"		// pseudo base class
#include "xlslib/docsumminfo.h"

using namespace std;
using namespace xlslib_core;

const signed32_t xlslib_core::property2docSummary[] = {
	-1,	// 0
	-1,
	DocSumInfo_Category,
	-1,
	DocSumInfo_Company,
	-1,
	-1,
	DocSumInfo_Manager,
	-1,
	-1,
	-1
};

/*
**********************************************************************
CDocSummaryInfo class implementation
**********************************************************************
*/

CDocSummaryInfo::CDocSummaryInfo()
{
	XTRACE("WRITE_DOC_SUMMARY");
	
	hpsf = new hpsf_doc_t(HPSF_DOCSUMMARY);
	if (hpsf)
	{
		hpsf->addItem(DocSumInfo_CodePage, (unsigned16_t)1200);				// UTF-16
	}
}

CDocSummaryInfo::~CDocSummaryInfo()
{
	if(hpsf) 
		delete hpsf;
}

/*
***********************************
***********************************
*/
bool CDocSummaryInfo::property(property_t prop, const string& content)
{
	signed32_t	val;
	
	val = property2docSummary[prop];
	XL_ASSERT(val > 0);
	hpsf->addItem((unsigned16_t)val, content);
	return true;
}


/*
***********************************
***********************************
*/
int CDocSummaryInfo::DumpData(CDataStorage &datastore)
{
   XTRACE("CDocSummaryInfo::DumpData");

   	CUnit* ret = hpsf->GetData(datastore);
	if (ret != NULL)
	{
		datastore += ret;
		// hpsf = NULL;	// DataStore owns it now
	}
	return NO_ERRORS;
}




/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: docsumminfo.cpp,v $
 * Revision 1.5  2009/03/02 04:08:43  dhoerl
 * Code is now compliant to gcc  -Weffc++
 *
 * Revision 1.4  2009/01/10 21:10:50  dhoerl
 * More tweaks
 *
 * Revision 1.3  2009/01/08 02:53:15  dhoerl
 * December Rework
 *
 * Revision 1.2  2008/10/25 18:39:54  dhoerl
 * 2008
 *
 * Revision 1.1.1.1  2004/08/27 16:31:43  darioglz
 * Initial Import.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

