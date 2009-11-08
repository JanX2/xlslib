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
 * $Source: /cvsroot/xlslib/xlslib/src/xlslib/range.h,v $
 * $Revision: 1.6 $
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

#ifndef RANGE_H
#define RANGE_H


#include <config.h>
#include <common.h>


#include <sheetrec.h>
#include <font.h>
#include <cell.h>
#include <extformat.h>

#ifdef RANGE_FEATURE

namespace xlslib_core
{
  class worksheet;
  class range :
    public xf_i,
    public font_i,
    private range_t 
  {
    public:
		range(unsigned16_t row1, unsigned16_t col1,	// inclusive
			unsigned16_t row2, unsigned16_t col2,
			worksheet* pws);
		virtual ~range();

		void cellcolor(color_name_t color); 
		void boxer(border_style_t border, fill_option_t fill, color_name_t borderColor, color_name_t fillFgColor, color_name_t fillBgColor);  

    private:
		worksheet* m_pWorkSheet;
		bool m_Atomic;

    public:
		range(const range& that);
		range& operator=(const range& right);

    public: // xf_i interface declaration
		void font(font_t* fontidx);
		void format(format_number_t formatidx);
		void format(format_t* format);
		void halign(halign_option_t ha_option);
		void valign(valign_option_t va_option);
		void indent(indent_option_t indent_option);
		void orientation(txtori_option_t ori_option);
		void fillfgcolor(color_name_t color);
		void fillfgcolor(unsigned8_t color);
		void fillbgcolor(color_name_t color);
		void fillbgcolor(unsigned8_t color);
		void fillstyle(fill_option_t fill);
		void locked(bool locked_opt);
		void hidden(bool hidden_opt);
		void wrap(bool wrap_opt);
		void borderstyle(border_side_t side, border_style_t style);
		void bordercolor(border_side_t side, unsigned8_t color);
		void bordercolor(border_side_t side, color_name_t color);

    public: // font_i interface declaration
		void fontname(std::string fntname);
		void fontheight(unsigned16_t fntheight);
		void fontbold(boldness_option_t fntboldness);
		void fontunderline(underline_option_t fntunderline);
		void fontscript(script_option_t fntscript);
		void fontcolor(color_name_t fntcolor);
		void fontcolor(unsigned8_t fntcolor);
		void fontitalic(bool italic);
		void fontstrikeout(bool so);
		void fontoutline(bool ol);
		void fontshadow(bool sh);
    };

	typedef enum {
		BORDER_BOTTOM_BIT	= 0x01,
		BORDER_TOP_BIT		= 0x02,
		BORDER_LEFT_BIT		= 0x04,
		BORDER_RIGHT_BIT	= 0x08
	} borderBits_t;

	typedef struct {
		unsigned32_t	flags;
		xf_t			*xft;
	} borderedXft;

	typedef std::pair<xlslib_core::xf_t *, borderedXft> xf_Pair_t;
	class xfSorter {
		public:
			bool operator() (const xf_Pair_t& left, const xf_Pair_t& right) const
			{
				if((left.first)->index < (right.first)->index) return true;
				if((left.first)->index > (right.first)->index) return false;
				return ((left.second).flags < (right.second).flags) ? true : false;
			}
	};
	
	typedef std::set<xf_Pair_t, xlslib_core::xfSorter> xf_Pair_Set_t;
	typedef xf_Pair_Set_t::iterator xf_Pair_Set_Itor_t;
}

#endif //RANGE_H 

#endif // RANGE_FEATURE
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: range.h,v $
 * Revision 1.6  2009/03/02 04:08:43  dhoerl
 * Code is now compliant to gcc  -Weffc++
 *
 * Revision 1.5  2009/01/23 16:09:55  dhoerl
 * General cleanup: headers and includes. Fixed issues building mainC and mainCPP
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
 * Revision 1.1.1.1  2004/08/27 16:31:49  darioglz
 * Initial Import.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
 