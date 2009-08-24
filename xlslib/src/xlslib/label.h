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
 * $Source: /cvsroot/xlslib/xlslib/src/xlslib/label.h,v $
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


#ifndef LABEL_H
#define LABEL_H

#include <config.h>
#include <common.h>
#include <cell.h>
#include <record.h>
#include <unit.h>
#include <extformat.h>

namespace xlslib_core
{

#define LABEL_DFLT_XFINDEX              ((unsigned16_t)0x000f)
#define LABEL_OFFSET_FIRSTCOL           ((unsigned8_t)6)
#define LABEL_OFFSET_LASTCOL            ((unsigned8_t)8)

  class label_t: public cell_t
    {
	  friend class worksheet;

    private:
      label_t(CGlobalRecords& gRecords, unsigned16_t rowval, unsigned16_t colval, const u16string& labelstrval, xf_t* pxfval = NULL);
      virtual ~label_t();

    private:
	  u16string	strLabel;
	  bool		isASCII;

    public:
      const u16string		*GetStrLabel() const { return &strLabel; };
      bool					GetIsASCII() const { return isASCII; };

      virtual unsigned16_t GetSize() const;
      virtual CUnit* GetData() const;
    };

  class CLabel: public CRecord
    {
    private:

    public:
      CLabel(unsigned16_t row,
             unsigned16_t col,
             const u16string& strlabel,
			 bool isASCII,
             const xf_t* pxfval = NULL);
      CLabel(label_t& labeldef);
      ~CLabel();
    };
}
#endif //LABEL_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: label.h,v $
 * Revision 1.5  2009/03/02 04:08:43  dhoerl
 * Code is now compliant to gcc  -Weffc++
 *
 * Revision 1.4  2009/01/10 21:10:50  dhoerl
 * More tweaks
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

