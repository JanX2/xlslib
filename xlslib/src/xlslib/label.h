/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * This file is part of xlslib -- A multiplatform, C/C++ library
 * for dynamic generation of Excel(TM) files.
 *
 * Copyright 2004 Yeico S. A. de C. V. All Rights Reserved.
 * Copyright 2008-2011 David Hoerl All Rights Reserved.
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
 *
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


#ifndef LABEL_H
#define LABEL_H

#include "common/xlsys.h"
#include "common/systype.h"

#include "xlslib/common.h"
#include "xlslib/cell.h"
#include "xlslib/record.h"
#include "xlslib/unit.h"
#include "xlslib/extformat.h"


// #include "common/xls_pshpack2.h"

namespace xlslib_core
{

#define LABEL_DFLT_XFINDEX              0x000f
#define LABEL_OFFSET_FIRSTCOL           6
#define LABEL_OFFSET_LASTCOL            8

	// forward ref
	class CDataStorage;

  class label_t: public cell_t
    {
	  friend class worksheet;

    private:
	  label_t(CGlobalRecords& gRecords, unsigned32_t rowval, unsigned32_t colval, const u16string& labelstrval, xf_t* pxfval = NULL);
	  label_t(CGlobalRecords& gRecords, unsigned32_t rowval, unsigned32_t colval, const std::string& labelstrval, xf_t* pxfval = NULL);
#ifndef __FRAMEWORK__
	  label_t(CGlobalRecords& gRecords, unsigned32_t rowval, unsigned32_t colval, const std::ustring& labelstrval, xf_t* pxfval = NULL);
#endif
      virtual ~label_t();

    private:
	  u16string	strLabel;
	  bool inSST;
	  void setType();

    public:
      const u16string& GetStrLabel() const { return strLabel; };

      virtual size_t GetSize(void) const;
      virtual CUnit* GetData(CDataStorage &datastore) const;
      bool GetInSST(void) const { return inSST; }
    };

  class CLabel: public CRecord
    {
#if defined(LEIGHTWEIGHT_UNIT_FEATURE)
	friend class CDataStorage;
#endif

    protected:
		CLabel(CDataStorage &datastore, const label_t& labeldef);
	private:
      virtual ~CLabel();
    };
}

  typedef std::vector<const xlslib_core::label_t* XLSLIB_DFLT_ALLOCATOR> Label_Vect_t;
  typedef Label_Vect_t::iterator Label_Vect_Itor_t;
  typedef Label_Vect_t::const_iterator cLabel_Vect_Itor_t;

// #include "common/xls_poppack.h"

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

