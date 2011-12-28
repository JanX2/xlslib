/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * This file is part of xlslib -- A multiplatform, C/C++ library
 * for dynamic generation of Excel(TM) files.
 *
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
 *	all include file for users
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// The defines are needed by the C library and OSX Framework Users
// The defines are needed by C++ Library Users

#if defined(__cplusplus)

#include <string>

#include "common/xlsys.h"
#include "common/xlstypes.h"

#include "xlslib/record.h"
#include "xlslib/globalrec.h"
#include "xlslib/range.h"
#include "xlslib/colinfo.h"
#include "xlslib/row.h"
#include "xlslib/formula.h"

#include "xlslib/sheetrec.h"
#include "xlslib/workbook.h"

#if defined(__FRAMEWORK__)
using namespace xlslib_core;
#endif

#else // defined(__cplusplus)

#ifndef __OBJC__    // framework needs a more qualified path
#include "common/xlstypes.h"
#endif

// Summary options
typedef enum {
	PROP_AUTHOR = 1,
	PROP_CATEGORY,
	PROP_COMMENTS,
	PROP_COMPANY,
	PROP_CREATINGAPPLICATION,	// [i_a] Cannot see anywhere this is displayed (TODO: remove? use? reserved for future use?)
	PROP_KEYWORDS,
	PROP_MANAGER,
	PROP_REVISION,
	PROP_SUBJECT,
	PROP_TITLE,
	
	PROP_LAST
} property_t;

// Format options
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
// good resource for format strings: http://www.mvps.org/dmcritchie/excel/formula.htm
// Good explanation of custom formats: http://www.ozgrid.com/Excel/CustomFormats.htm
// MS examples (need Windows): http://download.microsoft.com/download/excel97win/sample/1.0/WIN98Me/EN-US/Nmbrfrmt.exe
// Google this for MS help: "Create or delete a custom number format"

// Horizontal alignment
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

// Vertical alignment
typedef enum
{
  VALIGN_TOP = 0,
  VALIGN_CENTER,
  VALIGN_BOTTOM,
  VALIGN_JUSTIFY
} valign_option_t;

// Text options
typedef enum
{
  ORI_NONE = 0,      
  ORI_TOPBOTTOMTXT,				// Letters stacked top to bottom but no rotation
  ORI_90NOCLOCKTXT,				// Text rotated 90 degrees counterclockwise
  ORI_90CLOCKTXT				// Text rotated 90 degrees clockwise
} txtori_option_t;

// Text rotation angle (unimplemented, need it? Just ask for it)
// TODO

// Text indention - horizontal alignment must be Left
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

// Foreground, background, and text color options. Change to BIFF8 may cause some of these to be incorrect...
typedef enum
{
  COLOR_BLACK,
  COLOR_DARK_RED,
  COLOR_RED,
  COLOR_PINK,
  COLOR_ROSE,
  COLOR_BROWN,
  COLOR_ORANGE,
  COLOR_LIGHT_ORANGE,
  COLOR_GOLD,
  COLOR_TAN,
  COLOR_OLIVE_GREEN,
  COLOR_DARK_YELLOW,
  COLOR_LIME,
  COLOR_YELLOW,
  COLOR_LIGHT_YELLOW,
  COLOR_DARK_GREEN,
  COLOR_GREEN,
  COLOR_COMBINED08,
  COLOR_BRIGHT_GREEN,
  COLOR_LIGHT_GREEN,
  COLOR_DARK_TEAL,
  COLOR_TEAL,
  COLOR_AQUA,
  COLOR_TURQUOISE,
  COLOR_LIGHT_TURQUOISE,
  COLOR_DARK_BLUE,
  COLOR_BLUE,
  COLOR_LIGHT_BLUE,
  COLOR_SKY_BLUE,
  COLOR_PALEBLUE,
  COLOR_INDIGO,
  COLOR_BLUE_GRAY,
  COLOR_VIOLET,
  COLOR_PLUM,
  COLOR_LAVANDER,
  COLOR_GRAY80,
  COLOR_GRAY50,
  COLOR_GRAY40,
  COLOR_GRAY25,
  COLOR_WHITE,
  COLOR_SYS_WIND_FG,
  COLOR_SYS_WIND_BG        
} color_name_t;

// Cell fill
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

// Border lines
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

// Where to put border lines
typedef enum
{
  BORDER_BOTTOM = 0,
  BORDER_TOP,
  BORDER_LEFT,
  BORDER_RIGHT 
} border_side_t;

// Thickness
typedef enum
{
  BOLDNESS_BOLD = 0,
  BOLDNESS_HALF,
  BOLDNESS_NORMAL,
  BOLDNESS_DOUBLE
} boldness_option_t;

// Sub/super script
typedef enum
{
  SCRIPT_NONE = 0,
  SCRIPT_SUPER,
  SCRIPT_SUB
} script_option_t;

// Underlining
typedef enum
{
  UNDERLINE_NONE = 0,
  UNDERLINE_SINGLE,
  UNDERLINE_DOUBLE,
  UNDERLINE_SINGLEACC,
  UNDERLINE_DOUBLEACC

} underline_option_t;

typedef enum
{
	XLERR_NULL  = 0x00, // #NULL!
	XLERR_DIV0  = 0x07, // #DIV/0!
	XLERR_VALUE = 0x0F, // #VALUE!
	XLERR_REF   = 0x17, // #REF!
	XLERR_NAME  = 0x1D, // #NAME?
	XLERR_NUM   = 0x24, // #NUM!
	XLERR_N_A   = 0x2A, // #N/A!
} errcode_t;

#endif // ifdef __cpluplus

#if !(defined(__cplusplus) || defined(__OBJC__) ) || defined(CPP_BRIDGE_XLS)

#ifdef CPP_BRIDGE_XLS

#define EXTERN_TYPE
extern "C" {
using namespace xlslib_core;

#else

#define EXTERN_TYPE extern

typedef wchar_t unichar_t;

typedef struct _workbook workbook;
typedef struct _worksheet worksheet;
typedef struct _font_t font_t;
typedef struct _format_t format_t;
typedef struct _cell_t cell_t;
typedef struct _xf_t xf_t;
typedef struct _range range;

#endif // CPP_BRIDGE_XLS

// Workbook
EXTERN_TYPE workbook *xlsNewWorkbook(void);
EXTERN_TYPE void xlsDeleteWorkbook(workbook *w);

EXTERN_TYPE worksheet *xlsWorkbookSheet(workbook *w, const char *sheetname);
EXTERN_TYPE worksheet *xlsWorkbookSheetW(workbook *w, const unichar_t *sheetname);
EXTERN_TYPE worksheet *xlsWorkbookGetSheet(workbook *w, unsigned16_t sheetnum);
EXTERN_TYPE font_t *xlsWorkbookFont(workbook *w, const char *name);
EXTERN_TYPE format_t *xlsWorkbookFormat(workbook *w, const char *name);
EXTERN_TYPE format_t *xlsWorkbookFormatW(workbook *w, const unichar_t *name);
EXTERN_TYPE xf_t *xlsWorkbookxFormat(workbook *w);
EXTERN_TYPE xf_t *xlsWorkbookxFormatFont(workbook *w, font_t *font);
#ifdef HAVE_ICONV
EXTERN_TYPE	int xlsWorkbookIconvInType(workbook *w, const char *inType);
#endif
EXTERN_TYPE	unsigned8_t xlsWorkbookProperty(workbook *w, property_t prop, const char *s);
EXTERN_TYPE	void xlsWorkBookWindPosition(workbook *w, unsigned16_t horz, unsigned16_t vert);
EXTERN_TYPE	void xlsWorkBookWindSize(workbook *w, unsigned16_t horz, unsigned16_t vert);
EXTERN_TYPE	void xlsWorkBookFirstTab(workbook *w, unsigned16_t firstTab);
EXTERN_TYPE	void xlsWorkBookTabBarWidth(workbook *w, unsigned16_t width);
EXTERN_TYPE	int xlsWorkbookDump(workbook *w, const char *filename);

// Worksheet
EXTERN_TYPE void xlsWorksheetMakeActive(worksheet *w);	// Make this sheet the selected sheet
EXTERN_TYPE cell_t *xlsWorksheetFindCell(worksheet *w, unsigned32_t row, unsigned32_t col);
EXTERN_TYPE void xlsWorksheetMerge(worksheet *w, unsigned32_t first_row, unsigned32_t first_col, unsigned32_t last_row, unsigned32_t last_col);
EXTERN_TYPE void xlsWorksheetColwidth(worksheet *w, unsigned32_t col, unsigned16_t width, xf_t* pxformat);
EXTERN_TYPE void xlsWorksheetRowheight(worksheet *w, unsigned32_t row, unsigned16_t height, xf_t* pxformat);
#ifdef RANGE_FEATURE
EXTERN_TYPE range *xlsWorksheetRangegroup(worksheet *w, unsigned32_t row1, unsigned32_t col1, unsigned32_t row2, unsigned32_t col2);
#endif
EXTERN_TYPE cell_t *xlsWorksheetLabel(worksheet *w, unsigned32_t row, unsigned32_t col, const char *strlabel, xf_t *pxformat);
EXTERN_TYPE cell_t *xlsWorksheetLabelW(worksheet *w, unsigned32_t row, unsigned32_t col, const unichar_t *strlabel, xf_t *pxformat);
EXTERN_TYPE cell_t *xlsWorksheetBlank(worksheet *w, unsigned32_t row, unsigned32_t col, xf_t *pxformat);
EXTERN_TYPE cell_t *xlsWorksheetNumberDbl(worksheet *w, unsigned32_t row, unsigned32_t col, double numval, xf_t *pxformat);
// 536870911 >= numval >= -536870912
EXTERN_TYPE cell_t *xlsWorksheetNumberInt(worksheet *w, unsigned32_t row, unsigned32_t col, signed32_t numval, xf_t *pxformat);
EXTERN_TYPE cell_t *xlsWorksheetBoolean(worksheet *w, unsigned32_t row, unsigned32_t col, int boolval, xf_t *pxformat);
EXTERN_TYPE cell_t *xlsWorksheetError(worksheet *w, unsigned32_t row, unsigned32_t col, errcode_t errval, xf_t *pxformat);
EXTERN_TYPE cell_t *xlsWorksheetNote(worksheet *w, unsigned32_t row, unsigned32_t col, const char *remark, const char *author, xf_t *pxformat);
EXTERN_TYPE cell_t *xlsWorksheetNoteW(worksheet *w, unsigned32_t row, unsigned32_t col, const unichar_t *remark, const unichar_t *author, xf_t *pxformat);
#if 0
/*
The 'C' interface CANNOT support a 'formula' cell type as the formula expression is constructed
from a C++ object hierarchy (abstract syntax tree).
*/
EXTERN_TYPE cell_t *xlsWorksheetFormula(worksheet *w, unsigned32_t row, unsigned32_t col, expression_node_t *formula, bool auto_destruct_expression_tree, xf_t *pxformat);
#endif

// cell: xfi
EXTERN_TYPE void xlsCellFont(cell_t *c, font_t *fontidx);
EXTERN_TYPE void xlsCellFormat(cell_t *c, format_number_t format);
EXTERN_TYPE void xlsCellFormatP(cell_t *c, format_t *format);
EXTERN_TYPE void xlsCellHalign(cell_t *c, halign_option_t ha_option);
EXTERN_TYPE void xlsCellValign(cell_t *c, valign_option_t va_option);
EXTERN_TYPE void xlsCellOrientation(cell_t *c, txtori_option_t ori_option);
EXTERN_TYPE void xlsCellFillfgcolor(cell_t *c, color_name_t color);
EXTERN_TYPE void xlsCellFillbgcolor(cell_t *c, color_name_t color);
EXTERN_TYPE void xlsCellFillstyle(cell_t *c, fill_option_t fill);
EXTERN_TYPE void xlsCellLocked(cell_t *c, bool locked_opt);
EXTERN_TYPE void xlsCellHidden(cell_t *c, bool hidden_opt);
EXTERN_TYPE void xlsCellWrap(cell_t *c, bool wrap_opt);
EXTERN_TYPE void xlsCellBorderstyle(cell_t *c, border_side_t side, border_style_t style);
EXTERN_TYPE void xlsCellBordercolor(cell_t *c, border_side_t side, color_name_t color);
EXTERN_TYPE void xlsCellBordercolorIdx(cell_t *c, border_side_t side, unsigned8_t color);
EXTERN_TYPE void xlsCellFontname(cell_t *c, const char *fntname);
// cell: font
EXTERN_TYPE void xlsCellFontheight(cell_t *c, unsigned16_t fntheight);
EXTERN_TYPE void xlsCellFontbold(cell_t *c, boldness_option_t fntboldness);
EXTERN_TYPE void xlsCellFontunderline(cell_t *c, underline_option_t fntunderline);
EXTERN_TYPE void xlsCellFontscript(cell_t *c, script_option_t fntscript);
EXTERN_TYPE void xlsCellFontcolor(cell_t *c, color_name_t fntcolor);
//EXTERN_TYPE void xlsCellFontattr(cell_t *c, unsigned16_t attr);
EXTERN_TYPE void xlsCellFontitalic(cell_t *c, bool italic);
EXTERN_TYPE void xlsCellFontstrikeout(cell_t *c, bool so);
EXTERN_TYPE void xlsCellFontoutline(cell_t *c, bool ol);
EXTERN_TYPE void xlsCellFontshadow(cell_t *c, bool sh);
EXTERN_TYPE unsigned32_t xlsCellGetRow(cell_t *c);
EXTERN_TYPE unsigned32_t xlsCellGetCol(cell_t *c);
#ifdef RANGE_FEATURE
EXTERN_TYPE void xlsRangeCellcolor(range *r, color_name_t color);
#endif
EXTERN_TYPE unsigned16_t xlsCellGetXFIndex(cell_t *c);
EXTERN_TYPE void xlsCellSetXF(cell_t *c, xf_t *pxfval);
// xformat
EXTERN_TYPE void xlsXformatSetFont(xf_t *x, font_t* fontidx);
EXTERN_TYPE unsigned16_t xlsXformatGetFontIndex(xf_t *x);
EXTERN_TYPE font_t* xlsXformatGetFont(xf_t *x);
EXTERN_TYPE void xlsXformatSetFormat(xf_t *x, format_number_t formatidx);
EXTERN_TYPE void xlsXformatSetFormatP(xf_t *x, format_t *fmt);
EXTERN_TYPE unsigned16_t xlsXformatGetFormatIndex(xf_t *x);
EXTERN_TYPE format_number_t xlsXformatGetFormat(xf_t *x);
EXTERN_TYPE void xlsXformatSetHAlign(xf_t *x, halign_option_t ha_option);
EXTERN_TYPE unsigned8_t xlsXformatGetHAlign(xf_t *x);
EXTERN_TYPE void xlsXformatSetVAlign(xf_t *x, valign_option_t va_option);
EXTERN_TYPE unsigned8_t xlsXformatGetVAlign(xf_t *x);
EXTERN_TYPE void xlsXformatSetTxtOrientation(xf_t *x, txtori_option_t ori_option);
EXTERN_TYPE unsigned8_t xlsXformatGetTxtOrientation(xf_t *x);
EXTERN_TYPE void xlsXformatSetFillFGColor(xf_t *x, color_name_t color);
EXTERN_TYPE unsigned16_t xlsXformatGetFillFGColorIdx(xf_t *x);
EXTERN_TYPE void xlsXformatSetFillBGColor(xf_t *x, color_name_t color);
EXTERN_TYPE unsigned16_t xlsXformatGetFillBGColorIdx(xf_t *x);
EXTERN_TYPE void xlsXformatSetFillStyle(xf_t *x, fill_option_t fill);
EXTERN_TYPE unsigned8_t xlsXformatGetFillStyle(xf_t *x);
EXTERN_TYPE void xlsXformatSetLocked(xf_t *x, bool locked_opt);
EXTERN_TYPE bool xlsXformatIsLocked(xf_t *x);
EXTERN_TYPE void xlsXformatSetHidden(xf_t *x, bool hidden_opt);
EXTERN_TYPE bool xlsXformatIsHidden(xf_t *x);
EXTERN_TYPE void xlsXformatSetWrap(xf_t *x, bool wrap_opt);
EXTERN_TYPE bool xlsXformatIsWrap(xf_t *x);
EXTERN_TYPE void xlsXformatSetCellMode(xf_t *x, bool cellmode);
EXTERN_TYPE bool xlsXformatIsCell(xf_t *x);
EXTERN_TYPE void xlsXformatSetBorderStyle(xf_t *x, border_side_t side, border_style_t style);
EXTERN_TYPE void xlsXformatSetBorderColor(xf_t *x, border_side_t side, color_name_t color);
EXTERN_TYPE void xlsXformatSetBorderColorIdx(xf_t *x, border_side_t side, unsigned8_t color);
EXTERN_TYPE unsigned8_t xlsXformatGetBorderStyle(xf_t *x, border_side_t side);
EXTERN_TYPE unsigned16_t xlsXformatGetBorderColorIdx(xf_t *x, border_side_t side);
//EXTERN_TYPE unsigned32_t xlsXformatGetSignature(xf_t *x);
// Font
EXTERN_TYPE void xlsFontSetName(font_t *f, const char *name);
EXTERN_TYPE char *xlsFontGetName(font_t *f, char *namebuffer, size_t namebuffersize);
EXTERN_TYPE void xlsFontSetHeight(font_t *f, unsigned16_t fntheight);
EXTERN_TYPE unsigned16_t xlsFontGetHeight(font_t *f);
EXTERN_TYPE void xlsFontSetBoldStyle(font_t *f, boldness_option_t fntboldness);
EXTERN_TYPE unsigned16_t xlsFontGetBoldStyle(font_t *f);
EXTERN_TYPE void xlsFontSetUnderlineStyle(font_t *f, underline_option_t fntunderline);
EXTERN_TYPE unsigned8_t xlsFontGetUnderlineStyle(font_t *f);
EXTERN_TYPE void xlsFontSetScriptStyle(font_t *f, script_option_t fntscript);
EXTERN_TYPE unsigned16_t xlsFontGetScriptStyle(font_t *f);
EXTERN_TYPE void xlsFontSetColor(font_t *f, color_name_t fntcolor);
EXTERN_TYPE unsigned16_t xlsFontGetColorIdx(font_t *f);
EXTERN_TYPE void xlsFontSetItalic(font_t *f, bool italic);
EXTERN_TYPE void xlsFontSetStrikeout(font_t *f, bool so);
#if defined(DEPRECATED)
EXTERN_TYPE void xlsFontSetAttributes(font_t *f, unsigned16_t attr);
#endif
EXTERN_TYPE unsigned16_t xlsFontGetAttributes(font_t *f);
EXTERN_TYPE void xlsFontSetOutline(font_t *f, bool ol);
EXTERN_TYPE void xlsFontSetShadow(font_t *f, bool sh);

#ifdef CPP_BRIDGE_XLS
}
#endif

#endif // !defined(__cplusplus) || define(CPP_BRIDGE_XLS)
