/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * This file is part of xlslib -- A multiplatform, C/C++ library
 * for dynamic generation of Excel(TM) files.
 *
 * Copyright 2004 Yeico S. A. de C. V. All Rights Reserved.
 * Copyright 2008 David Hoerl All Rights Reserved.
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

#ifndef SHEETREC_H
#define SHEETREC_H

#include "common/xlsys.h"
#include "common/systype.h"

#include "xlslib/common.h"
#include "xlslib/unit.h"
#include "xlslib/biffsection.h"
#include "xlslib/recdef.h"
#include "xlslib/index.h"
#include "xlslib/extformat.h"
#include "xlslib/label.h"
#include "xlslib/boolean.h"
#include "xlslib/err.h"
#include "xlslib/blank.h"
#include "xlslib/number.h"
#include "xlslib/note.h"
#include "xlslib/formula.h"
#include "xlslib/row.h"
#include "xlslib/merged.h"
#include "xlslib/colinfo.h"
#include "xlslib/range.h"


// #include "common/xls_pshpack2.h"

namespace xlslib_core
{

  /*
***********************************
worksheet class declaration
***********************************
*/

#if 0
// old way
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


// what worked
  typedef enum
    {
      SHEET_INIT,
      SHEET_BOF,
      SHEET_INDEX,
	  SHEET_DFLT_ROW_HEIGHT,
	  SHEET_DFLT_COL_WIDTH,
      SHEET_DIMENSION,
      SHEET_ROWBLOCKS,
      SHEET_MERGED,
      SHEET_COLINFO,
      SHEET_WINDOW2,
      SHEET_H_LINKS,
      SHEET_EOF,
      SHEET_FINISH
    } SheetRecordDumpState_t;

// Mostly what I wanted that worked
  typedef enum
    {
      SHEET_INIT,
      SHEET_BOF,
      SHEET_INDEX,
	  SHEET_DFLT_ROW_HEIGHT,
	  SHEET_DFLT_COL_WIDTH,
      SHEET_DIMENSION,
      SHEET_ROWBLOCKS,
      SHEET_MERGED,
      SHEET_COLINFO,
      SHEET_WINDOW2,
      SHEET_H_LINKS,
      SHEET_EOF,
      SHEET_FINISH
    } SheetRecordDumpState_t;

// What I wanted
  typedef enum
    {
      SHEET_INIT,
      SHEET_BOF,
      SHEET_INDEX,
	  SHEET_DFLT_ROW_HEIGHT,
      SHEET_COLINFO,
	  SHEET_DFLT_COL_WIDTH,
      SHEET_DIMENSION,
      SHEET_ROWBLOCKS,
      SHEET_WINDOW2,
      SHEET_MERGED,
      SHEET_H_LINKS,
      SHEET_EOF,
      SHEET_FINISH
    } SheetRecordDumpState_t;
#endif

  typedef enum
    {
      SHEET_INIT,
      SHEET_BOF,
      SHEET_INDEX,
	  SHEET_DFLT_ROW_HEIGHT,
      SHEET_COLINFO,
	  SHEET_DFLT_COL_WIDTH,
      SHEET_DIMENSION,
      SHEET_ROWBLOCKS,
      SHEET_MERGED,
      SHEET_WINDOW2,
      SHEET_H_LINKS,
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

  struct HyperLink {
	unsigned16_t		row;
	unsigned16_t		col;
	u16string			url;
	u16string			mark;
  };
  typedef std::vector<xlslib_core::HyperLink * XLSLIB_DFLT_ALLOCATOR> HyperLinkList_t;
  typedef HyperLinkList_t::iterator HyperLinkList_Itor_t;

  typedef std::vector<xlslib_core::CUnit* XLSLIB_DFLT_ALLOCATOR> ColInfo_t;
  typedef ColInfo_t::iterator ColInfo_Itor_t;

//  class range;
  class worksheet : public CBiffSection
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
		Cell_Set_Itor_t			m_CurrentCell;		// Init this one in the RowBlocksDump INIT state
		Cell_Set_Itor_t			m_CurrentSizeCell;	// Init this one in the INIT state
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
		size_t					m_FirstRowOffset;
		CellOffsets_Vect_t		m_CellOffsets;

		//unsigned32_t			m_CurrentRowBlock;
		Cell_Set_Itor_t			m_Starting_RBCell;
		
		// cache a bit for speedups
		Cell_Set_Itor_t			cellIterHint;
		cell_t*					cellHint;

		bool					defRowsHidden;
		unsigned16_t			defRowHeight;
		unsigned16_t			defColWidth;

		HyperLinkList_t			m_HyperLinks;
		HyperLinkList_Itor_t	m_CurrentHlink;

		ColInfo_t				m_ColInfoUnits;
		ColInfo_Itor_t			m_ColInfoUnit;
		
    private:
		worksheet(CGlobalRecords& gRecords, unsigned16_t idx);
		~worksheet();

		unsigned32_t			ColInfoDump(CDataStorage &datastore);
		
		//void					GetFirstLastRows(unsigned32_t* first_row, unsigned32_t* last_row);
		size_t					GetNumRowBlocks(rowblocksize_t* rbsize_ref = NULL);
		bool					GetRowBlockSizes(rowblocksize_t& rbsize);
		CUnit*					RowBlocksDump(CDataStorage &datastore, const size_t offset);
		size_t					EstimateNumBiffUnitsNeeded(void);

		void					AddCell(cell_t* pcell);
		CUnit*					DumpData(CDataStorage &datastore, size_t offset, size_t writeLen/*, size_t &Last_BOF_offset*/);
		
		CUnit*					MakeHyperLink(CDataStorage& datastore, HyperLink* link);

    private:
		worksheet(const worksheet& that);
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
		void rowheight(unsigned32_t row, unsigned16_t height, xf_t* pxformat = NULL);		// in points (Excel uses twips, 1/20th of a point, but xlslib didn't)

		void defaultRowHeight(unsigned16_t width, bool hidden = false) { defRowHeight = width; defRowsHidden = hidden; } // sets column widths to 1/256 x width of "0"
		void defaultColwidth(unsigned16_t width) { defColWidth = width; } // in points (Excel uses twips, 1/20th of a point, but xlslib didn't)

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
		cell_t* number(unsigned32_t row, unsigned32_t col, 
					unsigned32_t numval, xf_t* pxformat = NULL);

		cell_t* boolean(unsigned32_t row, unsigned32_t col, 
			bool boolval, xf_t* pxformat = NULL);

		cell_t* error(unsigned32_t row, unsigned32_t col, 
			errcode_t errorcode, xf_t* pxformat = NULL);

		cell_t* note(unsigned32_t row, unsigned32_t col, 
			const std::string& remark, const std::string& author, xf_t* pxformat = NULL);
		cell_t* note(unsigned32_t row, unsigned32_t col, 
			const std::ustring& remark, const std::ustring& author, xf_t* pxformat = NULL);

		cell_t* formula(unsigned32_t row, unsigned32_t col, 
			expression_node_t* expression_root, bool auto_destruct_expression_tree = false,
			xf_t* pxformat = NULL);
		
		// define a cell (label, number, etc) - apply proper url (http://blah.blah), possible text mark too
		void hyperLink(const cell_t *cell, const std::string& url , const std::string& mark = ""); 
	};

  typedef std::vector<xlslib_core::worksheet*> Sheets_Vector_t;
  typedef Sheets_Vector_t::iterator Sheets_Vector_Itor_t;
}


// #include "common/xls_poppack.h"

#endif 
// SHEETREC_H

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

