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
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * File description:
 *
 *
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifdef HAVE_CONFIG_H
#include "common/xlconfig.h"
#elif defined(_MSC_VER) && defined(WIN32)
#include "ac-config.win32.h"
#endif

#ifdef __BCPLUSPLUS__
#  include "ac-config.win32.h"
// I am assuming this header file is created and include automatically by MSVC.
// Other compilers (I.e. BC++ ) don't have this, so I simply copied the file
// from the MSC project to the RadStudio project and included it. RLN 111208
#endif

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#ifdef _X_DEBUG_
#include <unistd.h>
#endif

// [i_a] 
#ifdef HAVE_STDINT_H
#include <stdint.h>
#endif
#ifdef HAVE_STDBOOL_H
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
#include "xlslib.h"

#include "md5.h"

extern void writeUnicodeLabel(worksheet *ws, unsigned int row, unsigned int col);
extern void test_compile(void);

static void my_xlslib_assertion_reporter(const char *expr, const char *filename, int lineno, const char *funcname)
{
	fprintf(stderr, "Assertion failed: %s at line %d", 
			(expr ? expr : "???"),
			lineno);
	if (funcname)
	{
		fprintf(stderr, " (%s)", funcname);
	}
	if (filename)
	{
		fprintf(stderr, " in %s\n", filename);
	}
	else
	{
		fprintf(stderr, " in [unidentified source file]\n");
	}
	exit(EXIT_FAILURE);
}


void writeUnicodeLabel(worksheet *ws, unsigned int row, unsigned int col)
{
   const wchar_t *latin1wstr = L"\x0055\x006e\x0069\x0063\x006f\x0064\x0065\x0020\x0074\x0065\x0078\x0074\x0020\x00e3\x00f5\x00f1\x00e1\x00e9\x00fa\x00ed\x00f3\x002c\x00e0\x00e8\x00ec\x00f2\x00f9\x00e4\x00eb\x00ef\x00f6\x00fc\x00f1\x00e2\x00ea\x00ee\x00f4\x00fb";
   const wchar_t *wstr = L"\x3042\x3043"; // 2 Hiragana characters

#if 0
   const char *latin1str = "Unicode text ãõñáéúíó,àèìòùäëïöüñâêîôû";
	wchar_t wbuf[256];
	int len;
	const wchar_t *wp;

    len = mbstowcs(NULL, latin1str, 0);
	len++; /* take NUL sentinel into account */
    len = mbstowcs(wbuf, latin1str, len);
    if (len == (size_t) (-1))
    {
       printf("Couldn't convert string--invalid multibyte character.\n");
    }
    printf( "  Characters converted: %u\n", (unsigned int)len );
    printf( "  Hex value of first 2" );
    printf( " wide characters: %#.4x %#.4x\n\n", wbuf[13], wbuf[14] );

	printf( " as Unicode string: L\"");
	for (wp = wbuf; *wp; wp++)
	{
	    printf("\\x%04x", *wp);
	}
	printf( "\";\n");
	xlsWorksheetLabelW(ws, row++, col, wbuf, NULL);
#endif

	xlsWorksheetLabel(ws, row++, col, "Two Japanese Hiragana:", NULL);
	xlsWorksheetLabelW(ws, row++, col, wstr, NULL);
	xlsWorksheetLabel(ws, row++, col, "A few words plus a series of Latin1 accented letters:", NULL);
	xlsWorksheetLabelW(ws, row++, col, latin1wstr, NULL);
}


int main(int argc, char *argv[]) 
{
	workbook *w;
	worksheet *ws;
	int ret;
	char check[40], *checkP = check;
	char fileName[128];
	
	fileName[0] = 0;
	
	// Used for internal testing
	if(argc == 2) {
#ifdef _X_DEBUG_
		chdir(argv[1]);
#endif
		strcpy(fileName, argv[1]);
		strcat(fileName, "/");		
	}
	strcat(fileName, "mainC.md5");
	
	{
		FILE *fp = fopen(fileName, "r");
		if(fp) {
			fscanf(fp, "%s", checkP);
			fclose(fp);
		} else {
			strcpy(checkP, "00000000000000000000000000000000");
		}
		//printf("MD5 = %s\n", checkP);
	}

	xlslib_register_assert_reporter(&my_xlslib_assertion_reporter);

	w = xlsNewWorkbook();
	ws = xlsWorkbookSheet(w, "xlslib C");

	xlsWorksheetNumberDbl(ws, (unsigned16_t)1, (unsigned16_t)1, 1.0, NULL);  
	xlsWorksheetNumberDbl(ws, (unsigned16_t)2, (unsigned16_t)1, 2.0, NULL);
	xlsWorksheetNumberDbl(ws, (unsigned16_t)3, (unsigned16_t)1, 3.0, NULL);
	xlsWorksheetLabel(ws, 4, 1, "ASCII text", NULL);
	writeUnicodeLabel(ws, 5, 1);
	ret = xlsWorkbookDump(w, "testC.xls");

	// printf("    # saved it ret=%d errno=%s\n", ret, strerror(errno));

	xlsDeleteWorkbook(w);

	if (ret != NO_ERRORS)
	{
		fprintf(stderr, "%s failed: I/O failure %d.\n", argv[0], ret);
		return EXIT_FAILURE;
	}
	if ((checkP = check_file("testC.xls", checkP)))
	{
		fprintf(stderr, "%s failed: MD5 of generated XLS mismatch or I/O failure.\n", argv[0]);

		if(argc == 2)
		{
			FILE *fp = fopen(fileName, "w");
			if(fp) {
				fprintf(fp, "%s\n", checkP);
				printf("UPDATE MD5 = %s\n", checkP);
				fclose(fp);
			} else {
				printf("FAILED TO WRITE MD5\n");
			}
		}
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}


#if 1

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
