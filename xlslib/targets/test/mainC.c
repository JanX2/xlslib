/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 2004-2006 Yeico S. A. de C. V.
 * Copyright (C) 2008 David Hoerl
 *
 * $Source: /cvsroot/xlslib/xlslib/targets/test/mainC.c,v $
 * $Revision: 1.5 $
 * $Author: dhoerl $
 * $Date: 2009/03/08 21:17:45 $
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * File description:
 *
 *
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifdef HAVE_CONFIG_H
#include <xlconfig.h>
#elif defined(_MSC_VER) && defined(WIN32)
#include "ac-config.win32.h"
#endif


#include <stdio.h>
#ifdef HAVE_STDINT_H	// [i_a] 
#include <stdint.h>
#endif
#ifdef HAVE_STDBOOL_H	// [i_a] 
#include <stdbool.h>
#else
typedef enum
{
	false = 0,
	true = 1
} bool;
#endif
#ifdef HAVE_WCHAR_H
#include <wchar.h>
#endif
#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif


//#define RANGE_FEATURE
#include <xlslib.h>

#include "md5.h"




int main(int argc, char *argv[]) 
{
	workbook *w;
	worksheet *ws;
	int ret;

	w = xlsNewWorkbook();
	ws = xlsWorkbookSheet(w, "xlslib C");

	xlsWorksheetNumberDbl(ws, (unsigned16_t)1, (unsigned16_t)1, 1.0, NULL);  
	xlsWorksheetNumberDbl(ws, (unsigned16_t)2, (unsigned16_t)1, 2.0, NULL);
	xlsWorksheetNumberDbl(ws, (unsigned16_t)3, (unsigned16_t)1, 3.0, NULL);
	ret = xlsWorkbookDump(w, "testC.xls");

	printf("    # saved it ret=%d!\n", ret);
	xlsDeleteWorkbook(w);

	if (ret != NO_ERRORS)
	{
		fprintf(stderr, "%s failed: I/O failure %d.\n", argv[0], ret);
		return -1;
	}
	if (0 != check_file("testC.xls", "87eb96e3dbd0074d1489560aafdb47e0"))
	{
		fprintf(stderr, "%s failed: MD5 of generated XLS mismatch or I/O failure.\n", argv[0]);
		return -1;
	}

	return 0;
}






#if 0

/*
only to test the compile and link phase: do we have 'em all?

... no, we don't, so it seems...
*/
void test_compile(void)
{
workbook *w = 0;
worksheet *s = 0;
const char *name = "";
char charbuf[5];
const unichar_t *uname = 0;
font_t *f = 0;
format_t *fmt = 0;
xf_t *xf = 0;
property_t prop = PROP_SUBJECT;
unsigned16_t i16 = 0;
unsigned32_t row = 0;
unsigned32_t col = 0;
#ifdef RANGE_FEATURE
range *r;
#endif
cell_t *c = 0;
double numval = 0.0;
format_number_t fmtnum = FMT_CURRENCY8;
halign_option_t ha = HALIGN_RIGHT;
valign_option_t va = VALIGN_BOTTOM;
txtori_option_t ori = ORI_90NOCLOCKTXT;
color_name_t *color = 0;
fill_option_t fill = FILL_HORIZ_LINES_THIN;
bool b = 0;
border_side_t side = BORDER_TOP;
border_style_t style = BORDER_DOTTED;
unsigned8_t i8 = 0;
boldness_option_t fntboldness = BOLDNESS_HALF;
underline_option_t fntunderline = UNDERLINE_SINGLE;
script_option_t fntscript = SCRIPT_SUPER;





w = xlsNewWorkbook();
xlsDeleteWorkbook(w);

s = xlsWorkbookSheet(w, name);
s = xlsWorkbookSheetW(w, uname);
s = xlsWorkbookGetSheet(w, i16);
f = xlsWorkbookFont(w, name);
fmt = xlsWorkbookFormat(w, name);
fmt = xlsWorkbookFormatW(w, uname);
xf = xlsWorkbookxFormat(w);
xf = xlsWorkbookxFormatFont(w, f);
#ifdef HAVE_ICONV
xlsWorkbookIconvInType(w, name);
#endif
xlsWorkbookProperty(w, prop, name);
xlsWorkBookWindPosition(w, i16, i16);
xlsWorkBookWindSize(w, i16, i16);
xlsWorkBookFirstTab(w, i16);
xlsWorkBookTabBarWidth(w, i16);
xlsWorkbookDump(w, name);

// Worksheet
xlsWorksheetMakeActive(s);	// Make this sheet the selected sheet
c =  xlsWorksheetFindCell(s, row, col);
xlsWorksheetMerge(s, row, col, row, col);
xlsWorksheetColwidth(s, col, i16, xf);
xlsWorksheetRowheight(s, row, i16, xf);
#ifdef RANGE_FEATURE
r = xlsWorksheetRangegroup(s, row, col, row, col);
#endif
c =  xlsWorksheetLabel(s, row, col, name, xf);
c =  xlsWorksheetLabelW(s, row, col, uname, xf);
c =  xlsWorksheetBlank(s, row, col, xf);
c =  xlsWorksheetNumberDbl(s, row, col, numval, xf);
// 536870911 >= numval >= -536870912
c =  xlsWorksheetNumberInt(s, row, col, 0, xf);
#ifndef FALSE
#define FALSE 0
#endif
c = xlsWorksheetBoolean(s, row, col, FALSE, xf);
c = xlsWorksheetError(s, row, col, XLERR_VALUE, xf);
c = xlsWorksheetNote(s, row, col, name, name, xf);
c = xlsWorksheetNoteW(s, row, col, uname, uname, xf);
//c = xlsWorksheetFormula(s, row, col, expr, xf);

// cell: xfi
xlsCellFont(c, f);
xlsCellFormat(c, fmtnum);
xlsCellFormatP(c, fmt);
xlsCellHalign(c, ha);
xlsCellValign(c, va);
xlsCellOrientation(c, ori);
xlsCellFillfgcolor(c, *color);
xlsCellFillbgcolor(c, *color);
xlsCellFillstyle(c, fill);
xlsCellLocked(c, b);
xlsCellHidden(c, b);
xlsCellWrap(c, b);
xlsCellBorderstyle(c, side, style);
xlsCellBordercolor(c, side, *color);
xlsCellBordercolorIdx(c, side, i8);
xlsCellFontname(c, name);
// cell: font
xlsCellFontheight(c, i16);
xlsCellFontbold(c, fntboldness);
xlsCellFontunderline(c, fntunderline);
xlsCellFontscript(c, fntscript);
xlsCellFontcolor(c, *color);
//xlsCellFontattr(c, i16);
xlsCellFontitalic(c, b);
xlsCellFontstrikeout(c, b);
xlsCellFontoutline(c, b);
xlsCellFontshadow(c, b);
xlsCellGetRow(c);
xlsCellGetCol(c);
xlsCellGetXFIndex(c);
xlsCellSetXF(c, xf);
// xformat
xlsXformatSetFont(xf, f);
xlsXformatGetFontIndex(xf);
xlsXformatGetFont(xf);
xlsXformatSetFormat(xf, fmtnum);
xlsXformatSetFormatP(xf, fmt);
xlsXformatGetFormatIndex(xf);
xlsXformatGetFormat(xf);
xlsXformatSetHAlign(xf, ha);
xlsXformatGetHAlign(xf);
xlsXformatSetVAlign(xf, va);
xlsXformatGetVAlign(xf);
xlsXformatSetTxtOrientation(xf, ori);
xlsXformatGetTxtOrientation(xf);
xlsXformatSetFillFGColor(xf, *color);
xlsXformatGetFillFGColorIdx(xf);
xlsXformatSetFillBGColor(xf, *color);
xlsXformatGetFillBGColorIdx(xf);
xlsXformatSetFillStyle(xf, fill);
xlsXformatGetFillStyle(xf);
xlsXformatSetLocked(xf, b);
xlsXformatIsLocked(xf);
xlsXformatSetHidden(xf, b);
xlsXformatIsHidden(xf);
xlsXformatSetWrap(xf, b);
xlsXformatIsWrap(xf);
xlsXformatSetCellMode(xf, b);
xlsXformatIsCell(xf);
xlsXformatSetBorderStyle(xf, side, style);
xlsXformatSetBorderColor(xf, side, *color);
xlsXformatSetBorderColorIdx(xf, side, i8);
xlsXformatGetBorderStyle(xf, side);
xlsXformatGetBorderColorIdx(xf, side);
//xlsXformatGetSignature(xf);

// Font
xlsFontSetName(f, name);
xlsFontGetName(f, charbuf, sizeof(charbuf));
xlsFontSetHeight(f, i16);
xlsFontGetHeight(f);
xlsFontSetBoldStyle(f, fntboldness);
xlsFontGetBoldStyle(f);
xlsFontSetUnderlineStyle(f, fntunderline);
xlsFontGetUnderlineStyle(f);
xlsFontSetScriptStyle(f, fntscript);
xlsFontGetScriptStyle(f);
xlsFontSetColor(f, *color);
xlsFontGetColorIdx(f);
xlsFontSetItalic(f, b);
xlsFontSetStrikeout(f, b);
#if defined(DEPRECATED)
xlsFontSetAttributes(f, i16);
#endif
xlsFontGetAttributes(f);
xlsFontSetOutline(f, b);
xlsFontSetShadow(f, b);
}


#endif
