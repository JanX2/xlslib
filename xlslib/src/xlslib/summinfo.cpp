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
 * $Source: /cvsroot/xlslib/xlslib/src/xlslib/summinfo.cpp,v $
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

#include <time.h>
#include <summinfo.h>

using namespace std;
using namespace xlslib_core;

const signed32_t xlslib_core::property2summary[] = {
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

#if defined(LEIGHTWEIGHT_UNIT_FEATURE)
	hpsf = MakeHPSFdoc(HPSF_SUMMARY);
#else
	hpsf = new HPSFdoc(*this, HPSF_SUMMARY);
#endif

	msTime = hpsf->unix2mstime(time(NULL));

	hpsf->addItem(SumInfo_Unknown, (unsigned16_t)1200);					// Excel 2004 on Mac writes this	// 0xfde9
	hpsf->addItem(SumInfo_CreateTime_Date, msTime);						// should be "right now"
	hpsf->addItem(SumInfo_LastSavedTime_Date, msTime);					// should be "right now"
	hpsf->addItem(SumInfo_Security, (unsigned32_t)0);					// Default
	hpsf->addItem(SumInfo_NameofCreatingApplication, s = PACKAGE_NAME);	// Default
}

CSummaryInfo::~CSummaryInfo()
{
#if 0 /* hpsf gets deleted from within the CDataStorage destructor as it is part of the m_FlushList. */
	if(hpsf) 
		delete hpsf;
#endif
}

/*
***********************************
***********************************
*/
bool CSummaryInfo::property(property_t prop, const string& content) {
	unsigned16_t val;
	
	val = static_cast<unsigned16_t>(property2summary[prop]);

	hpsf->addItem(val, content);
	return true;
}

/*
***********************************
***********************************
*/
void CSummaryInfo::DumpData(void)
{
   XTRACE("\tCSummaryInfo::DumpData");

#if 1
   	hpsf->DumpData();
	(*this) += hpsf;
	// hpsf = NULL;	// DataStore owns it now
#else
	CUnit* ptraildata = new CUnit;
	ptraildata->AddDataArray(CSummaryInfo::summ_info_data, sizeof(CSummaryInfo::summ_info_data));
	(*this) += ptraildata;
#endif
}
/*
***********************************
***********************************
*/


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

