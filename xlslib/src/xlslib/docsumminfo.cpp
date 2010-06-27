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
 * $Source: /cvsroot/xlslib/xlslib/src/xlslib/docsumminfo.cpp,v $
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

#include <summinfo.h>		// pseudo base class
#include <docsumminfo.h>

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
	
#if defined(LEIGHTWEIGHT_UNIT_FEATURE)
	hpsf = MakeHPSFdoc(HPSF_DOCSUMMARY);
#else
	hpsf = new HPSFdoc(*this, HPSF_DOCSUMMARY);
#endif

	hpsf->addItem(DocSumInfo_CodePage, (unsigned16_t)1200);				// UTF-16
}

CDocSummaryInfo::~CDocSummaryInfo()
{
#if defined(LEIGHTWEIGHT_UNIT_FEATURE)
	/* hpsf gets deleted from within the CDataStorage destructor as it is part of the m_FlushList. */
#else
#endif
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
int CDocSummaryInfo::DumpData(void)
{
   XTRACE("CDocSummaryInfo::DumpData");

   	int ret = hpsf->DumpData();
	if (ret == NO_ERRORS)
	{
		(*this) += hpsf;
	}
	return ret;
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

