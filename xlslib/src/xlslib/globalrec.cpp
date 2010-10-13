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
 * $Source: /cvsroot/xlslib/xlslib/src/xlslib/globalrec.cpp,v $
 * $Revision: 1.12 $
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

#include <globalrec.h>
#include <datast.h>


using namespace std;
using namespace xlslib_core;


/*
**********************************************************************
CGlobalRecords class implementation
**********************************************************************
*/

CGlobalRecords::CGlobalRecords() :
	m_Fonts(),
	m_DefaultFonts(),
	m_Formats(),
	m_XFs(),
	m_DefaultXFs(),
	m_Styles(),
	m_BoundSheets(),
	m_window1(),
	m_palette(),

	defaultXF(NULL),
	  
#ifdef HAVE_ICONV
	iconv_code(),
#endif
	m_DumpState(GLOBAL_INIT),
	font(),
	font_dflt(),
	fontIndex(0),
	formatIndex(FMTCODE_GENERAL),
	format(),
	xf(),
	xf_dflt(),
	xfIndex(0),
	style(),
	bsheet()
{
	// set to what Excel 2004 on Mac outputs 12/12/2008

	// Initialize default fonts
	font_t *newfont;
	font_t *font0;
	font_t *font1;
	font_t *font2;
	font_t *font4;

	newfont = new font_t(*this, 0, "Verdana", 200, BOLDNESS_NORMAL, UNDERLINE_NONE, SCRIPT_NONE, ORIG_COLOR_BLACK, FONT_DFLT_ATTRIBUTES, FONT_DFLT_FAMILY, FONT_DFLT_CHARSET);
	// mark as used TWICE to ensure these fonts are never discarded, even when 'unused'
	newfont->MarkUsed();
	newfont->MarkUsed();
	m_DefaultFonts.push_back(newfont);
	font0 = newfont;

	newfont = new font_t(*this, 1, "Verdana", 200, BOLDNESS_BOLD, UNDERLINE_NONE, SCRIPT_NONE, ORIG_COLOR_BLACK, FONT_ATTR_BOLD, FONT_DFLT_FAMILY, FONT_DFLT_CHARSET);
	newfont->MarkUsed();
	newfont->MarkUsed();
	m_DefaultFonts.push_back(newfont);
	font1 = newfont;

	newfont = new font_t(*this, 2, "Verdana", 200, BOLDNESS_NORMAL, UNDERLINE_NONE, SCRIPT_NONE, ORIG_COLOR_BLACK, FONT_ATTR_ITALIC, FONT_DFLT_FAMILY, FONT_DFLT_CHARSET);
	newfont->MarkUsed();
	newfont->MarkUsed();
	m_DefaultFonts.push_back(newfont);
	font2 = newfont;

	newfont = new font_t(*this, 3, "Verdana", 200, BOLDNESS_BOLD, UNDERLINE_NONE, SCRIPT_NONE, ORIG_COLOR_BLACK, FONT_ATTR_BOLD|FONT_ATTR_ITALIC, FONT_DFLT_FAMILY, FONT_DFLT_CHARSET);
	newfont->MarkUsed();
	newfont->MarkUsed();
	m_DefaultFonts.push_back(newfont);

	// Excel spec for FONT says ignore 4
	newfont = new font_t(*this, 5, "Verdana", 200, BOLDNESS_NORMAL, UNDERLINE_NONE, SCRIPT_NONE, ORIG_COLOR_BLACK, FONT_DFLT_ATTRIBUTES, FONT_DFLT_FAMILY, FONT_DFLT_CHARSET);
	newfont->MarkUsed();
	newfont->MarkUsed();
	m_DefaultFonts.push_back(newfont);
	font4 = newfont;

	fontIndex = 6;	// this will be 1 more than last standard font

	for(xfIndex=0; xfIndex<21; ++xfIndex)
	{
		xf_t*			newxf;
		font_t			*fnt;
		format_number_t	fmt;
		bool			is_cell;
		
		fnt		= font0;
		fmt		= FMT_GENERAL;
		is_cell	= XF_IS_STYLE;

		switch(xfIndex) 
		{
		case 0:
			fnt		= NULL;
			break;
		case 1:
		case 2:
			fnt		= font1;
			break;
		case 3:
		case 4:
			fnt		= font2;
			break;
		case 15:
			fnt		= NULL;
			is_cell	= XF_IS_CELL;
			break;
		case 16:
			fnt		= font4;
			fmt		= FMT_CURRENCY7;
			break;
		case 17:
			fnt		= font4;
			fmt		= FMT_CURRENCY5;
			break;
		case 18:
			fnt		= font4;
			fmt		= FMT_CURRENCY8;
			break;
		case 19:
			fnt		= font4;
			fmt		= FMT_CURRENCY6;
			break;
		case 20:
			fnt		= font4;
			fmt		= FMT_PERCENT1;
			break;
		}

//		newxf = is_cell == XF_IS_CELL ? new xf_t(false) : new xf_t(xfi, false/*userXF*/, is_cell, xfIndex?false:true);
		newxf = new xf_t(*this, false/*userXF*/, is_cell, xfIndex?false:true);
				
		// override defaults
		if(fnt != NULL)			newxf->SetFont(fnt);
		if(fnt == font4)		newxf->ClearFlag(XF_ALIGN_ATRFONT);	// Ask Mr Bill why...Done to make binary the same
		if(fmt != FMT_GENERAL)	newxf->SetFormat(fmt);
		
		// mark as used TWICE to ensure these formats are never discarded, even when 'unused'
		newxf->MarkUsed();
		newxf->MarkUsed();
		m_DefaultXFs.push_back(newxf);
		
		if(xfIndex == XF_PROP_XF_DEFAULT_CELL) 
		{
			newxf->SetIndex(XF_PROP_XF_DEFAULT_CELL);
			defaultXF = newxf;
		}
		newxf->SetIndex(xfIndex);	// for debugging - not really needed here
	}
	XL_ASSERT(defaultXF);

	formatIndex = FMT_CODE_FIRST_USER;

	style_t* newstyle;
	newstyle = new style_t;
	newstyle->xfindex = 0x0010;
	newstyle->builtintype = 0x03;
	newstyle->level = 0xFF;
	m_Styles.push_back(newstyle);

	newstyle = new style_t;
	newstyle->xfindex = 0x0011; 
	newstyle->builtintype = 0x06;
	newstyle->level = 0xFF;
	m_Styles.push_back(newstyle);

	newstyle = new style_t;
	newstyle->xfindex = 0x0012;
	newstyle->builtintype = 0x04;
	newstyle->level = 0xFF;
	m_Styles.push_back(newstyle);

	newstyle = new style_t;
	newstyle->xfindex = 0x0013;
	newstyle->builtintype = 0x07;
	newstyle->level = 0xFF;
	m_Styles.push_back(newstyle);

	newstyle = new style_t;
	newstyle->xfindex = 0x0000;
	newstyle->builtintype = 0x00;
	newstyle->level = 0xFF;
	m_Styles.push_back(newstyle);

	newstyle = new style_t;
	newstyle->xfindex = 0x0014;
	newstyle->builtintype = 0x05;
	newstyle->level = 0xFF;
	m_Styles.push_back(newstyle);

	// Initialize former static variables
	font	= m_Fonts.begin();        
	format	= m_Formats.begin();    
	xf		= m_XFs.begin();            
	style	= m_Styles.begin();      
	bsheet	= m_BoundSheets.begin();
}
/*
****************************************
****************************************
*/
CGlobalRecords::~CGlobalRecords()
{
   // Delete dynamically created lists elements
   
   if(!m_DefaultFonts.empty())
   {
      for(Font_Vect_Itor_t fnt = m_DefaultFonts.begin(); fnt != m_DefaultFonts.end(); fnt++)
         delete *fnt;
      m_DefaultFonts.clear();
   }
   if(!m_Fonts.empty())
   {
      for(Font_Vect_Itor_t fnt = m_Fonts.begin(); fnt != m_Fonts.end(); fnt++)
         delete *fnt;
      m_Fonts.clear();
   }
   if(!m_Formats.empty())
   {
      for(Format_Vect_Itor_t fnt = m_Formats.begin(); fnt != m_Formats.end(); fnt++)
         delete *fnt;
      m_Formats.clear();
   }
   if(!m_DefaultXFs.empty())   
   {
      for(XF_Vect_Itor_t xfi = m_DefaultXFs.begin(); xfi != m_DefaultXFs.end(); xfi++)
         delete *xfi;
      m_DefaultXFs.clear();
   }
   if(!m_XFs.empty())   
   {
      for(XF_Vect_Itor_t xfi = m_XFs.begin(); xfi != m_XFs.end(); xfi++)
         delete *xfi;
      m_XFs.clear();
   }
   if(!m_Styles.empty())   
   {
      for(Style_Vect_Itor_t xfi = m_Styles.begin(); xfi != m_Styles.end(); xfi++)
         delete *xfi;
      m_Styles.clear();
   }

   if(!m_BoundSheets.empty())
   {
      for(Boundsheet_Vect_Itor_t xfi = m_BoundSheets.begin(); xfi != m_BoundSheets.end(); xfi++)
         delete *xfi;
      m_BoundSheets.clear();
   }
}

/*
****************************************
****************************************
*/

size_t CGlobalRecords::EstimateNumBiffUnitsNeeded4Header(void)
{
	size_t ret = 5;

	ret += m_Fonts.size();        
	ret += m_DefaultFonts.size();
	ret += m_Formats.size();    
	ret += m_XFs.size();            
	ret += m_DefaultXFs.size();            
	ret += m_Styles.size();      
	ret += m_BoundSheets.size();

	return ret;
}

/*
****************************************
****************************************
*/

CUnit* CGlobalRecords::DumpData(CDataStorage &datastore)
{
	CUnit*	m_pCurrentData	= NULL;
	bool	repeat			= false;

	XTRACE("CGlobalRecords::DumpData");

	do
	{
	  switch(m_DumpState)
	  {
         case GLOBAL_INIT:
            XTRACE("\tINIT");

            repeat = true;

            font		= m_Fonts.begin();        
            font_dflt	= m_DefaultFonts.begin();
            format		= m_Formats.begin();    
            xf			= m_XFs.begin();            
            xf_dflt		= m_DefaultXFs.begin();            
            style		= m_Styles.begin();      
            bsheet		= m_BoundSheets.begin();

            m_DumpState = GLOBAL_BOF;
            break;

         case GLOBAL_BOF:			// ********** STATE 1A *************
            XTRACE("\tBOF");

            repeat = false;

#if defined(LEIGHTWEIGHT_UNIT_FEATURE)
            m_pCurrentData = datastore.MakeCBof(BOF_TYPE_WBGLOBALS);
#else
            // Delete_Pointer(m_pCurrentData);
            m_pCurrentData = (CUnit*)(new CBof(datastore, BOF_TYPE_WBGLOBALS));
#endif
			m_DumpState = GLOBAL_CODEPAGE; // DFH GLOBAL_WINDOW1;
            break;

         case GLOBAL_CODEPAGE:		// ********** STATE 1B *************
           XTRACE("\tCODEPAGE");
 
			repeat = false;

#if defined(LEIGHTWEIGHT_UNIT_FEATURE)
            m_pCurrentData = datastore.MakeCCodePage(1200);	// UTF-16
#else
            //Delete_Pointer(m_pCurrentData);
            m_pCurrentData = (CUnit*)(new CCodePage(datastore, 1200));	// UTF-16
#endif
			m_DumpState = GLOBAL_WINDOW1;
            break;

         case GLOBAL_WINDOW1:		// ********** STATE 2A *************
            XTRACE("\tWINDOW1");
 
			repeat = false;

#if defined(LEIGHTWEIGHT_UNIT_FEATURE)
			m_pCurrentData = datastore.MakeCWindow1(m_window1);
#else
            //Delete_Pointer(m_pCurrentData);
			m_pCurrentData = (CUnit*)(new CWindow1(datastore, m_window1));
#endif
			m_DumpState = GLOBAL_DATEMODE; // GLOBAL_DEFAULTFONTS;
            break;

         case GLOBAL_DATEMODE:		// ********** STATE 2B *************
            XTRACE("\tDATEMODE");
 
			repeat = false;

#if defined(LEIGHTWEIGHT_UNIT_FEATURE)
            m_pCurrentData = datastore.MakeCDateMode();
#else
            //Delete_Pointer(m_pCurrentData);
            m_pCurrentData = (CUnit*)(new CDateMode(datastore));
#endif
			m_DumpState = GLOBAL_DEFAULTFONTS;
            break;

         case GLOBAL_DEFAULTFONTS:	// ********** STATE 3A *************
			XTRACE("\tDEFAULTFONTS");
			
			repeat = false;
			
#if defined(LEIGHTWEIGHT_UNIT_FEATURE)
			m_pCurrentData = datastore.MakeCFont(*font_dflt);
#else
			m_pCurrentData = (CUnit*)(new CFont(datastore, *font_dflt));
#endif

			if(font_dflt != (--m_DefaultFonts.end()))
			{
			   // if it wasn't the last font from the list, increment to get the next one
			   font_dflt++;
			} else {
			   // if it was the last from the list, change the DumpState
			   m_DumpState = GLOBAL_FONTS;
			  // font_dflt = m_DefaultFonts.begin();
			}
			break;

         case GLOBAL_FONTS:// ********** STATE 3B *************
			XTRACE("\tFONTS");
			// First check if the list of fonts is not empty...
			if(!m_Fonts.empty())
			{
#if defined(LEIGHTWEIGHT_UNIT_FEATURE)
				m_pCurrentData = datastore.MakeCFont(*font);
#else
				m_pCurrentData = (CUnit*)(new CFont(datastore, *font));
#endif
				if(font != (--m_Fonts.end()))
				{
				  // if it was'nt the last font from the list, increment to get the next one
				  font++;
				} else {
				  // if it was the last from the list, change the DumpState
				  m_DumpState = GLOBAL_FORMATS;
				  font = m_Fonts.begin();
				}
				repeat = false;
			} else {
				// if the list is empty, change the dump state.
				m_DumpState = GLOBAL_FORMATS;
				//font = m_Fonts.begin();
				repeat = true;
			}
			break;

         case GLOBAL_FORMATS: // ********** STATE 4 *************
			XTRACE("\tFORMATS");

			if(!m_Formats.empty())
			{
#if defined(LEIGHTWEIGHT_UNIT_FEATURE)
				m_pCurrentData = datastore.MakeCFormat(*format);
#else
				m_pCurrentData = (CUnit*)(new CFormat(datastore, *format));
#endif
				if(format != (--m_Formats.end()))
				{
				  // if it wasn't the last font from the list, increment to get the next one
				  format++;
				} else {
				  // if it was the last from the list, change the DumpState
				  m_DumpState = GLOBAL_DEFAULTXFS;
				  format = m_Formats.begin();
				}
				repeat = false;
			} else {
			   // if the list is empty, change the dump state.
			   m_DumpState = GLOBAL_DEFAULTXFS;
			   // format = m_Formats.begin();
			   repeat = true;
			}
			break;

         case GLOBAL_DEFAULTXFS: // ********** STATE 5a *************

            XTRACE("\tXDEFAULTFS");
#if defined(LEIGHTWEIGHT_UNIT_FEATURE)
            m_pCurrentData = datastore.MakeCExtFormat(*xf_dflt);
#else
            m_pCurrentData = (CUnit*)(new CExtFormat(datastore, *xf_dflt));
#endif

            if(xf_dflt != (--m_DefaultXFs.end()))
            {
			   // if it wasn't the last font from the list, increment to get the next one
               xf_dflt++;
               repeat = false;
            } else {
			   // if it was the last from the list, change the DumpState
               m_DumpState = GLOBAL_XFS;
               //xf_dflt = m_DefaultXFs.begin();
			   repeat = false;
            } 
            break;

         case GLOBAL_XFS: // ********** STATE 5b *************
			XTRACE("\tXFS");
			if(!m_XFs.empty())
			{
#if defined(LEIGHTWEIGHT_UNIT_FEATURE)
				m_pCurrentData = datastore.MakeCExtFormat(*xf);
#else
				m_pCurrentData = (CUnit*)(new CExtFormat(datastore, *xf));
#endif

				if(xf != (--m_XFs.end()))
				{
				  // if it wasn't the last font from the list, increment to get the next one
				  xf++;
				} else {   
				  // if it was the last from the list, change the DumpState
				  m_DumpState = GLOBAL_STYLES;
				  xf = m_XFs.begin();
				}
				repeat = false;
			} else {
			   // if the list is empty, change the dump state.
			   m_DumpState = GLOBAL_STYLES;
			   //xf = m_XFs.begin();
			   repeat = true;
			}
			break;

         case GLOBAL_STYLES: // ********** STATE 6 *************
            XTRACE("\tSTYLES");

            if(!m_Styles.empty())
            {
				// First check if the list of fonts is not empty...
#if defined(LEIGHTWEIGHT_UNIT_FEATURE)
				m_pCurrentData = datastore.MakeCStyle(*style);
#else
				//Delete_Pointer(m_pCurrentData);
				m_pCurrentData = (CUnit*)(new CStyle(datastore, *style));
#endif

				if(style != (--m_Styles.end()))
				{
				  // if it wasn't the last font from the list, increment to get the next one
				  style++;
				} else {
				 // if it was the last from the list, change the DumpState
				  m_DumpState = GLOBAL_PALETTE;
				  //style = m_Styles.begin();
				}
				repeat = false;
            } else {
               // if the list is empty, change the dump state.
               m_DumpState = GLOBAL_PALETTE;
               //style = m_Styles.begin();
               repeat = true;
            }
            break;

         case GLOBAL_PALETTE: // ********** STATE 7 *************
            XTRACE("\tPALETTE");
 
			repeat = false;

            m_pCurrentData = m_palette.GetData(datastore);
            //Delete_Pointer(m_pCurrentData);
			//m_pCurrentData = (CUnit*)(new CPalette(datastore, (colors ? colors : default_palette)));
			m_DumpState = GLOBAL_BOUNDSHEETS;
            break;

         case GLOBAL_BOUNDSHEETS: // ********** STATE 8 *************
			XTRACE("\tBOUNDSHEETS");
			if(!m_BoundSheets.empty())
			{
				// First check if the list of sheets is not empty...
#if defined(LEIGHTWEIGHT_UNIT_FEATURE)
				m_pCurrentData = (*bsheet)->SetSheetData(datastore.MakeCBSheet(*bsheet));
#else
				//Delete_Pointer(m_pCurrentData);
				m_pCurrentData = (CUnit*)(new CBSheet(datastore, *bsheet));
				(*bsheet)->SetSheetData((CBSheet *)m_pCurrentData);
#endif

				if(bsheet != (--m_BoundSheets.end()))
				{
				  // if it wasn't the last sheet from the list, increment to get the next one
				  bsheet++;
				} else {
				  // if it was the last from the list, change the DumpState
				  m_DumpState = GLOBAL_EOF;
				  bsheet = m_BoundSheets.begin();
				}
				repeat = false;
			} else {
			   // if the list is empty, change the dump state.
			   m_DumpState = GLOBAL_EOF;
			   bsheet = m_BoundSheets.begin();
			   repeat = true;
			}
			break;

         case GLOBAL_EOF:// ********** STATE 9 *************
            XTRACE("\tEOF");

            repeat = false;

#if defined(LEIGHTWEIGHT_UNIT_FEATURE)
            m_pCurrentData = datastore.MakeCEof();
#else
            m_pCurrentData = (CUnit*)(new CEof(datastore));
#endif
			m_DumpState = GLOBAL_FINISH;
            break;

         case GLOBAL_FINISH: // ********** STATE 10 *************
            XTRACE("\tFINISH");

            repeat = false;

            m_pCurrentData = NULL;
            m_DumpState = GLOBAL_INIT;
            break;

         default:
            /* It shouldn't get here */
            XTRACE("\tDEFAULT");
            break;
      }
   } while(repeat);

   return m_pCurrentData;
}


/*
****************************************
****************************************
*/
void CGlobalRecords::AddBoundingSheet(unsigned32_t streampos,
                                      unsigned16_t attributes,
                                      u16string& sheetname)
{
   boundsheet_t* bsheetdef = new boundsheet_t(*this, sheetname, attributes, streampos);

   m_BoundSheets.push_back(bsheetdef);
}

/*
****************************************
****************************************
*/
void CGlobalRecords::AddBoundingSheet(boundsheet_t* bsheetdef)
{
   m_BoundSheets.push_back(bsheetdef);
}


/*
****************************************
It returns pointers to BoundingSheets one by one until
all are spanned, in which case the returned pointer is NULL
****************************************
*/
void CGlobalRecords::GetBoundingSheets(Boundsheet_Vect_Itor_t& bs)
{
   if(bs != m_BoundSheets.end())
      bs++;
   else
      bs = m_BoundSheets.begin();
}


Boundsheet_Vect_Itor_t CGlobalRecords::GetFirstBoundSheet()
{
   return m_BoundSheets.begin();
}

Boundsheet_Vect_Itor_t CGlobalRecords::GetBoundSheetAt(unsigned32_t idx)
{
	Boundsheet_Vect_Itor_t bs;

	bs = m_BoundSheets.begin();
	while(idx--) bs++;
	
	return bs;
}

Boundsheet_Vect_Itor_t CGlobalRecords::GetEndBoundSheet()
{
   return m_BoundSheets.end();
}

/*
****************************************
****************************************
*/
void CGlobalRecords::AddFormat(format_t* newformat)
{
	newformat->SetIndex(formatIndex++);
	m_Formats.push_back(newformat);
}

/*
****************************************
****************************************
*/
void CGlobalRecords::AddFont(font_t* newfont)
{
	newfont->SetIndex(fontIndex++);
	m_Fonts.push_back(newfont);
}
font_t* CGlobalRecords::GetDefaultFont() const
{
   return *m_DefaultFonts.begin();
}

/*
****************************************
****************************************
*/
void CGlobalRecords::AddXFormat(xf_t* xfi)
{
	xfi->SetIndex(xfIndex++);
	m_XFs.push_back(xfi);
}

/*
****************************************
****************************************
*/
bool CGlobalRecords::SetColor(unsigned8_t r, unsigned8_t g, unsigned8_t b, unsigned8_t idx)
{
	return m_palette.setColor(r, g, b, idx);
}

/*
****************************************
****************************************
*/
xf_t* CGlobalRecords::GetDefaultXF() const
{
	return defaultXF;
}
/*
***********************************
***********************************
*/
font_t* CGlobalRecords::fontdup(unsigned8_t fontnum) const
{
   return font_t::fontDup(m_DefaultFonts[fontnum]);
}
/*
***********************************
***********************************
*/

void CGlobalRecords::str16toascii(const u16string& str1, std::string& str2)
{
	u16string::const_iterator cBegin, cEnd;

	str2.clear();

	size_t len = str1.length();
	str2.reserve(len);


	cBegin	= str1.begin();
	cEnd	= str1.end();

	while(cBegin != cEnd) 
	{
		unsigned16_t c = *cBegin++;		

		if (c > 0x7F)
		{
			c = '?';
		}
		str2.push_back(c);
	}
}


#ifdef HAVE_ICONV
void  CGlobalRecords::wide2str16(const ustring& str1, u16string& str2)
{
	size_t					resultSize, inbytesleft, outbytesleft;
	const wchar_t			*inbuf;
	iconv_t					cd;
	unsigned16_t			*outbuf, *origOutbuf;
	static const unsigned16_t convFail[] = { 'i', 'c', 'o', 'n', 'v', ' ', 'f', 'a', 'i', 'l', 'e', 'd', '!' };
	
	cd = iconv_open(UCS_2_INTERNAL, iconv_code.c_str());
	// no need to test return code as we ALREADY did this when setting iconv_code in workbook
	
	inbytesleft		= str1.size() * sizeof(unichar_t);
	outbytesleft	= inbytesleft * 4 * sizeof(unsigned16_t); // Unicode expansion 4 to 2 ???

	inbuf		= str1.c_str();
	origOutbuf	= (unsigned16_t *)calloc(outbytesleft, 1);
	outbuf		= origOutbuf;

	resultSize = iconv(cd, (char **)&inbuf, &inbytesleft, (char **)&outbuf, &outbytesleft);
	iconv_close(cd);

	if(resultSize == (size_t)-1) {
		str2 = convFail;
	} else {
		str2.assign(origOutbuf, outbuf - origOutbuf);
	}
	free((void *)origOutbuf);
}
#else
void  CGlobalRecords::wide2str16(const ustring& str1, u16string& str2)
{
	ustring::const_iterator	cBegin, cEnd;
	size_t	len;

#if !defined(_MSC_VER) /*  MSVC2005 doesn't accept this for ANSI builds at least! */
	// if character size of both strings is the same, well, we should be able to just assign them
	if(sizeof(unichar_t) == sizeof(unsigned16_t)) {
		str2 = str1;
		return;
	}
#endif
	
	str2.clear();

	len = str1.length();
	str2.reserve(len);

	cBegin	= str1.begin();
	cEnd	= str1.end();
	
	while(cBegin != cEnd) 
	{
		str2.push_back((unsigned16_t)*cBegin++);		
	}
	XL_ASSERT(str2.length() == str1.length());
}

#endif

void  CGlobalRecords::char2str16(const string& str1, u16string& str2)
{
	string::const_iterator	cBegin, cEnd;
	size_t	len;
	
	str2.clear();

	len = str1.length();
	str2.reserve(len);

	cBegin	= str1.begin();
	cEnd	= str1.end();
	
	while(cBegin != cEnd) 
	{
		str2.push_back((unsigned16_t)*cBegin++);		
	}
	XL_ASSERT(str2.length() == str1.length());
}

bool CGlobalRecords::IsASCII(const std::string& str)
{
	std::string::const_iterator cBegin, cEnd;

	cBegin	= str.begin();
	cEnd	= str.end();

	unsigned16_t c = 0;

	while(cBegin != cEnd) 
	{
		c |= *cBegin++;		
	}

	return (c <= 0x7F);
}

bool CGlobalRecords::IsASCII(const u16string& str)
{
	u16string::const_iterator cBegin, cEnd;

	cBegin	= str.begin();
	cEnd	= str.end();

	unsigned16_t c = 0;

	while(cBegin != cEnd) 
	{
		c |= *cBegin++;		
	}

	return (c <= 0x7F);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: globalrec.cpp,v $
 * Revision 1.12  2009/03/02 04:08:43  dhoerl
 * Code is now compliant to gcc  -Weffc++
 *
 * Revision 1.11  2009/01/23 16:09:55  dhoerl
 * General cleanup: headers and includes. Fixed issues building mainC and mainCPP
 *
 * Revision 1.10  2009/01/09 15:04:26  dhoerl
 * GlobalRec now used only as a reference.
 *
 * Revision 1.9  2009/01/09 03:23:12  dhoerl
 * GlobalRec references tuning
 *
 * Revision 1.8  2009/01/08 22:16:06  dhoerl
 * January Rework
 *
 * Revision 1.7  2009/01/08 02:52:59  dhoerl
 * December Rework
 *
 * Revision 1.6  2008/12/20 15:49:05  dhoerl
 * 1.2.5 fixes
 *
 * Revision 1.5  2008/12/11 21:12:40  dhoerl
 * Cleanup
 *
 * Revision 1.4  2008/12/10 03:34:31  dhoerl
 * m_usage was 16bit and wrapped
 *
 * Revision 1.3  2008/12/06 01:42:57  dhoerl
 * John Peterson changes along with lots of tweaks. Many bugs that causes Excel crashes fixed.
 *
 * Revision 1.2  2008/10/25 18:39:54  dhoerl
 * 2008
 *
 * Revision 1.1.1.1  2004/08/27 16:31:56  darioglz
 * Initial Import.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */





#if defined(_MSC_VER)
#undef new
void *operator_new_dbg(size_t count, const char *f, int l)
{
	return operator new(count, _CLIENT_BLOCK, f, l);
}
#endif


