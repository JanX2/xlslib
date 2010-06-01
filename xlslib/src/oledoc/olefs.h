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
 * $Source: /cvsroot/xlslib/xlslib/src/oledoc/olefs.h,v $
 * $Revision: 1.4 $
 * $Author: dhoerl $
 * $Date: 2009/03/02 04:08:43 $
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * File description:
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef OLEFS_H
#define OLEFS_H
 
#include <xlsys.h>

#include <oleprop.h>
#include <datast.h>

namespace xlslib_core
{

#define FS_NO_ERRORS            (0)
#define FS_INVALID_PATH         (-1)
#define FS_NODE_ALREADY_EXISTS  (-2)
#define FS_NODE_NOT_A_DIRECTORY (-3)

/* 
***********************************
COleFileSystem class declaration
***********************************
*/
  class COleFileSystem
    {
    private:

    public:
      COleFileSystem();
      virtual ~COleFileSystem();

      COleProp& GetRootEntry();
      size_t GetTotalDataSize();
      unsigned32_t GetNumDataFiles();

    public:
      int GetNode(std::string const &path, Tree_Level_Itor_t& node);

      int AddDirectory(std::string const &dir_path);
      int AddFile(std::string const &dir_path, CDataStorage* pdata);

      int AddNode(COleProp* base_node, StringList_t& path_list);
      int SearchNode(COleProp* base_node, StringList_t& path_list, Tree_Level_Itor_t& node_level);

      void GetAllNodesList(NodeList_t& node_list, COleProp* base_node);
      void GetAllNodes(NodeList_t& node_list);
      void SortList(NodeList_t& node_list);

    public:
      COleProp m_RootEntry;  // temporally public (for debugging)
      signed32_t m_nProperty_Count;
    };

}
#endif //OLEFS_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: olefs.h,v $
 * Revision 1.4  2009/03/02 04:08:43  dhoerl
 * Code is now compliant to gcc  -Weffc++
 *
 * Revision 1.3  2009/01/23 16:09:55  dhoerl
 * General cleanup: headers and includes. Fixed issues building mainC and mainCPP
 *
 * Revision 1.2  2008/10/25 18:39:53  dhoerl
 * 2008
 *
 * Revision 1.1.1.1  2004/08/27 16:31:43  darioglz
 * Initial Import.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

