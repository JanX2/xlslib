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
 * $Source: /cvsroot/xlslib/xlslib/src/xlslib/extformat.h,v $
 * $Revision: 1.11 $
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

#ifndef EXTFORMAT_H
#define EXTFORMAT_H

#include <config.h>
#include <common.h>
#include <rectypes.h>
#include <record.h>
#include <colors.h>
#include <font.h>
#include <format.h>
//#include <workbook.h>

namespace xlslib_core
{

// COMMON

  // The Cell Properties bit or-masks:
#define XF_PROP_SHIFTPOS_PARENT  (4)

#define XF_PROP_LOCKED      ((unsigned16_t)0x0001)
#define XF_PROP_HIDDEN      ((unsigned16_t)0x0002)
#define XF_PROP_STYLE       ((unsigned16_t)0x0004)
//#define XF_PROP_123PREFIX   ((unsigned16_t)0x0008)
#define XF_PROP_XFPARENT    ((unsigned16_t)0xFFF0)
#define XF_PROP_XF_DEFAULT_CELL			   15

  // The Alignment field bit or-masks:
#define XF_ALIGN_HORIZONTAL  ((unsigned32_t)0x0007)
#define XF_ALIGN_WRAP        ((unsigned32_t)0x0008)
#define XF_ALIGN_VERTICAL    ((unsigned32_t)0x0070)
#define XF_ALIGN_JUSTLAST    ((unsigned32_t)0x0080) /* BIFF8: Used only in far-east versions of excel */

  // Style options
#define XF_FILL_NONE                ((unsigned8_t)0x00)
#define XF_FILL_SOLID               ((unsigned8_t)0x01)
#define XF_FILL_ATEN75              ((unsigned8_t)0x03)
#define XF_FILL_ATEN50              ((unsigned8_t)0x02)
#define XF_FILL_ATEN25              ((unsigned8_t)0x04)
#define XF_FILL_ATEN12              ((unsigned8_t)0x11)
#define XF_FILL_ATEN06              ((unsigned8_t)0x12)
#define XF_FILL_HORIZ_LIN           ((unsigned8_t)0x05)
#define XF_FILL_VERTICAL_LIN        ((unsigned8_t)0x06)
#define XF_FILL_DIAG                ((unsigned8_t)0x07)
#define XF_FILL_INV_DIAG            ((unsigned8_t)0x08)
#define XF_FILL_INTER_DIAG          ((unsigned8_t)0x09)
#define XF_FILL_DIAG_THICK_INTER    ((unsigned8_t)0x0a)
#define XF_FILL_HORIZ_LINES_THIN    ((unsigned8_t)0x0b)
#define XF_FILL_VERTICAL_LINES_THIN ((unsigned8_t)0x0c)
#define XF_FILL_DIAG_THIN           ((unsigned8_t)0x0d)
#define XF_FILL_INV_DIAG_THIN       ((unsigned8_t)0x0e)
#define XF_FILL_HORIZ_INT_THIN      ((unsigned8_t)0x0f)
#define XF_FILL_HORIZ_INTER_THICK   ((unsigned8_t)0x10)
  typedef enum
    {
      FILL_NONE = 0,
      FILL_SOLID,
      FILL_ATEN75,
      FILL_ATEN50,
      FILL_ATEN25,
      FILL_ATEN12,
      FILL_ATEN06,
      FILL_HORIZ_LIN,
      FILL_VERTICAL_LIN,
      FILL_DIAG,
      FILL_INV_DIAG,
      FILL_INTER_DIAG,
      FILL_DIAG_THICK_INTER,
      FILL_HORIZ_LINES_THIN,
      FILL_VERTICAL_LINES_THIN,
      FILL_DIAG_THIN,
      FILL_INV_DIAG_THIN,
      FILL_HORIZ_INT_THIN,
      FILL_HORIZ_INTER_THICK   
    } fill_option_t;

  // Border Options
#define XF_BRDOPTION_NONE       ((unsigned8_t)0x00)
#define XF_BRDOPTION_THIN       ((unsigned8_t)0x01)
#define XF_BRDOPTION_MEDIUM     ((unsigned8_t)0x02)
#define XF_BRDOPTION_DASHED     ((unsigned8_t)0x03)
#define XF_BRDOPTION_DOTTED     ((unsigned8_t)0x04)
#define XF_BRDOPTION_THICK      ((unsigned8_t)0x05)
#define XF_BRDOPTION_DOUBLE     ((unsigned8_t)0x06)
#define XF_BRDOPTION_HAIR       ((unsigned8_t)0x07)
  typedef enum
    {
      BORDER_NONE = 0,
      BORDER_THIN,
      BORDER_MEDIUM,
      BORDER_DASHED,
      BORDER_DOTTED,
      BORDER_THICK,
      BORDER_DOUBLE,
      BORDER_HAIR    
    } border_style_t;

   // Border options
#define XF_BORDER_BOTTOM  ((unsigned8_t)0)
#define XF_BORDER_TOP     ((unsigned8_t)1)
#define XF_BORDER_LEFT    ((unsigned8_t)2)
#define XF_BORDER_RIGHT   ((unsigned8_t)3)
  typedef enum
    {
      BORDER_BOTTOM = 0,
      BORDER_TOP,
      BORDER_LEFT,
      BORDER_RIGHT,
	  DIAGONALS,		// BIFF8
	  _NUM_BORDERS
    } border_side_t;
  // Horizontal Align options
#define XF_HALIGN_GENERAL         ((unsigned8_t)0)
#define XF_HALIGN_LEFT            ((unsigned8_t)1)
#define XF_HALIGN_CENTER          ((unsigned8_t)2)
#define XF_HALIGN_RIGHT           ((unsigned8_t)3)
#define XF_HALIGN_FILL            ((unsigned8_t)4)
#define XF_HALIGN_JUSTIFY         ((unsigned8_t)5)
#define XF_HALIGN_CENTERACCROSS   ((unsigned8_t)6)
  typedef enum
    {
      HALIGN_GENERAL = 0,
      HALIGN_LEFT,
      HALIGN_CENTER,
      HALIGN_RIGHT,
      HALIGN_FILL,
      HALIGN_JUSTIFY,
      HALIGN_CENTERACCROSS
    } halign_option_t;

   // Vertical Align options
#define XF_VALIGN_TOP     ((unsigned8_t)0)
#define XF_VALIGN_CENTER  ((unsigned8_t)1)
#define XF_VALIGN_BOTTOM  ((unsigned8_t)2)
#define XF_VALIGN_JUSTIFY ((unsigned8_t)3)
  typedef enum
    {
      VALIGN_TOP = 0,
      VALIGN_CENTER,
      VALIGN_BOTTOM,
      VALIGN_JUSTIFY
    } valign_option_t;

#define XF_LOCKED			true
#define XF_NO_LOCKED		false

#define XF_HIDDEN			true
#define XF_NO_HIDDEN		false

#define XF_WRAPPED			true
#define XF_NO_WRAPPED		false

#define XF_IS_CELL			true
#define XF_IS_STYLE			false

#define XF_OFFSET_FONT         ((unsigned32_t) 4)
#define XF_OFFSET_FORMAT       ((unsigned32_t) 6)
#define XF_OFFSET_PROP         ((unsigned32_t) 8)
#define XF_OFFSET_ALIGN        ((unsigned32_t)10)	// 4 bytes
#define XF_OFFSET_BORDERA      ((unsigned32_t)14)	// 4 bytes
#define XF_OFFSET_BORDERB      ((unsigned32_t)18)	// 4 bytes
#define XF_OFFSET_COLOR        ((unsigned32_t)22)

  // Geometric Align options
#define XF_ALIGN_SHIFTPOS_HALIGN  (0)
#define XF_ALIGN_SHIFTPOS_VALIGN  (4)

  // XF_USED_ATTRIB
#define XF_ALIGN_ATR_SHIFT			24

#define XF_ALIGN_ATRNUM				((unsigned8_t)0x04)
#define XF_ALIGN_ATRFONT			((unsigned8_t)0x08)
#define XF_ALIGN_ATRALC				((unsigned8_t)0x10)
#define XF_ALIGN_ATRBDR				((unsigned8_t)0x20)
#define XF_ALIGN_ATRPAT				((unsigned8_t)0x40)
#define XF_ALIGN_ATRPROT			((unsigned8_t)0x80)
#define XF_ALIGN_ALL			    (XF_ALIGN_ATRPROT|XF_ALIGN_ATRPAT|XF_ALIGN_ATRBDR|XF_ALIGN_ATRALC|XF_ALIGN_ATRFONT|XF_ALIGN_ATRNUM)
#define XF_ALIGN_ATR_MASK			((unsigned32_t)0xFC000000)

  // Text Orientation Options
#define XF_ORI_SHIFTPOS			(8)
#define XF_ORI_NONE             ((unsigned8_t)0)
#define XF_ORI_90NOCLOCKTXT     ((unsigned8_t)90)
#define XF_ORI_90CLOCKTXT       ((unsigned8_t)180)
#define XF_ORI_TOPBOTTOMTXT     ((unsigned8_t)255)
#define XF_ORI_MASK				((unsigned32_t)0x0000FF00)
  typedef enum
    {
      ORI_NONE = 0,      
      ORI_TOPBOTTOMTXT,
      ORI_90NOCLOCKTXT,
      ORI_90CLOCKTXT     
    } txtori_option_t;

  // Indent field
#define XF_INDENT_SHIFTPOS		(16)
#define XF_INDENT_LVL			((unsigned8_t)0x0F)
#define XF_INDENT_0				((unsigned8_t)0x00)
#define XF_INDENT_1				((unsigned8_t)0x01)
#define XF_INDENT_2				((unsigned8_t)0x02)
#define XF_INDENT_3				((unsigned8_t)0x03)
#define XF_INDENT_4				((unsigned8_t)0x04)
#define XF_INDENT_5				((unsigned8_t)0x05)
#define XF_INDENT_6				((unsigned8_t)0x06)
#define XF_INDENT_7				((unsigned8_t)0x07)
#define XF_INDENT_8				((unsigned8_t)0x08)
#define XF_INDENT_9				((unsigned8_t)0x09)
#define XF_INDENT_10			((unsigned8_t)0x0a)
#define XF_INDENT_11			((unsigned8_t)0x0b)
#define XF_INDENT_12			((unsigned8_t)0x0c)
#define XF_INDENT_13			((unsigned8_t)0x0d)
#define XF_INDENT_14			((unsigned8_t)0x0e)
#define XF_INDENT_15			((unsigned8_t)0x0f)
#define XF_INDENT_SHRINK2FIT    ((unsigned8_t)0x10)
#define XF_INDENT_DIR			((unsigned8_t)0xC0)
#define XF_INDENT_CONTEXT		((unsigned8_t)0x00)
#define XF_INDENT_L2R			((unsigned8_t)0x40)
#define XF_INDENT_R2L			((unsigned8_t)0x80)
  typedef enum
    {
      INDENT_0 = 0,
      INDENT_1,
      INDENT_2,
      INDENT_3,
      INDENT_4,
      INDENT_5,
      INDENT_6,
      INDENT_7,
      INDENT_8,
      INDENT_9,
      INDENT_10,
      INDENT_11,
      INDENT_12,
      INDENT_13,
      INDENT_14,
      INDENT_15,
      INDENT_SHRINK2FIT,
      INDENT_L2R,
	  INDENT_R2L
    } indent_option_t;

  // XF_USED_ATTRIB
#define XF_ATTRIB_SHIFTPOS		(16)	// bits shifted by 8 already

  // The Border A field bit or-masks:

#define XF_STYLE_SHIFTPOS_LEFT		(0)
#define XF_STYLE_SHIFTPOS_RIGHT		(4)
#define XF_STYLE_SHIFTPOS_TOP		(8)
#define XF_STYLE_SHIFTPOS_BOTTOM	(12)

  // The BorderA field bit or-masks:
#define XF_COLOR_SHIFTPOS_LEFT  (16)
#define XF_COLOR_SHIFTPOS_RIGHT (23)

#define XF_BORDER_LEFTSTYLE		((unsigned32_t)0x0000000F)
#define XF_BORDER_RIGHTSTYLE	((unsigned32_t)0x000000F0)
#define XF_BORDER_TOPSTYLE		((unsigned32_t)0x00000F00)
#define XF_BORDER_BOTTOMSTYLE   ((unsigned32_t)0x0000F000)

#define XF_BORDER_LEFTCOLOR		((unsigned32_t)0x007f0000)
#define XF_BORDER_RIGHTCOLOR	((unsigned32_t)0x3f800000)
#define XF_DIAG_TL2BR			((unsigned32_t)0x40000000)
#define XF_DIAG_BL2TR			((unsigned32_t)0x80000000)

  // BORDER B 

#define XF_COLOR_SHIFTPOS_TOP		(0)
#define XF_COLOR_SHIFTPOS_BOTTOM	(7)
#define XF_COLOR_SHIFTPOS_DIAG		(14)
#define XF_STYLE_SHIFTPOS_DIAG		(21)
#define XF_SHIFTPOS_FILLPATTERN		(26)

#define XF_BORDER_TOPCOLOR     ((unsigned16_t)0x0000007f)
#define XF_BORDER_BOTTOMCOLOR  ((unsigned16_t)0x00003f80)
#define XF_BORDER_DIAGCOLOR    ((unsigned16_t)0x001fc000)
#define XF_BORDER_DIAGSTYLE    ((unsigned16_t)0x01e00000)
#define XF_BORDER_FILLPATTERN  ((unsigned32_t)0xFC000000)

//#define XF_STYLE_SHIFTPOS_LEFT  (3)
//#define XF_STYLE_SHIFTPOS_RIGHT (6)
//#define XF_COLOR_SHIFTPOS_TOP   (9)
//#define XF_BORDER1_TOPSTYLE     ((unsigned16_t)0x0007)
//#define XF_BORDER1_LEFTSTYLE    ((unsigned16_t)0x0038)
//#define XF_BORDER1_RIGHTSTYLE   ((unsigned16_t)0x01c0)

  // The Color field bit or-masks:
#define XF_COLOR_SHIFTPOS_FG  (0)
#define XF_COLOR_SHIFTPOS_BG  (7)
#define XF_COLOR_FOREGROUND    ((unsigned16_t)0x007f)
#define XF_COLOR_BACKGROUND    ((unsigned16_t)0x3f80)
#define XF_COLOR_DIAG          ((unsigned16_t)0xc000)

  /*
******************************
CExtFormat class declaration
******************************
*/
  class xf_init_t
  {
	public:
		xf_init_t();
		~xf_init_t();

		font_t*			font;
		//format_number_t format;	problem is user formats are in their own space
		unsigned16_t	formatIndex;

		halign_option_t halign;
		valign_option_t valign;
		indent_option_t indent;
		txtori_option_t txt_orient;

		fill_option_t	fillstyle;
		color_name_t	fill_fgcolor;
		color_name_t	fill_bgcolor;

		bool locked;
		bool hidden;
		bool wrap;

		border_style_t	border_style[_NUM_BORDERS];
		color_name_t	border_color[_NUM_BORDERS];

		bool operator==(const xf_init_t& right);

	private:
		xf_init_t(const xf_init_t &that);
		xf_init_t& operator=(const xf_init_t& right);
  };

  //class xf_t;

  class xf_i
  {
   public:
	xf_i() { }
	virtual ~xf_i() { }

    virtual void font(font_t* fontidx) = 0;
    virtual void format(format_number_t formatidx) = 0;
    virtual void format(format_t* format) = 0;
    virtual void halign(halign_option_t ha_option) = 0;
    virtual void valign(valign_option_t va_option) = 0;
    virtual void indent(indent_option_t indent_opt) = 0;
    virtual void orientation(txtori_option_t ori_option) = 0;
    virtual void fillfgcolor(color_name_t color) = 0;
    virtual void fillfgcolor(unsigned8_t color) = 0;
    virtual void fillbgcolor(color_name_t color) = 0;
    virtual void fillbgcolor(unsigned8_t color) = 0;
    virtual void fillstyle(fill_option_t fill) = 0;
    virtual void locked(bool locked_opt) = 0;
    virtual void hidden(bool hidden_opt) = 0;
    virtual void wrap(bool wrap_opt) = 0;
    virtual void borderstyle(border_side_t side, border_style_t style) = 0;
    virtual void bordercolor(border_side_t side, color_name_t color) = 0;
    virtual void bordercolor(border_side_t side, unsigned8_t color) = 0;
  };

  
  class xf_t
    {
		// The reason for these is to advise end users on which methods they should use.
		//friend xf_t* workbook::xformat(void);
		friend class workbook;
		friend class worksheet;
		friend class range;
		friend class CGlobalRecords;
		friend class CExtFormat;
		friend class cell_t;
		friend class xfSorter;
		
	private:
		xf_t(CGlobalRecords& gRecords, bool userXF=true, bool isCell=true, bool isMasterXF=false);
		xf_t(CGlobalRecords& gRecords, const xf_init_t& xfinit);
		xf_t(const xf_t& orig);

		//xf_t(const xlslib_core::xf_t* origP) { };	// block - not sure why this was there??? Old???
		~xf_t();									// block (was virtual)
		
		//static xf_init_t::xf_init_t		xfiInit;
		static const xf_init_t		xfiInit;

		xf_t& operator=(const xf_t& right);

	private:
		void			SetFormatIndex(unsigned16_t formatidx);
		unsigned16_t	GetFormatIndex(void) const;
        format_number_t	GetFormat(void) const;
		void			UnMarkUsed(void);
		void			MarkUsed(void);
		unsigned32_t	Usage() const;

      /* XF Index wrappers*/
      void				SetIndex(unsigned16_t xfidx){index = xfidx;};
	  std::string		Description() const;

    public:
	  // end user copy method
	  static xf_t* xfDup(const xf_t* orig)
		{
			xf_t*	xft = new xf_t(*orig);
			return xft;
		}
      bool operator==(const xf_t& right);

      unsigned16_t	GetIndex(void) const {return index;};

	  // XF_ALIGN_ATRNUM
      /* Format Index wrappers*/
      void			SetFormat(format_number_t formatidx);
      void			SetFormat(format_t *fmt);

	  // XF_ALIGN_ATRFONT
      /* Font Index wrappers*/
      void         SetFont(font_t* newfont);
      unsigned16_t GetFontIndex(void) const;
      font_t*      GetFont(void) const;

	  // XF_ALIGN_ATRALC controlled attributes (all set in one Excel pane)
      /* Horizontal Align option wrappers*/
      void        SetHAlign(halign_option_t ha_option);
      unsigned8_t GetHAlign(void) const;

      /* Vertical Align option wrappers*/
      void        SetVAlign(valign_option_t va_option);
      unsigned8_t GetVAlign(void) const;

      /* Wrap option wrappers*/
      void			SetWrap(bool wrap_opt);
      bool			IsWrap(void) const;

      /* Horizontal alignment options */
      void        SetIndent(indent_option_t indent_option);
      unsigned8_t GetIndent(void) const;

      /* Text orientation option wrappers*/
      void		  SetTxtOrientation(txtori_option_t ori_option);
      unsigned8_t GetTxtOrientation(void) const;
	  // END XF_ALIGN_ATRALC
	
	  // XF_ALIGN_ATRBDR - border
      /* Cell option wrappers*/
      void         SetBorderStyle(border_side_t side, border_style_t style);
      unsigned8_t  GetBorderStyle(border_side_t side) const;

      void         SetBorderColor(border_side_t side, color_name_t color);
      void         SetBorderColor(border_side_t side, unsigned8_t color);
      unsigned16_t GetBorderColorIdx(border_side_t side) const;
	  // END XF_ALIGN_ATRBDR

	  // XF_ALIGN_ATRPAT
      /* Fill Foreground color option wrappers*/
      void        SetFillFGColor(color_name_t color);
      void        SetFillFGColor(unsigned8_t color);
      unsigned16_t GetFillFGColorIdx(void) const;

      /* Fill Background color option wrappers*/
      void        SetFillBGColor(color_name_t color);
      void        SetFillBGColor(unsigned8_t color);
      unsigned16_t GetFillBGColorIdx(void) const;

      /* Fill Style option wrappers*/
      void        SetFillStyle(fill_option_t fill);
      unsigned8_t GetFillStyle(void) const;
	  // END XF_ALIGN_ATRPAT

 	  // XF_ALIGN_ATRPROT
      /* Locked option wrappers*/
      void SetLocked(bool locked_opt);
      bool IsLocked(void) const;

      /* Hidden option wrappers*/
      void SetHidden(bool hidden_opt);
      bool IsHidden(void) const;
	  // END XF_ALIGN_ATRPROT
	  
	private:
	  /* Cell option wrappers*/
      void SetCellMode(bool cellmode);
      bool IsCell(void) const;
      unsigned8_t GetFlags() const;
      void SetFlag(unsigned8_t flag);
      void ClearFlag(unsigned8_t flag);

    private:
      //xlslib_core::CGlobalRecords *m_GlobalRecords;
      CGlobalRecords& m_GlobalRecords;
      xf_init_t		xfi;	// shadow options used to create this object

      unsigned32_t m_usage_counter;
      unsigned16_t index;

      font_t* font;
      //format_number_t format;
	  unsigned16_t formatIndex;

      unsigned8_t halign;
      unsigned8_t valign;
      unsigned8_t indent;
      unsigned8_t txt_orient;

      unsigned8_t fillstyle;
      unsigned8_t fill_fgcolor;
      unsigned8_t fill_bgcolor;

      bool locked;
      bool hidden;
      bool wrap;
      bool is_cell;
	  bool is_userXF;

      unsigned8_t	border_style[_NUM_BORDERS];
      unsigned8_t	border_color[_NUM_BORDERS];

	  unsigned8_t flags;

      // Lookup tables for options
      static const unsigned8_t HALIGN_OPTIONS_TABLE[];
      static const unsigned8_t VALIGN_OPTIONS_TABLE[];
      static const unsigned8_t INDENT_OPTIONS_TABLE[];
      static const unsigned8_t TXTORI_OPTIONS_TABLE[];
      static const unsigned8_t COLOR_OPTIONS_TABLE[];
      static const unsigned8_t FILL_OPTIONS_TABLE[];
      static const unsigned8_t BORDERSTYLE_OPTIONS_TABLE[];
    };

  typedef std::vector<xlslib_core::xf_t* XLSLIB_DFLT_ALLOCATOR> XF_Vect_t;
  typedef XF_Vect_t::iterator XF_Vect_Itor_t;

  class CExtFormat: public CRecord
    {
    private:
      void InitDummy(bool is_cell);

    public:

      CExtFormat(xf_t* xfdef);
      ~CExtFormat();

      bool IsCell();

      int SetFontIndex(unsigned16_t fontindex);
      unsigned16_t GetFontIndex(void);

      int SetFormatIndex(unsigned16_t formatindex);
      unsigned16_t GetFormatIndex(void);

      void SetLocked();
      void SetHidden();
      void SetHorizAlign(unsigned8_t alignval);
      void SetWrap();
	  void SetIndent(unsigned8_t indentval);
      void SetVertAlign(unsigned8_t alignval);
      void SetTxtOrientation(unsigned8_t alignval);
      void SetFGColorIndex(unsigned8_t color);
      void SetBGColorIndex(unsigned8_t color);
      void SetFillPattern(unsigned8_t color);
      void SetBorder(border_side_t border, unsigned16_t style, unsigned16_t color);
	  void SetFlags(unsigned8_t flags);
//    void SetXFParent(unsigned16_t parent); // maybe later
    };

}
#endif //EXTFORMAT_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: extformat.h,v $
 * Revision 1.11  2009/03/02 04:08:43  dhoerl
 * Code is now compliant to gcc  -Weffc++
 *
 * Revision 1.10  2009/01/23 16:09:55  dhoerl
 * General cleanup: headers and includes. Fixed issues building mainC and mainCPP
 *
 * Revision 1.9  2009/01/10 21:10:50  dhoerl
 * More tweaks
 *
 * Revision 1.8  2009/01/09 15:04:26  dhoerl
 * GlobalRec now used only as a reference.
 *
 * Revision 1.7  2009/01/09 03:23:12  dhoerl
 * GlobalRec references tuning
 *
 * Revision 1.6  2009/01/08 02:53:15  dhoerl
 * December Rework
 *
 * Revision 1.5  2008/12/20 15:49:05  dhoerl
 * 1.2.5 fixes
 *
 * Revision 1.4  2008/12/10 03:33:53  dhoerl
 * m_usage was 16bit and wrapped
 *
 * Revision 1.3  2008/12/06 01:42:57  dhoerl
 * John Peterson changes along with lots of tweaks. Many bugs that causes Excel crashes fixed.
 *
 * Revision 1.2  2008/10/25 18:39:54  dhoerl
 * 2008
 *
 * Revision 1.1.1.1  2004/08/27 16:31:46  darioglz
 * Initial Import.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


