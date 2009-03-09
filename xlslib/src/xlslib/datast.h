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
 * $Source: /cvsroot/xlslib/xlslib/src/xlslib/datast.h,v $
 * $Revision: 1.4 $
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



#ifndef DATAST_H
#define DATAST_H 

#include <config.h>
#include <common.h>
#include <unit.h>

/* 
******************************
COleFile class declaration
******************************
*/

namespace xlslib_core
{

  typedef std::vector<xlslib_core::CUnit* XLSLIB_DFLT_ALLOCATOR> DataList_t;
  typedef DataList_t::iterator DataList_Itor_t;

  class CDataStorage
    {
    private:
		DataList_t data;
		unsigned32_t m_DataSize;
   
    public:
		CDataStorage ();
		CDataStorage (unsigned32_t blobs);
		virtual ~CDataStorage ();

		DataList_Itor_t begin();
		DataList_Itor_t end();

    private:
		CDataStorage(const CDataStorage& that);
		CDataStorage& operator=(const CDataStorage& right);

    public:
		
		void operator+=(CUnit* from);
		unsigned32_t GetDataSize();

    };
}
#endif //DATAST_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: datast.h,v $
 * Revision 1.4  2009/03/02 04:08:43  dhoerl
 * Code is now compliant to gcc  -Weffc++
 *
 * Revision 1.3  2009/01/08 02:53:15  dhoerl
 * December Rework
 *
 * Revision 1.2  2008/10/25 18:39:54  dhoerl
 * 2008
 *
 * Revision 1.1.1.1  2004/08/27 16:31:54  darioglz
 * Initial Import.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

