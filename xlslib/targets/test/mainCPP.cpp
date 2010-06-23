/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 2004-2006 Yeico S. A. de C. V.
 * Copyright (C) 2008 David Hoerl
 *
 * $Source: /cvsroot/xlslib/xlslib/targets/test/mainCPP.cpp,v $
 * $Revision: 1.5 $
 * $Author: dhoerl $
 * $Date: 2009/03/08 21:51:40 $
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * File description:
 *
 *
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <xlslib.h>

#include <timespan.h>

#include "md5.h"

#include <string>
#include <sstream>
#include <iostream>
#include <stdio.h>
#include <limits.h>

using namespace std;
using namespace xlslib_core;

// set to 'true' if you want cell format print-out while generating the XLS
static const bool PRINT_CELL_FORMAT	= 0;


#define __REPORT_TIME__

// local defines for the Random test
#define OPT_HALIGN        0
#define OPT_VALIGN        1
#define OPT_ORIENTATION   2
#define OPT_FILLFGCOLOR   3
#define OPT_FILLBGCOLOR   4
#define OPT_FILLSTYLE     5
#define OPT_LOCKED        6
#define OPT_HIDDEN        7
#define OPT_WRAP          8
#define OPT_BORDERSTYLE   9
#define OPT_MAX           9

#define OPT_FONTHEIGHT      0 
#define OPT_FONTBOLD        1 
#define OPT_FONTUNDERLINE   2 
#define OPT_FONTSCRIPT      3 
#define OPT_FONTCOLOR       4 
#define OPT_FONTITALIC      5 
#define OPT_FONTSTRIKEOUT   6 
#define OPT_FONTOUTLINE     7 
#define OPT_FONTSHADOW      8 
#define OPT_FONTMAX         8

// Macros
#define TIMESPAN_START(id)                      \
   CCpuClockTimespan span_##id;                 \
   span_##id.StartClock()

#define TIMESPAN_END(id,str)                                      \
   span_##id.StopClock();                                         \
   std::cerr<<"    # "<<str<<" "<<span_##id.GetUsedMilliseconds()<<" ms"<<std::endl

int StressTest(int a,int b,int c, const char *md5_checksum);
int RandomTest(int a,int b,int c, int random_seed, const char *md5_checksum);

int RandomCellAndFormatTest(int sheets_sz, int rows_sz, int cols_sz, int random_seed, const char *md5_checksum);
static void RandomFormat(cell_t* cell, bool profile = false);
static void RandomFontName(cell_t* cell, bool profile = false);
static void RandomFontOption(cell_t* cell, bool profile = false);

int RandomFormatTest(int sheets_sz, int rows_sz, int cols_sz, int random_seed, const char *md5_checksum);

int RandomCellAndFormatTestProf(int sheets_sz, int rows_sz, int cols_sz, int random_seed, const char *md5_checksum);

int StandardTest(const char *md5_checksum);
int StandardTest2(const char *md5_checksum);
int BlankTest(const char *md5_checksum);

static void SeedRndNumber(int seed);
static int GetRndNumber(int max);

/*
*********************************
********************************* 
*/



int main(int argc, char *argv[])
{
	int rv = 0;

	// comment and uncomment the below to try various tests
#if 01
	rv |= StandardTest("a2fa7ad755b8bf18fd84117d1e0170db");
#endif
	rv |= StandardTest2("10cf918df1ff423f6d182c791b1f0d85");
#if 01
	rv |= BlankTest("e80a45e13f92863f0d2dffaca7c3834c");

	rv |= StressTest(3,100,100, "8895da93f04a8334f60348233b451205");
	rv |= StressTest(3,4,4, "be6c2c2ac695b7dd990479bd5368cab0");
	rv |= RandomTest(3,200,200, 42424242, "4887e07fd832f92416787c7a473dfe24");
	rv |= RandomCellAndFormatTest(1,15,10, 123456789, "3d2cbe79e866bfb0d63198c589333378");
	rv |= RandomCellAndFormatTestProf(1,15,10, 987654321, "c207fbe4f83efe206bfc35c402661872");
	rv |= RandomFormatTest(1,15,10, 42004200, "41f7c54d1bf00951dad103d568969667");
#endif

	std::cerr << "    # Test finished" << std::endl;

	return (rv == 0 ? EXIT_SUCCESS : EXIT_FAILURE);
}

int StandardTest(const char *md5_checksum)
{
   // Get the workbook handler
   workbook wb;
   worksheet* sh = wb.sheet("sheet");
   worksheet* sh2 = wb.sheet("sheet2");

	// Title
   sh->label(1,1,"test1");
   sh->rowheight(1,100);
   sh->rowheight(2,150);

	// Title
   sh2->label(1,1,"test3");
   sh2->rowheight(1,100);
   sh2->rowheight(2,150);
   sh2->label(2,1,"test4");
   sh2->number(3,1,1.0);
   sh2->number(4,1,2.0);
   sh2->number(5,1,3.0);

   sh->label(2,1,"test2");
   sh->number(3,1,1.0);
   sh->number(4,1,2.0);
   sh->number(5,1,3.0);

#if 0
   printf("    # limit: "); 
   char buf[100];
   gets(buf);
   int lim = atoi(buf);
#else
   int lim = 42;
#endif
   printf("\n    # limit: %d\n", lim); // >= 14 --> error to load sheet #2; <= 13 = ok

   int j, k;
	for (j = 0 ; j <= 127; j++)
	{
		for (k = 0; k <= 128; k++)
		{
			//sh->number(j, k, (0.0 + j + k ) / 1000);
			if (k == 128 && j > lim)
				;
			else
				sh->blank(j, k);
		}
	}

   // WARNING: column and row numbers are zero based in xlslib, but Excel starts numbering the buggers at '1' instead!
   sh->number(130,128,1.0);
   sh->number(131,172,2.0);
   sh->number(132,128,3.0);
   std::string s("This tab should should show three numbers: two in row 131/133 + one in row 132 at columns 129(DY)/173(FQ) respectively");
   sh->label(1,1,s);

   int err = wb.Dump("./testCPP.xls");

   if (err != NO_ERRORS)
   {
	   cerr << "StandardTest failed: I/O failure: " << err << std::endl;
	   return -1;
   }
   if (0 != check_file("./testCPP.xls", md5_checksum))
   {
	   cerr << "StandardTest failed: MD5 of generated XLS mismatch or I/O failure." << std::endl;
	   return -1;
   }
	return 0;
}



int BlankTest(const char *md5_checksum)
{
	workbook wb;
	wb.sheet("Sheet_01");
	int err = wb.Dump("blank.xls");

	if (err != NO_ERRORS)
	{
		cerr << "BlankTest failed: I/O failure: " << err << std::endl;
		return -1;
	}
	if (0 != check_file("blank.xls", md5_checksum))
	{
		cerr << "BlankTest failed: MD5 of generated XLS mismatch or I/O failure." << std::endl;
		return -1;
	}
	return 0;
}


static expression_node_t *build_formula(int row, int col, worksheet *sh3, worksheet *sh4, int val, workbook &wb)
{
	expression_node_factory_t& maker = wb.GetFormulaFactory();

	cell_t *cref = sh4->FindCellOrMakeBlank(row+1, col+1);
	assert(cref);
	expression_node_t *root = maker.op(OP_ADD, maker.integer(val), maker.cell(*cref, sh4, CELL_RELATIVE_A1));
	//expression_node_t *root = maker.integer(val);

	return root;
}

int StandardTest2(const char *md5_checksum)
{
	// Get the workbook handler
	workbook wb;
	worksheet* sh1 = wb.sheet("error");
	worksheet* sh2 = wb.sheet("boolean");
	worksheet* sh3 = wb.sheet("formula");
	worksheet* sh4 = wb.sheet("notes");

	// Title
	sh1->label(1,1,"test1");
	sh1->rowheight(1,100);
	sh1->rowheight(2,150);

	// Title
	sh2->label(1,1,"test2");
	sh2->rowheight(1,100);
	sh2->rowheight(2,150);
	
	sh3->label(1,1,"test3");
	sh3->rowheight(1,100);
	sh3->rowheight(2,150);

	sh4->label(2,1,"test4");
	sh4->rowheight(1,100);
	sh4->rowheight(2,150);

	// WARNING: column and row numbers are zero based in xlslib, but Excel starts numbering the buggers at '1' instead!

	int j, k;
	for (j = 0 ; j <= 127; j++)
	{
		for (k = 0; k <= 128; k++)
		{
			static const errcode_t errcvt[] =
			{
				XLERR_NULL, // #NULL!
				XLERR_DIV0, // #DIV/0!
				XLERR_VALUE, // #VALUE!
				XLERR_REF, // #REF!
				XLERR_NAME, // #NAME?
				XLERR_NUM, // #NUM!
				XLERR_N_A, // #N/A!
			};

			int v = (j + k) % (sizeof(errcvt)/sizeof(errcvt[0]));
			sh1->error(j + 4, k, errcvt[v]);

			bool v2 = (((j + k) / 10) % 7 > 3) ^ ((j + k) % 129 == 1);
			sh2->boolean(j + 4, k, v2);

			expression_node_t *ast = build_formula(j, k, sh3, sh4, v, wb);
			sh3->formula(j + 4, k, ast, true);

			char buf[256];
			sprintf(buf, "Remark item %d/%d/%d", j, k, v);

			sh4->number(j + 4, k, v);
			//sh4->note(j + 4, k, buf, "GHO");
		}
	}

	int err = wb.Dump("./testCPP2.xls");

	if (err != NO_ERRORS)
	{
		cerr << "StandardTest2 failed: I/O failure: " << err << std::endl;
		return -1;
	}
	if (0 != check_file("./testCPP2.xls", md5_checksum))
	{
		cerr << "StandardTest2 failed: MD5 of generated XLS mismatch or I/O failure." << std::endl;
		return -1;
	}
	return 0;
}

static unsigned32_t seed = 0;

static void SeedRndNumber(int sv)
{
	seed = sv;
}
static int GetRndNumber(int max)
{
	// this is NOT a good random generator but suffices for our purposes!
	seed *= 15482893;
	seed %= 792241;

	int rndnum;
	rndnum = (int)(seed * ((max + 1.0) / (792241 - 1.0)));
	return rndnum;
}

static errcode_t PickErrorCode(int value)
{
	static const errcode_t elist[] =
	{
		XLERR_NULL  , // #NULL!
		XLERR_DIV0  , // #DIV/0!
		XLERR_VALUE , // #VALUE!
		XLERR_REF   , // #REF!
		XLERR_NAME  , // #NAME?
		XLERR_NUM   , // #NUM!
		XLERR_N_A   , // #N/A!
	};
	const double divider = (sizeof(elist[0]) * (double)INT_MAX) / sizeof(elist);

	value = (int)(value / divider);
	XL_ASSERT(value >= 0);
	XL_ASSERT(value <= sizeof(elist)/sizeof(elist[0]));
	return elist[value];
}


int RandomCellAndFormatTest(int sheets_sz, int rows_sz, int cols_sz, int random_seed, const char *md5_checksum)
{
   workbook wb;
   worksheet* sh;

   SeedRndNumber(random_seed);

   TIMESPAN_START(1);

   for(int shnum = 0; shnum < sheets_sz; shnum++)
   {
	   char tmp[256];

	  sprintf(tmp, "DUH_%d", shnum);
	  string snamesheet(tmp);

      sh = wb.sheet(snamesheet);

      for(int row = 0; row<rows_sz; row++)
      {
	     // height unit = point! Internally Excel works with 'twip': 1/20th of a point, but the interface works in /points/.
         sh->rowheight(row,GetRndNumber(13)+20);
         for(int col = 0; col<cols_sz; col++)
         {
		    // width unit = 1/256th of the width of '0'
            sh->colwidth(row,GetRndNumber(2000)+4000);

            int rndcol = GetRndNumber(rows_sz);
            int rndrow = GetRndNumber(cols_sz);

			sprintf(tmp, "S%d:%d-%d#%d-%d", shnum, row+1, col+1, rndrow, rndcol);
			string snamelabel(tmp);

            cell_t* cell = sh->label(rndrow, rndcol, snamelabel);

            int k, fmtries = GetRndNumber(OPT_MAX);
            for(k = 0; k<fmtries; k++)
               RandomFormat(cell);

            fmtries = GetRndNumber(OPT_FONTMAX);
            for(k = 0; k<fmtries; k++)
               RandomFontOption(cell);

            RandomFontName(cell);
         }
      }
   }

   int err = wb.Dump("rndcellandformat.xls");
   TIMESPAN_END(1,"Random Cell and Format test:");

   if (err != NO_ERRORS)
   {
	   cerr << "RandomCellAndFormatTest failed: I/O failure: " << err << std::endl;
	   return -1;
   }
   if (0 != check_file("rndcellandformat.xls", md5_checksum))
   {
	   cerr << "RandomCellAndFormatTest failed: MD5 of generated XLS mismatch or I/O failure." << std::endl;
	   return -1;
   }
   return 0;
}

int RandomCellAndFormatTestProf(int sheets_sz, int rows_sz, int cols_sz, int random_seed, const char *md5_checksum)
{
   workbook wb;
   worksheet* sh;

   SeedRndNumber(random_seed);

   for(int shnum = 0; shnum < sheets_sz; shnum++)
   {
	   char tmp[256];

	  sprintf(tmp, "DUH_%d", shnum);
	  string snamesheet(tmp);

      sh = wb.sheet(snamesheet);

      for(int row = 0; row<rows_sz; row++)
      {
	     // height unit = point! Internally Excel works with 'twip': 1/20th of a point, but the interface works in /points/.
		  sh->rowheight(row,GetRndNumber(13)+20);
		  for(int col = 0; col<cols_sz; col++)
		  {
			  // width unit = 1/256th of the width of '0'
			  sh->colwidth(row,GetRndNumber(2000)+4000);

            int rndcol = GetRndNumber(rows_sz);
            int rndrow = GetRndNumber(cols_sz);

			sprintf(tmp, "S%d:%d-%d#%d-%d", shnum, row+1, col+1, rndrow, rndcol);
			string snamelabel(tmp);

            cell_t* cell = sh->label(rndrow, rndcol, snamelabel);

			if (PRINT_CELL_FORMAT)
			{
			cout<<"CELL ";
            cout.fill('0');
            cout.width(2);
            cout<<rndrow<<",";
            cout.fill('0');
            cout.width(2);
            cout<<rndcol<<": ";
			}

			int k, fmtries = GetRndNumber(OPT_MAX);

			if (PRINT_CELL_FORMAT)
			{
            cout<<endl<<"**FORMAT** ";
			}

			for(k = 0; k<fmtries; k++)
				RandomFormat(cell, PRINT_CELL_FORMAT);

			if (PRINT_CELL_FORMAT)
			{
            cout<<endl<<"**FONT**   ";
			}

			fmtries = GetRndNumber(OPT_FONTMAX);
            for(k = 0; k<fmtries; k++)
				RandomFontOption(cell, PRINT_CELL_FORMAT);

            RandomFontName(cell,PRINT_CELL_FORMAT);
			if (PRINT_CELL_FORMAT)
			{
            cout<<endl;
			}
		 }
      }
   }

   int err = wb.Dump("rndcellandformat_prof.xls");

   if (err != NO_ERRORS)
   {
	   cerr << "RandomCellAndFormatTestProf failed: I/O failure: " << err << std::endl;
	   return -1;
   }
   if (0 != check_file("rndcellandformat_prof.xls", md5_checksum))
   {
	   cerr << "RandomCellAndFormatTestProf failed: MD5 of generated XLS mismatch or I/O failure." << std::endl;
	   return -1;
   }
   return 0;
}


int RandomFormatTest(int sheets_sz, int rows_sz, int cols_sz, int random_seed, const char *md5_checksum)
{
   SeedRndNumber(random_seed);

   TIMESPAN_START(1);

      workbook wb;
   worksheet* sh;

   for(int shnum = 0; shnum < sheets_sz; shnum++)
   {
	   char tmp[256];

	  sprintf(tmp, "DUH_%d", shnum);
	  string snamesheet(tmp);

      sh = wb.sheet(snamesheet);

      for(int row = 0; row<rows_sz; row++)
      {
	     // height unit = point! Internally Excel works with 'twip': 1/20th of a point, but the interface works in /points/.
		  sh->rowheight(row,GetRndNumber(13)+20);
		  for(int col = 0; col<cols_sz; col++)
		  {
			  // width unit = 1/256th of the width of '0'
			  sh->colwidth(row,GetRndNumber(2000)+4000);

			sprintf(tmp, "S%d:%d-%d", shnum, row+1, col+1);
			string snamelabel(tmp);

            cell_t* cell = sh->label(row, col, snamelabel);

            int k, fmtries = GetRndNumber(OPT_MAX);
            for(k = 0; k<fmtries; k++)
               RandomFormat(cell);

            fmtries = GetRndNumber(OPT_FONTMAX);
            for(k = 0; k<fmtries; k++)
               RandomFontOption(cell);

            RandomFontName(cell);
         }
      }
   }

   int err = wb.Dump("rndformat.xls");
   TIMESPAN_END(1,"Random Format test:");

   if (err != NO_ERRORS)
   {
	   cerr << "RandomFormatTest failed: I/O failure: " << err << std::endl;
	   return -1;
   }
   if (0 != check_file("rndformat.xls", md5_checksum))
   {
	   cerr << "RandomFormatTest failed: MD5 of generated XLS mismatch or I/O failure." << std::endl;
	   return -1;
   }
   return 0;
}


#define OPT_FNTNAME_ARIAL             0
#define OPT_FNTNAME_TIMES             1
#define OPT_FNTNAME_TAHOMA            2
#define OPT_FNTNAME_ARIALBLACK        3
#define OPT_FNTNAME_CENTURYGOTHIC     4
#define OPT_FNTNAME_COMICSANSMS       5
#define OPT_FNTNAME_COURIER           6
#define OPT_FNTNAME_COURIERNEW        7
#define OPT_FNTNAME_FIXEDSYS          8
#define OPT_FNTNAME_GARAMOND          9
#define OPT_FNTNAME_IMPACT            10
#define OPT_FNTNAME_LUCIDACONSOLE     11
#define OPT_FNTNAME_MONOTYPECORSIVA   12
#define OPT_FNTNAME_SYMBOL            13
#define OPT_FNTNAME_WEBDINGS          14
#define OPT_FNTNAME_WINGDINGS         15
#define OPT_FNTNAME_MAX               15

static void RandomFontName(cell_t* cell, bool profile)
{
   switch(GetRndNumber(OPT_FNTNAME_MAX))
   {
      case OPT_FNTNAME_ARIAL:
         cell->fontname("Arial");
         if(profile) cout<<"Arial Font, ";
         break;

      case OPT_FNTNAME_TIMES:
         cell->fontname("Times New Roman");
         if(profile) cout<<"Times New Roman Font, ";
         break;

      case OPT_FNTNAME_TAHOMA:
         cell->fontname("Tahoma");
         if(profile) cout<<"Tahoma Font, ";
         break;

      case OPT_FNTNAME_ARIALBLACK:
         cell->fontname("Arial Black");
         if(profile) cout<<"Arial Black Font, ";
         break;

      case OPT_FNTNAME_CENTURYGOTHIC:
         cell->fontname("Century Gothic");
         if(profile) cout<<"Century Gothic Font, ";
         break;

      case OPT_FNTNAME_COMICSANSMS:
         cell->fontname("Comic Sans MS");
         if(profile) cout<<"Comic Sans MS Font, ";
         break;

      case OPT_FNTNAME_COURIER:
         cell->fontname("Courier");
         if(profile) cout<<"Courier Font, ";
         break;

      case OPT_FNTNAME_COURIERNEW:
         cell->fontname("Courier New");
         if(profile) cout<<"Courier New Font, ";
         break;

      case OPT_FNTNAME_FIXEDSYS:
         cell->fontname("Fixedsys");
         if(profile) cout<<"Fixedsys Font, ";
         break;

      case OPT_FNTNAME_GARAMOND:
         cell->fontname("Garamond");
         if(profile) cout<<"Garamond Font, ";
         break;

      case OPT_FNTNAME_IMPACT:
         cell->fontname("Impact");
         if(profile) cout<<"Impact Font, ";
         break;

      case OPT_FNTNAME_LUCIDACONSOLE:
         cell->fontname("Lucida Console");
         if(profile) cout<<"Lucida Console Font, ";
         break;

      case OPT_FNTNAME_MONOTYPECORSIVA:
         cell->fontname("Monotype Corsiva");
         if(profile) cout<<"Monotype Corsiva Font, ";
         break;

      case OPT_FNTNAME_SYMBOL:
         cell->fontname("Symbol");
         if(profile) cout<<"Symbol Font, ";
         break;

      case OPT_FNTNAME_WEBDINGS:
         cell->fontname("Webdings");
         if(profile) cout<<"Webdings Font, ";
         break;

      case OPT_FNTNAME_WINGDINGS:
         cell->fontname("Wingdings");
         if(profile) cout<<"Wingdings Font, ";
         break;
   }
}

string BOLD[] = 
{
   "BOLD",
   "HALF ",
   "NORMAL",
   "DOUBLE"
};

string SCRIPT[] = 
{
   "NONE",
   "SUPER ",
   "SUB"
};


string UNDERLINE[] = 
{
   "NONE",
   "SINGLE ",
   "DOUBLE",
   "SINGLEACC",
   "DOUBLEACC"
};


string COLOR[] = 
{
   "BLACK",
   "DARK_RED",
   "RED",
   "FUCSIA",
   "COMBINED01",
   "COMBINED02",
   "COMBINED03",
   "COMBINED04",
   "COMBINED05",
   "COMBINED06",
   "OLIVE",
   "DARK_YELLOW",
   "COMBINED07",
   "YELLOW",
   "LIGHT_YELLOW",
   "DARK_GREEN",
   "GREEN",
   "COMBINED08",
   "LIVING_GREEN",
   "LIGHT_GREEN",
   "COMBINED09",
   "BLUE_GREEN",
   "AQUAMARINA",
   "TURQOISE",
   "COMBINED10",
   "DARK_BLUE",
   "BLUE",
   "LIGHT_BLUE",
   "SKY_BLUE",
   "COMBINED11",
   "INDIGO",
   "BLUE_GRAY",
   "VIOLET",
   "PLUM",
   "LAVANDER",
   "COMBINED12",
   "GRAY50",
   "GRAY40",
   "GRAY25",
   "WHITE"
};

static void RandomFontOption(cell_t* cell, bool profile)
{
   switch(GetRndNumber(OPT_FONTMAX))
   {
      case OPT_FONTHEIGHT:
      {
         unsigned16_t height = (unsigned16_t)GetRndNumber(TWIP*40);
         cell->fontheight(height);
         if(profile) cout<<"Height "<<height<<", ";
         break;
      }
      case OPT_FONTBOLD:
      {
         int bold = GetRndNumber((int)BOLDNESS_DOUBLE);
         cell->fontbold((boldness_option_t)bold);
         if(profile) cout<<"Bold "<<BOLD[bold]<<", ";         
         break;
      }
      case OPT_FONTUNDERLINE:
      {
         int ul = GetRndNumber((int)UNDERLINE_DOUBLEACC);
         cell->fontunderline((underline_option_t)ul);
         if(profile) cout<<"Underline "<<UNDERLINE[ul]<<", ";          
         break;
      }
      case OPT_FONTSCRIPT:
      {
         int script = GetRndNumber((int)SCRIPT_SUB);
         cell->fontscript((script_option_t)script);
         if(profile) cout<<"Script "<<SCRIPT[script]<<", ";       
         break;
      }
      case OPT_FONTCOLOR:
      {
         int color = GetRndNumber((int)CLR_WHITE);
         cell->fontcolor((color_name_t)color);
         if(profile) cout<<"Font color "<<COLOR[color]<<", ";         
         break;
      }
      case OPT_FONTITALIC:
      {
         bool italic  = GetRndNumber(1)!=0;
         cell->fontitalic(italic);
         if(profile) italic?cout<<"Italic, ":cout<<"";                  
         break;
      }
      case OPT_FONTSTRIKEOUT:
      {
         bool so  = GetRndNumber(1)!=0;
         cell->fontstrikeout(so);
         if(profile) so?cout<<"Strikedout, ":cout<<"";                  
         break;
      }
      case OPT_FONTOUTLINE:
      {
         bool ol  = GetRndNumber(1)!=0;
         cell->fontoutline(ol);
         if(profile) ol?cout<<"Outlined, ":cout<<"";                  
         break;
      }
      case OPT_FONTSHADOW:
      {
         bool sh  = GetRndNumber(1)!=0;
         cell->fontshadow(sh);
         if(profile) sh?cout<<"Shadowed, ":cout<<"";                  
         break;
      }
   }
}

string HALIGN[] = 
{
   "GENERAL",
   "LEFT",
   "CENTER",
   "RIGHT",
   "FILL",
   "JUSTIFY",
   "CENTERACCROSS"
};

string VALIGN[] =
{
   "TOP",
   "CENTER",
   "BOTTOM",
   "JUSTIFY"
};

string TXTORI[] =
{
   "NONE",
   "TOPBOTTOMTXT",
   "90NOCLOCKTXT",
   "90CLOCKTXT"
};

string FILLSTYLE[] =
{
   "NONE",
   "SOLID",
   "ATEN75",
   "ATEN50",
   "ATEN25",
   "ATEN12",
   "ATEN06",
   "HORIZ_LIN",
   "VERTICAL_LIN",
   "DIAG",
   "INV_DIAG",
   "INTER_DIAG",
   "DIAG_THICK_INTER",
   "HORIZ_LINES_THIN",
   "VERTICAL_LINES_THIN",
   "DIAG_THIN",
   "INV_DIAG_THIN",
   "HORIZ_INT_THIN",
   "HORIZ_INTER_THICK"
};

string BORDERSTYLE[] =
{
   "NONE",
   "THIN",
   "MEDIUM",
   "DASHED",
   "DOTTED",
   "THICK",
   "DOUBLE",
   "HAIR"
};

string BORDERSIDE[] =
{
   "BOTTOM",
   "TOP",
   "LEFT",
   "RIGHT"
};




static void RandomFormat(cell_t* cell, bool profile)
{
   switch(GetRndNumber(OPT_MAX))
   {
      case OPT_HALIGN:
      {
         int ha = GetRndNumber((int)HALIGN_CENTERACCROSS);
         cell->halign((halign_option_t)ha);
         if(profile) cout<<"Halign "<<HALIGN[ha]<<", ";
         break;
      }

      case OPT_VALIGN:
      {
         int va = GetRndNumber((int)VALIGN_JUSTIFY);
         cell->valign((valign_option_t)va);
         if(profile) cout<<"Valign "<<VALIGN[va]<<", ";
         break;
      }

      case OPT_ORIENTATION:
      {
         int ori = GetRndNumber((int) ORI_90CLOCKTXT);
         cell->orientation((txtori_option_t)ori);
         if(profile) cout<<"TxtOri "<<TXTORI[ori]<<", ";
         break;
      }

      case OPT_FILLFGCOLOR:
      {
         int col = GetRndNumber((int) CLR_WHITE);
         cell->fillfgcolor((color_name_t)col);
         if(profile) cout<<"FillFGColor "<<COLOR[col]<<", ";
         break;
      }

      case OPT_FILLBGCOLOR:
      {
         int col = GetRndNumber((int) CLR_WHITE);
         cell->fillbgcolor((color_name_t)col);
         if(profile) cout<<"FillBGColor "<<COLOR[col]<<", ";
         break;
      }

      case OPT_FILLSTYLE:
      {
         int fsty = GetRndNumber((int) FILL_HORIZ_INTER_THICK);
         cell->fillstyle((fill_option_t)fsty);
         if(profile) cout<<"FillStyle "<<FILLSTYLE[fsty]<<", ";
         break;
      }

      case OPT_WRAP:
      {
         bool wr = GetRndNumber(1)!=0;
         cell->wrap(wr);
         if(profile) wr?cout<<"Wrapped, ":cout<<"";
         break;
      }

      case OPT_BORDERSTYLE:
      {
         int side = GetRndNumber((int) BORDER_RIGHT);
         int sty = GetRndNumber((int) BORDER_HAIR);
         int col = GetRndNumber((int) CLR_WHITE);

         cell->borderstyle((border_side_t)side,
                           (border_style_t)sty);
         cell->bordercolor((border_side_t)side,
                           (color_name_t)col);

         if(profile) cout<<BORDERSIDE[side]<<" side with "
                         <<BORDERSTYLE[sty]<<" style and "
                         <<COLOR[col]<<" color, ";
         break;
      }
   }
}

/*
*********************************
*********************************
*/
int StressTest(int sheets_sz, int rows_sz, int cols_sz, const char *md5_checksum)
{
   // Get the workbook handler
   workbook swb;

   xf_t* sxf1 = swb.xformat();

   worksheet* ssh;
   for(int sshnum = 0; sshnum < sheets_sz; sshnum++)
   {
      char* snum = (char*)malloc(42);
      sprintf(snum,"_%d",sshnum);
      string snamesheet = "DUH" + string(snum);

      ssh = swb.sheet(snamesheet);
      ssh->colwidth(1,10);
      ssh->rowheight(4,20);
      ssh->merge(0,1,1,4);

      free(snum);

      for(int srow = rows_sz; srow>=0; srow--)
	  {
         for(int scol = cols_sz; scol>=0; scol--)
         {
            char *slabel = (char*) malloc(42);
            sprintf(slabel,"s%d: %d_%d",sshnum,srow,scol);
            string snamelabel = string(slabel);
            ssh->label(srow,scol,snamelabel,sxf1);
            free(slabel);
         }
	  }
   }

   char fnamebuf[128];
   sprintf(fnamebuf, "stress%d-%d-%d.xls", sheets_sz, rows_sz, cols_sz);

   TIMESPAN_START(1);
   int err = swb.Dump(fnamebuf);
   TIMESPAN_END(1,"Cell-stress test:");

   if (err != NO_ERRORS)
   {
	   cerr << "StressTest(" << sheets_sz << ", " << rows_sz << ", " << cols_sz << ") failed: I/O failure: " << err << std::endl;
	   return -1;
   }
   if (0 != check_file(fnamebuf, md5_checksum))
   {
	   cerr << "StressTest(" << sheets_sz << ", " << rows_sz << ", " << cols_sz << ") failed: MD5 of generated XLS mismatch or I/O failure." << std::endl;
	   return -1;
   }
   return 0;
}

/*
*********************************
*********************************
*/

int RandomTest(int sheets_sz, int rows_sz, int cols_sz, int random_seed, const char *md5_checksum)
{
   SeedRndNumber(random_seed);
   TIMESPAN_START(1);
   // Get the workbook handler
   workbook swb;

   xf_t* sxf1 = swb.xformat();


   worksheet* ssh;
   for(int sshnum = 0; sshnum < sheets_sz; sshnum++)
   {
      char* snum = (char*)malloc(42);
      sprintf(snum,"_%d",sshnum);
      string snamesheet = "DUH" + string(snum);

      ssh = swb.sheet(snamesheet);
      free(snum);

      for(int srow = 0; srow<rows_sz; srow++)
	  {
         for(int scol = 0; scol<cols_sz; scol++)
         {
            int rndcol, rndrow;
			int koc = GetRndNumber(6);

            rndcol = GetRndNumber(cols_sz - 1);
            rndrow = GetRndNumber(rows_sz - 1);

			switch (koc)
			{
			default:
			case 0: // string
				{
					char slabel[80];
					sprintf(slabel,"s%d: %d_%d#%d-%d", sshnum, srow, scol, rndrow, rndcol);
					string snamelabel = string(slabel);
					ssh->label(rndrow,rndcol,snamelabel,sxf1);
				}
				break;

			case 1: // integer (most probably; depends a bit: large ints end up as float anyhow in there
				{
					ssh->number(rndrow,rndcol,GetRndNumber(INT_MAX) - INT_MAX/2,sxf1);
				}
				break;

			case 2: // double
				{
					ssh->number(rndrow,rndcol,GetRndNumber(INT_MAX)* 2.0/INT_MAX - 1.0,sxf1);
				}
				break;

			case 3: // blank
				{
					ssh->blank(rndrow,rndcol,sxf1);
				}
				break;

			case 4: // bool
				{
					ssh->boolean(rndrow,rndcol,(GetRndNumber(INT_MAX) >= INT_MAX/2), sxf1);
				}
				break;

			case 5: // err
				{
					ssh->error(rndrow,rndcol,PickErrorCode(GetRndNumber(INT_MAX)), sxf1);
				}
				break;
			}
         }
	  }
   }

   int err = swb.Dump("random.xls");
   TIMESPAN_END(1,"Random cell test:");

   // cannot MD5 the file; not a real unit test, this one...

   if (err != NO_ERRORS)
   {
	   cerr << "RandomTest failed: I/O failure: " << err << std::endl;
	   return -1;
   }
   if (0 != check_file("random.xls", md5_checksum))
   {
	   cerr << "RandomTest failed: MD5 of generated XLS mismatch or I/O failure." << std::endl;
	   return -1;
   }
   return 0;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: mainCPP.cpp,v $
 * Revision 1.5  2009/03/08 21:51:40  dhoerl
 * minor tweak
 *
 * Revision 1.4  2009/03/08 21:26:28  dhoerl
 * minor tweak
 *
 * Revision 1.3  2009/03/02 04:39:46  dhoerl
 * borderstyle
 *
 * Revision 1.2  2009/01/23 16:09:56  dhoerl
 * General cleanup: headers and includes. Fixed issues building mainC and mainCPP
 *
 * Revision 1.1  2008/10/25 20:26:48  dhoerl
 * Rename
 *
 * Revision 1.2  2008/10/25 18:55:44  dhoerl
 * 2008
 *
 * Revision 1.1.1.1  2004/08/27 16:32:05  darioglz
 * Initial Import.
 *
 * Revision 1.40  2004/04/06 22:36:38  dgonzalez
 * + Added xlslib_core namespace usage declaration
 *
 * Revision 1.39  2004/02/21 02:44:34  dgonzalez
 * + Standard test tries to locate abug with rowheights
 *
 * Revision 1.38  2004/01/29 03:24:19  dgonzalez
 * + Using config.h
 *
 * Revision 1.37  2004/01/28 17:13:43  dgonzalez
 * + Rearranged directory-tree structure
 * + Removed unused variable warnings
 *
 * Revision 1.36  2004/01/15 17:20:04  dgonzalez
 * + More tests
 *
 * Revision 1.35  2004/01/09 00:15:08  dgonzalez
 * + Test
 *
 * Revision 1.34  2003/12/12 02:08:52  dgonzalez
 * + StandardTest implements a very basic check for numbers and
 *    format numbers.
 *
 * Revision 1.33  2003/12/11 18:50:53  dgonzalez
 * + Testing of numbers in the Standard test
 *
 * Revision 1.32  2003/12/10 17:16:29  dgonzalez
 * + No changes
 *
 * Revision 1.31  2003/12/10 01:25:15  dgonzalez
 * + The RandomCellAndFormatTestProf function was added. this
 *    function logs the cell definitions randomly generated to the standard
 *    output.
 * + The RandomFormat and RandomFont and RandomFontOption
 *     functions now take an optional arg to tell them that the log info has
 *     to be generated.
 *
 * Revision 1.30  2003/12/09 18:56:57  dgonzalez
 * + The tests were implemented in separated functions and are called
 *     from main.
 * + All functionality of discrete format and font settings of cells was tested.
 * + A test to generate random cells with random format and font style
 *    was implemented successfully. NOTE: It seem to be a limit in
 *    the number of FONT and XF records (only calculated for FONTS: 500).
 * + TODO: Get thi exact number of max records and generate an error me
 *    ssage in the code when these limits have been reached.
 *
 * Revision 1.29  2003/12/05 00:41:10  dgonzalez
 * + This is a test for discrete format/font mainpulation of cells and range
 *     manipulation (range only works for format... next revision will do
 *     that).
 *
 * Revision 1.28  2003/12/02 22:53:19  dgonzalez
 * + Test of the cell's discrete formating
 *
 * Revision 1.27  2003/11/28 02:07:34  dgonzalez
 * + Test to check that unused fonts and format aren't written to the file
 *
 * Revision 1.26  2003/11/27 17:29:15  dgonzalez
 * + Added a RandomTest() function, which adds labels to the sheets in random
 *    order.
 * + The canonical test has been executed in 0.14 secs.
 * + The maximum filesize that can be generated with the current COleDoc implementation
 *    is ~6MB (29 sheets, 100X100 cells each) is performed in 1.3 secs.
 *
 * Revision 1.25  2003/11/26 16:49:26  dgonzalez
 * + Indented and untabified
 *
 * Revision 1.24  2003/11/25 23:18:42  dgonzalez
 * + The canonical test throw the following timing (in Windows-Unix
 *     platforms):
 *     - MinGW (-O3) 0.125 secs
 *     - MinGW (-O2) 0.141 secs
 *     - MinGW (plain) 0.297 secs
 *  + A function to perform the stress test was created (StressTest()).
 * + The cell overwrite feature is working
 *
 * Revision 1.23  2003/11/22 19:43:16  dgonzalez
 * + BLANK cell tested successfully
 * + Used new CSheetRecords' intarface function names.
 *
 * Revision 1.22  2003/11/20 02:51:25  dgonzalez
 * + Column widths and row heights can be set now
 *
 * Revision 1.21  2003/11/18 23:39:43  dgonzalez
 * + After the CStorageClass modification, the speed of the program improved as follows
 *    (all tests were performed generating a 3sheet-100x100label-each workbook file):
 *    - Under Cygwin (MinGW compiler):
 *       2.375 secs (with the -O3 optimization flag)
 *    - Under Linux
 *       2.8 secs  (with the -O3 optimization flag)
 *       2.32 secs (with the -O2 optimization flag)
 *
 * Revision 1.20  2003/11/16 18:41:45  dgonzalez
 * + Just another stress example:
 *    - 3 sheets, 10000 cells each generated in 61.046 secs compiled
 *      with MinGW gcc compiler (in Cygwing framework).
 *    - BTW, the merging of cells is working.
 *
 * Revision 1.19  2003/11/15 17:32:57  dgonzalez
 * + test example.
 *
 * Revision 1.18  2003/11/10 22:08:50  dgonzalez
 * + New directory structure
 *
 * Revision 1.17  2003/11/04 01:04:42  dgonzalez
 * + All the list containers changed to manage pointers.
 * + Valgrind utility reports 960 posible memory leaks. TODO: Verify it
 *
 * Revision 1.16  2003/11/03 17:12:09  dgonzalez
 * + With this version all font attributes can be set.
 *
 * Revision 1.15  2003/11/01 21:31:44  dgonzalez
 * + The Document's properties are updated
 *
 * Revision 1.14  2003/11/01 19:49:21  dgonzalez
 * + The program doesn't crash when nothing is defined, but the generated file cannot be
 *    read by Excel (it crashes). Gnumeric complains saying that there are no sheets but
 *    the BiffView utility can read it.
 *
 * Revision 1.13  2003/11/01 19:11:16  dgonzalez
 * + With this revision, the following bugs are solved:
 *      - The program crashed with a seg-fault when only one cell was being defined
 *        in one seet.
 *      - The program crashed with a seg-fault when no cells were being defined in one
 *        seet.
 *
 * Revision 1.12  2003/11/01 00:50:50  dgonzalez
 * + The following features are tested with this main.cpp file:
 *      - Three text orientation modes
 *      - Seven horizontal alignment modes
 *      - Four vertical alignment modes
 *
 * + Also insertion of accented chars and newlines (\n) were tested.
 *
 * -> A bug was detected: The generated excel file cannot be "saved as..."
 *     because Excel crashes.
 *
 * Revision 1.11  2003/10/31 23:46:41  dgonzalez
 * + The following functionalities were well tested:
 *     - Adding a new extended format.
 *     - Set color and style of cell's borders.
 *     - Set (fore)background colors of pattern (40 different colors
 *       supported). In order to get a cell colored, the FILL_SOLID
 *       option has to be selected along with the desired FG color.
 *     - All cell patterns are functional.
 *     - Cell's wrap text works.
 *     - The hidden and locked cell's option is working, though is
 *       useless sofar because there is no way to set the protect-sheet
 *       option. (TODO)
 *
 * + A bug was detected: The program crashes with a seg-fault when
 *   only one cell is added. The source of the fault is identified
 *   and it'll be corrected the next revision (TODO).
 * + Check it in the following scenarios (it may crash):
 *     - zero cells
 *     - zero sheets
 *
 * Revision 1.10  2003/10/30 20:33:27  dgonzalez
 * + The test was changed to verify only 2 cells and its format.
 *
 * -> TODO: A bug was detected. The program crashes when a single cell was
 *    added to a sheet.
 *
 * Revision 1.9  2003/10/30 18:15:49  dgonzalez
 * + Memory leaks were removed
 * + Harmless warnings were removed
 *
 * Revision 1.8  2003/10/29 16:25:12  dgonzalez
 * + Minor testing changes
 *
 * Revision 1.7  2003/10/24 23:24:32  dgonzalez
 * + Added CVS-Keyword substitution.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

