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
#ifndef XL_ERR_H
#define XL_ERR_H

#include <xlsys.h>
#include <common.h>

#include <cell.h>
#include <record.h>
#include <unit.h>


#include <xls_pshpack2.h>

namespace xlslib_core
{
	typedef enum
	{
		XLERR_NULL  = 0x00, // #NULL!
		XLERR_DIV0  = 0x07, // #DIV/0!
		XLERR_VALUE = 0x0F, // #VALUE!
		XLERR_REF   = 0x17, // #REF!
		XLERR_NAME  = 0x1D, // #NAME?
		XLERR_NUM   = 0x24, // #NUM!
		XLERR_N_A   = 0x2A, // #N/A!
	} errcode_t;

  class err_t: public cell_t
    {
	  friend class worksheet;

    private:
      err_t(CGlobalRecords& gRecords, unsigned32_t rowval, unsigned32_t colval, errcode_t value, xf_t* pxfval = NULL);
	  virtual ~err_t(){};
	  
    public:
      virtual size_t GetSize(void) const {return 12;};
      virtual CUnit* GetData(CDataStorage &datastore) const;

	private:
		errcode_t ecode;

	public:
		unsigned8_t GetErr(void) const {return ecode;};
    };


  // forward ref
	class CDataStorage;

  class CErr: public CRecord
    {
#if defined(LEIGHTWEIGHT_UNIT_FEATURE)
	friend class CDataStorage;
#endif

    protected:
      CErr(CDataStorage &datastore, const err_t& errdef);

	private:
      virtual ~CErr();
    };

}


#include <xls_poppack.h>

#endif

