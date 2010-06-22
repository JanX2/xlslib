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
 * $Source: /cvsroot/xlslib/xlslib/src/xlslib/range.cpp,v $
 * $Revision: 1.7 $
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

#include <range.h>
#include <extformat.h>

#ifdef RANGE_FEATURE

using namespace std;
using namespace xlslib_core;

range::range(unsigned32_t row1,
             unsigned32_t col1,
             unsigned32_t row2,
             unsigned32_t col2,
             worksheet* pws) :
	m_pWorkSheet(pws),
	m_Atomic(false)
{
	XL_ASSERT(row2>=row1);
	XL_ASSERT(col2>=col1);
	
	// cannot do in initializer list, since these are inherited
	first_row	= row1;
	last_row	= row2;
	first_col	= col1;
	last_col	= col2;
}

range::~range()
{
}

#if 1
// Special case: two methods are called
void range::cellcolor(color_name_t color)
{
	unsigned32_t					r,c;
	xf_Pair_Set_t					prSet;
	xf_t							*xf;
	pair<xf_Pair_Set_Itor_t, bool>	ret;
	xf_Pair_Set_Itor_t				prIter;
	borderedXft						dummy;

	for(r = first_row; r <= last_row; r++) {
		for(c = first_col; c <= last_col; c++)
		{
			cell_t* cell = m_pWorkSheet->FindCellOrMakeBlank(r,c);
			xf = cell->GetXF();

			dummy.flags = xf->flags;
			dummy.xft	= NULL;
			prIter = prSet.find(xf_Pair_t(xf, dummy));

			if(prIter != prSet.end()) {
				// cerr << "hit: row=" << r << " col=" << c << endl;
				// Match - reuse it!
				cell->SetXF((*prIter).second.xft);
			} else {				
				//cerr << "NEW: row=" << r << " col=" << c << endl;
				cell->fillfgcolor(color);
				cell->fillstyle(FILL_SOLID);
				
				dummy.flags = xf->flags;
				dummy.xft	= cell->GetXF();

				xf_Pair_t pr = xf_Pair_t(xf, dummy);
				prSet.insert(pr);	// cannot fail, as we just tested for the xf key!
			}
		}  
	}
}
#else
// Special case: two methods are called
void range::cellcolor(color_name_t color)
{
	unsigned32_t					r,c;

	for(r = first_row; r <= last_row; r++) {
		for(c = first_col; c <= last_col; c++)
		{
				cell_t* cell = m_pWorkSheet->FindCellOrMakeBlank(r,c);
				cell->fillfgcolor(color);
				cell->fillstyle(FILL_SOLID);
		}  
	}
}
#endif

void range::boxer(border_style_t borderStyle, fill_option_t fillStyle, color_name_t borderColor, color_name_t fillFgColor, color_name_t fillBgColor)
{
	unsigned32_t					r,c;
	xf_Pair_Set_t					prSet;
	xf_t							*xf;
	pair<xf_Pair_Set_Itor_t, bool>	ret;
	xf_Pair_Set_Itor_t				prIter;
	borderedXft						bxtf;

	for(r = first_row; r <= last_row; r++) {
		for(c = first_col; c <= last_col; c++)
		{
			// Set 1: figure out the border since it varies the most
			bxtf.flags = 0;
			
			if(r == first_row)	bxtf.flags |= BORDER_TOP_BIT;
			if(r == last_row)	bxtf.flags |= BORDER_BOTTOM_BIT;
			if(c == first_col)	bxtf.flags |= BORDER_LEFT_BIT;
			if(c == last_col)	bxtf.flags |= BORDER_RIGHT_BIT;

			cell_t* cell = m_pWorkSheet->FindCellOrMakeBlank(r,c);
			xf = cell->GetXF();

			// see if we already created a new modified format...
			bxtf.xft	= NULL;
			prIter		= prSet.find(xf_Pair_t(xf, bxtf));

			if(prIter != prSet.end()) {
				// Match - reuse it!
				//cerr << "hit: row=" << r << " col=" << c << endl;
				cell->SetXF((*prIter).second.xft);
			} else {				
				//cerr << "NEW: row=" << r << " col=" << c << endl;
				if(r == first_row) {
					cell->borderstyle(BORDER_TOP, borderStyle);
					cell->bordercolor(BORDER_TOP, borderColor);
				}
				if(r == last_row) {
					cell->borderstyle(BORDER_BOTTOM, borderStyle);
					cell->bordercolor(BORDER_BOTTOM, borderColor);
				}
				if(c == first_col) {
					cell->borderstyle(BORDER_LEFT, borderStyle);
					cell->bordercolor(BORDER_LEFT, borderColor);
				}
				if(c == last_col) {
					cell->borderstyle(BORDER_RIGHT, borderStyle);
					cell->bordercolor(BORDER_RIGHT, borderColor);
				}
				
				cell->fillfgcolor(fillFgColor);
				cell->fillfgcolor(fillBgColor);
				cell->fillstyle(fillStyle);
				
				bxtf.xft = cell->GetXF();
				prSet.insert(xf_Pair_t(xf, bxtf));	// cannot fail, as we just tested for the xf key!
			}
		}  
	}
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
   xf_i interface implementation for range class
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
   
#define SET_RANGE_FUNCTION(function, value)							\
   {																\
      unsigned32_t r,c;												\
      for(r = first_row; r <= last_row; r++)						\
         for(c = first_col; c <= last_col; c++)						\
         {															\
            cell_t* cell = m_pWorkSheet->FindCellOrMakeBlank(r,c);	\
            cell->function(value);									\
         }															\
   }

void range::font(font_t* fontidx)
{
   SET_RANGE_FUNCTION(font, fontidx);
}
void range::format(format_number_t formatidx)
{
   SET_RANGE_FUNCTION(format, formatidx);
}
void range::format(format_t* fmt)
{
   SET_RANGE_FUNCTION(format, fmt);
}
void range::borderstyle(border_side_t side, border_style_t style)
{
   unsigned32_t r,c;
   for(r = first_row; r <= last_row; r++)
      for(c = first_col; c <= last_col; c++)
      {
         cell_t* cell = m_pWorkSheet->FindCellOrMakeBlank(r,c);
         cell->borderstyle(side, style);
      }
}          
void range::bordercolor(border_side_t side, color_name_t color)
{
   unsigned32_t r,c;
   for(r = first_row; r <= last_row; r++)
      for(c = first_col; c <= last_col; c++)
      {
         cell_t* cell = m_pWorkSheet->FindCellOrMakeBlank(r,c);
         cell->bordercolor(side, color);
      }
}          
void range::bordercolor(border_side_t side, unsigned8_t color)
{
   unsigned32_t r,c;
   for(r = first_row; r <= last_row; r++)
      for(c = first_col; c <= last_col; c++)
      {
         cell_t* cell = m_pWorkSheet->FindCellOrMakeBlank(r,c);
         cell->bordercolor(side, color);
      }
}          

void range::halign(halign_option_t ha_option)
{
   SET_RANGE_FUNCTION(halign,ha_option);
}

void range::valign(valign_option_t va_option)
{
   SET_RANGE_FUNCTION(valign,va_option);
}
void range::indent(indent_option_t indent_option)
{
   SET_RANGE_FUNCTION(indent,indent_option);
}
void range::orientation(txtori_option_t ori_option)
{
   SET_RANGE_FUNCTION(orientation,ori_option);
}

void range::fillfgcolor(color_name_t color)
{
   SET_RANGE_FUNCTION(fillfgcolor,color);
}

void range::fillfgcolor(unsigned8_t color)
{
   SET_RANGE_FUNCTION(fillfgcolor,color);
}

void range::fillbgcolor(color_name_t color)
{
   SET_RANGE_FUNCTION(fillbgcolor,color);
}

void range::fillbgcolor(unsigned8_t color)
{
   SET_RANGE_FUNCTION(fillbgcolor,color);
}

void range::fillstyle(fill_option_t fill)
{
   SET_RANGE_FUNCTION(fillstyle,fill);
}

void range::locked(bool locked_opt)
{
   SET_RANGE_FUNCTION(locked,locked_opt);
}

void range::hidden(bool hidden_opt)
{
   SET_RANGE_FUNCTION(hidden,hidden_opt);
}

void range::wrap(bool wrap_opt)
{
   SET_RANGE_FUNCTION(wrap,wrap_opt);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
   font_i interface implementation for range class
   * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void range::fontname(string fntname)
{
   SET_RANGE_FUNCTION(fontname,fntname);
}

void range::fontheight(unsigned16_t fntheight)
{
   SET_RANGE_FUNCTION(fontheight,fntheight);
}

void range::fontbold(boldness_option_t fntboldness)
{
   SET_RANGE_FUNCTION(fontbold,fntboldness);
}

void range::fontunderline(underline_option_t fntunderline)
{
   SET_RANGE_FUNCTION(fontunderline,fntunderline);
}

void range::fontscript(script_option_t fntscript)
{
   SET_RANGE_FUNCTION(fontscript,fntscript);
}

void range::fontcolor(color_name_t fntcolor)
{
   SET_RANGE_FUNCTION(fontcolor,fntcolor);
}

void range::fontcolor(unsigned8_t fntcolor)
{
   SET_RANGE_FUNCTION(fontcolor,fntcolor);
}

void range::fontitalic(bool italic)
{
   SET_RANGE_FUNCTION(fontitalic,italic);
}

void range::fontstrikeout(bool so)
{
   SET_RANGE_FUNCTION(fontstrikeout,so);
}

void range::fontoutline(bool ol)
{
   SET_RANGE_FUNCTION(fontoutline,ol);
}

void range::fontshadow(bool sh)
{
   SET_RANGE_FUNCTION(fontshadow,sh);
}

#endif // RANGE_FEATURE


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: range.cpp,v $
 * Revision 1.7  2009/03/02 04:08:43  dhoerl
 * Code is now compliant to gcc  -Weffc++
 *
 * Revision 1.6  2009/01/23 16:09:55  dhoerl
 * General cleanup: headers and includes. Fixed issues building mainC and mainCPP
 *
 * Revision 1.5  2009/01/10 21:10:50  dhoerl
 * More tweaks
 *
 * Revision 1.4  2009/01/09 03:23:12  dhoerl
 * GlobalRec references tuning
 *
 * Revision 1.3  2009/01/08 02:52:47  dhoerl
 * December Rework
 *
 * Revision 1.2  2008/10/25 18:39:54  dhoerl
 * 2008
 *
 * Revision 1.1.1.1  2004/08/27 16:31:56  darioglz
 * Initial Import.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

