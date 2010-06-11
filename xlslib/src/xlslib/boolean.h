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
 * $Source: /cvsroot/xlslib/xlslib/src/xlslib/blank.h,v $
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
#ifndef BOOLEAN_XL_H
#define BOOLEAN_XL_H

#include <xlsys.h>
#include <common.h>

#include <cell.h>
#include <record.h>
#include <unit.h>


#include <xls_pshpack2.h>

namespace xlslib_core
{

  class boolean_t: public cell_t
    {
	  friend class worksheet;

    private:
      boolean_t(CGlobalRecords& gRecords, unsigned32_t rowval, unsigned32_t colval, bool value, xf_t* pxfval = NULL);
	  virtual ~boolean_t(){};
	  
    public:
      virtual size_t GetSize(void) const {return 12;};
      virtual CUnit* GetData(CDataStorage &datastore) const;

	private:
		bool num;

	public:
		bool GetBoolean(void) const {return !!num;};
    };


  // forward ref
	class CDataStorage;

  class CBoolean: public CRecord
    {
#if defined(LEIGHTWEIGHT_UNIT_FEATURE)
	friend class CDataStorage;
#endif

    protected:
      CBoolean(CDataStorage &datastore, const boolean_t& booldef);

	private:
      virtual ~CBoolean();
    };

}


#include <xls_poppack.h>

#endif

