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
 * $Source: /cvsroot/xlslib/xlslib/src/xlslib/sheetrec.h,v $
 * $Revision: 1.8 $
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

#ifndef SHEETREC_H
#define SHEETREC_H

#include <xlsys.h>
#include <common.h>
#include <unit.h>
#include <biffsection.h>
#include <recdef.h>
#include <index.h>
#include <extformat.h>
#include <label.h>
#include <boolean.h>
#include <err.h>
#include <blank.h>
#include <number.h>
#include <row.h>
#include <merged.h>
#include <colinfo.h>
#include <range.h>


#include <xls_pshpack2.h>

namespace xlslib_core
{

  /*
***********************************
worksheet class declaration
***********************************
*/

  typedef enum
    {
      SHEET_INIT,
      SHEET_BOF,
      SHEET_INDEX,
      SHEET_DIMENSION,
      SHEET_ROWBLOCKS,
      SHEET_MERGED,
      SHEET_COLINFO,
      SHEET_WINDOW2,
      SHEET_EOF,
      SHEET_FINISH
    } SheetRecordDumpState_t;

  typedef struct rowblocksize_t
  {
    size_t rowandcell_size;
    size_t dbcell_size;
    size_t rows_sofar;
    size_t cells_sofar;

	unsigned32_t first_col;
	unsigned32_t last_col;
	unsigned32_t first_row;
	unsigned32_t last_row;

	// -------------------------------------

	rowblocksize_t():
		rowandcell_size(0),
		dbcell_size(0),
		rows_sofar(0),
		cells_sofar(0),
		first_col((unsigned32_t)(-1)),
		last_col(0),
		first_row((unsigned32_t)(-1)),
		last_row(0)
	{
	}

	void reset(void)
	{
		rowandcell_size = 0;
		dbcell_size = 0;
		rows_sofar = 0;
		cells_sofar = 0;

		first_col = (unsigned32_t)(-1);
		last_col = 0;
		first_row = (unsigned32_t)(-1);
		last_row = 0;
	}
  } rowblocksize_t;

  typedef std::vector<xlslib_core::rowblocksize_t XLSLIB_DFLT_ALLOCATOR> RBSize_Vect_t;
  typedef RBSize_Vect_t::iterator RBSize_Vect_Itor_t;

  typedef enum
    {
      RB_INIT,
      RB_FIRST_ROW,
      RB_ROWS,
      RB_FIRSTCELL,
      RB_CELLS,
      RB_DBCELL,
      RB_FINISH

    } DumpRowBlocksState_t;

  typedef std::vector<size_t XLSLIB_DFLT_ALLOCATOR> CellOffsets_Vect_t;
  typedef CellOffsets_Vect_t::iterator CellOffsets_Vect_Itor_t;

//  class range;
  class worksheet
    : public CBiffSection
    {
	  friend class workbook;

    private:
		CGlobalRecords&			m_GlobalRecords;
		SheetRecordDumpState_t	m_DumpState;
		CUnit*					m_pCurrentData;

		Range_Vect_t			m_MergedRanges;

		Colinfo_Set_t			m_Colinfos;
		Colinfo_Set_Itor_t		m_Current_Colinfo;

		RowHeight_Vect_t		m_RowHeights;
		RowHeight_Vect_Itor_t	m_Current_RowHeight;

		unsigned32_t			minRow, minCol, maxRow, maxCol;
		unsigned16_t			sheetIndex;

		Cell_Set_t				m_Cells;
		Cell_Set_Itor_t m_CurrentCell;		// Init this one in the RowBlocksDump INIT state
		Cell_Set_Itor_t m_CurrentSizeCell; // Init this one in the INIT state
		//bool m_CellsSorted;

#ifdef RANGE_FEATURE
		RangeObj_Vect_t			m_Ranges;
#endif
		RBSize_Vect_t			m_RBSizes;
		RBSize_Vect_Itor_t		m_Current_RBSize;
		bool					m_SizesCalculated;

		DumpRowBlocksState_t	m_DumpRBState;
		unsigned32_t			m_RowCounter;
		unsigned32_t			m_CellCounter;
		size_t					m_DBCellOffset;
		CellOffsets_Vect_t		m_CellOffsets;

		//unsigned32_t			m_CurrentRowBlock;
		Cell_Set_Itor_t			m_Starting_RBCell;
		
		// cache a bit for speedups
		Cell_Set_Itor_t			cellIterHint;
		cell_t*					cellHint;

    private:
		worksheet(CGlobalRecords& gRecords, unsigned16_t idx);
		~worksheet();

		//void					GetFirstLastRows(unsigned32_t* first_row, unsigned32_t* last_row);
		size_t GetNumRowBlocks(rowblocksize_t* rbsize_ref = NULL);
		bool					GetRowBlockSizes(rowblocksize_t& rbsize);
		CUnit*					RowBlocksDump(CDataStorage &datastore);
		size_t EstimateNumBiffUnitsNeeded(void);

		void					AddCell(cell_t* pcell);
		CUnit*					DumpData(CDataStorage &datastore, size_t offset);
		
    private:
		worksheet( const worksheet& that);
		worksheet& operator=(const worksheet& right);

    public:
		void					MakeActive();	// makes this sheet come up first
		size_t NumCells() const { return m_Cells.size(); };

		cell_t*					FindCell(unsigned32_t row, unsigned32_t col) const;
		cell_t*					FindCellOrMakeBlank(unsigned32_t row, unsigned32_t col);

		void					GetFirstLastRowsAndColumns(unsigned32_t* first_row, unsigned32_t* last_row, unsigned32_t* first_col, unsigned32_t* last_col); /* [i_a] */

		// Cell operations
		void merge(unsigned32_t first_row, unsigned32_t first_col, 
				 unsigned32_t last_row, unsigned32_t last_col);
		void colwidth(unsigned32_t col, unsigned16_t width, xf_t* pxformat = NULL);			// sets column widths to 1/256 x width of "0"
		void rowheight(unsigned32_t row, unsigned16_t height, xf_t* pxformat = NULL);		// in points (Excel uses twips, 1/20th of a point, but we dont)

#ifdef RANGE_FEATURE
		// Ranges
		range* rangegroup(unsigned32_t row1, unsigned32_t col1,
						unsigned32_t row2, unsigned32_t col2);
#endif
		// Cells
		cell_t* blank(unsigned32_t row, unsigned32_t col, 
					xf_t* pxformat = NULL);

		cell_t* label(unsigned32_t row, unsigned32_t col, 
					const std::string& strlabel, xf_t* pxformat = NULL);
		cell_t* label(unsigned32_t row, unsigned32_t col, 
					const std::ustring& strlabel, xf_t* pxformat = NULL);

		cell_t* number(unsigned32_t row, unsigned32_t col, 
					 double numval,  format_number_t fmtval, xf_t* pxformat);	// Deprecated
		cell_t* number(unsigned32_t row, unsigned32_t col, 
					double numval, xf_t* pxformat = NULL);
		// 536870911 >= numval >= -536870912
		cell_t* number(unsigned32_t row, unsigned32_t col, 
					signed32_t numval, xf_t* pxformat = NULL);

		cell_t* boolean(unsigned32_t row, unsigned32_t col, 
			bool boolval, xf_t* pxformat = NULL);

		cell_t* error(unsigned32_t row, unsigned32_t col, 
			errcode_t errorcode, xf_t* pxformat = NULL);
	};

  typedef std::vector<xlslib_core::worksheet*> Sheets_Vector_t;
  typedef Sheets_Vector_t::iterator Sheets_Vector_Itor_t;
}


#include <xls_poppack.h>

#endif // SHEETREC_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: sheetrec.h,v $
 * Revision 1.8  2009/03/02 04:08:43  dhoerl
 * Code is now compliant to gcc  -Weffc++
 *
 * Revision 1.7  2009/01/23 16:09:55  dhoerl
 * General cleanup: headers and includes. Fixed issues building mainC and mainCPP
 *
 * Revision 1.6  2009/01/10 21:10:51  dhoerl
 * More tweaks
 *
 * Revision 1.5  2009/01/08 22:16:06  dhoerl
 * January Rework
 *
 * Revision 1.4  2009/01/08 02:52:47  dhoerl
 * December Rework
 *
 * Revision 1.3  2008/12/06 01:42:57  dhoerl
 * John Peterson changes along with lots of tweaks. Many bugs that causes Excel crashes fixed.
 *
 * Revision 1.2  2008/10/25 18:39:54  dhoerl
 * 2008
 *
 * Revision 1.1.1.1  2004/08/27 16:31:44  darioglz
 * Initial Import.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

