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
 * $Source: /cvsroot/xlslib/xlslib/src/xlslib/extformat.cpp,v $
 * $Revision: 1.10 $
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

#include <sstream>

#include <extformat.h>
#include <globalrec.h>	// order real important here

using namespace std;
using namespace xlslib_core;


/* 
**********************************
xf_t class implementation
**********************************
*/

const xf_init_t xf_t::xfiInit;

const unsigned8_t xf_t::HALIGN_OPTIONS_TABLE[] = 
{
   XF_HALIGN_GENERAL      ,
   XF_HALIGN_LEFT         ,
   XF_HALIGN_CENTER       ,
   XF_HALIGN_RIGHT        ,
   XF_HALIGN_FILL         ,
   XF_HALIGN_JUSTIFY      ,
   XF_HALIGN_CENTERACCROSS
};

const unsigned8_t xf_t::VALIGN_OPTIONS_TABLE[] = 
{
   XF_VALIGN_TOP    ,
   XF_VALIGN_CENTER ,
   XF_VALIGN_BOTTOM ,
   XF_VALIGN_JUSTIFY
};

const unsigned8_t xf_t::INDENT_OPTIONS_TABLE[] = 
{
	XF_INDENT_0,
	XF_INDENT_1,
	XF_INDENT_2,
	XF_INDENT_3,
	XF_INDENT_4,
	XF_INDENT_5,
	XF_INDENT_6,
	XF_INDENT_7,
	XF_INDENT_8,
	XF_INDENT_9,
	XF_INDENT_10,
	XF_INDENT_11,
	XF_INDENT_12,
	XF_INDENT_13,
	XF_INDENT_14,
	XF_INDENT_15,
	XF_INDENT_SHRINK2FIT,
	XF_INDENT_L2R,
	XF_INDENT_R2L
};

const unsigned8_t xf_t::TXTORI_OPTIONS_TABLE[] = 
{
   XF_ORI_NONE        ,
   XF_ORI_TOPBOTTOMTXT,
   XF_ORI_90NOCLOCKTXT,
   XF_ORI_90CLOCKTXT  
};

const unsigned8_t xf_t::COLOR_OPTIONS_TABLE[] =
{
	0,	// Black as used in the default fonts
	COLOR_CODE_BLACK,
	COLOR_CODE_BROWN,
	COLOR_CODE_OLIVE_GREEN,
	COLOR_CODE_DARK_GREEN,
	COLOR_CODE_DARK_TEAL,
	COLOR_CODE_DARK_BLUE,
	COLOR_CODE_INDIGO,
	COLOR_CODE_GRAY80,

	COLOR_CODE_DARK_RED,
	COLOR_CODE_ORANGE,
	COLOR_CODE_DARK_YELLOW,
	COLOR_CODE_GREEN,
	COLOR_CODE_TEAL,
	COLOR_CODE_BLUE,
	COLOR_CODE_BLUE_GRAY,
	COLOR_CODE_GRAY50,

	COLOR_CODE_RED,
	COLOR_CODE_LIGHT_ORANGE,
	COLOR_CODE_LIME,
	COLOR_CODE_SEA_GREEN,
	COLOR_CODE_AQUA,
	COLOR_CODE_LIGHT_BLUE,
	COLOR_CODE_VIOLET,
	COLOR_CODE_GRAY40,

	COLOR_CODE_PINK,
	COLOR_CODE_GOLD,
	COLOR_CODE_YELLOW,
	COLOR_CODE_BRIGHT_GREEN,
	COLOR_CODE_TURQUOISE,
	COLOR_CODE_SKY_BLUE,
	COLOR_CODE_PLUM,
	COLOR_CODE_GRAY25,

	COLOR_CODE_ROSE,
	COLOR_CODE_TAN,
	COLOR_CODE_LIGHT_YELLOW,
	COLOR_CODE_LIGHT_GREEN,
	COLOR_CODE_LIGHT_TURQUOISE,
	COLOR_CODE_PALEBLUE,
	COLOR_CODE_LAVENDER,
	COLOR_CODE_WHITE,

	COLOR_CODE_PERIWINKLE,
	COLOR_CODE_DARK_BLUE2,
	COLOR_CODE_PLUM2,
	COLOR_CODE_PINK2,
	COLOR_CODE_IVORY,
	COLOR_CODE_YELLOW2,
	COLOR_CODE_LIGHT_TURQUOISE2,
	COLOR_CODE_TURQUOISE2,

	COLOR_CODE_DARK_PURPLE,
	COLOR_CODE_VIOLET2,
	COLOR_CODE_CORAL,
	COLOR_CODE_DARK_RED2,
	COLOR_CODE_OCEAN_BLUE,
	COLOR_CODE_TEAL2,
	COLOR_CODE_ICE_BLUE,
	COLOR_CODE_BLUE2,

	COLOR_CODE_SYS_WIND_FG,
	COLOR_CODE_SYS_WIND_BG
};

const unsigned8_t xf_t::FILL_OPTIONS_TABLE[] =
{
   XF_FILL_NONE,
   XF_FILL_SOLID,
   XF_FILL_ATEN75,
   XF_FILL_ATEN50,
   XF_FILL_ATEN25,
   XF_FILL_ATEN12,
   XF_FILL_ATEN06,
   XF_FILL_HORIZ_LIN,
   XF_FILL_VERTICAL_LIN,
   XF_FILL_DIAG,
   XF_FILL_INV_DIAG,
   XF_FILL_INTER_DIAG,
   XF_FILL_DIAG_THICK_INTER,
   XF_FILL_HORIZ_LINES_THIN,
   XF_FILL_VERTICAL_LINES_THIN,
   XF_FILL_DIAG_THIN,
   XF_FILL_INV_DIAG_THIN,
   XF_FILL_HORIZ_INT_THIN,
   XF_FILL_HORIZ_INTER_THICK   
};

const unsigned8_t xf_t::BORDERSTYLE_OPTIONS_TABLE[] =
{
   XF_BRDOPTION_NONE,
   XF_BRDOPTION_THIN,
   XF_BRDOPTION_MEDIUM,
   XF_BRDOPTION_DASHED,
   XF_BRDOPTION_DOTTED,
   XF_BRDOPTION_THICK,
   XF_BRDOPTION_DOUBLE,
   XF_BRDOPTION_HAIR
};

/* 
******************************************************
class xf_init_t: convenience structure to init an xf_t
******************************************************
*/
xf_init_t::xf_init_t() :
	font(0),
	
	formatIndex(FMTCODE_GENERAL),
	halign(HALIGN_GENERAL),
	valign(VALIGN_BOTTOM),
	
	indent(INDENT_0),
	txt_orient(ORI_NONE),
	
	fillstyle(FILL_NONE),
	fill_fgcolor(CLR_SYS_WIND_FG),
	fill_bgcolor(CLR_SYS_WIND_BG),
	
	locked(XF_LOCKED),
	hidden(XF_NO_HIDDEN),
	wrap(XF_NO_WRAPPED),
	border_style(),
	border_color()
{
	for(int i=0; i<_NUM_BORDERS; ++i) {
		border_style[i]	= BORDER_NONE;
		border_color[i] = ORIG_COLOR_BLACK;
	}
}
xf_init_t::~xf_init_t()
{
}
bool xf_init_t::operator==(const xf_init_t& right)
{
	// used by "range" in doing mass changes. Try to arrange so most
	// likely failures occur early

	if(font != right.font)					return false;
	
	if(fill_fgcolor != right.fill_fgcolor)	return false;
	if(fill_bgcolor != right.fill_bgcolor)	return false;
	if(fillstyle != right.fillstyle)		return false;
	
	for(int i=0; i<_NUM_BORDERS; ++i) {
		if(border_style[i] != right.border_style[i]) return false;
		if(border_color[i] != right.border_color[i]) return false;
	}
	
	if(halign != right.halign)			return false;
	if(valign != right.valign)			return false;
	if(indent != right.indent)			return false;
	if(txt_orient != right.txt_orient)	return false;

	if(locked != right.locked)			return false;
	if(hidden != right.hidden)			return false;
	if(wrap != right.wrap)				return false;

	return true;
}

/* 
******************************************
GlobalRec, xftDup, and range(userXF==no)
******************************************
*/
xf_t::xf_t(xlslib_core::CGlobalRecords& gRecords, bool userXF, bool isCell, bool isMasterXF) :
    m_GlobalRecords(gRecords),
	xfi(),
	m_usage_counter(0),
	index(0),

	font(NULL),
	format(NULL),
	formatIndex(FMTCODE_GENERAL),
	halign(0),
	valign(0),
	indent(0),
	txt_orient(0),
	fillstyle(0),
	fill_fgcolor(0),
	fill_bgcolor(0),

	locked(false),
	hidden(false),
	wrap(false),
	is_cell(isCell),
	is_userXF(userXF),

 	border_style(),
	border_color(),
	
	flags(0)
{
//	SetCellMode(isCell);

	// Set a default value for class members
	SetFormatIndex(xfi.formatIndex);
	SetHAlign(xfi.halign);
	SetVAlign(xfi.valign);
	SetIndent(xfi.indent);
	SetTxtOrientation(xfi.txt_orient);
	SetFillFGColor(xfi.fill_fgcolor);
	SetFillBGColor(xfi.fill_bgcolor);
	SetFillStyle(xfi.fillstyle);
	
	SetLocked(xfi.locked);
	SetHidden(xfi.hidden);
	SetWrap(xfi.wrap);
	
	for(int i=0; i<_NUM_BORDERS; ++i) {
		SetBorderStyle((border_side_t)i, xfi.border_style[i]);
		SetBorderColor((border_side_t)i, xfi.border_color[i]);
	}

	// Flags have different meanings for Cell or Style formats
	flags = (isCell || isMasterXF) ? 0 : XF_ALIGN_ALL;

	is_userXF = userXF;
	if(is_userXF) {
		m_GlobalRecords.AddXFormat(this);
	}
}

/* 
**********************************
Only for use by users
**********************************
*/
xf_t::xf_t(const xf_t& orig) :
	m_GlobalRecords(orig.m_GlobalRecords),
	xfi(),
	m_usage_counter(0),
	index(0),

	font(NULL),							// yes, need this as SetFont below depends on a set value
	format(NULL),
	formatIndex(orig.formatIndex),
	halign(orig.halign),
	valign(orig.valign),
	indent(orig.indent),
	txt_orient(orig.txt_orient),
	fillstyle(orig.fillstyle),
	fill_fgcolor(orig.fill_fgcolor),
	fill_bgcolor(orig.fill_bgcolor),

	locked(orig.locked),
	hidden(orig.hidden),
	wrap(orig.wrap),
	is_cell(true),
	is_userXF(true),

 	border_style(),
	border_color(),
	
	flags(0)
{
    SetFont(orig.font);	// side effects
	if (orig.format)
	{
		SetFormat(orig.format);	// side effects
	}
	
	for(int i=0; i<_NUM_BORDERS; ++i) {
		border_style[i]	= orig.border_style[i];
		border_color[i] = orig.border_color[i];
	}
	
	flags = orig.flags;
	m_GlobalRecords.AddXFormat(this);
}

/* 
**********************************
Constructor using pre-set values: range
**********************************
*/
xf_t::xf_t(CGlobalRecords& gRecords, const xf_init_t& xfinit) :
    m_GlobalRecords(gRecords),
	xfi(),
	m_usage_counter(0),
	index(0),

	font(NULL),
	format(NULL),
	formatIndex(FMTCODE_GENERAL),
	halign(0),
	valign(0),
	indent(0),
	txt_orient(0),
	fillstyle(0),
	fill_fgcolor(0),
	fill_bgcolor(0),

	locked(false),
	hidden(false),
	wrap(false),
	is_cell(true),
	is_userXF(true),

 	border_style(),
	border_color(),
	
	flags(0)
{
	SetFont(xfinit.font);	// side effects
	//SetFormat(xfinit.format);	// side effects
	SetFormatIndex(xfinit.formatIndex);

	SetHAlign(xfinit.halign);
	SetVAlign(xfinit.valign);
	SetIndent(xfinit.indent);
	SetTxtOrientation(xfinit.txt_orient);
	SetFillFGColor(xfinit.fill_fgcolor);
	SetFillBGColor(xfinit.fill_bgcolor);
	SetFillStyle(xfinit.fillstyle);
	SetLocked(xfinit.locked);
	SetHidden(xfinit.hidden);
	SetWrap(xfinit.wrap);

	for(int i=0; i<_NUM_BORDERS; ++i) {
		SetBorderStyle((border_side_t)i, xfinit.border_style[i]);
		SetBorderColor((border_side_t)i, xfinit.border_color[i]);
	}
	m_GlobalRecords.AddXFormat(this);
}

/* 
**********************************
**********************************
*/
xf_t::~xf_t()
{
}

/* 
**********************************
**********************************
*/
void xf_t::SetFlag(unsigned8_t flag)
{
   if(IsCell())
   {
      if((flags & flag) == 0) {
	     // The flag forces all these fields to be defined, so init them to defaults
		 switch(flag) {
		 case XF_ALIGN_ATRALC:
			valign = XF_VALIGN_BOTTOM;
			break;
		 case XF_ALIGN_ATRPAT:
			fill_fgcolor = COLOR_CODE_SYS_WIND_FG;
			fill_bgcolor = COLOR_CODE_SYS_WIND_BG;
			break;
		 }
	  }
      // Cells indicate that a characteristic is not equal
      //  from its parent with the flag set.
      flags |= flag; 
   } else {
      // Styles indicate that a characteristic is 
      // being implemented with the flag cleared.
      flags &= ~flag;
   }
}

/* 
**********************************
**********************************
*/
void xf_t::ClearFlag(unsigned8_t flag)
{
   if(!IsCell())
   {
      // Cells indicate that a characteristic is not equal
      //  from its parent with the flag set.
      flags |= flag; 
   } else {
      // Styles indicate that a characteristic is 
      // being implemented with the flag cleared.
      flags &= ~flag;
   }
}

/* 
**********************************
**********************************
*/
unsigned8_t xf_t::GetFlags() const
{
	return flags;
}


/* 
**********************************
**********************************
*/
void xf_t::MarkUsed(void) 
{
   m_usage_counter++;
}
void xf_t::UnMarkUsed(void) 
{
   if(m_usage_counter)
      m_usage_counter--;
   
   if(m_usage_counter == 0)
   {
	   if (font && font->Usage())
			font->UnMarkUsed();
		if (format && format->Usage())
			format->UnMarkUsed();
   }
}
unsigned32_t xf_t::Usage(void) const
{
   return m_usage_counter;
}

/* 
**********************************
**********************************
*/
void xf_t::SetFont(font_t* newfont)
{
	// Set the related flag
	if(newfont != xf_t::xfiInit.font)
		SetFlag(XF_ALIGN_ATRFONT);
	if(font)
	  font->UnMarkUsed();
	  
	font = newfont;
	if(font)
	  font->MarkUsed();
}

font_t* xf_t::GetFont(void) const
{
   return font;
}

unsigned16_t xf_t::GetFontIndex(void) const
{
   if(font != NULL)
      return font->GetIndex();
   else
      return 0;
}

/* 
**********************************
**********************************
*/

void xf_t::SetFormatIndex(unsigned16_t formatidx)
{
	// Set the related flag.
	if(formatidx != xf_t::xfiInit.formatIndex)
	{
		SetFlag(XF_ALIGN_ATRNUM);
	}
	formatIndex = formatidx;
	format = NULL;
}
void xf_t::SetFormat(format_number_t fmt)
{
	unsigned16_t	idx;

	if(fmt > FMT_TEXT) 
	{
		fmt = FMT_GENERAL;
	}
	idx = format_t::format2index(fmt);

	// Set the related flag.
	if(idx != xf_t::xfiInit.formatIndex) 
	{
		SetFlag(XF_ALIGN_ATRNUM);
	}
	formatIndex = idx;
	format = NULL;
}

void xf_t::SetFormat(format_t *fmt)
{
	if (!fmt)
		return;

	if (format)
	{
		format->UnMarkUsed();
	}

	unsigned16_t idx = fmt->GetIndex();

	// Set the related flag.
	if(idx != xf_t::xfiInit.formatIndex)
	{
		SetFlag(XF_ALIGN_ATRNUM);
	}

	formatIndex = idx;
	format = fmt;
	
	format->MarkUsed();
	//cerr << "ndx=" << formatIndex << endl << flush;
}

unsigned16_t xf_t::GetFormatIndex(void) const
{
	return formatIndex;
}

format_number_t xf_t::GetFormat(void) const
{
	int format;
	
	for(format=FMT_GENERAL; format<=FMT_TEXT; ++format) 
	{
		if(formatIndex == format_t::format2index((format_number_t)format))
			return (format_number_t)format;
	}
	return FMT_GENERAL;	// should never get here...
}

std::string xf_t::Description() const
{
	basic_ostringstream<char>	buf;

	buf << "-----------------------------------------" << endl;
		
	buf << "      INDEX: " << index << " usage=" <<  m_usage_counter << endl;
    buf << "       Font: " << hex << font << dec << endl;
    buf << "  FormatIdx: " << formatIndex << endl;
    buf << "      Align: " << "h=" << hex << (int)halign << " v=" << (int)valign << " indent=" << (int)indent << " orient=" << (int)txt_orient << dec << endl;
	buf << "       Fill: " << "fgClr=" << (int)fill_fgcolor << " bgClr=" << (int)fill_bgcolor << " style=" << (int)fillstyle << dec << endl;
	buf << "  TopBorder: " << "style=" << hex << (int)border_style[BORDER_TOP] << " color=" << (int)border_color[BORDER_TOP] << dec << endl;
	buf << "  BotBorder: " << "style=" << hex << (int)border_style[BORDER_BOTTOM] << " color=" << (int)border_color[BORDER_BOTTOM] << dec << endl;
	buf << " LeftBorder: " << "style=" << hex << (int)border_style[BORDER_LEFT] << " color=" << (int)border_color[BORDER_LEFT] << dec << endl;
	buf << "RightBorder: " << "style=" << hex << (int)border_style[BORDER_RIGHT] << " color=" << (int)border_color[BORDER_RIGHT] << dec << endl;
	buf << "      Logic: " << "locked=" << locked << " hidden=" << hidden << " wrap=" << wrap << " isCell=" << is_cell << " isUserXF=" << is_userXF << endl;
	buf << "      FLAGS: " << hex << (int)flags << dec << endl;

	return buf.str();
}

/* 
**********************************
**********************************
*/

/* Cell option wrappers*/
void xf_t::SetBorderStyle(border_side_t side, border_style_t style)
{
	border_style[side] = (border_style_t)BORDERSTYLE_OPTIONS_TABLE[style];

	if(border_style[side] != xf_t::xfiInit.border_style[side]) {
		SetFlag(XF_ALIGN_ATRBDR);
	}
}
void xf_t::SetBorderColor(border_side_t side, color_name_t color) 
{
	border_color[side] = COLOR_OPTIONS_TABLE[color];

	if(border_color[side] != xf_t::xfiInit.border_color[side]) {
		SetFlag(XF_ALIGN_ATRBDR);
	}
}
void xf_t::SetBorderColor(border_side_t side, unsigned8_t color) 
{
	border_color[side] = color;

	if(border_color[side] != xf_t::xfiInit.border_color[side]) {
		SetFlag(XF_ALIGN_ATRBDR);
	}
}

/* 
**********************************
**********************************
*/
unsigned8_t xf_t::GetBorderStyle(border_side_t side) const
{
	return border_style[side];
}

/* 
**********************************
**********************************
*/
unsigned16_t xf_t::GetBorderColorIdx(border_side_t side) const
{
	return border_color[side];
}

xf_t& xf_t::operator=(const xf_t& right)
{
	index		= 0x0000;
	font		= right.font;
	formatIndex = right.formatIndex;

	halign = right.halign;
	valign = right.valign;
	indent = right.indent;

	txt_orient		= right.txt_orient;  
	fill_fgcolor	= right.fill_fgcolor;
	fill_bgcolor	= right.fill_bgcolor;
	fillstyle		= right.fillstyle;       

	locked		= right.locked; 
	hidden		= right.hidden; 
	wrap		= right.wrap;  
	is_cell		= right.is_cell;   
	is_userXF	= right.is_userXF;

	for(int i=0; i<_NUM_BORDERS; ++i) {
		border_style[i]	= right.border_style[i];
		border_color[i] = right.border_color[i];
	}

	return *this;
}
bool xf_t::operator==(const xf_t& right)
{
	return this->xfi == right.xfi;
}

/* Horizontal Align option wrappers*/
void xf_t::SetHAlign(halign_option_t ha_option)
{
	// Set the related flag.
	if(ha_option != xf_t::xfiInit.halign)
		SetFlag(XF_ALIGN_ATRALC);

	halign = xf_t::HALIGN_OPTIONS_TABLE[ha_option];
}
 
unsigned8_t xf_t::GetHAlign(void) const
{
   return halign;
}

/* Vertical Align option wrappers*/
void xf_t::SetVAlign(valign_option_t va_option)
{
	// Set the related flag.
	if(va_option != xf_t::xfiInit.valign)
		SetFlag(XF_ALIGN_ATRALC);

	valign = xf_t::VALIGN_OPTIONS_TABLE[va_option];
}

unsigned8_t xf_t::GetVAlign(void) const
{
   return valign;
}

void xf_t::SetIndent(indent_option_t indent_option)
{
	// Set the related flag.
	if(indent_option != xf_t::xfiInit.indent)
		SetFlag(XF_ALIGN_ATRALC);

	indent = xf_t::INDENT_OPTIONS_TABLE[indent_option];
}
unsigned8_t xf_t::GetIndent(void) const
{
   return indent;
}

/* Text orientation option wrappers*/
void xf_t::SetTxtOrientation(txtori_option_t ori_option)  
{
	// Set the related flag.
	if(ori_option != xf_t::xfiInit.txt_orient)
		SetFlag(XF_ALIGN_ATRALC);

	txt_orient = xf_t::TXTORI_OPTIONS_TABLE[ori_option];
}

unsigned8_t xf_t::GetTxtOrientation(void) const
{
   return txt_orient;
}

/* Fill Foreground color option wrappers*/
void xf_t::SetFillFGColor(color_name_t color)
{
	// Set the related flag.
	if(color != xf_t::xfiInit.fill_fgcolor)
		SetFlag(XF_ALIGN_ATRPAT);

	fill_fgcolor = xf_t::COLOR_OPTIONS_TABLE[color];
}
void xf_t::SetFillFGColor(unsigned8_t color) 
{
	SetFlag(XF_ALIGN_ATRPAT);

	fill_fgcolor = color;
}

unsigned16_t xf_t::GetFillFGColorIdx(void) const
{
   return fill_fgcolor;
}


/* Fill Background color option wrappers*/
void xf_t::SetFillBGColor(color_name_t color)
{
	// Set the related flag.
	if(color != xf_t::xfiInit.fill_bgcolor)
		SetFlag(XF_ALIGN_ATRPAT);

	fill_bgcolor = xf_t::COLOR_OPTIONS_TABLE[color];
}
void xf_t::SetFillBGColor(unsigned8_t color)
{
	SetFlag(XF_ALIGN_ATRPAT);

	fill_bgcolor = color;
}

unsigned16_t xf_t::GetFillBGColorIdx(void) const
{
   return fill_bgcolor;
}

/* Fill Style option wrappers*/
void xf_t::SetFillStyle(fill_option_t fill)
{
	// Set the related flag.
	if(fill != xf_t::xfiInit.fillstyle)
		SetFlag(XF_ALIGN_ATRPAT);

	fillstyle = xf_t::FILL_OPTIONS_TABLE[fill];
}

unsigned8_t xf_t::GetFillStyle(void) const
{
   return fillstyle;
}

/* Locked option wrappers*/
void xf_t::SetLocked(bool locked_opt)
{
	// Set the related flag.
	if(locked_opt != xf_t::xfiInit.locked)
		SetFlag(XF_ALIGN_ATRPROT);

	locked = locked_opt;
}

bool xf_t::IsLocked(void) const
{
   return locked;
}

/* Hidden option wrappers*/
void xf_t::SetHidden(bool hidden_opt)
{
	// Set the related flag.
	if(hidden_opt != xf_t::xfiInit.hidden)
		SetFlag(XF_ALIGN_ATRPROT);

	hidden = hidden_opt;
}
bool xf_t::IsHidden(void) const
{
   return hidden;
}

/* Wrap option wrappers*/
void xf_t::SetWrap(bool wrap_opt)
{
	// Set the related flag.
	if(wrap_opt != xf_t::xfiInit.wrap)
		SetFlag(XF_ALIGN_ATRALC);

	wrap = wrap_opt;
}

bool xf_t::IsWrap(void) const
{
   return wrap;
}

/* Cell option wrappers*/
void xf_t::SetCellMode(bool cellmode)
{
   is_cell = cellmode;
}

bool xf_t::IsCell(void) const
{
   return is_cell;
}


CExtFormat::CExtFormat(CDataStorage &datastore, const xf_t* xfdef):
		CRecord(datastore)
{
	//cerr << "CExtFormat:" << endl << xfdef->Description() << endl;

	SetRecordType(RECTYPE_XF);
	InitDummy(xfdef->IsCell());
	SetRecordLength(GetDataSize()-4);
	  
	SetFontIndex(xfdef->GetFontIndex());
	SetFormatIndex(xfdef->GetFormatIndex());
	SetHorizAlign(xfdef->GetHAlign());
	SetVertAlign(xfdef->GetVAlign());
	SetIndent(xfdef->GetIndent());
	SetTxtOrientation(xfdef->GetTxtOrientation());

	SetFGColorIndex(xfdef->GetFillFGColorIdx());
	SetBGColorIndex(xfdef->GetFillBGColorIdx());
	SetFillPattern(xfdef->GetFillStyle());

	if(xfdef->IsLocked())	SetLocked();
	if(xfdef->IsHidden())	SetHidden(); 
	if(xfdef->IsWrap())		SetWrap(); 

	SetBorder(BORDER_BOTTOM, xfdef->GetBorderStyle(BORDER_BOTTOM), 
			 xfdef->GetBorderColorIdx(BORDER_BOTTOM));
	SetBorder(BORDER_TOP, xfdef->GetBorderStyle(BORDER_TOP), 
			 xfdef->GetBorderColorIdx(BORDER_TOP));
	SetBorder(BORDER_LEFT, xfdef->GetBorderStyle(BORDER_LEFT), 
			 xfdef->GetBorderColorIdx(BORDER_LEFT));
	SetBorder(BORDER_RIGHT, xfdef->GetBorderStyle(BORDER_RIGHT), 
			 xfdef->GetBorderColorIdx(BORDER_RIGHT));

	SetFlags(xfdef->GetFlags());
}

CExtFormat::~CExtFormat()
{
}

/* 
**********************************
**********************************
*/
void CExtFormat::InitDummy(bool is_cell)
{

   // An style-XF record is initialized as below
   // Each field has to be modified individually before use it

   //The default style is a dummy. The flags that indicate what the style affects (byte 11)
   // are disabled (set to 1).
   static const unsigned8_t xfCellDefault[] = {
/*    0         2         4         6         8         10        12        14         16       18        20 */
      0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
   };
   static const unsigned8_t xfStyleDefault[] = {
      // Open Office offsets
/*    0         2         4         6         8         10        12        14         16       18        20 */
      0x00,0x00,0x00,0x00,0xf5,0xff,0x20,0x00,0x00,0xfc,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xc0,0x20
						  // STYLE_XF | INDEX=0xFFF -> Style
									// HALIGN -> General, VALIGN -> BOTTOM
												  // Cells, use this value, Style, not used
																						   // Fill Pattern -> 1 Black, left shifted 2 bits, (well, fully colored, not a pattern)
																								// 0x40 Pattern Color, then lowest bit of 0x41 (next)
																									 // 0x41 Pattern background color right shifted one bit
   };
    
   if(is_cell)
   {
	  AddDataArray(xfCellDefault, sizeof(xfCellDefault));
   } else {
      AddDataArray(xfStyleDefault, sizeof(xfStyleDefault));
   }
}

/* 
**********************************
CExtFormat class implementation
**********************************
*/

/* 
**********************************
**********************************
*/
bool CExtFormat::IsCell()
{
   unsigned16_t val;

   GetValue16From((signed16_t*)&val, XF_OFFSET_PROP);

   return((val&XF_PROP_STYLE) ? false : true);
}

/* 
**********************************
**********************************
*/
int CExtFormat::SetFontIndex(unsigned16_t fontindex)
{
   // Set the index value
   int errcode = SetValueAt16((unsigned16_t)fontindex, XF_OFFSET_FONT);

   return errcode;
}

/* 
**********************************
**********************************
*/
unsigned16_t CExtFormat::GetFontIndex(void)
{
   unsigned16_t fontval;
   GetValue16From((signed16_t*)&fontval, XF_OFFSET_FONT);

   return(fontval);
}

/* 
**********************************
**********************************
*/
int CExtFormat::SetFormatIndex(unsigned16_t formatindex)
{
   // Set the index value
   int errcode = SetValueAt16((unsigned16_t)formatindex, XF_OFFSET_FORMAT);
   
   return errcode;
}

/* 
**********************************
**********************************
*/
unsigned16_t CExtFormat::GetFormatIndex(void)
{
   unsigned16_t formatval;
   GetValue16From((signed16_t*)&formatval, XF_OFFSET_FORMAT);

   return(formatval);
}

/* 
**********************************
**********************************
*/
void CExtFormat::SetLocked()
{
   unsigned16_t value;

   GetValue16From((signed16_t*)&value, XF_OFFSET_PROP);
   value |= XF_PROP_LOCKED;
   SetValueAt16((unsigned16_t)value, XF_OFFSET_PROP);
}

/* 
**********************************
**********************************
*/
void CExtFormat::SetHidden()
{
   unsigned16_t value;

   GetValue16From((signed16_t*)&value, XF_OFFSET_PROP);
   value |= XF_PROP_HIDDEN;
   SetValueAt16((unsigned16_t)value, XF_OFFSET_PROP);
}

/* 
**********************************
**********************************
*/
void CExtFormat::SetHorizAlign(unsigned8_t alignval)
{
   unsigned32_t value;
   
   GetValue32From((signed32_t*)&value, XF_OFFSET_ALIGN);
   value = (value&(~XF_ALIGN_HORIZONTAL))|(alignval & XF_ALIGN_HORIZONTAL);
   SetValueAt32((unsigned32_t)value, XF_OFFSET_ALIGN);
}

/* 
**********************************
**********************************
*/
void CExtFormat::SetVertAlign(unsigned8_t alignval)
{
   unsigned32_t value, alignval32;

   GetValue32From((signed32_t*)&value, XF_OFFSET_ALIGN);
   alignval32 = (unsigned32_t)alignval << XF_ALIGN_SHIFTPOS_VALIGN;				// Place the option at the right bit position
   value = (value&(~XF_ALIGN_VERTICAL))|(alignval32 & XF_ALIGN_VERTICAL);
   SetValueAt32((unsigned32_t)value, XF_OFFSET_ALIGN);
}

/* 
**********************************
**********************************
*/
void CExtFormat::SetWrap()
{
   unsigned32_t value;

   GetValue32From((signed32_t*)&value, XF_OFFSET_ALIGN);
   value |= XF_ALIGN_WRAP;
   SetValueAt32((unsigned32_t)value, XF_OFFSET_ALIGN);
}
/* 
**********************************
**********************************
*/
void CExtFormat::SetIndent(unsigned8_t indentval)
{
	unsigned32_t value, mask;

	if(indentval & XF_INDENT_LVL) {
		mask = XF_INDENT_LVL;
	} else
	if(indentval & XF_INDENT_SHRINK2FIT) {
		mask = XF_INDENT_SHRINK2FIT;
	} else
	if(indentval & (XF_INDENT_CONTEXT|XF_INDENT_L2R|XF_INDENT_R2L)) {
		mask = XF_INDENT_DIR; 
	} else {
		mask = XF_INDENT_LVL | XF_INDENT_SHRINK2FIT | XF_INDENT_DIR;
	}
		
	mask <<= XF_INDENT_SHIFTPOS;

	GetValue32From((signed32_t*)&value, XF_OFFSET_ALIGN);

	unsigned32_t indentval32 = (unsigned32_t)indentval << XF_INDENT_SHIFTPOS; // Place the option at the right bit position
	value = (value&(~mask))|(indentval32 & mask);

	SetValueAt32((unsigned32_t)value, XF_OFFSET_ALIGN);
}

/* 
**********************************
**********************************
*/
void CExtFormat::SetTxtOrientation(unsigned8_t alignval)
{
   unsigned32_t value;

   GetValue32From((signed32_t*)&value, XF_OFFSET_ALIGN);
   unsigned32_t alignval32 = alignval;
   alignval32 <<= XF_ORI_SHIFTPOS; // Place the option at the right bit position

   value = (value&(~XF_ORI_MASK))|(alignval32 & XF_ORI_MASK);
   SetValueAt32((unsigned32_t)value, XF_OFFSET_ALIGN);
}

/* 
**********************************
**********************************
*/
void CExtFormat::SetFGColorIndex(unsigned16_t color)
{
   unsigned16_t value;
   
   GetValue16From((signed16_t*)&value, XF_OFFSET_COLOR);
   
   // Clear the field for Foreground color
   value &= (~XF_COLOR_FOREGROUND);
   // Set the new color
   value |= (color & XF_COLOR_FOREGROUND);

   SetValueAt16((unsigned16_t)value, XF_OFFSET_COLOR);
}

/* 
**********************************
**********************************
*/
void CExtFormat::SetBGColorIndex(unsigned16_t color)
{
   unsigned16_t value;
   
   color <<= XF_COLOR_SHIFTPOS_BG;
  
   GetValue16From((signed16_t*)&value, XF_OFFSET_COLOR);

   // Clear the field for Foreground color
   value &= (~XF_COLOR_BACKGROUND);
   // Set the new color
   value |= (color & XF_COLOR_BACKGROUND);

   SetValueAt16((unsigned16_t)value, XF_OFFSET_COLOR);
}

/* 
**********************************
**********************************
*/
void CExtFormat::SetFillPattern(unsigned8_t pattern)
{
	unsigned32_t value, pattern32 = pattern;
	GetValue32From((signed32_t*)&value, XF_OFFSET_BORDERB);

	value &= ~ XF_BORDER_FILLPATTERN;

	pattern32 <<= XF_SHIFTPOS_FILLPATTERN;
	value |= (pattern32 & XF_BORDER_FILLPATTERN);
	SetValueAt32((unsigned32_t)value, XF_OFFSET_BORDERB);
}


/* 
**********************************
**********************************
*/
void CExtFormat::SetBorder(border_side_t border, unsigned16_t style, unsigned16_t color)
{
	switch(border) 
	{
	case BORDER_BOTTOM:
		{
		 signed32_t value, color32 = color, style32 = style;

		 GetValue32From((signed32_t*)&value, XF_OFFSET_BORDERA);
		 value &= (~XF_BORDER_BOTTOMSTYLE);
		 style32 <<= XF_STYLE_SHIFTPOS_BOTTOM;
		 value |= (style32 & XF_BORDER_BOTTOMSTYLE);
		 SetValueAt32((unsigned32_t)value, XF_OFFSET_BORDERA);

		 GetValue32From((signed32_t*)&value, XF_OFFSET_BORDERB);
		 value &= (~XF_BORDER_BOTTOMCOLOR);
		 color32 <<= XF_COLOR_SHIFTPOS_BOTTOM;
		 value |= (color32 & XF_BORDER_BOTTOMCOLOR);
		 SetValueAt32((unsigned32_t)value, XF_OFFSET_BORDERB);
		}
		break;
	case BORDER_TOP:
		{
		 signed32_t value, color32 = color, style32 = style;

		 GetValue32From((signed32_t*)&value, XF_OFFSET_BORDERA);
		 value &= (~XF_BORDER_TOPSTYLE);
		 style32 <<= XF_STYLE_SHIFTPOS_TOP;
		 value |= (style32 & XF_BORDER_TOPSTYLE);
		 SetValueAt32((unsigned32_t)value, XF_OFFSET_BORDERA);

		 GetValue32From((signed32_t*)&value, XF_OFFSET_BORDERB);
		 value &= (~XF_BORDER_TOPCOLOR);
		 color32 <<= XF_COLOR_SHIFTPOS_TOP;
		 value |= (color32 & XF_BORDER_TOPCOLOR);
		 SetValueAt32((unsigned32_t)value, XF_OFFSET_BORDERB);
		}
		break;
	case BORDER_LEFT:   
		{
		 unsigned32_t value, color32 = color, style32 = style;

		 GetValue32From((signed32_t*)&value, XF_OFFSET_BORDERA);
		 value &= ~(XF_BORDER_LEFTSTYLE|XF_BORDER_LEFTCOLOR);

		 color32 <<= XF_COLOR_SHIFTPOS_LEFT;
		 style32 <<= XF_STYLE_SHIFTPOS_LEFT;
		 value |= (color32 & XF_BORDER_LEFTCOLOR) | (style32 & XF_BORDER_LEFTSTYLE);

		 SetValueAt32((unsigned32_t)value, XF_OFFSET_BORDERA);
		}
		break;
	case BORDER_RIGHT:  
		{
		 unsigned32_t value, color32 = color, style32 = style;

		 GetValue32From((signed32_t*)&value, XF_OFFSET_BORDERA);
		 value &= ~(XF_BORDER_RIGHTSTYLE|XF_BORDER_RIGHTCOLOR);

		 color32 <<= XF_COLOR_SHIFTPOS_RIGHT;
		 style32 <<= XF_STYLE_SHIFTPOS_RIGHT;
		 value |= (color32 & XF_BORDER_RIGHTCOLOR) | (style32 & XF_BORDER_RIGHTSTYLE);

		 SetValueAt32((unsigned32_t)value, XF_OFFSET_BORDERA);
		}
		break;

	default:
		break;
	}
}

/* 
**********************************
**********************************
*/
void CExtFormat::SetFlags(unsigned8_t flags)
{
   unsigned32_t value, flags32;

   flags32 = (unsigned32_t)flags << XF_ALIGN_ATR_SHIFT;

   GetValue32From((signed32_t*)&value, XF_OFFSET_ALIGN);
   value = (value&(~XF_ALIGN_ATR_MASK))|(flags32 & XF_ALIGN_ATR_MASK);
   SetValueAt32((unsigned32_t)value, XF_OFFSET_ALIGN);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: extformat.cpp,v $
 * Revision 1.10  2009/03/02 04:08:43  dhoerl
 * Code is now compliant to gcc  -Weffc++
 *
 * Revision 1.9  2009/01/23 16:09:55  dhoerl
 * General cleanup: headers and includes. Fixed issues building mainC and mainCPP
 *
 * Revision 1.8  2009/01/10 21:10:50  dhoerl
 * More tweaks
 *
 * Revision 1.7  2009/01/09 15:04:26  dhoerl
 * GlobalRec now used only as a reference.
 *
 * Revision 1.6  2009/01/09 03:23:12  dhoerl
 * GlobalRec references tuning
 *
 * Revision 1.5  2009/01/08 02:53:15  dhoerl
 * December Rework
 *
 * Revision 1.4  2008/12/20 15:49:05  dhoerl
 * 1.2.5 fixes
 *
 * Revision 1.3  2008/12/06 01:42:57  dhoerl
 * John Peterson changes along with lots of tweaks. Many bugs that causes Excel crashes fixed.
 *
 * Revision 1.2  2008/10/25 18:39:54  dhoerl
 * 2008
 *
 * Revision 1.1.1.1  2004/08/27 16:31:48  darioglz
 * Initial Import.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

