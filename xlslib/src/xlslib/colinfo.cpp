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
 * $Source: /cvsroot/xlslib/xlslib/src/xlslib/colinfo.cpp,v $
 * $Revision: 1.3 $
 * $Author: dhoerl $
 * $Date: 2009/01/08 02:53:15 $
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * File description:
 *
 *
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <xlsys.h>

#include <colinfo.h>

using namespace std;
using namespace xlslib_core;

CColInfo::CColInfo(CDataStorage &datastore, const colinfo_t* newci):
		CRecord(datastore)
{
   SetRecordType(RECTYPE_COLINFO);

   AddValue16((unsigned16_t)newci->colfirst);
   AddValue16((unsigned16_t)newci->collast);
   AddValue16(newci->width);

   if(newci->xformat != NULL)
      AddValue16(newci->xformat->GetIndex());
   else
      AddValue16(0x0F);	// was 0 - but default CELL format is 15

   AddValue16(newci->flags);
   AddValue16(0);

   SetRecordLength(GetDataSize()-4);
}

CColInfo::~CColInfo()
{
      
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: colinfo.cpp,v $
 * Revision 1.3  2009/01/08 02:53:15  dhoerl
 * December Rework
 *
 * Revision 1.2  2008/10/25 18:39:54  dhoerl
 * 2008
 *
 * Revision 1.1.1.1  2004/08/27 16:31:49  darioglz
 * Initial Import.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

