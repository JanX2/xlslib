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
 * $Source: /cvsroot/xlslib/xlslib/src/xlslib/number.h,v $
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

#ifndef NUMBER_H
#define NUMBER_H


#include <config.h>
#include <common.h>
#include <cell.h>
#include <record.h>
#include <unit.h>


namespace xlslib_core
{

  class number_t: public cell_t
    {
	  friend class worksheet;
	  friend class CNumber; // ::CNumber(number_t& blankdef);

    private:
      number_t(CGlobalRecords& gRecords, unsigned16_t rowval, unsigned16_t colval, 
               double numval, xf_t* pxfval = NULL);
      // 536870911 >= numval >= -536870912
      number_t(CGlobalRecords& gRecords, unsigned16_t rowval, unsigned16_t colval, 
               signed32_t numval, xf_t* pxfval = NULL);
      ~number_t(){};
	  
      virtual unsigned16_t GetSize() const {return isDouble ? 18 : 14;};
      virtual CUnit* GetData() const;

    private:
      bool isDouble;
      union {
		double			dblNum;
		signed32_t		intNum;
      } num;

    public:
      double GetNumber(double unused) const {return isDouble ? num.dblNum : (double)num.intNum;};
      signed32_t GetNumber(signed32_t unused) const {return isDouble ? (signed32_t)num.dblNum : num.intNum;};
    };

  class number_t;
  class CNumber: public CRecord
    {
    private:

    public:
#if 0
      CNumber(unsigned16_t row,
              unsigned16_t col,
              double num,
              const xf_t* pxfval = NULL);
#endif
      CNumber(const number_t& blankdef);
      ~CNumber();
    };


}

#endif //NUMBER_H


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: number.h,v $
 * Revision 1.6  2009/03/02 04:08:43  dhoerl
 * Code is now compliant to gcc  -Weffc++
 *
 * Revision 1.5  2009/01/23 16:09:55  dhoerl
 * General cleanup: headers and includes. Fixed issues building mainC and mainCPP
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
 * Revision 1.1.1.1  2004/08/27 16:31:49  darioglz
 * Initial Import.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

