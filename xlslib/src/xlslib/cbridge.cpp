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
 * Copyright 2008 David Hoerl
 *  
 * $Source: /cvsroot/xlslib/xlslib/src/xlslib/cbridge.cpp,v $
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

#include <xlsys.h>

#include <sys/types.h>
#include <string>

#define CPP_BRIDGE_XLS

#include <common.h> /* RANGE_FEATURE */
#include <xlslib.h>

using namespace std;
using namespace xlslib_core;

extern "C" {
	// Workbook
	workbook *xlsNewWorkbook(void)								{ return new workbook; }
	void xlsDeleteWorkbook(workbook *w)							{ delete w; }
	
	worksheet *xlsWorkbookSheet(workbook *w, const char *sheetname) {
																	std::string str = sheetname;
																	return w->sheet(str);
																}
	worksheet *xlsWorkbookSheetW(workbook *w, const unichar_t *sheetname)
																{
																	std::ustring str = sheetname;
																	return w->sheet(str);
																}
	worksheet *xlsWorkbookGetSheet(workbook *w, unsigned16_t sheetnum)
																{ return w->GetSheet(sheetnum); }

	font_t *xlsWorkbookFont(workbook *w, const char *name)		{
																	std::string str = name;
																	return w->font(str);
																}
	format_t *xlsWorkbookFormat(workbook *w, const char *name)	{
																	std::string str = name;
																	return w->format(str);
																}
	format_t *xlsWorkbookFormatW(workbook *w, const unichar_t *name) {
																	std::ustring str = name;
																	return w->format(str);
																}
	xf_t *xlsWorkbookxFormat(workbook *w)						{ return w->xformat(); }
	xf_t *xlsWorkbookxFormatFont(workbook *w, font_t *font)		{ return w->xformat(font); }


#ifdef HAVE_ICONV
	int xlsWorkbookIconvInType(workbook *w, const char *inType)	{ return w->iconvInType(inType); }
#endif
	uint8_t xlsWorkbookProperty(workbook *w, property_t prop, const char *s)
																{ 
																	std::string str = s;
																	return w->property(prop, str) ? 1 : 0;
																}
	void xlsWorkBookWindPosition(workbook *w, unsigned16_t horz, unsigned16_t vert)
																{ w->windPosition(horz, vert); }
	void xlsWorkBookWindSize(workbook *w, unsigned16_t horz, unsigned16_t vert)
																{ w->windSize(horz, vert); }
	void xlsWorkBookFirstTab(workbook *w, unsigned16_t firstTab)
																{ w->firstTab(firstTab); }
	void xlsWorkBookTabBarWidth(workbook *w, unsigned16_t width)
																{ w->tabBarWidth(width); }
	int xlsWorkbookDump(workbook *w, const char *filename)			{
																	std::string str = filename;
																	return w->Dump(str);
																}
	// Worksheet
	void xlsWorksheetMakeActive(worksheet *w)					{ return w->MakeActive(); }
	cell_t *xlsWorksheetFindCell(worksheet *w, unsigned32_t row, unsigned32_t col)
																{ return w->FindCell(row, col); }
	// Cell operations
	void xlsWorksheetMerge(worksheet *w, unsigned32_t first_row, unsigned32_t first_col, unsigned32_t last_row, unsigned32_t last_col)		
																{ return w->merge(first_row, first_col, last_row, last_col); }
	void xlsWorksheetColwidth(worksheet *w, unsigned32_t col, unsigned16_t width, xf_t* pxformat)
																{ return w->colwidth(col, width, pxformat); }
	void xlsWorksheetRowheight(worksheet *w, unsigned32_t row, unsigned16_t height, xf_t* pxformat)
																{ return w->rowheight(row, height, pxformat); } 
																
#ifdef RANGE_FEATURE
																	// Ranges
	range *xlsWorksheetRangegroup(worksheet *w, unsigned32_t row1, unsigned32_t col1, unsigned32_t row2, unsigned32_t col2)
																{ return w->rangegroup(row1, col1, row2, col2); }
#endif // RANGE_FEATURE

	// Cells
	cell_t *xlsWorksheetLabel(worksheet *w, unsigned32_t row, unsigned32_t col, const char *strlabel, xf_t *pxformat)
																{ 
																	std::string str = strlabel;
																	return w->label(row, col, strlabel, pxformat);
																}
	cell_t *xlsWorksheetLabelW(worksheet *w, unsigned32_t row, unsigned32_t col, const unichar_t *strlabel, xf_t *pxformat)
																{ 
																	std::ustring str = strlabel;
																	return w->label(row, col, strlabel, pxformat);
																}
	cell_t *xlsWorksheetBlank(worksheet *w, unsigned32_t row, unsigned32_t col, xf_t *pxformat)
																{ return w->blank(row, col, pxformat); }

	cell_t *xlsWorksheetNumberDbl(worksheet *w, unsigned32_t row, unsigned32_t col, double numval, xf_t *pxformat)
																{ return w->number(row, col, numval, pxformat); }
	// 536870911 >= numval >= -536870912
	cell_t *xlsWorksheetNumberInt(worksheet *w, unsigned32_t row, unsigned32_t col, signed32_t numval, xf_t *pxformat)
																{ return w->number(row, col, numval, pxformat); }

	cell_t *xlsWorksheetBoolean(worksheet *w, unsigned32_t row, unsigned32_t col, bool boolval, xf_t *pxformat)
																{ return w->boolean(row, col, boolval, pxformat); }

	cell_t *xlsWorksheetError(worksheet *w, unsigned32_t row, unsigned32_t col, errcode_t errval, xf_t *pxformat)
																{ return w->error(row, col, errval, pxformat); }

	cell_t *xlsWorksheetNote(worksheet *w, unsigned32_t row, unsigned32_t col, const char *remark, const char *author, xf_t *pxformat)
																{ 
																	std::string cmt = remark;
																	std::string auth = author;

																	return w->note(row, col, cmt, auth, pxformat); 
																}
	cell_t *xlsWorksheetNoteW(worksheet *w, unsigned32_t row, unsigned32_t col, const unichar_t *remark, const unichar_t *author, xf_t *pxformat)
																{ 
																	std::ustring cmt = remark;
																	std::ustring auth = author;

																	return w->note(row, col, cmt, auth, pxformat); 
																}
																
	// Cells
	// xf_i interface
	void xlsCellFont(cell_t *c, font_t *fontidx)				{ return c->font(fontidx); }
	void xlsCellFormat(cell_t *c, format_number_t format)		{ return c->format(format); }
	void xlsCellFormatP(cell_t *c, format_t *format)			{ return c->format(format); }
	void xlsCellHalign(cell_t *c, halign_option_t ha_option)	{ return c->halign(ha_option); }
	void xlsCellValign(cell_t *c, valign_option_t va_option)	{ return c->valign(va_option); }	
	void xlsCellOrientation(cell_t *c, txtori_option_t ori_option)
																{ return c->orientation(ori_option); }
	void xlsCellFillfgcolor(cell_t *c, color_name_t color)		{ return c->fillfgcolor(color); }
	void xlsCellFillbgcolor(cell_t *c, color_name_t color)		{ return c->fillbgcolor(color); }
	void xlsCellFillstyle(cell_t *c, fill_option_t fill)		{ return c->fillstyle(fill); }
	void xlsCellLocked(cell_t *c, bool locked_opt)				{ return c->locked(locked_opt); }
	void xlsCellHidden(cell_t *c, bool hidden_opt)				{ return c->hidden(hidden_opt); }
	void xlsCellWrap(cell_t *c, bool wrap_opt)					{ return c->wrap(wrap_opt); }
	void xlsCellBorderstyle(cell_t *c, border_side_t side, border_style_t style)
																{ return c->borderstyle(side, style); }
	void xlsCellBordercolor(cell_t *c, border_side_t side, color_name_t color)
																{ return c->bordercolor(side, color); }
	void xlsCellBordercolorIdx(cell_t *c, border_side_t side, unsigned8_t color)
																{ return c->bordercolor(side, color); }
	//font_i interface
	void xlsCellFontname(cell_t *c, const char *fntname)		{
																	std::string str = fntname;
																	c->fontname(str);
																}
	void xlsCellFontheight(cell_t *c, unsigned16_t fntheight)	{ return c->fontheight(fntheight); }
	void xlsCellFontbold(cell_t *c, boldness_option_t fntboldness)
																{ return c->fontbold(fntboldness); }
	void xlsCellFontunderline(cell_t *c, underline_option_t fntunderline)
																{ return c->fontunderline(fntunderline); }
	void xlsCellFontscript(cell_t *c, script_option_t fntscript){ return c->fontscript(fntscript); }
	void xlsCellFontcolor(cell_t *c, color_name_t fntcolor)		{ return c->fontcolor(fntcolor); }
	void xlsCellFontitalic(cell_t *c, bool italic)				{ return c->fontitalic(italic); }
	void xlsCellFontstrikeout(cell_t *c, bool so)				{ return c->fontstrikeout(so); }
	void xlsCellFontoutline(cell_t *c, bool ol)					{ return c->fontoutline(ol); }
	void xlsCellFontshadow(cell_t *c, bool sh)					{ return c->fontshadow(sh); }

	unsigned32_t xlsCellGetRow(cell_t *c)						{ return c->GetRow(); }
	unsigned32_t xlsCellGetCol(cell_t *c)						{ return c->GetCol(); }
#ifdef RANGE_FEATURE	
	// range
	void xlsRangeCellcolor(range *r, color_name_t color)		{ return r->cellcolor(color); }
#endif // RANGE_FEATURE
	
	// xformat
	void xlsXformatSetFont(xf_t *x, font_t* fontidx)			{ return x->SetFont(fontidx); }
	unsigned16_t xlsXformatGetFontIndex(xf_t *x)				{ return x->GetFontIndex(); }
	font_t* xlsXformatGetFont(xf_t *x)							{ return x->GetFont(); }
	/* Format Index wrappers*/
	void xlsXformatSetFormat(xf_t *x, format_number_t formatidx){ return x->SetFormat(formatidx); }
	void xlsXformatSetFormatP(xf_t *x, format_t *fmt)			{ return x->SetFormat(fmt); }
	/* Horizontal Align option wrappers*/
	void xlsXformatSetHAlign(xf_t *x, halign_option_t ha_option){ return x->SetHAlign(ha_option); }
	unsigned8_t xlsXformatGetHAlign(xf_t *x)					{ return x->GetHAlign(); }
	/* Vertical Align option wrappers*/
	void xlsXformatSetVAlign(xf_t *x, valign_option_t va_option){ return x->SetVAlign(va_option); }
	unsigned8_t xlsXformatGetVAlign(xf_t *x)					{ return x->GetVAlign(); }
	/* Text orientation option wrappers*/
	void xlsXformatSetTxtOrientation(xf_t *x, txtori_option_t ori_option)
																{ return x->SetTxtOrientation(ori_option); }
	unsigned8_t xlsXformatGetTxtOrientation(xf_t *x)			{ return x->GetTxtOrientation(); }
	/* Fill Foreground color option wrappers*/
	void xlsXformatSetFillFGColor(xf_t *x, color_name_t color)	{ return x->SetFillFGColor(color); }
	unsigned16_t xlsXformatGetFillFGColorIdx(xf_t *x)			{ return x->GetFillFGColorIdx(); }
	/* Fill Background color option wrappers*/
	void xlsXformatSetFillBGColor(xf_t *x, color_name_t color)	{ return x->SetFillBGColor(color); }
	unsigned16_t xlsXformatGetFillBGColorIdx(xf_t *x)			{ return x->GetFillBGColorIdx(); }
	/* Fill Style option wrappers*/
	void xlsXformatSetFillStyle(xf_t *x, fill_option_t fill)	{ return x->SetFillStyle(fill); }
	unsigned8_t xlsXformatGetFillStyle(xf_t *x)					{ return x->GetFillStyle(); }
	/* Locked option wrappers*/
	void xlsXformatSetLocked(xf_t *x, bool locked_opt)			{ return x->SetLocked(locked_opt); }
	bool xlsXformatIsLocked(xf_t *x)							{ return x->IsLocked(); }
	/* Hidden option wrappers*/
	void xlsXformatSetHidden(xf_t *x, bool hidden_opt)			{ return x->SetHidden(hidden_opt); }
	bool xlsXformatIsHidden(xf_t *x)							{ return x->IsHidden(); }
	/* Wrap option wrappers*/
	void xlsXformatSetWrap(xf_t *x, bool wrap_opt)				{ return x->SetWrap(wrap_opt); }
	bool xlsXformatIsWrap(xf_t *x)								{ return x->IsWrap(); }
	/* Cell option wrappers*/
	void xlsXformatSetBorderStyle(xf_t *x, border_side_t side, border_style_t style)
																{ return x->SetBorderStyle(side, style); }
	void xlsXformatSetBorderColor(xf_t *x, border_side_t side, color_name_t color)
																{ return x->SetBorderColor(side, color); }
	void xlsXformatSetBorderColorIdx(xf_t *x, border_side_t side, unsigned8_t color)
																{ return x->SetBorderColor(side, color); }
	unsigned8_t xlsXformatGetBorderStyle(xf_t *x, border_side_t side)
																{ return x->GetBorderStyle(side); }
	unsigned16_t xlsXformatGetBorderColorIdx(xf_t *x, border_side_t side)
																{ return x->GetBorderColorIdx(side); }

	// Font
	void xlsFontSetName(font_t *f, const char *fntname)			{
																	std::string str = fntname;
																	f->SetName(str);
																}
	char *xlsFontGetName(font_t *f, char *dst, size_t dstsize)	{
																	const char *ptr = f->GetName().c_str();
																	size_t len = strlen(ptr) + 1;
																	if (len > dstsize)
																		len = dstsize;
																	memcpy(dst, ptr, len);
																	dst[dstsize - 1] = 0;
																	return dst;
																}										
	/* FONT height wrappers*/
	void xlsFontSetHeight(font_t *f, unsigned16_t fntheight)	{ return f->SetHeight(fntheight); }
	unsigned16_t xlsFontGetHeight(font_t *f)					{ return f->GetHeight(); }
	/* FONT boldstyle wrappers*/
	void xlsFontSetBoldStyle(font_t *f, boldness_option_t fntboldness)
																{ return f->SetBoldStyle(fntboldness); }
	unsigned16_t xlsFontGetBoldStyle(font_t *f)					{ return f->GetBoldStyle(); }
	/* FONT underline wrappers*/
	void xlsFontSetUnderlineStyle(font_t *f, underline_option_t fntunderline)
																{ return f->SetUnderlineStyle(fntunderline); }
	unsigned8_t xlsFontGetUnderlineStyle(font_t *f)				{ return f->GetUnderlineStyle(); }
	/* FONT script wrappers*/
	void xlsFontSetScriptStyle(font_t *f, script_option_t fntscript)
																{ return f->SetScriptStyle(fntscript); }
	unsigned16_t xlsFontGetScriptStyle(font_t *f)				{ return f->GetScriptStyle(); }
	/* FONT script wrappers*/
	void xlsFontSetColor(font_t *f, color_name_t fntcolor)		{ return f->SetColor(fntcolor); }
	unsigned16_t xlsFontGetColorIdx(font_t *f)					{ return f->GetColorIdx(); }
	void xlsFontSetItalic(font_t *f, bool italic)				{ return f->SetItalic(italic); }
	void xlsFontSetStrikeout(font_t *f, bool so)				{ return f->SetStrikeout(so); }
	/* FONT  attributes wrappers */
	// Macintosh only
	void xlsFontSetOutline(font_t *f, bool ol)					{ return f->SetOutline(ol); }
	void xlsFontSetShadow(font_t *f, bool sh)					{ return f->SetShadow(sh); }

// these are accessing private members. Is this intended?
	unsigned16_t xlsFontGetAttributes(font_t *f)				{ return f->GetAttributes(); }
#if defined(DEPRECATED)
	void xlsFontSetAttributes(font_t *f, unsigned16_t attr)		{ f->SetAttributes(attr); }
#endif
	//unsigned32_t xlsXformatGetSignature(xf_t *x)				{ return x->GetSignature(); }
	bool xlsXformatIsCell(xf_t *x)								{ return x->IsCell(); }
	void xlsXformatSetCellMode(xf_t *x, bool cellmode)			{ x->SetCellMode(cellmode); }
	unsigned16_t xlsXformatGetFormatIndex(xf_t *x)				{ return x->GetFormatIndex(); }
	format_number_t xlsXformatGetFormat(xf_t *x)				{ return x->GetFormat(); }
	void xlsCellSetXF(cell_t *c, xf_t *pxfval)					{ c->SetXF(pxfval); }
	unsigned16_t xlsCellGetXFIndex(cell_t *c)					{ return c->GetXFIndex(); }
	//void xlsCellFontattr(cell_t *c, unsigned16_t attr)		{ c->Fontattr(attr); }
}
