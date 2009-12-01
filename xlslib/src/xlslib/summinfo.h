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
 * $Source: /cvsroot/xlslib/xlslib/src/xlslib/summinfo.h,v $
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



#ifndef SUMMARYINFO_H
#define SUMMARYINFO_H 

#include <config.h>
#include <common.h>
#include <datast.h>
#include <HPSF.h>



#include <xls_pshpack2.h>

// all of these defined, not all used
#define SumInfo_Unknown						 1
#define SumInfo_Title						 2
#define SumInfo_Subject						 3
#define SumInfo_Author						 4
#define SumInfo_Keywords					 5
#define SumInfo_Comments					 6
#define SumInfo_Template					 7
#define SumInfo_LastSavedBy					 8
#define SumInfo_RevisionNumber				 9
#define SumInfo_TotalEditingTime			10
#define SumInfo_LastPrinted					11
#define SumInfo_CreateTime_Date				12
#define SumInfo_LastSavedTime_Date			13
#define SumInfo_NumberofPages				14
#define SumInfo_NumberofWords				15
#define SumInfo_NumberofCharacters			16
#define SumInfo_Thumbnail					17
#define SumInfo_NameofCreatingApplication	18
#define SumInfo_Security					19
#define		READONLY_RECOMMENDED			0x02
#define		READONLY_ENFORCED				0x04
#define SumInfo_Max							SumInfo_Security

namespace xlslib_core
{
	typedef enum {
		PROP_AUTHOR = 1,
		PROP_CATEGORY,
		PROP_COMMENTS,
		PROP_COMPANY,
		PROP_CREATINGAPPLICATION,	// Cannot see anywhere this is displayed
		PROP_KEYWORDS,
		PROP_MANAGER,
		PROP_REVISION,
		PROP_SUBJECT,
		PROP_TITLE,
		
		PROP_LAST
	} property_t;

	extern const signed32_t property2summary[];

/*
********************************
CSummaryInfo class declaration
********************************
*/
  class CSummaryInfo: public CDataStorage
	{
	private:
		static const  unsigned8_t summ_info_data[];
		HPSFdoc		*hpsf;

	public:
		CSummaryInfo();
		virtual ~CSummaryInfo();
		
		bool property(property_t prop, const std::string& content);
		void DumpData();

	private:
		CSummaryInfo(const CSummaryInfo& that);
		CSummaryInfo& operator=(const CSummaryInfo& right);
    };
}

#include <xls_poppack.h>

#endif


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: summinfo.h,v $
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
 * Revision 1.1.1.1  2004/08/27 16:31:49  darioglz
 * Initial Import.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

