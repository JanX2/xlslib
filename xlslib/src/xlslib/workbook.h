/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * This file is part of xlslib -- A multiplatform, C/C++ library
 * for dynamic generation of Excel(TM) files.
 *
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

#ifndef WORKBOOK_H
#define WORKBOOK_H

#include "common/xlsys.h"
#include "common/systype.h"

// #include "common/xls_pshpack2.h"

#include "oledoc/oledoc.h"

#include "xlslib/common.h"
#include "xlslib/datast.h"
#include "xlslib/sheetrec.h"
#include "xlslib/recdef.h"
#include "xlslib/summinfo.h"
#include "xlslib/docsumminfo.h"
#include "xlslib/formula.h"

#include "xlslib/globalrec.h"
//111206 Added by RLN

#ifndef _MSC_VER
#  include "ac-config.win32.h"
// I am assuming this header file is created and include automatically by MSVC.
// Other compilers (I.e. BC++ ) don't have this, so I simply copied the file
// from the MSC project to the RadStudio project and included it. RLN 111208
#endif

#define XLSLIB_VERSION PACKAGE_VERSION 
// "2.0.0"

namespace xlslib_core
{

  typedef enum
    {
      WB_INIT,
      WB_GLOBALRECORDS,
      WB_SHEETS,
      WB_CONTINUE_REC,
      WB_FINISH
    } WorkbookDumpState_t;

  class CSummaryInfo;
  class CDocSummaryInfo;

  class workbook // : public COleDoc
    {
	public:
		workbook();
		virtual ~workbook();
		
		const char*	version() const { return XLSLIB_VERSION; }

		worksheet*	sheet(const std::string& sheetname);
		worksheet*	sheet(const std::ustring& sheetname);
		worksheet*	GetSheet(unsigned16_t sheetnum);

		expression_node_factory_t& GetFormulaFactory(void);

		font_t*		font(unsigned8_t fontnum);			// use as a way to get a font to modify
		font_t*		font(const std::string& name);
		format_t*	format(const std::string& formatstr);
		format_t*	format(const std::ustring& formatstr);
		
		bool		setColor(unsigned8_t r, unsigned8_t g, unsigned8_t b, unsigned8_t idx);

		xf_t* xformat(void);
		xf_t* xformat(font_t* font);
		xf_t* xformat(format_t *format);
		xf_t* xformat(font_t* font, format_t *format);

#ifdef HAVE_ICONV
		int iconvInType(const char *inType);
#endif

		bool property(property_t prop, const std::string& content);
		
		void windPosition(unsigned16_t horz, unsigned16_t vert);
		void windSize(unsigned16_t width, unsigned16_t height);
		void firstTab(unsigned16_t firstTab);
		void tabBarWidth(unsigned16_t width);

		int Dump(const std::string& filename);

	private:
		workbook(const workbook& that);
		workbook& operator=(const workbook& right);

    private:
		CUnit* DumpData(CDataStorage &datastore);	// oledoc use

    private:
		CGlobalRecords			m_GlobalRecords;
		expression_node_factory_t m_ExprFactory;
		
		CSummaryInfo			m_SummaryInfo;
		CDocSummaryInfo			m_DocSummaryInfo;
		Sheets_Vector_t			m_Sheets;
		WorkbookDumpState_t		m_DumpState;
		WorkbookDumpState_t		m_PreviousDumpState;
		unsigned16_t			sheetIndex;
		
		CUnit*					m_pCurrentData;

		// Continue record variables:
		CUnit*					m_pContinueRecord;
		size_t					m_ContinuesRealRecordSize;
		unsigned16_t			m_ContinueIndex;
		
		// INDEX / DBCELL assistant variables:
		size_t					writeLen;
		size_t					offset;              
		unsigned16_t			current_sheet;
		//size_t					Last_BOF_offset;
    };
}

// #include "common/xls_poppack.h"

#endif //WORKBOOK_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: workbook.h,v $
 * Revision 1.9  2009/03/02 04:08:43  dhoerl
 * Code is now compliant to gcc  -Weffc++
 *
 * Revision 1.8  2009/01/23 16:09:55  dhoerl
 * General cleanup: headers and includes. Fixed issues building mainC and mainCPP
 *
 * Revision 1.7  2009/01/10 21:10:51  dhoerl
 * More tweaks
 *
 * Revision 1.6  2009/01/08 22:16:06  dhoerl
 * January Rework
 *
 * Revision 1.5  2009/01/08 02:52:31  dhoerl
 * December Rework
 *
 * Revision 1.4  2008/10/27 01:12:20  dhoerl
 * Remove PHP
 *
 * Revision 1.3  2008/10/25 18:39:54  dhoerl
 * 2008
 *
 * Revision 1.2  2004/09/01 00:47:21  darioglz
 * + Modified to gain independence of target
 *
 * Revision 1.1.1.1  2004/08/27 16:31:44  darioglz
 * Initial Import.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


