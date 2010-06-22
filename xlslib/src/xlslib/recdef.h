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
 * $Source: /cvsroot/xlslib/xlslib/src/xlslib/recdef.h,v $
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


#ifndef RECDEF_H
#define RECDEF_H

//#include <xlsys.h>
#include <common.h>
#include <rectypes.h>
#include <record.h>

#include <xls_pshpack2.h>

namespace xlslib_core
{

/*
******************************
CBof class declaration
******************************
*/
#define  VERSION_BIFF				0x0600	// BIFF8, the last one!

#define BOF_TYPE_WBGLOBALS           0x0005
#define BOF_TYPE_VBMODULE            0x0006
#define BOF_TYPE_WORKSHEET           0x0010
#define BOF_TYPE_CHART               0x0020
#define BOF_TYPE_EXCEL4_MACROSH      0x0040
#define BOF_TYPE_WSFILE              0x0100

#define BOF_BUILD_DFLT               0x1d5f	// from some old file???
#define BOF_YEAR_DFLT                0x07cd	// 1997

#define TWIP						 20


#if 0
#define CODEPAGE_IBMPC               0x01b5
#define CODEPAGE_APPLE               0x8000
#define CODEPAGE_ANSI                0x04e4
#endif


#define BOF_RECORD_SIZE  12								// used when computing blocks during output stage

	// forward ref
	class CDataStorage;

  class CBof: public CRecord
    {
#if defined(LEIGHTWEIGHT_UNIT_FEATURE)
	friend class CDataStorage;
#endif

    protected:
      CBof(CDataStorage &datastore, unsigned16_t boftype);
	private:
      virtual ~CBof();
    };

/*
******************************
CEof class declaration
******************************
*/
  class CEof: public CRecord
    {
#if defined(LEIGHTWEIGHT_UNIT_FEATURE)
	friend class CDataStorage;
#endif

    protected:
      CEof(CDataStorage &datastore);
	private:
      virtual ~CEof();
    };

/*
******************************
CCodePage class declaration
******************************
*/
  class CCodePage: public CRecord
    {
#if defined(LEIGHTWEIGHT_UNIT_FEATURE)
	friend class CDataStorage;
#endif

    protected:
      CCodePage(CDataStorage &datastore, unsigned16_t boftype);
	private:
      virtual ~CCodePage();
    };

/*
******************************
CWindow1 class declaration
******************************
*/
  class CWindow1;

  class window1
    {
		friend class CWindow1;

    private:
		unsigned16_t	horzPos, vertPos;	// points
		unsigned16_t	windWidth, windHeight;		// points
		unsigned16_t	activeSheet;		// 0 offset
		unsigned16_t	firstVisibleTab;	// 0 offset
		unsigned16_t	tabBarWidth;		// 0 - 1000, from no tab bar up to no scroll bar

    public:
		window1();
		virtual ~window1();

		// access from workBook
		void SetPosition(unsigned16_t horz, unsigned16_t vert) { horzPos=horz; vertPos=vert;}
		void SetSize(unsigned16_t width, unsigned16_t height) { windWidth=width; windHeight=height;}
		void SetFirstTab(unsigned16_t firstTab) { firstVisibleTab=firstTab;}
		void SetTabBarWidth(unsigned16_t width) { tabBarWidth = (width > 1000) ? 1000 : width;}

		// access from workSheet
		void SetActiveSheet(unsigned16_t theSheet) { activeSheet=theSheet;}
		unsigned16_t GetActiveSheet() const { return activeSheet;}
    };

  class CWindow1 : public CRecord
    {
#if defined(LEIGHTWEIGHT_UNIT_FEATURE)
	friend class CDataStorage;
#endif

    protected:
		CWindow1(CDataStorage &datastore, const window1& wind1);
	private:
		virtual ~CWindow1();
    };
/*
******************************
CDateMode class declaration
******************************
*/
  class CDateMode: public CRecord
    {
#if defined(LEIGHTWEIGHT_UNIT_FEATURE)
	friend class CDataStorage;
#endif

    protected:
      CDateMode(CDataStorage &datastore);
	private:
      virtual ~CDateMode();

	public:
	  static bool Is_In_1904_Mode(void);
    };



/*
******************************
CWindow2 class declaration
******************************
*/

#define W2_OFFSET_GRBIT          4
#define W2_OFFSET_TOPROW         6
#define W2_OFFSET_LEFTCOL        8
#define W2_OFFSET_COLOR          10
#define W2_OFFSET_ZOOMPREVIEW    14
#define W2_OFFSET_ZOOMNORMAL     16
#define W2_OFFSET_RESERVED       18


#define W2_DFLT_TOPROW     0x0000
#define W2_DFLT_LEFTCOL    0x0000
#define W2_DFLT_COLOR      0x00000000
  // NOTE: Check a BIFF8 example to verify the units of the two following values
#define W2_DFLT_ZOOMPBPREV 0x0100
#define W2_DFLT_ZOOMNORMAL 0x0100
#define W2_DFLT_RESERVED   0x00000000

  // GRBIT mask-flags:
#define W2_GRBITMASK_FMLA          0x0001
#define W2_GRBITMASK_GRIDS         0x0002
#define W2_GRBITMASK_HROWCOL       0x0004
#define W2_GRBITMASK_FROZEN        0x0008
#define W2_GRBITMASK_ZEROS         0x0010
#define W2_GRBITMASK_DFLTHDRCOLOR  0x0020
#define W2_GRBITMASK_ARABIC        0x0040
#define W2_GRBITMASK_GUTS          0x0080
#define W2_GRBITMASK_FRZNOSPLIT    0x0100
#define W2_GRBITMASK_SELECTED      0x0200
#define W2_GRBITMASK_ACTIVE        0x0400
#define W2_GRBITMASK_PAGEBRK       0x0800
#define W2_GRBITMASK_RESERVED      0xf000

  /*
	NOTE: Hardcoded from an excel example
	#define W2_DFLT_GRBIT ((unsigned16_t)0x06b6)
    #define W2_DFLT_GRBIT ((unsigned16_t) \
    (W2_GRBITMASK_GRIDS|W2_GRBITMASK_HROWCOL|W2_GRBITMASK_DFLTHDRCOLOR)) 
  */
  class CWindow2: public CRecord
    {
#if defined(LEIGHTWEIGHT_UNIT_FEATURE)
	friend class CDataStorage;
#endif

    protected:
      // TODO: Create a constructor that gets user-defined arguments that specify the appearence
      // The following constructor establishes default values.
      CWindow2(CDataStorage &datastore, bool isActive);
	private:
      virtual ~CWindow2();

	public:
      void SetSelected();
      void SetPaged();
      void ClearSelected();
      void ClearPaged();
    };

/*
******************************
CDimension class declaration
******************************
*/
  class CDimension: public CRecord
    {
#if defined(LEIGHTWEIGHT_UNIT_FEATURE)
	friend class CDataStorage;
#endif

    protected:
      CDimension(CDataStorage &datastore, 
		  unsigned32_t minRow, 
		  unsigned32_t maxRow, 
		  unsigned32_t minCol, 
		  unsigned32_t maxCol);
	private:
      virtual ~CDimension();
    };

/*
******************************
CStyle class declaration
******************************
*/

  struct style_t
  {
    unsigned16_t xfindex;
    unsigned8_t builtintype;
    unsigned8_t level;
  };

  typedef std::vector<xlslib_core::style_t* XLSLIB_DFLT_ALLOCATOR> Style_Vect_t;
  typedef Style_Vect_t::iterator Style_Vect_Itor_t;

  class CStyle: public CRecord
    {
#if defined(LEIGHTWEIGHT_UNIT_FEATURE)
	friend class CDataStorage;
#endif

    protected:
		CStyle(CDataStorage &datastore, const style_t* styledef);
	private:
      virtual ~CStyle();
    };


/*
******************************
CBSheet class declaration
******************************
*/

#define BSHEET_OFFSET_POSITION    4
#define BSHEET_OFFSET_FLAGS       8
#define BSHEET_OFFSET_NAMELENGHT  10
#define BSHEET_OFFSET_B7NAME      11
#define BSHEET_OFFSET_B8NAME      12

#define BSHEET_ATTR_WORKSHEET  0x0000
#define BSHEET_ATTR_EX4MACRO   0x0001
#define BSHEET_ATTR_CHART      0x0002
#define BSHEET_ATTR_VBMODULE   0x0006

#define BSHEET_ATTR_VISIBLE     0x0000
#define BSHEET_ATTR_HIDDEN      0x0100
#define BSHEET_ATTR_VERYHIDDEN  0x0200

  class CBSheet;
  class CGlobalRecords;

  class boundsheet_t
  {
  public:
	boundsheet_t(CGlobalRecords& gRecords);
	boundsheet_t(CGlobalRecords& gRecords, const u16string& sheetname, unsigned16_t attributes, unsigned32_t streampos);
	virtual ~boundsheet_t();

  private:
	boundsheet_t(const boundsheet_t& that);
	boundsheet_t& operator=(const boundsheet_t& right);

  protected:
    unsigned32_t streampos;
    u16string sheetname;
    bool worksheet:1;      
    bool ex4macro:1;
    bool chart:1;
    bool vbmodule:1;
    bool visible:1;      
    bool hidden:1;
    bool veryhidden:1;
	
	CBSheet	*sheetData;
	
	CGlobalRecords& m_GlobalRecords; 

  public:
	  unsigned32_t GetStreamPos(void) const { return streampos; };
	  const u16string& GetSheetName(void) const { return sheetname; };

	  void SetAttributes(unsigned16_t attributes);
	  bool IsWorkSheet(void) const { return worksheet; };      
	  bool IsEx4macro(void) const { return ex4macro; };
	  bool IsChart(void) const { return chart; };
	  bool IsVBModule(void) const { return vbmodule; };
	  bool IsVisible(void) const { return visible; };      
	  bool IsHidden(void) const { return hidden; };
	  bool IsVeryHidden(void) const { return veryhidden; };

	  CBSheet *SetSheetData(CBSheet *sh) { sheetData = sh; return sh; };
	  void SetSheetStreamPosition(size_t offset);
	  const CBSheet *GetSheetData(void) const { return sheetData; };

	  CGlobalRecords& GetGlobalRecords(void) const { return m_GlobalRecords; }; 
  };

  typedef std::vector<xlslib_core::boundsheet_t* XLSLIB_DFLT_ALLOCATOR> Boundsheet_Vect_t;
  typedef Boundsheet_Vect_t::iterator Boundsheet_Vect_Itor_t;

  class CBSheet: public CRecord
    {
#if defined(LEIGHTWEIGHT_UNIT_FEATURE)
	friend class CDataStorage;
#endif

    protected:
      CBSheet(CDataStorage &datastore, const boundsheet_t* bsheetdef);
	private:
      virtual ~CBSheet();

	public:
      void SetStreamPosition(size_t pos);
    };
}

#include <xls_poppack.h>

#endif //RECDEF_H


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: recdef.h,v $
 * Revision 1.6  2009/03/02 04:08:43  dhoerl
 * Code is now compliant to gcc  -Weffc++
 *
 * Revision 1.5  2009/01/08 22:16:06  dhoerl
 * January Rework
 *
 * Revision 1.4  2009/01/08 02:52:47  dhoerl
 * December Rework
 *
 * Revision 1.3  2008/12/20 15:49:05  dhoerl
 * 1.2.5 fixes
 *
 * Revision 1.2  2008/10/25 18:39:54  dhoerl
 * 2008
 *
 * Revision 1.1.1.1  2004/08/27 16:31:57  darioglz
 * Initial Import.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

