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
 * $Source: /cvsroot/xlslib/xlslib/src/xlslib/globalrec.h,v $
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

#ifndef GLOBALREC_H
#define GLOBALREC_H

#include <xlsys.h>
#include <common.h>
#include <biffsection.h>
#include <font.h>
#include <format.h>
#include <extformat.h>
#include <recdef.h>
#include <unit.h>


#include <xls_pshpack2.h>

namespace xlslib_core
{
/*
***********************************
CGlobalRecords class declaration
***********************************
*/

#define GLOBAL_NUM_DEFAULT_FONT    5
#define GLOBAL_NUM_DEFAULT_FORMATS 8
#define GLOBAL_NUM_DEFAULT_XFS     16
#define GLOBAL_NUM_DEFAULT_STYLES  6

  class xf_t;
  
  typedef enum
    {
      GLOBAL_INIT = 0,
      GLOBAL_BOF,
      GLOBAL_CODEPAGE,
      GLOBAL_WINDOW1,
      GLOBAL_DATEMODE,
      GLOBAL_DEFAULTFONTS,
      GLOBAL_FONTS,
      GLOBAL_FORMATS,
      GLOBAL_DEFAULTXFS,
      GLOBAL_XFS,
      GLOBAL_STYLES,
	  GLOBAL_PALETTE,
      GLOBAL_BOUNDSHEETS,
      GLOBAL_EOF,
      GLOBAL_FINISH
    } GlobalRecordDumpState_t;

  class CGlobalRecords : public CBiffSection
    {
    public:
      CGlobalRecords();
      ~CGlobalRecords();
//      static		CGlobalRecords& Instance();
//      static void	Clean();
#if HAVE_ICONV
	  void			SetIconvCode(const std::string& code){iconv_code=code;}
#endif
      void			AddBoundingSheet(unsigned32_t streampos,
                            unsigned16_t attributes,
                            u16string& sheetname
							);
      void			AddBoundingSheet(boundsheet_t* bsheetdef);
      void			AddFont(font_t* newfont);
      void			AddFormat(format_t*);
      void			AddXFormat(xf_t* xf);

      bool			SetColor(unsigned8_t r, unsigned8_t g, unsigned8_t b, unsigned8_t idx);

      void			GetBoundingSheets(Boundsheet_Vect_Itor_t &bs);
   
      // A Kind of state machine that will return
      // a non-null pointer to the data unit until all data has been retrieved.
      CUnit*		DumpData(CDataStorage &datastore); 

      Boundsheet_Vect_Itor_t GetFirstBoundSheet();
      Boundsheet_Vect_Itor_t GetEndBoundSheet();
	  Boundsheet_Vect_Itor_t GetBoundSheetAt(unsigned32_t idx);

      font_t*		GetDefaultFont() const;
      xf_t*			GetDefaultXF() const;
	  window1&		GetWindow1() {return m_window1;}
	  
	  font_t*		fontdup(unsigned8_t fontnum) const;

	  size_t		EstimateNumBiffUnitsNeeded4Header(void);
	  

      void wide2str16(const std::ustring& str1, u16string& str2);
      void char2str16(const std::string& str1, u16string& str2);
	  void str16toascii(const u16string& str1, std::string& str2);

	  static bool IsASCII(const std::string& str);
	  static bool IsASCII(const u16string& str);

	private:
		CGlobalRecords(const CGlobalRecords& that);
		CGlobalRecords& operator=(const CGlobalRecords& right);

    private:
		Font_Vect_t			m_Fonts;
		Font_Vect_t			m_DefaultFonts;
		Format_Vect_t		m_Formats;
		XF_Vect_t			m_XFs;
		XF_Vect_t			m_DefaultXFs;
		Style_Vect_t		m_Styles;
		Boundsheet_Vect_t	m_BoundSheets;
		window1				m_window1;
		colors_t			m_palette;
		
		xf_t				*defaultXF;			// 15th xfFormat is the default cell format

#ifdef HAVE_ICONV
		std::string	iconv_code;
#endif
      // State Machine variable
      GlobalRecordDumpState_t m_DumpState;

      /*
        static const font_init_t   m_Default_Fonts  [GLOBAL_NUM_DEFAULT_FONT    ];
        static const format_t m_Default_Formats[GLOBAL_NUM_DEFAULT_FORMATS ];
        static const xf_init_t     m_Default_XFs    [GLOBAL_NUM_DEFAULT_XFS     ];
        static const style_t  m_Default_Styles [GLOBAL_NUM_DEFAULT_STYLES  ];
      */

      Font_Vect_Itor_t       font;
      Font_Vect_Itor_t       font_dflt;
      unsigned16_t           fontIndex;
	
      Format_Vect_Itor_t     format;
      unsigned16_t           formatIndex;

      XF_Vect_Itor_t         xf;
      XF_Vect_Itor_t         xf_dflt;
      unsigned16_t           xfIndex;

      Style_Vect_Itor_t      style;
      Boundsheet_Vect_Itor_t bsheet;
    };

}

#include <xls_poppack.h>

#endif // GLOBALREC_H


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: globalrec.h,v $
 * Revision 1.9  2009/03/02 04:08:43  dhoerl
 * Code is now compliant to gcc  -Weffc++
 *
 * Revision 1.8  2009/01/23 16:09:55  dhoerl
 * General cleanup: headers and includes. Fixed issues building mainC and mainCPP
 *
 * Revision 1.7  2009/01/10 21:10:50  dhoerl
 * More tweaks
 *
 * Revision 1.6  2009/01/09 15:04:26  dhoerl
 * GlobalRec now used only as a reference.
 *
 * Revision 1.5  2009/01/08 22:16:06  dhoerl
 * January Rework
 *
 * Revision 1.4  2009/01/08 02:52:59  dhoerl
 * December Rework
 *
 * Revision 1.3  2008/12/20 15:49:05  dhoerl
 * 1.2.5 fixes
 *
 * Revision 1.2  2008/10/25 18:39:54  dhoerl
 * 2008
 *
 * Revision 1.1.1.1  2004/08/27 16:31:49  darioglz
 * Initial Import.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
