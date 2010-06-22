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
 * $Source: /cvsroot/xlslib/xlslib/src/oledoc/oleprop.cpp,v $
 * $Revision: 1.6 $
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

#include <xlsys.h>

#include <oleprop.h>

/* 
***********************************
COleProp class Implementation
***********************************
*/

using namespace xlslib_core;

COleProp::COleProp() :
	m_sName(),
	m_nNameSize(0),
	m_nPropType(0),
	m_nNodeColor(0),

	m_nPrevPropIndex(0), 
	m_nNextPropIndex(0), 
	m_nChildPropIndex(0),

	m_nTSCreatedSeconds(0), 
	m_nTSCreatedDays(0),    
	m_nTSModifiedSeconds(0),
	m_nTSModifiedDays(0),   

	m_nStartBlock(0),
	m_nSize(0),
	m_nIndex(0),
	m_pData(NULL),
	m_Child_List()
{
}
	COleProp::COleProp(signed32_t node_index, const std::string& name, CDataStorage* data) :
	m_sName(name),
	m_nNameSize(0),
	m_nPropType(0),
	m_nNodeColor(0),

	m_nPrevPropIndex(0), 
	m_nNextPropIndex(0), 
	m_nChildPropIndex(0),

	m_nTSCreatedSeconds(0), 
	m_nTSCreatedDays(0),    
	m_nTSModifiedSeconds(0),
	m_nTSModifiedDays(0),   

	m_nStartBlock(0),
	m_nSize(0),
	m_nIndex(node_index),
	m_pData(data),
	m_Child_List()
{
}
COleProp::COleProp(signed32_t node_index, const char *name, CDataStorage* data) :
	m_sName(name),
	m_nNameSize(0),
	m_nPropType(0),
	m_nNodeColor(0),

	m_nPrevPropIndex(0), 
	m_nNextPropIndex(0), 
	m_nChildPropIndex(0),

	m_nTSCreatedSeconds(0), 
	m_nTSCreatedDays(0),    
	m_nTSModifiedSeconds(0),
	m_nTSModifiedDays(0),   

	m_nStartBlock(0),
	m_nSize(0),
	m_nIndex(node_index),
	m_pData(data),
	m_Child_List()
{
}

COleProp::~COleProp()
{

   for(Tree_Level_Itor_t chld = m_Child_List.begin();  chld != m_Child_List.end(); chld++)
      delete *chld;
}



/* 
***********************************
***********************************
*/

int COleProp::Init(signed32_t node_index, const std::string& name, CDataStorage* data)
{
   int errcode = NO_ERRORS;

   SetName(name);
   m_pData = data;
   m_nIndex = node_index;
   
   return errcode;

}

/* 
***********************************
***********************************
*/

int COleProp::SetName(const std::string& name)
{
   int errcode = NO_ERRORS;
  
   m_nNameSize = static_cast<unsigned16_t>(name.size() + 1);
   m_sName = name;

   return errcode;
}

/* 
***********************************
***********************************
*/

const std::string& COleProp::GetName(void) const
{
   return m_sName;
}

/* 
***********************************
***********************************
*/
int COleProp::SetIndex(signed32_t newindex)
{
   int errcode = NO_ERRORS;
  
   m_nIndex = newindex;

   return errcode;

}

/* 
***********************************
***********************************
*/
int COleProp::SetType(unsigned8_t newtype)
{
   int errcode = NO_ERRORS;
  
   m_nPropType = newtype;

   return errcode;
}

/* 
***********************************
***********************************
*/
unsigned8_t COleProp::GetType(void) const
{
   return m_nPropType;
}

/* 
***********************************
***********************************
*/
int COleProp::SetColor(unsigned8_t newcolor)
{
   int errcode = NO_ERRORS;

   m_nNodeColor = newcolor;

   return errcode;

}

/* 
***********************************
***********************************
*/
unsigned8_t COleProp::GetColor(void) const
{
   return m_nNodeColor;
}

/* 
***********************************
***********************************
*/
int COleProp::SetPreviousIndex(signed32_t prev)
{
   int errcode = NO_ERRORS;

   m_nPrevPropIndex = prev;

   return errcode;

}

/* 
***********************************
***********************************
*/
signed32_t COleProp::GetPreviousIndex(void) const
{
   return m_nPrevPropIndex;
}

/* 
***********************************
***********************************
*/
int COleProp::SetNextIndex(signed32_t next)
{
   int errcode = NO_ERRORS;

   m_nNextPropIndex = next;

   return errcode;

}

/* 
***********************************
***********************************
*/
signed32_t COleProp::GetNextIndex(void) const
{
   return m_nNextPropIndex;
}

/* 
***********************************
***********************************
*/
int COleProp::SetChildIndex(signed32_t child)
{
   int errcode = NO_ERRORS;

   m_nChildPropIndex = child;

   return errcode;

}

/* 
***********************************
***********************************
*/
signed32_t COleProp::GetChildIndex(void) const
{
   return m_nChildPropIndex;
}

int COleProp::SetStartBlock(signed32_t sb)
{
   int errcode = NO_ERRORS;
  
   m_nStartBlock = sb;

   return errcode;

}
signed32_t COleProp::GetStartBlock(void) const
{
   return m_nStartBlock;
}

/* 
***********************************
***********************************
*/
int COleProp::SetSize(size_t size)
{
   int errcode = NO_ERRORS;
  
   m_nSize = size;

   return errcode;

}

/* 
***********************************
***********************************
*/

size_t COleProp::GetSize(void) const
{
   return m_nSize;
}

/* 
***********************************
***********************************
*/


void COleProp::SetDataPointer(CDataStorage* pdata)
{
   m_pData = pdata;
}

/* 
***********************************
***********************************
*/

CDataStorage* COleProp::GetDataPointer(void) const
{
   return m_pData;
}

/* 
***********************************
***********************************
*/
void COleProp::SetCreatedSecs(signed32_t secs1)
{
   m_nTSCreatedSeconds = secs1;
}
/* 
***********************************
***********************************
*/
signed32_t COleProp::GetCreatedSecs(void) const
{
   return m_nTSCreatedSeconds;
}


/* 
***********************************
***********************************
*/
void COleProp::SetCreatedDays(signed32_t days1)
{
   m_nTSCreatedDays = days1;
}

/* 
***********************************
***********************************
*/
signed32_t COleProp::GetCreatedDays(void) const
{
   return m_nTSCreatedDays;
}


/* 
***********************************
***********************************
*/
void COleProp::SetModifiedSecs(signed32_t secs2)
{
   m_nTSModifiedSeconds = secs2;
}

/* 
***********************************
***********************************
*/
signed32_t COleProp::GetModifiedSecs(void) const
{
   return m_nTSModifiedSeconds;
}

/* 
***********************************
***********************************
*/

void COleProp::SetModifiedDays(signed32_t days2)
{
   m_nTSModifiedDays = days2;
}

/* 
***********************************
***********************************
*/
signed32_t COleProp::GetModifiedDays(void) const
{
   return  m_nTSModifiedDays;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: oleprop.cpp,v $
 * Revision 1.6  2009/03/02 04:08:43  dhoerl
 * Code is now compliant to gcc  -Weffc++
 *
 * Revision 1.5  2009/01/23 16:09:55  dhoerl
 * General cleanup: headers and includes. Fixed issues building mainC and mainCPP
 *
 * Revision 1.4  2009/01/08 02:53:29  dhoerl
 * December Rework
 *
 * Revision 1.3  2008/10/25 18:39:53  dhoerl
 * 2008
 *
 * Revision 1.2  2004/09/01 00:47:04  darioglz
 * + Modified to gain independence of target
 *
 * Revision 1.1.1.1  2004/08/27 16:31:43  darioglz
 * Initial Import.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

