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


#ifndef FORMAT_H
#define FORMAT_H

#include "common/xlsys.h"
#include "common/systype.h"

#include "xlslib/common.h"
#include "xlslib/rectypes.h"
#include "xlslib/record.h"


// #include "common/xls_pshpack2.h"

namespace xlslib_core
{

  // The font-record field offsets:
#define FORMAT_OFFSET_INDEX         4
#define FORMAT_OFFSET_NAMELENGTH    6
#define FORMAT_OFFSET_NAME          7

#define FMTCODE_GENERAL            0x0000
#define FMTCODE_NUMBER1            0x0001
#define FMTCODE_NUMBER2            0x0002
#define FMTCODE_NUMBER3            0x0003
#define FMTCODE_NUMBER4            0x0004
#define FMTCODE_CURRENCY1          0x0005 
#define FMTCODE_CURRENCY2          0x0006 
#define FMTCODE_CURRENCY3          0x0007 
#define FMTCODE_CURRENCY4          0x0008 
// CURRENCY ends up 'customized' in XLS
#define FMTCODE_PERCENT1           0x0009
#define FMTCODE_PERCENT2           0x000a
#define FMTCODE_SCIENTIFIC1        0x000b
#define FMTCODE_FRACTION1          0x000c
#define FMTCODE_FRACTION2          0x000d
#define FMTCODE_DATE1              0x000e
#define FMTCODE_DATE2              0x000f
#define FMTCODE_DATE3              0x0010
#define FMTCODE_DATE4              0x0011
#define FMTCODE_HOUR1              0x0012
#define FMTCODE_HOUR2              0x0013
#define FMTCODE_HOUR3              0x0014
#define FMTCODE_HOUR4              0x0015
#define FMTCODE_HOURDATE           0x0016
#define FMTCODE_ACCOUNTING1        0x0025
#define FMTCODE_ACCOUNTING2        0x0026
#define FMTCODE_ACCOUNTING3        0x0027
#define FMTCODE_ACCOUNTING4        0x0028
#define FMTCODE_CURRENCY5          0x0029 
#define FMTCODE_CURRENCY6          0x002a 
#define FMTCODE_CURRENCY7          0x002b 
#define FMTCODE_CURRENCY8          0x002c 
// CURRENCY ends up 'customized' in XLS
#define FMTCODE_HOUR5              0x002d
#define FMTCODE_HOUR6              0x002e
#define FMTCODE_HOUR7              0x002f
#define FMTCODE_SCIENTIFIC2        0x0030
#define FMTCODE_TEXT               0x0031

#define FMT_CODE_FIRST_USER			164 /* 0xA4 - the first index used by Excel2003 for any user defined format */

// good resource for format strings: http://www.mvps.org/dmcritchie/excel/formula.htm
// Good explanation of custom formats: http://www.ozgrid.com/Excel/CustomFormats.htm
// MS examples (need Windows): http://download.microsoft.com/download/excel97win/sample/1.0/WIN98Me/EN-US/Nmbrfrmt.exe
// Google this for MS help: "Create or delete a custom number format"
typedef enum
{
  FMT_GENERAL = 0,
  FMT_NUMBER1,					// 0
  FMT_NUMBER2,					// 0.00
  FMT_NUMBER3,					// #,##0
  FMT_NUMBER4,					// #,##0.00
  FMT_CURRENCY1,				// "$"#,##0_);("$"#,##0)
  FMT_CURRENCY2,				// "$"#,##0_);[Red]("$"#,##0)
  FMT_CURRENCY3,				// "$"#,##0.00_);("$"#,##0.00)
  FMT_CURRENCY4,				// "$"#,##0.00_);[Red]("$"#,##0.00)
  FMT_PERCENT1,					// 0%
  FMT_PERCENT2,					// 0.00%
  FMT_SCIENTIFIC1,				// 0.00E+00
  FMT_FRACTION1,				// # ?/?
  FMT_FRACTION2,				// # ??/??
  FMT_DATE1,					// M/D/YY
  FMT_DATE2,					// D-MMM-YY
  FMT_DATE3,					// D-MMM
  FMT_DATE4,					// MMM-YY
  FMT_TIME1,					// h:mm AM/PM
  FMT_TIME2,					// h:mm:ss AM/PM
  FMT_TIME3,					// h:mm
  FMT_TIME4,					// h:mm:ss
  FMT_DATETIME,					// M/D/YY h:mm
  FMT_ACCOUNTING1,				// _(#,##0_);(#,##0)
  FMT_ACCOUNTING2,				// _(#,##0_);[Red](#,##0)
  FMT_ACCOUNTING3,				// _(#,##0.00_);(#,##0.00)
  FMT_ACCOUNTING4,				// _(#,##0.00_);[Red](#,##0.00)
  FMT_CURRENCY5,				// _("$"* #,##0_);_("$"* (#,##0);_("$"* "-"_);_(@_)
  FMT_CURRENCY6,				// _(* #,##0_);_(* (#,##0);_(* "-"_);_(@_)
  FMT_CURRENCY7,				// _("$"* #,##0.00_);_("$"* (#,##0.00);_("$"* "-"??_);_(@_)
  FMT_CURRENCY8,				// _(* #,##0.00_);_(* (#,##0.00);_(* "-"??_);_(@_)
  FMT_TIME5,					// mm:ss
  FMT_TIME6,					// [h]:mm:ss
  FMT_TIME7,					// mm:ss.0
  FMT_SCIENTIFIC2,				// ##0.0E+0
  FMT_TEXT          			// @
} format_number_t;


class CGlobalRecords;

/* 
******************************
CFormat class declaration
******************************
*/
class format_t
{
	  friend class workbook;
	  friend class CGlobalRecords;

    private:
  	  format_t(const format_t& orig);
	  format_t(CGlobalRecords& gRecords, const std::string& fmtstr);
	  format_t(CGlobalRecords& gRecords, const std::ustring& fmtstr);
#ifndef __FRAMEWORK__
      format_t(CGlobalRecords& gRecords, const u16string& fmtstr);
#endif
      virtual ~format_t(){};
	  /* MSVC2005: C4512: 'xlslib_core::format_t' : assignment operator could not be generated */
	  format_t &operator =(const format_t &src);
   
    public:
		static format_t* formatDup(const format_t* orig)
		{
			format_t* fmt = new format_t(*orig);
			return fmt;
		}
		void MarkUsed();
		void UnMarkUsed();
		unsigned32_t Usage() const;

	  unsigned16_t GetIndex() const {return index;};
      void SetIndex(unsigned16_t idx) {index = idx;};
      
	  // good resource for format strings: http://www.mvps.org/dmcritchie/excel/formula.htm
      const u16string& GetFormatStr(void) const {return formatstr;};
      void SetFormatStr(const u16string& fmtstr) {formatstr = fmtstr;};

	public:
		static unsigned16_t format2index(format_number_t idx);

    private:
      u16string formatstr;
      unsigned16_t index;
      unsigned32_t m_usage_counter;    
	  
	  CGlobalRecords& m_GlobalRecords;

	public:
		CGlobalRecords& GetGlobalRecords(void) const { return m_GlobalRecords; }; 
  };
	
  typedef std::vector<xlslib_core::format_t* XLSLIB_DFLT_ALLOCATOR> Format_Vect_t;
  typedef Format_Vect_t::iterator Format_Vect_Itor_t;


	// forward ref
	class CDataStorage;

  class CFormat: public CRecord
    {
#if defined(LEIGHTWEIGHT_UNIT_FEATURE)
	friend class CDataStorage;
#endif

    protected:
      CFormat(CDataStorage &datastore, const format_t* formatdef);
	private:
      virtual ~CFormat();
    };

}


// #include "common/xls_poppack.h"

#endif 
//FORMAT_H


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: format.h,v $
 * Revision 1.7  2009/01/23 16:09:55  dhoerl
 * General cleanup: headers and includes. Fixed issues building mainC and mainCPP
 *
 * Revision 1.6  2009/01/10 21:10:50  dhoerl
 * More tweaks
 *
 * Revision 1.5  2009/01/08 02:53:15  dhoerl
 * December Rework
 *
 * Revision 1.4  2008/12/10 03:34:22  dhoerl
 * m_usage was 16bit and wrapped
 *
 * Revision 1.3  2008/12/06 01:42:57  dhoerl
 * John Peterson changes along with lots of tweaks. Many bugs that causes Excel crashes fixed.
 *
 * Revision 1.2  2008/10/25 18:39:54  dhoerl
 * 2008
 *
 * Revision 1.1.1.1  2004/08/27 16:31:57  darioglz
 * Initial Import.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


