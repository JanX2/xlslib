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
 * $Source: /cvsroot/xlslib/xlslib/src/xlslib/datast.cpp,v $
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

#include <datast.h>

namespace xlslib_core
{

/* 
***********************************
CDataStorage class Implementation
***********************************
*/

CDataStorage::CDataStorage() :
	data(),
	m_DataSize(0)
{
	data.reserve(100);
}
CDataStorage::CDataStorage(unsigned32_t blobs) :
	data(), 
	m_DataSize(0)
{
	data.reserve(blobs);
}

CDataStorage::~CDataStorage()
{
  // Delete all the data. (Only if it exists)
  if(!data.empty())
  {
	DataList_Itor_t x0, x1;

	x0 = data.begin();
	x1 = data.end();
	for(DataList_Itor_t di = x0; di != x1; ++di)
	 {
		delete (*di);
	 }
	 data.resize(0);
  }  
}

void CDataStorage::operator+=(CUnit* from)
{
  data.push_back(from);
  m_DataSize += from->GetDataSize();
}

unsigned32_t CDataStorage::GetDataSize()
{
  return m_DataSize;
}

DataList_Itor_t CDataStorage::begin()
{
  return data.begin();
}
DataList_Itor_t CDataStorage::end()
{
  return data.end();
}

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: datast.cpp,v $
 * Revision 1.4  2009/03/02 04:08:43  dhoerl
 * Code is now compliant to gcc  -Weffc++
 *
 * Revision 1.3  2009/01/08 02:53:15  dhoerl
 * December Rework
 *
 * Revision 1.2  2008/10/25 18:39:54  dhoerl
 * 2008
 *
 * Revision 1.1.1.1  2004/08/27 16:31:46  darioglz
 * Initial Import.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

