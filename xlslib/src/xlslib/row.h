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
 * $Source: /cvsroot/xlslib/xlslib/src/xlslib/row.h,v $
 * $Revision: 1.5 $
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


#ifndef ROW_H
#define ROW_H

#include <config.h>
#include <common.h>
#include <record.h>
#include <extformat.h>

/*
******************************
CRow class declaration
******************************
*/


#include <xls_pshpack2.h>

namespace xlslib_core
{

#define ROW_DFLT_HEIGHT        0x0108
#define ROW_DFLT_GRBIT         0x0080
#define ROW_GRBIT_UNSYNC       0x0040
#define ROW_DFLT_IXFE          XF_PROP_XF_DEFAULT_CELL
#define ROW_OFFSET_FIRSTCOL    6
#define ROW_OFFSET_LASTCOL     8
#define ROW_MASK_STDHEIGHT     0x0108
#define ROW_RECORD_SIZE        (20)


	// forward ref
	class CDataStorage;

  class rowheight_t
    {
    public:
      rowheight_t() : num(0), height(0), xformat(NULL) {};
      rowheight_t(unsigned32_t rownum, unsigned16_t rowheight, xf_t *pxformat) 
        : num(rownum), height(rowheight), xformat(pxformat) {}
      ~rowheight_t() {} // TODO: ?? {if(xformat)xformat->UnMarkUsed();};

      unsigned32_t GetRowNum() {return num;};
      void SetRowNum(unsigned32_t rownum) {num = rownum;};

      unsigned16_t GetRowHeight() {return height;};
      void SetRowHeight(unsigned16_t rowheight) {height = rowheight;};

      xf_t* GetXF(void) const {return xformat;};

      bool operator<(const rowheight_t& right) const{
        return (num < right.num);
      };

      bool operator>(const rowheight_t& right) const{
        return (num > right.num);
      };

      bool operator==(const rowheight_t& right) const{
        return (num == right.num);
      };
  
      bool operator!=(const rowheight_t& right) const{
        return (num != right.num);
      };

	private:
		rowheight_t(const rowheight_t& that);
		rowheight_t& operator=(const rowheight_t& right);

    private:
		unsigned32_t num;
		unsigned16_t height;
		xf_t* xformat;
    };

  class rowheightsort
  {
    public:
		bool operator()(rowheight_t* const &a, rowheight_t* const &b) const
							{
							  return (a->GetRowNum() < b->GetRowNum());
							};
  };

  typedef std::set<xlslib_core::rowheight_t*,rowheightsort XLSLIB_DFLT_ALLOCATOR> RowHeight_Vect_t;
  typedef RowHeight_Vect_t::iterator RowHeight_Vect_Itor_t;

#if 0
  // NOTE: row_t has no height field
  typedef struct
  {
    unsigned32_t rownum;
    unsigned32_t firstcol;
    unsigned32_t lastcol;
	xf_t		 *xformat;
  } row_t;
  typedef std::vector<xlslib_core::row_t* XLSLIB_DFLT_ALLOCATOR> Row_Vect_t;
  typedef Row_Vect_t::iterator Row_Vect_Itor_t;
#endif

  class CRow: public CRecord
    {
#if defined(LEIGHTWEIGHT_UNIT_FEATURE)
	friend class CDataStorage;
#endif

    protected:
      CRow(CDataStorage &datastore, 
		   unsigned32_t rownum,
           unsigned32_t firstcol,
           unsigned32_t lastcol,
           unsigned16_t rowheight  = ROW_DFLT_HEIGHT,
		   const xf_t* xformat = NULL);
      //CRow(row_t& rowdef);
	private:
      virtual ~CRow();

    public:

#if 0
	  void SetFirstCol(unsigned32_t firstrow);
      void SetLastCol(unsigned32_t lastrow);
      unsigned32_t GetFirstCol(void);
      unsigned32_t GetLastCol(void);
#endif
  };


  /*
******************************
CDBCell class declaration
******************************
*/

#define DBC_DFLT_STARTBLOCK  (0x00000000)

  class CDBCell: public CRecord
    {
#if defined(LEIGHTWEIGHT_UNIT_FEATURE)
	friend class CDataStorage;
#endif

    protected:
      CDBCell(CDataStorage &datastore, size_t startblock = DBC_DFLT_STARTBLOCK);
	private:
      virtual ~CDBCell();

	public:
      void AddRowOffset(size_t rowoffset);
    };

}


#include <xls_poppack.h>

#endif //ROW_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: row.h,v $
 * Revision 1.5  2009/03/02 04:08:43  dhoerl
 * Code is now compliant to gcc  -Weffc++
 *
 * Revision 1.4  2009/01/23 16:09:55  dhoerl
 * General cleanup: headers and includes. Fixed issues building mainC and mainCPP
 *
 * Revision 1.3  2009/01/08 02:52:47  dhoerl
 * December Rework
 *
 * Revision 1.2  2008/10/25 18:39:54  dhoerl
 * 2008
 *
 * Revision 1.1.1.1  2004/08/27 16:31:46  darioglz
 * Initial Import.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

