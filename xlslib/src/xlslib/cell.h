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
 * $Source: /cvsroot/xlslib/xlslib/src/xlslib/cell.h,v $
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
 
#ifndef CELL_H
#define CELL_H

#include <xlsys.h>
#include"common.h"

#include <unit.h>
#include <font.h>
#include <extformat.h>


#include <xls_pshpack2.h>

namespace xlslib_core
{
  class insertsort;
  class xf_t;
  
  class cell_t  // : public xf_i , public font_i // Bugs item #2840335 - appears not required (this was here when I got the project :-)
  {
		friend class insertsort;
		friend class worksheet;

    protected:
		cell_t(CGlobalRecords& gRecord, unsigned32_t row, unsigned32_t col);
		virtual ~cell_t();	// "C++ Coding Standards" rule 50 (protected and non-virtual)     [i_a] MUST be virtual or you'll get blown out of the sky by memleaks (label_t instances in cell_t lists)

    private:
		cell_t(const cell_t& that);
		cell_t& operator=(const cell_t& right);

		void set_xf_common(void);
		void set_cell_font(void);

    public:
		unsigned16_t GetXFIndex(void) const;
		unsigned32_t GetRow(void) const;
		unsigned32_t GetCol(void) const;

		void SetXF(xf_t* pxfval);
		xf_t* GetXF(void) const;

		virtual size_t GetSize(void) const = 0;
		virtual CUnit* GetData(CDataStorage &datastore) const = 0;

	protected:
		CGlobalRecords& m_GlobalRecords;
		static const unsigned16_t FORMAT_NUM_OPTIONS_TABLE[];
		unsigned32_t row;
		unsigned32_t col;
		xf_t* pxf;

    public: // xf_i interface
		void font(font_t* font);
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
		void bordercolor(border_side_t side, color_name_t color);
		void bordercolor(border_side_t side, unsigned8_t color);
    public: //font_i interface
		void fontname(const std::string& fntname);
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
#if defined(DEPRECATED)
		void fontattr(unsigned16_t attr);
#endif
  };

  class insertsort
  {
    public:
		bool operator() (cell_t* a, cell_t* b) const
		{
			if(a->row != b->row) {
				return a->row < b->row ? true : false;
			} else {
				return a->col < b->col ? true : false;
			}
		}
  };

  typedef std::set<xlslib_core::cell_t*,insertsort XLSLIB_DFLT_ALLOCATOR> Cell_Set_t;
  typedef Cell_Set_t::iterator Cell_Set_Itor_t;
  typedef Cell_Set_t::const_iterator Cell_Set_CItor_t;
}


#include <xls_poppack.h>

#endif // CELL_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: cell.h,v $
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
 * Revision 1.3  2009/01/08 02:53:15  dhoerl
 * December Rework
 *
 * Revision 1.2  2008/10/25 18:39:53  dhoerl
 * 2008
 *
 * Revision 1.1.1.1  2004/08/27 16:31:47  darioglz
 * Initial Import.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


