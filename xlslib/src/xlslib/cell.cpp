/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * This file is part of xlslib -- A multiplatform, C/C++ library
 * for dynamic generation of Excel(TM) files.
 *
 * Copyright 2004 Yeico S. A. de C. V. All Rights Reserved.
 * Copyright 2008-2011 David Hoerl All Rights Reserved.
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
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "xlslib/record.h"
#include "xlslib/cell.h"
#include "xlslib/globalrec.h"

using namespace xlslib_core;

cell_t::cell_t(CGlobalRecords& gRecord, unsigned32_t rowNum, unsigned32_t colNum, xf_t* pxfval) :
	m_GlobalRecords(gRecord),
	pxf(pxfval ? pxfval : gRecord.GetDefaultXF()),
	row(rowNum), col(colNum)
{
	XL_ASSERT(pxf);
	pxf->MarkUsed();

	//cerr << "INITIAL XFT" << endl << pxf->Description() << endl;
}

cell_t::~cell_t()
{
	pxf->UnMarkUsed();
}

unsigned16_t cell_t::GetXFIndex() const
{
	XL_ASSERT(pxf);
	//return pxf ? pxf->GetIndex() : XF_PROP_XF_DEFAULT_CELL;
	return pxf->GetIndex();
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// Implementation of the XF record interface (xf_i pure virtual interface)
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

// This is why the default formats get marked used twice! (see test for > 1)

void cell_t::set_xf_common(void)
{
	if(pxf->Usage() > 1) {
		/* not sole user	*/
		pxf->UnMarkUsed();
		pxf = xf_t::xfDup(pxf);
		pxf->MarkUsed();
	}
}

void cell_t::borderstyle(border_side_t side, border_style_t style)
{
	set_xf_common();
	pxf->SetBorderStyle(side, style);
}

void cell_t::bordercolor(border_side_t side, color_name_t color)
{
	set_xf_common();
	pxf->SetBorderColor(side, color);
}

void cell_t::bordercolor(border_side_t side, unsigned8_t color)
{
	set_xf_common();
	pxf->SetBorderColor(side, color);
}

void cell_t::font(font_t* fnt)
{
	set_xf_common();
	pxf->SetFont(fnt);
}

void cell_t::format(format_number_t formatidx)
{
	set_xf_common();
	pxf->SetFormat(formatidx);
}

void cell_t::format(format_t* fmt)
{
	set_xf_common();
	pxf->SetFormat(fmt);
}

void cell_t::halign(halign_option_t ha_option)
{
	set_xf_common();
	pxf->SetHAlign(ha_option);
}

void cell_t::valign(valign_option_t va_option)
{
	set_xf_common();
	pxf->SetVAlign(va_option);
}

void cell_t::indent(indent_option_t indent_option)
{
	set_xf_common();
	pxf->SetIndent(indent_option);
}

void cell_t::orientation(txtori_option_t ori_option)
{
	set_xf_common();
	pxf->SetTxtOrientation(ori_option);
}

void cell_t::fillfgcolor(color_name_t color)
{
	set_xf_common();
	pxf->SetFillFGColor(color);
}

void cell_t::fillfgcolor(unsigned8_t color)
{
	set_xf_common();
	pxf->SetFillFGColor(color);
}

void cell_t::fillbgcolor(color_name_t color)
{
	set_xf_common();
	pxf->SetFillBGColor(color);
}

void cell_t::fillbgcolor(unsigned8_t color)
{
	set_xf_common();
	pxf->SetFillBGColor(color);
}

void cell_t::fillstyle(fill_option_t fill)
{
	set_xf_common();
	pxf->SetFillStyle(fill);
}

void cell_t::locked(bool locked_opt)
{
	set_xf_common();
	pxf->SetLocked(locked_opt);
}

void cell_t::hidden(bool hidden_opt)
{
	set_xf_common();
	pxf->SetHidden(hidden_opt);
}

void cell_t::wrap(bool wrap_opt)
{
	set_xf_common();
	pxf->SetWrap(wrap_opt);
}

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
// Implementation of the FONT record interface (font_i pure virtual interface)
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
void cell_t::set_cell_font(void)
{
	set_xf_common();

	font_t* currentfont = pxf->GetFont();

	if(currentfont == NULL)	{
		currentfont = m_GlobalRecords.GetDefaultFont();
		font_t* fntnew = font_t::fontDup(currentfont);
		pxf->SetFont(fntnew);
	} else
	if(currentfont->Usage() > 1) {
		font_t* fntnew = font_t::fontDup(currentfont);
		pxf->SetFont(fntnew);
	}
}

void cell_t::fontname(const std::string& fntname)
{
	set_cell_font();
	pxf->GetFont()->SetName(fntname);
}

void cell_t::fontheight(unsigned16_t fntheight)
{
	set_cell_font();
	pxf->GetFont()->SetHeight(fntheight);
}

void cell_t::fontbold(boldness_option_t fntboldness)
{
	set_cell_font();
	pxf->GetFont()->SetBoldStyle(fntboldness);
}

void cell_t::fontunderline(underline_option_t fntunderline)
{
	set_cell_font();
	pxf->GetFont()->SetUnderlineStyle(fntunderline);
}

void cell_t::fontscript(script_option_t fntscript)
{
	set_cell_font();
	pxf->GetFont()->SetScriptStyle(fntscript);
}

void cell_t::fontcolor(color_name_t fntcolor)
{
	set_cell_font();
	pxf->GetFont()->SetColor(fntcolor);
}

void cell_t::fontcolor(unsigned8_t fntcolor)
{
	set_cell_font();
	pxf->GetFont()->SetColor(fntcolor);
}

#if defined(DEPRECATED)
void cell_t::fontattr(unsigned16_t attr)
{
	set_cell_font();
	pxf->GetFont()->SetAttributes(attr);
}

#endif

void cell_t::fontitalic(bool italic)
{
	set_cell_font();
	pxf->GetFont()->SetItalic(italic);
}

void cell_t::fontstrikeout(bool so)
{
	set_cell_font();
	pxf->GetFont()->SetStrikeout(so);
}

void cell_t::fontoutline(bool ol)
{
	set_cell_font();
	pxf->GetFont()->SetOutline(ol);
}

void cell_t::fontshadow(bool sh)
{
	set_cell_font();
	pxf->GetFont()->SetShadow(sh);
}

void cell_t::SetXF(xf_t* pxfval)
{
	if(!pxfval) {return; }

	XL_ASSERT(pxf);
	pxfval->MarkUsed();
	pxf->UnMarkUsed();
	pxf = pxfval;
}

xf_t* cell_t::GetXF(void) const {return pxf; }
