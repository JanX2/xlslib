/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * This file is part of xlslib -- A multiplatform, C/C++ library
 * for dynamic generation of Excel(TM) files.
 *
 * Copyright 2004 Yeico S. A. de C. V. All Rights Reserved.
 * Copyright 2008 David Hoerl All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are
 * permitted provided that the following conditions are met:
 * 
 *    1. Redistributions of source code must retain the above copyright notice, this list of
 *       conditions and the following disclaimer.
 * 
 *    2. Redistributions in binary form must reproduce the above copyright notice, this list
 *       of conditions and the following disclaimer in the documentation and/or other materials
 *       provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY David Hoerl ''AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL David Hoerl OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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

