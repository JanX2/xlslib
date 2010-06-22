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
 * $Source: /cvsroot/xlslib/xlslib/src/xlslib/recdef.cpp,v $
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

#include <colors.h>
#include <recdef.h>
#include <datast.h>

using namespace std;
using namespace xlslib_core;

/*
******************************
CBof class implementation
******************************
*/
CBof::CBof(CDataStorage &datastore, unsigned16_t boftype):
		CRecord(datastore)
{
   SetRecordType(RECTYPE_BOF);

	AddValue16(VERSION_BIFF);
	AddValue16(boftype);
	AddValue16(BOF_BUILD_DFLT);
	AddValue16(BOF_YEAR_DFLT);

	AddValue32(0);					//The file hystory flags are all set to zero
	AddValue32(VERSION_BIFF);		// The lowest BIFF version

   SetRecordLength(GetDataSize()-4);
}

CBof::~CBof()
{}

/*
******************************
CEof class implementation
******************************
*/
CEof::CEof(CDataStorage &datastore):
		CRecord(datastore)
{
   SetRecordType(RECTYPE_EOF);
   SetRecordLength(GetDataSize()-4);
}

CEof::~CEof()
{
}

/*
**********************************
CCodePage class implementation
**********************************
*/
CCodePage::CCodePage(CDataStorage &datastore, unsigned16_t boftype):
		CRecord(datastore)
{
   SetRecordType(RECTYPE_CODENAME);

   AddValue16(boftype);

   SetRecordLength(GetDataSize()-4);
}

CCodePage::~CCodePage()
{}

/*
**********************************
window1 class implementation
**********************************
*/
window1::window1() :
	horzPos(0), vertPos(0), 
	windWidth(0x37e0/TWIP), windHeight(0x25e0/TWIP),
	activeSheet(0),
	firstVisibleTab(0),
	tabBarWidth(500) 
{
}
window1::~window1()
{
}

/*
**********************************
CWindow1 class implementation
**********************************
*/
CWindow1::CWindow1(CDataStorage &datastore, const window1& wind1):
		CRecord(datastore)
{
	SetRecordType(RECTYPE_WINDOW1);

	AddValue16(wind1.horzPos*TWIP);
	AddValue16(wind1.vertPos*TWIP);
	AddValue16(wind1.windWidth*TWIP);
	AddValue16(wind1.windHeight*TWIP);
	AddValue16(0x0038);					// FLAGS: tabBar, vertScroller, horzScroller
	AddValue16(wind1.activeSheet);
	AddValue16(wind1.firstVisibleTab);		// only useful for when you have so many tabs the tab scroller is active
	AddValue16(1);						// number of selected sheets
	AddValue16(wind1.tabBarWidth);

	SetRecordLength(GetDataSize()-4);
}
CWindow1::~CWindow1()
{
}


/*
**********************************
CDateMode class implementation
**********************************
*/

CDateMode::CDateMode(CDataStorage &datastore):
		CRecord(datastore)
{
   SetRecordType(RECTYPE_DATEMODE);

   AddValue16(Is_In_1904_Mode() ? 1 : 0);

   SetRecordLength(GetDataSize()-4);
}

CDateMode::~CDateMode()
{}


bool CDateMode::Is_In_1904_Mode(void)
{
#ifdef __APPLE__ 
   return true;	// 1904  [i_a]
#else
   return false;	// 1900
#endif
}

/*
**********************************
CWindow2 class implementation
**********************************
*/
CWindow2::CWindow2(CDataStorage &datastore, bool isActive):
		CRecord(datastore)
{
	unsigned16_t	flags;
	
	SetRecordType(RECTYPE_WINDOW2);

	flags =	W2_GRBITMASK_GUTS|W2_GRBITMASK_DFLTHDRCOLOR|W2_GRBITMASK_ZEROS|		// 0x00B0
			W2_GRBITMASK_HROWCOL|W2_GRBITMASK_GRIDS;							// 0x0006
	if(isActive) {
		flags |= W2_GRBITMASK_ACTIVE|W2_GRBITMASK_SELECTED;
	}

	AddValue16(flags);
	AddValue16(W2_DFLT_TOPROW);
	AddValue16(W2_DFLT_LEFTCOL);

	AddValue16(COLOR_CODE_SYS_WIND_FG);	// grid color
	AddValue16(0);						// UNUSED
	AddValue16(0);						// zoom page break preview, default == 0 (W2_DFLT_ZOOMPBPREV ???)
	AddValue16(0);						// xoom normal view, default == 0 (W2_DFLT_ZOOMNORMAL ???)
	AddValue32(W2_DFLT_RESERVED); 

	SetRecordLength(GetDataSize()-4);
}

CWindow2::~CWindow2()
{
}

/*
**********************************
CDimension class implementation
**********************************
*/
CDimension::CDimension(CDataStorage &datastore, 
		   unsigned32_t minRow, 
		   unsigned32_t maxRow, 
		   unsigned32_t minCol, 
		   unsigned32_t maxCol):
		CRecord(datastore)
{
	SetRecordType(RECTYPE_DIMENSIONS);

	AddValue32(minRow);
	AddValue32(maxRow+1);
	AddValue16((unsigned16_t)minCol);
	AddValue16((unsigned16_t)(maxCol+1));						// zoom, default == 0 (W2_DFLT_ZOOMPBPREV ???)
	AddValue16(W2_DFLT_RESERVED); 

	SetRecordLength(GetDataSize()-4);
}

CDimension::~CDimension()
{
}

/*
**********************************
**********************************
*/
void CWindow2::SetSelected()
{
   signed16_t grbitval;
   GetValue16From(&grbitval, W2_OFFSET_GRBIT);

   grbitval |= W2_GRBITMASK_SELECTED;

   SetValueAt16((unsigned16_t)grbitval, W2_OFFSET_GRBIT);
}

/*
**********************************
**********************************
*/
void CWindow2::SetPaged()
{
   unsigned16_t grbitval;
   GetValue16From((signed16_t*)&grbitval, W2_OFFSET_GRBIT);

   grbitval |= W2_GRBITMASK_PAGEBRK;

   SetValueAt16((unsigned16_t)grbitval, W2_OFFSET_GRBIT);
}

/*
**********************************
**********************************
*/
void CWindow2::ClearSelected()
{
   unsigned16_t grbitval;
   GetValue16From((signed16_t*)&grbitval, W2_OFFSET_GRBIT);

   grbitval &= (~W2_GRBITMASK_SELECTED);

   SetValueAt16((unsigned16_t)grbitval, W2_OFFSET_GRBIT);
}

/*
**********************************
**********************************
*/
void CWindow2::ClearPaged()
{
   unsigned16_t grbitval;
   GetValue16From((signed16_t*)&grbitval, W2_OFFSET_GRBIT);

   grbitval &= (W2_GRBITMASK_PAGEBRK);

   SetValueAt16((unsigned16_t)grbitval, W2_OFFSET_GRBIT);
}


/*
******************************
CStyle class implementation
******************************
*/
#define STYLE_BUILTIN_NORMAL      ((unsigned8_t)0x00)
#define STYLE_BUILTIN_ROWLEVELN   ((unsigned8_t)0x01)
#define STYLE_BUILTIN_COLLEVELN   ((unsigned8_t)0x02)
#define STYLE_BUILTIN_COMMA       ((unsigned8_t)0x03)
#define STYLE_BUILTIN_CURRENCY    ((unsigned8_t)0x04)
#define STYLE_BUILTIN_PERCENT     ((unsigned8_t)0x05)
#define STYLE_BUILTIN_COMMAT      ((unsigned8_t)0x06)
#define STYLE_BUILTIN_CURRENCYT   ((unsigned8_t)0x07)

#define STYLE_BUILTIN_BIT   ((unsigned16_t)0x8000)

//#define STYLE_LEVEL_DUMMY         ((unsigned8_t)0x00)

CStyle::CStyle(CDataStorage &datastore, const style_t* styledef):
		CRecord(datastore)
{
   // TODO: Implement user-defined styles. So far only built-in are used.
   SetRecordType(RECTYPE_STYLE);

   AddValue16(styledef->xfindex|STYLE_BUILTIN_BIT);
   AddValue8(styledef->builtintype);
   AddValue8(styledef->level);

   SetRecordLength(GetDataSize()-4);
}

#if 0
CStyle::CStyle(CDataStorage &datastore, 
		   unsigned16_t xfindex, unsigned8_t builtintype, unsigned8_t level):
		CRecord(datastore)
{
   // TODO: Implement user-defined styles. So far only built-in are used.
   SetRecordType(RECTYPE_STYLE);

   AddValue16(xfindex|STYLE_BUILTIN_BIT);
   AddValue8(builtintype);
   AddValue8(level);

   SetRecordLength(GetDataSize()-4);
}
#endif


CStyle::~CStyle()
{
}

/*
******************************
CBSheet class implementation
******************************
*/
#if 0
CBSheet::CBSheet(unsigned32_t streampos, 
                 unsigned16_t attributes, 
                 u16string& sheetname,
				 bool is_ascii):
		CRecord(datastore)
{
	SetRecordType(RECTYPE_BOUNDSHEET);
	streamposOffset = GetDataSize();	// no work
	AddValue32(streampos);
	AddValue16(attributes);
	AddUnicodeString(&sheetname, sizeof(unsigned8_t), is_ascii);

	SetRecordLength(GetDataSize()-4);
}
#endif
CBSheet::CBSheet(CDataStorage &datastore, const boundsheet_t* bsheetdef):
		CRecord(datastore)
{
#if defined(LEIGHTWEIGHT_UNIT_FEATURE)
	m_Backpatching_Level = 3;
#else
#endif

	SetRecordType(RECTYPE_BOUNDSHEET);
	AddValue32(bsheetdef->streampos);

	// Set the flags in the attribute variables
	unsigned16_t attrflags = 0;
	attrflags |=  bsheetdef->worksheet	? BSHEET_ATTR_WORKSHEET:0;
	attrflags |=  bsheetdef->ex4macro	? BSHEET_ATTR_EX4MACRO:0;
	attrflags |=  bsheetdef->chart		? BSHEET_ATTR_CHART:0;
	attrflags |=  bsheetdef->vbmodule	? BSHEET_ATTR_VBMODULE:0;
	attrflags |=  bsheetdef->visible	? BSHEET_ATTR_VISIBLE:0;
	attrflags |=  bsheetdef->hidden		? BSHEET_ATTR_HIDDEN:0;
	attrflags |=  bsheetdef->veryhidden	? BSHEET_ATTR_VERYHIDDEN:0;

	AddValue16(attrflags);

	AddUnicodeString(&bsheetdef->sheetname, sizeof(unsigned8_t), bsheetdef->isASCII );

	SetRecordLength(GetDataSize()-4);   
}

CBSheet::~CBSheet()
{
}

/* 
**********************************
**********************************
*/

void CBSheet::SetStreamPosition(size_t pos)
{
   SetValueAt32((unsigned32_t)pos, BSHEET_OFFSET_POSITION);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: recdef.cpp,v $
 * Revision 1.9  2009/03/02 04:08:43  dhoerl
 * Code is now compliant to gcc  -Weffc++
 *
 * Revision 1.8  2009/01/10 21:10:50  dhoerl
 * More tweaks
 *
 * Revision 1.7  2009/01/08 22:16:06  dhoerl
 * January Rework
 *
 * Revision 1.6  2009/01/08 02:52:47  dhoerl
 * December Rework
 *
 * Revision 1.5  2008/12/20 15:49:05  dhoerl
 * 1.2.5 fixes
 *
 * Revision 1.4  2008/12/11 21:12:32  dhoerl
 * Cleanup
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

