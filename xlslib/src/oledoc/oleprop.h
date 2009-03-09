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
 * $Source: /cvsroot/xlslib/xlslib/src/oledoc/oleprop.h,v $
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

#ifndef OLEPROP_H
#define OLEPROP_H

#include <config.h>

#include <list>
#include <string>
#include <datast.h>

 
namespace xlslib_core
{

  class COleProp;

  typedef std::vector<xlslib_core::COleProp* XLSLIB_DFLT_ALLOCATOR> Tree_Level_Vect_t;
  typedef Tree_Level_Vect_t::iterator Tree_Level_Itor_t;

  typedef std::vector<xlslib_core::COleProp* XLSLIB_DFLT_ALLOCATOR> NodeList_t;
  typedef NodeList_t::iterator NodeList_Itor_t;

  /* 
******************************
COleProp class declaration
******************************
*/
	class oleSort;

  class COleProp
    {
    protected:
      std::string m_sName;
      unsigned16_t m_nNameSize;
      unsigned8_t m_nPropType;
      unsigned8_t m_nNodeColor;

      signed32_t m_nPrevPropIndex; 
      signed32_t m_nNextPropIndex; 
      signed32_t m_nChildPropIndex;

      signed32_t m_nTSCreatedSeconds; 
      signed32_t m_nTSCreatedDays;    
      signed32_t m_nTSModifiedSeconds;
      signed32_t m_nTSModifiedDays;   

      signed32_t m_nStartBlock;
      signed32_t m_nSize;
  
      // The following set of attributes are not part of the definition of 
      // an OleDoc's property:

      signed32_t m_nIndex;
      CDataStorage* m_pData;

    private:
      COleProp(const COleProp& that);
      COleProp& operator=(const COleProp& right);

    public:
      Tree_Level_Vect_t m_Child_List;

      COleProp();
      COleProp(signed32_t node_index, std::string& name, CDataStorage* data = NULL);
      COleProp(signed32_t node_index, const char *name, CDataStorage* data = NULL);
      ~COleProp();

      int Init(signed32_t node_index, std::string& name, CDataStorage* data = NULL);

      int SetName(std::string name);
      std::string GetName(void);

      int SetIndex(signed32_t newindex);
      inline signed32_t GetIndex(void) const {return m_nIndex;}

      int SetSize(signed32_t size);
      signed32_t GetSize(void);

      int SetType(unsigned8_t newtype);
      unsigned8_t GetType(void);

      int SetColor(unsigned8_t newcolor);
      unsigned8_t GetColor(void);

      int SetPreviousIndex(signed32_t prev);
      signed32_t GetPreviousIndex(void);

      int SetNextIndex(signed32_t next);
      signed32_t GetNextIndex(void);

      int SetChildIndex(signed32_t child);
      signed32_t GetChildIndex(void);

      int SetStartBlock(signed32_t sb);
      signed32_t GetStartBlock(void);

      void SetDataPointer(CDataStorage* pdata);
      CDataStorage* GetDataPointer(void);

      void SetCreatedSecs(signed32_t sec1);
      signed32_t GetCreatedSecs(void);
      void SetCreatedDays(signed32_t day1);
      signed32_t GetCreatedDays(void);

      void SetModifiedSecs(signed32_t sec2);
      signed32_t GetModifiedSecs(void);
      void SetModifiedDays(signed32_t day2);
      signed32_t GetModifiedDays(void);
    };
}


#endif //OLEPROP_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: oleprop.h,v $
 * Revision 1.5  2009/03/02 04:08:43  dhoerl
 * Code is now compliant to gcc  -Weffc++
 *
 * Revision 1.4  2009/01/23 16:09:55  dhoerl
 * General cleanup: headers and includes. Fixed issues building mainC and mainCPP
 *
 * Revision 1.3  2009/01/08 02:53:29  dhoerl
 * December Rework
 *
 * Revision 1.2  2008/10/25 18:39:53  dhoerl
 * 2008
 *
 * Revision 1.1.1.1  2004/08/27 16:31:43  darioglz
 * Initial Import.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

