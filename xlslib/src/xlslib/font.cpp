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
 * $Source: /cvsroot/xlslib/xlslib/src/xlslib/font.cpp,v $
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

#include <font.h>
#include <globalrec.h>

using namespace xlslib_core;

/* 
**********************************
font_t class implementation
**********************************
*/
const unsigned16_t font_t::BOLD_OPTION_TABLE[] = {
	FONT_BOLDNESS_BOLD,
	FONT_BOLDNESS_HALF,
	FONT_BOLDNESS_NORMAL,
	FONT_BOLDNESS_DOUBLE 
};

const unsigned16_t font_t::SCRIPT_OPTION_TABLE[] = {
	FONT_SCRIPT_NONE,
	FONT_SCRIPT_SUPER,
	FONT_SCRIPT_SUB
};

const unsigned8_t font_t::UNDERLINE_OPTION_TABLE[] = {
	FONT_UNDERLINE_NONE,
	FONT_UNDERLINE_SINGLE,
	FONT_UNDERLINE_DOUBLE,
	FONT_UNDERLINE_SINGLEACC,
	FONT_UNDERLINE_DOUBLEACC
};

const unsigned8_t font_t::COLOR_OPTION_TABLE[] =
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


font_t::font_t(CGlobalRecords& gRecords) :
   m_GlobalRecords(gRecords),
   name(FONT_DFLT_FONTNAME),
   index(0x0000),
   height(FONT_DFLT_HEIGHT),
   boldstyle(FONT_BOLDNESS_NORMAL),
   script(FONT_SCRIPT_NONE),
   attributes(FONT_DFLT_ATTRIBUTES),
   color(FONT_DFLT_PALETTE),
   underline(FONT_UNDERLINE_NONE),
   family(FONT_DFLT_FAMILY),
   charset(FONT_DFLT_CHARSET),
   m_usage_counter(0)
{ 
}

font_t::font_t(const font_t& right) :
   m_GlobalRecords(right.m_GlobalRecords),
   name(right.name),
   index(0x0000),
   height(right.height),
   boldstyle(right.boldstyle),
   script(right.script),
   attributes(right.attributes),
   color(right.color),
   underline(right.underline),
   family(right.family),
   charset(right.charset),
   m_usage_counter(0)
{
	m_GlobalRecords.AddFont(this);
}
// only used by globalRec for defaults
font_t::font_t(CGlobalRecords& gRecords,
		unsigned16_t index_,
		const std::string& name_,
		unsigned16_t height_,
		boldness_option_t boldstyle_,
		underline_option_t underline_,
		script_option_t script_,
		color_name_t color_,
		unsigned16_t attributes_,
		unsigned8_t family_,
		unsigned8_t charset_) :
	m_GlobalRecords(gRecords),
	//index(0x0000),
	//name(FONT_DFLT_FONTNAME),
	//height(FONT_DFLT_HEIGHT),
	//boldstyle(FONT_BOLDNESS_NORMAL),
	//script(FONT_SCRIPT_NONE),
	attributes(attributes_),
	//color(FONT_DFLT_PALETTE),
	//underline(FONT_UNDERLINE_NONE),
	//family(FONT_DFLT_FAMILY),
	//charset(FONT_DFLT_CHARSET),
	m_usage_counter(0)
{
   SetIndex(index_);
   SetName(name_);
   SetHeight(height_);
   SetBoldStyle(boldstyle_);
   SetUnderlineStyle(underline_);
   SetScriptStyle(script_);
   SetColor(color_);
   SetFamily(family_);
   SetCharset(charset_);
}

font_t &font_t::operator =(const font_t &src)
{
	throw std::string("Should never have invoked the font_t copy operator!");
}


/* 
**********************************
**********************************
*/
void font_t::MarkUsed(void) 
{
   m_usage_counter++;
}
void font_t::UnMarkUsed(void) 
{
   if(m_usage_counter)
      m_usage_counter--;
}
unsigned32_t font_t::Usage(void) const
{
   return m_usage_counter;
}
/* 
**********************************
**********************************
*/
void font_t::SetItalic(bool italic) {
   if(italic)
      attributes |= FONT_ATTR_ITALIC;
   else
      attributes &= (~FONT_ATTR_ITALIC);

  // m_sigchanged = true;
}

void font_t::SetStrikeout(bool so) {
   if(so)
      attributes |= FONT_ATTR_STRIKEOUT;
   else
      attributes &= (~FONT_ATTR_STRIKEOUT);

  // m_sigchanged = true;
}

// OSX (Mac) only
void font_t::SetOutline(bool ol) {
   if(ol)
      attributes |= FONT_ATTR_OUTLINEMACH;
   else
      attributes &= (~FONT_ATTR_OUTLINEMACH);

  // m_sigchanged = true;
}

// OSX (Mac) only
void font_t::SetShadow(bool sh) {
   if(sh)
      attributes |= FONT_ATTR_SHADOWMACH;
   else
      attributes &= (~FONT_ATTR_SHADOWMACH);

  // m_sigchanged = true;
}


/* FONT Index wrappers*/
void font_t::SetIndex(unsigned16_t fntidx) 
{
   index = fntidx;
  // m_sigchanged = true;
}

unsigned16_t font_t::GetIndex(void) const
{
   return index;
}

/* FONT Index wrappers*/
void font_t::SetName(const std::string& fntname) 
{
   name = fntname;
 //  m_sigchanged = true;
}

/* FONT height wrappers*/
void font_t::SetHeight(unsigned16_t fntheight) 
{
   height = fntheight;
 //  m_sigchanged = true;
}
unsigned16_t font_t::GetHeight(void) const
{
   return height;
}

/* FONT boldstyle wrappers*/
void font_t::SetBoldStyle(boldness_option_t fntboldness) 
{
   boldstyle = font_t::BOLD_OPTION_TABLE[fntboldness];
 //  m_sigchanged = true;
}
void font_t::_SetBoldStyle(unsigned16_t fntboldness) 
{
	XL_ASSERT(fntboldness >= 100);
	XL_ASSERT(fntboldness <= 1000);
   boldstyle = fntboldness;
 //  m_sigchanged = true;
}
unsigned16_t font_t::GetBoldStyle(void) const
{
   return boldstyle;
}

/* FONT underline wrappers*/
void font_t::SetUnderlineStyle(underline_option_t fntunderline) 
{
   underline = font_t::UNDERLINE_OPTION_TABLE[fntunderline];
  // m_sigchanged = true;
}
unsigned8_t font_t::GetUnderlineStyle(void) const
{
   return underline;
}

/* FONT script wrappers*/
void font_t::SetScriptStyle(script_option_t fntscript) 
{
   script = font_t::SCRIPT_OPTION_TABLE[fntscript];
 //  m_sigchanged = true;
}
unsigned16_t font_t::GetScriptStyle(void) const
{
   return script;
}

/* FONT script wrappers*/
void font_t::SetColor(color_name_t fntcolor) 
{
   color = font_t::COLOR_OPTION_TABLE[fntcolor];
//   m_sigchanged = true;
}
void font_t::SetColor(unsigned8_t fntcolor) 
{
   color = fntcolor;
//   m_sigchanged = true;
}
unsigned16_t font_t::GetColorIdx(void) const
{
   return color;
}

/* FONT  attributes wrappers */
#if defined(DEPRECATED)
void font_t::SetAttributes(unsigned16_t attr) 
{
   attributes = attr;
//   m_sigchanged = true;
}
#endif

unsigned16_t font_t::GetAttributes(void) const
{
   return attributes;
}

// Miscellaneous;
void font_t::SetFamily(unsigned8_t fam) 
{
   family = fam;
  // m_sigchanged = true;
}
unsigned8_t font_t::GetFamily(void) const
{
   return family;
}

void font_t::SetCharset(unsigned8_t chrset) 
{
   charset = chrset;
 //  m_sigchanged = true;
}

unsigned8_t font_t::GetCharset(void) const
{
   return charset;
}

const std::string& font_t::GetName(void) const {return name;};
bool font_t::GetItalic() const {return (attributes & FONT_ATTR_ITALIC) ? true : false; };
bool font_t::GetStrikeout() const {return (attributes & FONT_ATTR_STRIKEOUT) ? true : false; }
bool font_t::GetOutline() const {return (attributes & FONT_ATTR_OUTLINEMACH) ? true : false; };
bool font_t::GetShadow() const {return (attributes & FONT_ATTR_SHADOWMACH) ? true : false; };

/* 
**********************************
CFont class implementation
**********************************
*/
CFont::CFont(CDataStorage &datastore, const font_t* fontdef):
		CRecord(datastore)
{
	SetRecordType(RECTYPE_FONT);

	AddValue16(fontdef->GetHeight());
	AddValue16(fontdef->GetAttributes());
	AddValue16(fontdef->GetColorIdx());
	AddValue16(fontdef->GetBoldStyle());
	AddValue16(fontdef->GetScriptStyle());
	AddValue8(fontdef->GetUnderlineStyle());
	AddValue8(fontdef->GetFamily());
	AddValue8(fontdef->GetCharset());
	AddValue8(FONT_RESERVED);
	AddUnicodeString(fontdef->GetGlobalRecords(), fontdef->GetName(), LEN1_NOFLAGS_ASCII);

	SetRecordLength(GetDataSize()-4);
}

CFont::~CFont()
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: font.cpp,v $
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
 * Revision 1.4  2008/12/10 03:34:02  dhoerl
 * m_usage was 16bit and wrapped
 *
 * Revision 1.3  2008/12/06 01:42:57  dhoerl
 * John Peterson changes along with lots of tweaks. Many bugs that causes Excel crashes fixed.
 *
 * Revision 1.2  2008/10/25 18:39:54  dhoerl
 * 2008
 *
 * Revision 1.1.1.1  2004/08/27 16:31:52  darioglz
 * Initial Import.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

