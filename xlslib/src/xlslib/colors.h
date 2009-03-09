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
 * $Source: /cvsroot/xlslib/xlslib/src/xlslib/colors.h,v $
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

#ifndef COLORS_H
#define COLORS_H

#include <config.h>
#include <common.h>

#include <record.h>
#include <unit.h>

namespace xlslib_core
{
// Colors can be "base" (< 8) or palette (changes with BIFF)
#define COLOR_CODE_BLACK				(0x08)	// If you use 0x00, Excel won't open the Cell Format dialog box!
#define COLOR_CODE_WHITE				(0x09)	// If you use 0x01, Excel won't open the Cell Format dialog box!

#define COLOR_CODE_RED					(0x0a)
#define COLOR_CODE_BRIGHT_GREEN			(0x0b)
#define COLOR_CODE_BLUE					(0x0c)
#define COLOR_CODE_YELLOW				(0x0d)
#define COLOR_CODE_PINK					(0x0e)
#define COLOR_CODE_TURQUOISE			(0x0f)

#define COLOR_CODE_DARK_RED				(0x10)
#define COLOR_CODE_GREEN				(0x11)
#define COLOR_CODE_DARK_BLUE			(0x12)
#define COLOR_CODE_DARK_YELLOW			(0x13)
#define COLOR_CODE_VIOLET				(0x14)
#define COLOR_CODE_TEAL					(0x15)
#define COLOR_CODE_GRAY25				(0x16)
#define COLOR_CODE_GRAY50				(0x17)

// In Excel2004 on Mac, these represent the lower 16 colors, ordered left to right, 
// starting at the top row and moving down
#define COLOR_CODE_PERIWINKLE			(0x18)
#define COLOR_CODE_DARK_BLUE2			(0x19)
#define COLOR_CODE_PLUM2				(0x1a)
#define COLOR_CODE_PINK2				(0x1b)
#define COLOR_CODE_IVORY				(0x1c)
#define COLOR_CODE_YELLOW2				(0x1d)
#define COLOR_CODE_LIGHT_TURQUOISE2		(0x1e)
#define COLOR_CODE_TURQUOISE2			(0x1f)
// ---
#define COLOR_CODE_DARK_PURPLE			(0x20)
#define COLOR_CODE_VIOLET2				(0x21)
#define COLOR_CODE_CORAL				(0x22)
#define COLOR_CODE_DARK_RED2			(0x23)
#define COLOR_CODE_OCEAN_BLUE			(0x24)
#define COLOR_CODE_TEAL2				(0x25)
#define COLOR_CODE_ICE_BLUE				(0x26)
#define COLOR_CODE_BLUE2				(0x27)

#define COLOR_CODE_SKY_BLUE				(0x28)
#define COLOR_CODE_LIGHT_TURQUOISE		(0x29)
#define COLOR_CODE_LIGHT_GREEN			(0x2a)
#define COLOR_CODE_LIGHT_YELLOW			(0x2b)
#define COLOR_CODE_PALEBLUE				(0x2c)
#define COLOR_CODE_ROSE					(0x2d)
#define COLOR_CODE_LAVENDER				(0x2e)
#define COLOR_CODE_TAN					(0x2f)

#define COLOR_CODE_LIGHT_BLUE			(0x30)
#define COLOR_CODE_AQUA					(0x31)
#define COLOR_CODE_LIME					(0x32)
#define COLOR_CODE_GOLD					(0x33)
#define COLOR_CODE_LIGHT_ORANGE         (0x34)
#define COLOR_CODE_ORANGE				(0x35)
#define COLOR_CODE_BLUE_GRAY			(0x36)
#define COLOR_CODE_GRAY40				(0x37)
#define COLOR_CODE_DARK_TEAL			(0x38)
#define COLOR_CODE_SEA_GREEN			(0x39)
#define COLOR_CODE_DARK_GREEN			(0x3a)
#define COLOR_CODE_OLIVE_GREEN			(0x3b)
#define COLOR_CODE_BROWN				(0x3c)
#define COLOR_CODE_PLUM					(0x3d)
#define COLOR_CODE_INDIGO				(0x3e)
#define COLOR_CODE_GRAY80				(0x3f)

#define COLOR_CODE_SYS_WIND_FG			(0x40)
#define COLOR_CODE_SYS_WIND_BG			(0x41)

// Good reference: http://www.mvps.org/dmcritchie/excel/colors.htm

typedef enum
{
 // Excel top 40 colors
 CLR_BLACK=1,    CLR_BROWN,       CLR_OLIVE_GREEN, CLR_DARK_GREEN,      CLR_DARK_TEAL,      CLR_DARK_BLUE,  CLR_INDIGO,     CLR_GRAY80,
 CLR_DARK_RED,   CLR_ORANGE,      CLR_DARK_YELLOW, CLR_GREEN,           CLR_TEAL,           CLR_BLUE,       CLR_BLUE_GRAY,  CLR_GRAY50,
 CLR_RED,        CLR_LITE_ORANGE, CLR_LIME,        CLR_SEA_GREEN,       CLR_AQUA,           CLR_LITE_BLUE,  CLR_VIOLET,     CLR_GRAY40,
 CLR_PINK,       CLR_GOLD,        CLR_YELLOW,      CLR_BRITE_GREEN,     CLR_TURQUOISE,      CLR_SKY_BLUE,   CLR_PLUM,       CLR_GRAY25,
 CLR_ROSE,       CLR_TAN,         CLR_LITE_YELLOW, CLR_LITE_GREEN,      CLR_LITE_TURQUOISE, CLR_PALE_BLUE,  CLR_LAVENDER,   CLR_WHITE,

 // Bottom 16 colors
 CLR_PERIWINKLE, CLR_PLUM2,       CLR_IVORY,       CLR_LITE_TURQUOISE2, CLR_DARK_PURPLE,     CLR_CORAL,     CLR_OCEAN_BLUE, CLR_ICE_BLUE,  
 CLR_DARK_BLUE2, CLR_PINK2,       CLR_YELLOW2,     CLR_TURQUOISE2,      CLR_VIOLET2,         CLR_DARK_RED2, CLR_TEAL2,      CLR_BLUE2,

 CLR_SYS_WIND_FG, CLR_SYS_WIND_BG
	      
} color_name_t;
#define ORIG_COLOR_BLACK	(color_name_t)0		// Well, to get the default fonts etc to use same value as Excel outputs

	typedef struct {
		unsigned8_t r, g, b, nuttin;
	} color_entry_t;

	class colors_t : public CRecord
	{
	  public:
		colors_t();
		~colors_t();

		bool setColor(unsigned8_t r, unsigned8_t g, unsigned8_t b, unsigned8_t idx); // 8 <= idx <= 64
		CUnit* colors_t::GetData() const;

	  private:
		colors_t(const colors_t &that);
		colors_t& operator=(const colors_t& right);

	  private:
		color_entry_t	*colors;
	};

/*
******************************
CPalette class declaration
******************************
*/
  class CPalette: public CRecord
    {
    protected:

    public:
      CPalette(const color_entry_t *colors);
      ~CPalette();
    };

}
#endif //COLORS_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: colors.h,v $
 * Revision 1.6  2009/03/02 04:08:43  dhoerl
 * Code is now compliant to gcc  -Weffc++
 *
 * Revision 1.5  2009/01/23 16:09:55  dhoerl
 * General cleanup: headers and includes. Fixed issues building mainC and mainCPP
 *
 * Revision 1.4  2009/01/08 02:53:15  dhoerl
 * December Rework
 *
 * Revision 1.3  2008/12/20 15:49:05  dhoerl
 * 1.2.5 fixes
 *
 * Revision 1.2  2008/10/25 18:39:54  dhoerl
 * 2008
 *
 * Revision 1.1.1.1  2004/08/27 16:31:44  darioglz
 * Initial Import.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

