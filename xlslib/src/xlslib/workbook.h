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
 * $Source: /cvsroot/xlslib/xlslib/src/xlslib/workbook.h,v $
 * $Revision: 1.9 $
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

#ifndef WORKBOOK_H
#define WORKBOOK_H 

#include <config.h>

#include <common.h>
#include <oledoc.h>
#include <datast.h>
#include <sheetrec.h>
#include <recdef.h>
#include <continue.h>
#include <summinfo.h>
#include <docsumminfo.h>

#define XLSLIB_VERSION "2.0.0"


#include <xls_pshpack2.h>

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

  class workbook
    : public COleDoc
    {
	public:
		workbook();
		virtual ~workbook();
		
		const char*	version() const { return XLSLIB_VERSION; }

		worksheet*	sheet(const std::string& sheetname);
		worksheet*	sheet(const std::ustring& sheetname);
		worksheet*	GetSheet(unsigned16_t sheetnum);

		font_t*		font(unsigned8_t fontnum);			// use as a way to get a font to modify
		font_t*		font(const std::string& name);
		format_t*	format(const std::string& formatstr);
		format_t*	format(const std::ustring& formatstr);
		
		bool		setColor(unsigned8_t r, unsigned8_t g, unsigned8_t b, unsigned8_t idx);

		xf_t* xformat(void);
		xf_t* xformat(font_t* font);

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
		
		CSummaryInfo			m_SummaryInfo;
		CDocSummaryInfo			m_DocSummaryInfo;
		Sheets_Vector_t			m_Sheets;
		WorkbookDumpState_t		m_DumpState;
		WorkbookDumpState_t		m_PreviousDumpState;
		unsigned16_t			sheetIndex;

		CUnit*					m_pCurrentData;
		

		// Continue record variables:
		size_t			writeLen;
		CUnit*					m_pContinueRecord;
		unsigned16_t			m_ContinueIndex;
		
		unsigned16_t			current_sheet;
		size_t			offset;              
    };

}

#include <xls_poppack.h>

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


