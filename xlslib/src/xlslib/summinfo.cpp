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

#include <time.h>
#include "xlslib/summinfo.h"

#if 0
//ndef _MSC_VER
#  include "ac-config.win32.h"
// I am assuming this header file is created and include automatically by MSVC.
// Other compilers (I.e. BC++ ) don't have this, so I simply copied the file
// from the MSC project to the RadStudio project and included it. RLN 111208
#endif


using namespace std;
using namespace xlslib_core;

const signed32_t xlslib_core::property2summary[] = 
{
	0,
	SumInfo_Author,
	-1,
	SumInfo_Comments,
	-1,
	SumInfo_NameofCreatingApplication,			// Does not seem to do anything
	SumInfo_Keywords,
	-1,
	SumInfo_RevisionNumber,
	SumInfo_Subject,
	SumInfo_Title
};

/*
**********************************************************************
CSummaryInfo class implementation
**********************************************************************
*/
CSummaryInfo::CSummaryInfo()
{
	unsigned64_t	msTime;
	string			s;
	
	XTRACE("WRITE_SUMMARY");

	hpsf = new hpsf_doc_t(HPSF_SUMMARY);
	if (hpsf)
	{
		msTime = hpsf->unix2mstime(time(NULL));

		hpsf->addItem(SumInfo_Unknown, (unsigned16_t)1200);					// Excel 2004 on Mac writes this	// 0xfde9
		hpsf->addItem(SumInfo_CreateTime_Date, msTime);						// should be "right now"
		hpsf->addItem(SumInfo_LastSavedTime_Date, msTime);					// should be "right now"
		hpsf->addItem(SumInfo_Security, (unsigned32_t)0);					// Default
		hpsf->addItem(SumInfo_NameofCreatingApplication, s = PACKAGE_NAME);	// Default
	}
}

CSummaryInfo::~CSummaryInfo()
{
	if(hpsf) 
		delete hpsf;
}

/*
***********************************
***********************************
*/
bool CSummaryInfo::property(property_t prop, const string& content) 
{
	unsigned16_t val;
	
	val = static_cast<unsigned16_t>(property2summary[prop]);

	hpsf->addItem(val, content);
	return true;
}

/*
***********************************
***********************************
*/
int CSummaryInfo::DumpData(CDataStorage &datastore)
{
   XTRACE("\tCSummaryInfo::DumpData");

   	CUnit* ret = hpsf->GetData(datastore);
	if (ret != NULL)
	{
		datastore += ret;
		// hpsf = NULL;	// DataStore owns it now
	}
	return NO_ERRORS;
}





/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: summinfo.cpp,v $
 * Revision 1.5  2009/03/02 04:08:43  dhoerl
 * Code is now compliant to gcc  -Weffc++
 *
 * Revision 1.4  2009/01/10 21:10:51  dhoerl
 * More tweaks
 *
 * Revision 1.3  2009/01/08 02:52:47  dhoerl
 * December Rework
 *
 * Revision 1.2  2008/10/25 18:39:54  dhoerl
 * 2008
 *
 * Revision 1.1.1.1  2004/08/27 16:31:57  darioglz
 * Initial Import.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

