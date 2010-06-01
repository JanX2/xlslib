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
 * $Source: /cvsroot/xlslib/xlslib/src/xlslib/format.h,v $
 * $Revision: 1.7 $
 * $Author: dhoerl $
 * $Date: 2009/01/23 16:09:55 $
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

#include <xlsys.h>
#include <common.h>
#include <rectypes.h>
#include <record.h>


#include <xls_pshpack2.h>

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
#define FMTCODE_HOUR5              0x002d
#define FMTCODE_HOUR6              0x002e
#define FMTCODE_HOUR7              0x002f
#define FMTCODE_SCIENTIFIC2        0x0030
#define FMTCODE_TEXT               0x0031

#define FMT_CODE_FIRST_USER			164

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

extern const unsigned16_t format2index[];

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
      format_t(u16string fmtstr);
      ~format_t(){};
   
    public:
      unsigned16_t GetIndex() const {return index;};
      void SetIndex(unsigned16_t idx) {index = idx;};
      
	  // good resource for format strings: http://www.mvps.org/dmcritchie/excel/formula.htm
      const u16string *GetFormatStr(void) const {return &formatstr;};
      void SetFormatStr(u16string& fmtstr) {formatstr = fmtstr;};

      bool GetIsASCII() const { return isASCII; };

    private:
      unsigned16_t index;
      u16string formatstr;
	  bool isASCII;
      //unsigned32_t m_usage_counter;    
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
      //CFormat(CDataStorage &datastore, u16string&  formatstr, bool isASCII, unsigned16_t index);
      CFormat(CDataStorage &datastore, const format_t* formatdef);
	private:
      virtual ~CFormat();

	public:

     // int SetFormatStr(u16string& formatstr);
      //int GetFormatStr(u16string& formatstr) const;

     // int SetIndex(unsigned16_t index);
     // unsigned16_t GetIndex(void) const;
    };

}


#include <xls_poppack.h>

#endif //FORMAT_H


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


