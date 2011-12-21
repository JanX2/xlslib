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


#ifndef DOCSUMMARYINFO_H
#define DOCSUMMARYINFO_H

#include "common/xlsys.h"
#include "common/systype.h"

#include "xlslib/common.h"
#include "xlslib/datast.h"
#include "xlslib/HPSF.h"


// All known fields, but not all used
#define DocSumInfo_Dictionary				 0
#define DocSumInfo_CodePage					 1
#define DocSumInfo_Category					 2
#define DocSumInfo_PresentationTarget		 3
#define DocSumInfo_Bytes					 4
#define DocSumInfo_Lines					 5
#define DocSumInfo_Paragraphs				 6
#define DocSumInfo_Slides					 7
#define DocSumInfo_Notes					 8
#define DocSumInfo_HiddenSlides				 9
#define DocSumInfo_MMClips					10
#define DocSumInfo_ScaleCrop				11
#define DocSumInfo_HeadingPairs				12
#define DocSumInfo_TitlesofParts			13
#define DocSumInfo_Manager					14
#define DocSumInfo_Company					15
#define DocSumInfo_LinksUpToDate			16
#define DocSumInfo_Max						DocSumInfo_LinksUpToDate


// #include "common/xls_pshpack2.h"

namespace xlslib_core
{

	extern const signed32_t property2docSummary[];

/*
********************************
CDocSummaryInfo class declaration
********************************
*/

  class CDocSummaryInfo //: public CDataStorage
	{
    private:
		static const unsigned8_t doc_summ_info_data[];
		hpsf_doc_t *hpsf;

    public:
		CDocSummaryInfo();
		virtual ~CDocSummaryInfo();
		
		bool property(property_t prop, const std::string& content);

		int DumpData(CDataStorage &datastore);

	private:
		CDocSummaryInfo(const CDocSummaryInfo& that);
		CDocSummaryInfo& operator=(const CDocSummaryInfo& right);
    };

}

// #include "common/xls_poppack.h"

#endif //DOCSUMMARYINFO_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: docsumminfo.h,v $
 * Revision 1.4  2009/03/02 04:08:43  dhoerl
 * Code is now compliant to gcc  -Weffc++
 *
 * Revision 1.3  2009/01/08 02:53:15  dhoerl
 * December Rework
 *
 * Revision 1.2  2008/10/25 18:39:54  dhoerl
 * 2008
 *
 * Revision 1.1.1.1  2004/08/27 16:31:54  darioglz
 * Initial Import.
 *
 * Revision 1.6  2004/04/06 22:35:43  dgonzalez
 * + Added namespace xlslib_core to isolate the core xlslib library names.
 *
 * Revision 1.5  2004/01/29 03:18:55  dgonzalez
 * + Using the config.h file
 *
 * Revision 1.4  2003/12/05 00:30:06  dgonzalez
 * + Common include files were placed in common.h (and this file was
 *     included instead.
 *
 * Revision 1.3  2003/11/26 16:49:26  dgonzalez
 * + Indented and untabified
 *
 * Revision 1.2  2003/10/24 23:24:32  dgonzalez
 * + Added CVS-Keyword substitution.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
