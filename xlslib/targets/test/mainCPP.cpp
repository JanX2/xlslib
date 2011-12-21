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
 *
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <xlslib.h>

#include "common/timespan.h"

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
#define OPT_MAX           (unsigned32_t)9

#define OPT_FONTHEIGHT      0
#define OPT_FONTBOLD        1
#define OPT_FONTUNDERLINE   2
#define OPT_FONTSCRIPT      3
#define OPT_FONTCOLOR       4
#define OPT_FONTITALIC      5
#define OPT_FONTSTRIKEOUT   6
#define OPT_FONTOUTLINE     7
#define OPT_FONTSHADOW      8
#define OPT_FONTMAX         (unsigned32_t)8

// Macros
#define TIMESPAN_START(id)                      \
   CCpuClockTimespan span_##id;                 \
   span_##id.StartClock()

#define TIMESPAN_END(id,str)                                      \
   span_##id.StopClock();                                         \
   std::cerr<<"    # "<<str<<" "<<span_##id.GetUsedMilliseconds()<<" ms"<<std::endl

static char file_err[] = "00000000000000000000000000000000";

static void RandomFormat(cell_t* cell, bool profile = false);
static void RandomFontName(cell_t* cell, bool profile = false);
static void RandomFontOption(cell_t* cell, bool profile = false);
static void SeedRndNumber(unsigned32_t seed);
static unsigned32_t GetRndNumber(unsigned32_t max);

char *StressTest(unsigned32_t a,unsigned32_t b,unsigned32_t c, const char *md5_checksum);
char *RandomTest(unsigned32_t a,unsigned32_t b,unsigned32_t c, unsigned32_t random_seed, const char *md5_checksum);
char *RandomCellAndFormatTest(unsigned32_t sheets_sz, unsigned32_t rows_sz, unsigned32_t cols_sz, unsigned32_t random_seed, const char *md5_checksum);
char *RandomFormatTest(unsigned32_t sheets_sz, unsigned32_t rows_sz, unsigned32_t cols_sz, unsigned32_t random_seed, const char *md5_checksum);
char *RandomCellAndFormatTestProf(unsigned32_t sheets_sz, unsigned32_t rows_sz, unsigned32_t cols_sz, unsigned32_t random_seed, const char *md5_checksum);
char *StandardTest(const char *md5_checksum);
char *StandardTest2(const char *md5_checksum);
char *BlankTest(const char *md5_checksum);
char *FormulaFunctionsTest(const char *md5_checksum);

/*
*********************************
********************************* 
*/

#define NUM_TESTS	10

int main(int argc, char *argv[])
{
	int rv = 0;
	char check[NUM_TESTS][40];
	char fileName[128];
	
	fileName[0] = 0;

	// Used for internal testing
	if(argc == 2) {
#ifdef _X_DEBUG_
		chdir(argv[1]);
#endif
		strcpy(fileName, argv[1]);
		strcat(fileName, "/");		
	}
	strcat(fileName, "mainCPP.md5");
	
	{
		FILE *fp = fopen(fileName, "r");
		for(int i=0; i<NUM_TESTS; ++i) {
			char *checkP = check[i];
			if(fp) {
				fscanf(fp, "%s", checkP);
			} else {
				strcpy(checkP, "00000000000000000000000000000000");
			}
			// printf("MD5 = %s\n", checkP);
		}
		fclose(fp);
	}
	
	try
	{
		int idx = 0;
		int failed = 1;
		char *checkP;
		// comment and uncomment the below to try various tests
#if 1
		{
			checkP = check[idx];
			if((checkP = StandardTest(checkP)))
			{
				rv |= failed;
				strcpy(check[idx], checkP);
			}
		}
#endif
		failed <<= 1, ++idx;
		
#if 1
		{
			checkP = check[idx];
			if((checkP = StandardTest2(checkP)))
			{
				rv |= failed;
				strcpy(check[idx], checkP);
			}
		}
#endif
		failed <<= 1, ++idx;
#if 1
		{
			checkP = check[idx];
			if((checkP = BlankTest(checkP)))
			{
				rv |= failed;
				strcpy(check[idx], checkP);
			}
		}
#endif
		failed <<= 1, ++idx;

#if 1
		{
			checkP = check[idx];
			if((checkP = FormulaFunctionsTest(checkP)))
			{
				rv |= failed;
				strcpy(check[idx], checkP);
			}
		}
#endif
		failed <<= 1, ++idx;

#if 1
		{
			checkP = check[idx];
			if((checkP = StressTest(3,100,100, checkP)))
			{
				rv |= failed;
				strcpy(check[idx], checkP);
			}
		}
#endif
		failed <<= 1, ++idx;

#if 1
		{
			checkP = check[idx];
			if((checkP = StressTest(3,4,4, checkP)))
			{
				rv |= failed;
				strcpy(check[idx], checkP);
			}
		}
#endif
		failed <<= 1, ++idx;

#if 1
		{
			checkP = check[idx];
			if((checkP = RandomTest(3,200,200, 42424242, checkP)))
			{
				rv |= failed;
				strcpy(check[idx], checkP);
			}
		}
#endif
		failed <<= 1, ++idx;

#if 1
		{
			checkP = check[idx];
			if((checkP = RandomCellAndFormatTest(1,15,10, 123456789, checkP)))
			{
				rv |= failed;
				strcpy(check[idx], checkP);
			}
		}
#endif
		failed <<= 1, ++idx;

#if 1
		{
			checkP = check[idx];
			if((checkP = RandomCellAndFormatTestProf(1,15,10, 987654321, checkP)))
			{
				rv |= failed;
				strcpy(check[idx], checkP);
			}
		}
#endif
		failed <<= 1, ++idx;

#if 1
		{
			checkP = check[idx];
			if((checkP = RandomFormatTest(1,15,10, 42004200, checkP)))
			{
				rv |= failed;
				strcpy(check[idx], checkP);
			}
		}
#endif
		failed <<= 1, ++idx;

		std::cerr << "    # Test finished" << std::endl;

		if(rv && argc == 2) {
			FILE *fp = fopen(fileName, "w");
			if(fp) {
				for(int i=0; i<NUM_TESTS; ++i) {
					checkP = check[i];
					if(fp) {
						fprintf(fp, "%s\n", checkP);
					}
					printf("MD5 = %s\n", checkP);
				}
				fclose(fp);
			}
		}
	}
	catch (std::string &errmsg)
	{
		std::cerr << errmsg << std::endl;
		std::cerr << "    # Test aborted" << std::endl;
		rv = 1;
	}

	return (rv == 0 ? EXIT_SUCCESS : EXIT_FAILURE);
}

char *StandardTest(const char *md5_checksum)
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
   unsigned32_t lim = atoi(buf);
#else
   unsigned32_t lim = 42;
#endif
   printf("\n    # limit: %d\n", lim); // >= 14 --> error to load sheet #2; <= 13 = ok

   unsigned32_t j, k;
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

   int err = wb.Dump("testCPP.xls");

   char *checkP = file_err;
   if (err != NO_ERRORS)
   {
	   cerr << "StandardTest failed: I/O failure: " << err << std::endl;
	   return checkP;
   }
   if ((checkP = check_file("./testCPP.xls", md5_checksum)))
   {
	   cerr << "StandardTest failed: MD5 of generated XLS mismatch or I/O failure." << std::endl;
   }
   return checkP;
}



char *BlankTest(const char *md5_checksum)
{
	workbook wb;
	wb.sheet("Sheet_01");
	int err = wb.Dump("blank.xls");

    char *checkP = file_err;
	if (err != NO_ERRORS)
	{
		cerr << "BlankTest failed: I/O failure: " << err << std::endl;
		return checkP;
	}
	if ((checkP = check_file("blank.xls", md5_checksum)))
	{
		cerr << "BlankTest failed: MD5 of generated XLS mismatch or I/O failure." << std::endl;
	}
	return checkP;
}


static expression_node_t *build_formula(unsigned32_t row, unsigned32_t col, worksheet *sh3, worksheet *sh4, unsigned32_t val, workbook &wb)
{
	expression_node_factory_t& maker = wb.GetFormulaFactory();

	if(!sh3) return NULL;
	
	cell_t *cref = sh4->FindCellOrMakeBlank(row+1, col+1);
	assert(cref);
	expression_node_t *root = maker.op(OP_ADD, maker.integer((signed32_t)val), maker.cell(*cref, sh4, CELL_RELATIVE_A1));
	//expression_node_t *root = maker.integer(val);

	return root;
}

char *StandardTest2(const char *md5_checksum)
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

	unsigned32_t j, k;
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

			unsigned32_t v = (j + k) % (sizeof(errcvt)/sizeof(errcvt[0]));
			sh1->error(j + 4, k, errcvt[v]);

			bool v2 = (((j + k) / 10) % 7 > 3) ^ ((128 * j + k) % 129 == 1);
			sh2->boolean(j + 4, k, v2);

			expression_node_t *ast = build_formula(j, k, sh3, sh4, v, wb);
			sh3->formula(j + 4, k, ast, true);

			char buf[256];
			sprintf(buf, "Remark item %d/%d/%d", j, k, v);

			sh4->number(j + 4, k, (signed32_t)v);
			//sh4->note(j + 4, k, buf, "GHO");
		}
	}

	int err = wb.Dump("testCPP2.xls");

	char *checkP = file_err;
	if (err != NO_ERRORS)
	{
		cerr << "StandardTest2 failed: I/O failure: " << err << std::endl;
		return file_err;
	}
	if ((checkP = check_file("./testCPP2.xls", md5_checksum)))
	{
		cerr << "StandardTest2 failed: MD5 of generated XLS mismatch or I/O failure." << std::endl;
	}
	return checkP;
}

static unsigned32_t seed = 0;

static void SeedRndNumber(unsigned32_t sv)
{
	seed = sv;
}
static unsigned32_t GetRndNumber(unsigned32_t max)
{
	// this is NOT a good random generator but suffices for our purposes!
	seed *= 15482893;
	seed %= 792241;

	unsigned32_t rndnum;
	rndnum = (unsigned32_t)(seed * ((max + 1.0) / (792241 - 1.0)));
	return rndnum;
}

static errcode_t PickErrorCode(unsigned32_t value)
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

	value = (unsigned32_t)(value / divider);
	XL_ASSERT((signed32_t)value >= 0);
	XL_ASSERT(value <= sizeof(elist)/sizeof(elist[0]));
	return elist[value];
}


char *RandomCellAndFormatTest(unsigned32_t sheets_sz, unsigned32_t rows_sz, unsigned32_t cols_sz, unsigned32_t random_seed, const char *md5_checksum)
{
   workbook wb;
   worksheet* sh;

   SeedRndNumber(random_seed);

   TIMESPAN_START(1);

   for(unsigned32_t shnum = 0; shnum < sheets_sz; shnum++)
   {
      char tmp[256];

	  sprintf(tmp, "DUH_%d", shnum);
	  string snamesheet(tmp);

      sh = wb.sheet(snamesheet);

      for(unsigned32_t row=0; row<rows_sz; row++)
      {
	     // height unit = point! Internally Excel works with 'twip': 1/20th of a point, but the interface works in /points/.
         sh->rowheight(row, (unsigned16_t)(GetRndNumber(13)+20));
         for(unsigned32_t col=0; col<cols_sz; col++)
         {
		    // width unit = 1/256th of the width of '0'
            sh->colwidth(row, (unsigned16_t)(GetRndNumber(2000)+4000));

            unsigned32_t rndcol = GetRndNumber(rows_sz);
            unsigned32_t rndrow = GetRndNumber(cols_sz);

			sprintf(tmp, "S%d:%d-%d#%d-%d", shnum, row+1, col+1, rndrow, rndcol);
			string snamelabel(tmp);

            cell_t* cell = sh->label(rndrow, rndcol, snamelabel);

            unsigned32_t k, fmtries = GetRndNumber(OPT_MAX);
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

   char *checkP = file_err;
   if (err != NO_ERRORS)
   {
	   cerr << "RandomCellAndFormatTest failed: I/O failure: " << err << std::endl;
	   return file_err;
   }
   if ((checkP = check_file("rndcellandformat.xls", md5_checksum)))
   {
	   cerr << "RandomCellAndFormatTest failed: MD5 of generated XLS mismatch or I/O failure." << std::endl;
   }
   return checkP;
}

char *RandomCellAndFormatTestProf(unsigned32_t sheets_sz, unsigned32_t rows_sz, unsigned32_t cols_sz, unsigned32_t random_seed, const char *md5_checksum)
{
   workbook wb;
   worksheet* sh;

   SeedRndNumber(random_seed);

   for(unsigned32_t shnum = 0; shnum < sheets_sz; shnum++)
   {
      char tmp[256];

	  sprintf(tmp, "DUH_%d", shnum);
	  string snamesheet(tmp);

      sh = wb.sheet(snamesheet);

      for(unsigned32_t row = 0; row<rows_sz; row++)
      {
	     // height unit = point! Internally Excel works with 'twip': 1/20th of a point, but the interface works in /points/.
		  sh->rowheight(row,(unsigned16_t)(GetRndNumber(13)+20));
		  for(unsigned32_t col = 0; col<cols_sz; col++)
		  {
			  // width unit = 1/256th of the width of '0'
            sh->colwidth(row, (unsigned16_t)(GetRndNumber(2000)+4000));

            unsigned32_t rndcol = GetRndNumber(rows_sz);
            unsigned32_t rndrow = GetRndNumber(cols_sz);

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

			unsigned32_t k, fmtries = GetRndNumber(OPT_MAX);

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

   char *checkP = file_err;
   if (err != NO_ERRORS)
   {
	   cerr << "RandomCellAndFormatTestProf failed: I/O failure: " << err << std::endl;
	   return file_err;
   }
   if ((checkP = check_file("rndcellandformat_prof.xls", md5_checksum)))
   {
	   cerr << "RandomCellAndFormatTestProf failed: MD5 of generated XLS mismatch or I/O failure." << std::endl;
   }
   return checkP;
}


char *RandomFormatTest(unsigned32_t sheets_sz, unsigned32_t rows_sz, unsigned32_t cols_sz, unsigned32_t random_seed, const char *md5_checksum)
{
   SeedRndNumber(random_seed);

   TIMESPAN_START(1);

   workbook wb;
   worksheet* sh;

   for(unsigned32_t shnum = 0; shnum < sheets_sz; shnum++)
   {
      char tmp[256];

	  sprintf(tmp, "DUH_%d", shnum);
	  string snamesheet(tmp);

      sh = wb.sheet(snamesheet);

      for(unsigned32_t row = 0; row<rows_sz; row++)
      {
	     // height unit = point! Internally Excel works with 'twip': 1/20th of a point, but the interface works in /points/.
		  sh->rowheight(row, (unsigned16_t)(GetRndNumber(13)+20));
		  for(unsigned32_t col=0; col<cols_sz; col++)
		  {
			  // width unit = 1/256th of the width of '0'
            sh->colwidth(row, (unsigned16_t)(GetRndNumber(2000)+4000));

			sprintf(tmp, "S%d:%d-%d", shnum, row+1, col+1);
			string snamelabel(tmp);

            cell_t* cell = sh->label(row, col, snamelabel);

            unsigned32_t k, fmtries = GetRndNumber(OPT_MAX);
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

   char *checkP = file_err;
   if (err != NO_ERRORS)
   {
	   cerr << "RandomFormatTest failed: I/O failure: " << err << std::endl;
	   return checkP;
   }
   if ((checkP = check_file("rndformat.xls", md5_checksum)))
   {
	   cerr << "RandomFormatTest failed: MD5 of generated XLS mismatch or I/O failure." << std::endl;
   }
   return checkP;
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
         unsigned32_t bold = GetRndNumber((unsigned32_t)BOLDNESS_DOUBLE);
         cell->fontbold((boldness_option_t)bold);
         if(profile) cout<<"Bold "<<BOLD[bold]<<", ";         
         break;
      }
      case OPT_FONTUNDERLINE:
      {
         unsigned32_t ul = GetRndNumber((unsigned32_t)UNDERLINE_DOUBLEACC);
         cell->fontunderline((underline_option_t)ul);
         if(profile) cout<<"Underline "<<UNDERLINE[ul]<<", ";          
         break;
      }
      case OPT_FONTSCRIPT:
      {
         unsigned32_t script = GetRndNumber((unsigned32_t)SCRIPT_SUB);
         cell->fontscript((script_option_t)script);
         if(profile) cout<<"Script "<<SCRIPT[script]<<", ";       
         break;
      }
      case OPT_FONTCOLOR:
      {
         unsigned32_t color = GetRndNumber((unsigned32_t)CLR_WHITE);
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
         unsigned32_t ha = GetRndNumber((int)HALIGN_CENTERACCROSS);
         cell->halign((halign_option_t)ha);
         if(profile) cout<<"Halign "<<HALIGN[ha]<<", ";
         break;
      }

      case OPT_VALIGN:
      {
         unsigned32_t va = GetRndNumber((int)VALIGN_JUSTIFY);
         cell->valign((valign_option_t)va);
         if(profile) cout<<"Valign "<<VALIGN[va]<<", ";
         break;
      }

      case OPT_ORIENTATION:
      {
         unsigned32_t ori = GetRndNumber((int) ORI_90CLOCKTXT);
         cell->orientation((txtori_option_t)ori);
         if(profile) cout<<"TxtOri "<<TXTORI[ori]<<", ";
         break;
      }

      case OPT_FILLFGCOLOR:
      {
         unsigned32_t col = GetRndNumber((int) CLR_WHITE);
         cell->fillfgcolor((color_name_t)col);
         if(profile) cout<<"FillFGColor "<<COLOR[col]<<", ";
         break;
      }

      case OPT_FILLBGCOLOR:
      {
         unsigned32_t col = GetRndNumber((int) CLR_WHITE);
         cell->fillbgcolor((color_name_t)col);
         if(profile) cout<<"FillBGColor "<<COLOR[col]<<", ";
         break;
      }

      case OPT_FILLSTYLE:
      {
         unsigned32_t fsty = GetRndNumber((int) FILL_HORIZ_INTER_THICK);
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
         unsigned32_t side = GetRndNumber((int) BORDER_RIGHT);
         unsigned32_t sty = GetRndNumber((int) BORDER_HAIR);
         unsigned32_t col = GetRndNumber((int) CLR_WHITE);

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
char *StressTest(unsigned32_t sheets_sz, unsigned32_t rows_sz, unsigned32_t cols_sz, const char *md5_checksum)
{
   // Get the workbook handler
   workbook swb;

   xf_t* sxf1 = swb.xformat();

   worksheet* ssh;
   for(unsigned32_t sshnum = 0; sshnum < sheets_sz; sshnum++)
   {
      char* snum = (char*)malloc(42);
      sprintf(snum,"_%d",sshnum);
      string snamesheet = "DUH" + string(snum);

      ssh = swb.sheet(snamesheet);
      ssh->colwidth(1,10);
      ssh->rowheight(4,20);
      ssh->merge(0,1,1,4);

      free(snum);

      for(unsigned32_t srow = rows_sz; (signed32_t)srow>=0; srow--)
	  {
         for(unsigned32_t scol = cols_sz; (signed32_t)scol>=0; scol--)
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

   char *checkP = file_err;
   if (err != NO_ERRORS)
   {
	   cerr << "StressTest(" << sheets_sz << ", " << rows_sz << ", " << cols_sz << ") failed: I/O failure: " << err << std::endl;
	   return file_err;
   }
   if ((checkP = check_file(fnamebuf, md5_checksum)))
   {
	   cerr << "StressTest(" << sheets_sz << ", " << rows_sz << ", " << cols_sz << ") failed: MD5 of generated XLS mismatch or I/O failure." << std::endl;
   }
   return checkP;
}

/*
*********************************
*********************************
*/

char *RandomTest(unsigned32_t sheets_sz, unsigned32_t rows_sz, unsigned32_t cols_sz, unsigned32_t random_seed, const char *md5_checksum)
{
   SeedRndNumber(random_seed);
   TIMESPAN_START(1);
   // Get the workbook handler
   workbook swb;

   xf_t* sxf1 = swb.xformat();

   worksheet* ssh;
   for(unsigned32_t sshnum = 0; sshnum < sheets_sz; sshnum++)
   {
      char* snum = (char*)malloc(42);
      sprintf(snum,"_%d",sshnum);
      string snamesheet = "DUH" + string(snum);

      ssh = swb.sheet(snamesheet);
      free(snum);

      for(unsigned32_t srow = 0; srow<rows_sz; srow++)
	  {
         for(unsigned32_t scol = 0; scol<cols_sz; scol++)
         {
            unsigned32_t rndcol, rndrow;
			unsigned32_t koc = GetRndNumber((unsigned32_t)6);

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
					ssh->number(rndrow,rndcol,(signed32_t)(GetRndNumber(INT_MAX) - INT_MAX/2),sxf1);
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
					ssh->error(rndrow,rndcol,PickErrorCode(GetRndNumber((unsigned32_t)INT_MAX)), sxf1);
				}
				break;
			}
         }
	  }
   }

   int err = swb.Dump("random.xls");
   TIMESPAN_END(1,"Random cell test:");

   // cannot MD5 the file; not a real unit test, this one...
   char *checkP = file_err;
   if (err != NO_ERRORS)
   {
	   cerr << "RandomTest failed: I/O failure: " << err << std::endl;
	   return checkP;
   }
   if ((checkP = check_file("random.xls", md5_checksum)))
   {
	   cerr << "RandomTest failed: MD5 of generated XLS mismatch or I/O failure." << std::endl;
   }
   return checkP;
}



static const struct
{
	const char *name;
	expr_function_code_t code;
} function_arr[] = 
{
	{ "COUNT", FUNC_COUNT },
	{ "ISNA", FUNC_ISNA },
	{ "ISERROR", FUNC_ISERROR },
	{ "SUM", FUNC_SUM },
	{ "AVERAGE", FUNC_AVERAGE },
	{ "MIN", FUNC_MIN },
	{ "MAX", FUNC_MAX },
	{ "ROW", FUNC_ROW },
	{ "COLUMN", FUNC_COLUMN },
	{ "NA", FUNC_NA },
	{ "NPV", FUNC_NPV },
	{ "STDEV", FUNC_STDEV },
	{ "DOLLAR", FUNC_DOLLAR },
	{ "FIXED", FUNC_FIXED },
	{ "SIN", FUNC_SIN },
	{ "COS", FUNC_COS },
	{ "TAN", FUNC_TAN },
	{ "ATAN", FUNC_ATAN },
	{ "PI", FUNC_PI },
	{ "SQRT", FUNC_SQRT },
	{ "EXP", FUNC_EXP },
	{ "LN", FUNC_LN },
	{ "LOG10", FUNC_LOG10 },
	{ "ABS", FUNC_ABS },
	{ "INT", FUNC_INT },
	{ "SIGN", FUNC_SIGN },
	{ "ROUND", FUNC_ROUND },
	{ "LOOKUP", FUNC_LOOKUP },
	{ "INDEX", FUNC_INDEX },
	{ "REPT", FUNC_REPT },
	{ "MID", FUNC_MID },
	{ "LEN", FUNC_LEN },
	{ "VALUE", FUNC_VALUE },
	{ "TRUE", FUNC_TRUE },
	{ "FALSE", FUNC_FALSE },
	{ "AND", FUNC_AND },
	{ "OR", FUNC_OR },
	{ "NOT", FUNC_NOT },
	{ "MOD", FUNC_MOD },
	{ "DCOUNT", FUNC_DCOUNT },
	{ "DSUM", FUNC_DSUM },
	{ "DAVERAGE", FUNC_DAVERAGE },
	{ "DMIN", FUNC_DMIN },
	{ "DMAX", FUNC_DMAX },
	{ "DSTDEV", FUNC_DSTDEV },
	{ "VAR", FUNC_VAR },
	{ "DVAR", FUNC_DVAR },
	{ "TEXT", FUNC_TEXT },
	{ "LINEST", FUNC_LINEST },
	{ "TREND", FUNC_TREND },
	{ "LOGEST", FUNC_LOGEST },
	{ "GROWTH", FUNC_GROWTH },
	{ "GOTO", FUNC_GOTO },
	{ "HALT", FUNC_HALT },
	{ "PV", FUNC_PV },
	{ "FV", FUNC_FV },
	{ "NPER", FUNC_NPER },
	{ "PMT", FUNC_PMT },
	{ "RATE", FUNC_RATE },
	{ "MIRR", FUNC_MIRR },
	{ "IRR", FUNC_IRR },
	{ "RAND", FUNC_RAND },
	{ "MATCH", FUNC_MATCH },
	{ "DATE", FUNC_DATE },
	{ "TIME", FUNC_TIME },
	{ "DAY", FUNC_DAY },
	{ "MONTH", FUNC_MONTH },
	{ "YEAR", FUNC_YEAR },
	{ "WEEKDAY", FUNC_WEEKDAY },
	{ "HOUR", FUNC_HOUR },
	{ "MINUTE", FUNC_MINUTE },
	{ "SECOND", FUNC_SECOND },
	{ "NOW", FUNC_NOW },
	{ "AREAS", FUNC_AREAS },
	{ "ROWS", FUNC_ROWS },
	{ "COLUMNS", FUNC_COLUMNS },
	{ "OFFSET", FUNC_OFFSET },
	{ "ABSREF", FUNC_ABSREF },
	{ "RELREF", FUNC_RELREF },
	{ "ARGUMENT", FUNC_ARGUMENT },
	{ "SEARCH", FUNC_SEARCH },
	{ "TRANSPOSE", FUNC_TRANSPOSE },
	{ "ERROR", FUNC_ERROR },
	{ "STEP", FUNC_STEP },
	{ "TYPE", FUNC_TYPE },
	{ "ECHO", FUNC_ECHO },
	{ "SETNAME", FUNC_SETNAME },
	{ "CALLER", FUNC_CALLER },
	{ "DEREF", FUNC_DEREF },
	{ "WINDOWS", FUNC_WINDOWS },
	{ "SERIES", FUNC_SERIES },
	{ "DOCUMENTS", FUNC_DOCUMENTS },
	{ "ACTIVECELL", FUNC_ACTIVECELL },
	{ "SELECTION", FUNC_SELECTION },
	{ "RESULT", FUNC_RESULT },
	{ "ATAN2", FUNC_ATAN2 },
	{ "ASIN", FUNC_ASIN },
	{ "ACOS", FUNC_ACOS },
	{ "CHOOSE", FUNC_CHOOSE },
	{ "HLOOKUP", FUNC_HLOOKUP },
	{ "VLOOKUP", FUNC_VLOOKUP },
	{ "LINKS", FUNC_LINKS },
	{ "INPUT", FUNC_INPUT },
	{ "ISREF", FUNC_ISREF },
	{ "GETFORMULA", FUNC_GETFORMULA },
	{ "GETNAME", FUNC_GETNAME },
	{ "SETVALUE", FUNC_SETVALUE },
	{ "LOG", FUNC_LOG },
	{ "EXEC", FUNC_EXEC },
	{ "CHAR", FUNC_CHAR },
	{ "LOWER", FUNC_LOWER },
	{ "UPPER", FUNC_UPPER },
	{ "PROPER", FUNC_PROPER },
	{ "LEFT", FUNC_LEFT },
	{ "RIGHT", FUNC_RIGHT },
	{ "EXACT", FUNC_EXACT },
	{ "TRIM", FUNC_TRIM },
	{ "REPLACE", FUNC_REPLACE },
	{ "SUBSTITUTE", FUNC_SUBSTITUTE },
	{ "CODE", FUNC_CODE },
	{ "NAMES", FUNC_NAMES },
	{ "DIRECTORY", FUNC_DIRECTORY },
	{ "FIND", FUNC_FIND },
	{ "CELL", FUNC_CELL },
	{ "ISERR", FUNC_ISERR },
	{ "ISTEXT", FUNC_ISTEXT },
	{ "ISNUMBER", FUNC_ISNUMBER },
	{ "ISBLANK", FUNC_ISBLANK },
	{ "T", FUNC_T },
	{ "N", FUNC_N },
	{ "FOPEN", FUNC_FOPEN },
	{ "FCLOSE", FUNC_FCLOSE },
	{ "FSIZE", FUNC_FSIZE },
	{ "FREADLN", FUNC_FREADLN },
	{ "FREAD", FUNC_FREAD },
	{ "FWRITELN", FUNC_FWRITELN },
	{ "FWRITE", FUNC_FWRITE },
	{ "FPOS", FUNC_FPOS },
	{ "DATEVALUE", FUNC_DATEVALUE },
	{ "TIMEVALUE", FUNC_TIMEVALUE },
	{ "SLN", FUNC_SLN },
	{ "SYD", FUNC_SYD },
	{ "DDB", FUNC_DDB },
	{ "GETDEF", FUNC_GETDEF },
	{ "REFTEXT", FUNC_REFTEXT },
	{ "TEXTREF", FUNC_TEXTREF },
	{ "INDIRECT", FUNC_INDIRECT },
	{ "REGISTER", FUNC_REGISTER },
	{ "CALL", FUNC_CALL },
	{ "ADDBAR", FUNC_ADDBAR },
	{ "ADDMENU", FUNC_ADDMENU },
	{ "ADDCOMMAND", FUNC_ADDCOMMAND },
	{ "ENABLECOMMAND", FUNC_ENABLECOMMAND },
	{ "CHECKCOMMAND", FUNC_CHECKCOMMAND },
	{ "RENAMECOMMAND", FUNC_RENAMECOMMAND },
	{ "SHOWBAR", FUNC_SHOWBAR },
	{ "DELETEMENU", FUNC_DELETEMENU },
	{ "DELETECOMMAND", FUNC_DELETECOMMAND },
	{ "GETCHARTITEM", FUNC_GETCHARTITEM },
	{ "DIALOGBOX", FUNC_DIALOGBOX },
	{ "CLEAN", FUNC_CLEAN },
	{ "MDETERM", FUNC_MDETERM },
	{ "MINVERSE", FUNC_MINVERSE },
	{ "MMULT", FUNC_MMULT },
	{ "FILES", FUNC_FILES },
	{ "IPMT", FUNC_IPMT },
	{ "PPMT", FUNC_PPMT },
	{ "COUNTA", FUNC_COUNTA },
	{ "CANCELKEY", FUNC_CANCELKEY },
	{ "INITIATE", FUNC_INITIATE },
	{ "REQUEST", FUNC_REQUEST },
	{ "POKE", FUNC_POKE },
	{ "EXECUTE", FUNC_EXECUTE },
	{ "TERMINATE", FUNC_TERMINATE },
	{ "RESTART", FUNC_RESTART },
	{ "HELP", FUNC_HELP },
	{ "GETBAR", FUNC_GETBAR },
	{ "PRODUCT", FUNC_PRODUCT },
	{ "FACT", FUNC_FACT },
	{ "GETCELL", FUNC_GETCELL },
	{ "GETWORKSPACE", FUNC_GETWORKSPACE },
	{ "GETWINDOW", FUNC_GETWINDOW },
	{ "GETDOCUMENT", FUNC_GETDOCUMENT },
	{ "DPRODUCT", FUNC_DPRODUCT },
	{ "ISNONTEXT", FUNC_ISNONTEXT },
	{ "GETNOTE", FUNC_GETNOTE },
	{ "NOTE", FUNC_NOTE },
	{ "STDEVP", FUNC_STDEVP },
	{ "VARP", FUNC_VARP },
	{ "DSTDEVP", FUNC_DSTDEVP },
	{ "DVARP", FUNC_DVARP },
	{ "TRUNC", FUNC_TRUNC },
	{ "ISLOGICAL", FUNC_ISLOGICAL },
	{ "DCOUNTA", FUNC_DCOUNTA },
	{ "DELETEBAR", FUNC_DELETEBAR },
	{ "UNREGISTER", FUNC_UNREGISTER },
	{ "USDOLLAR", FUNC_USDOLLAR },
	{ "FINDB", FUNC_FINDB },
	{ "SEARCHB", FUNC_SEARCHB },
	{ "REPLACEB", FUNC_REPLACEB },
	{ "LEFTB", FUNC_LEFTB },
	{ "RIGHTB", FUNC_RIGHTB },
	{ "MIDB", FUNC_MIDB },
	{ "LENB", FUNC_LENB },
	{ "ROUNDUP", FUNC_ROUNDUP },
	{ "ROUNDDOWN", FUNC_ROUNDDOWN },
	{ "ASC", FUNC_ASC },
	{ "DBCS", FUNC_DBCS },
	{ "RANK", FUNC_RANK },
	{ "ADDRESS", FUNC_ADDRESS },
	{ "DAYS360", FUNC_DAYS360 },
	{ "TODAY", FUNC_TODAY },
	{ "VDB", FUNC_VDB },
	{ "MEDIAN", FUNC_MEDIAN },
	{ "SUMPRODUCT", FUNC_SUMPRODUCT },
	{ "SINH", FUNC_SINH },
	{ "COSH", FUNC_COSH },
	{ "TANH", FUNC_TANH },
	{ "ASINH", FUNC_ASINH },
	{ "ACOSH", FUNC_ACOSH },
	{ "ATANH", FUNC_ATANH },
	{ "DGET", FUNC_DGET },
	{ "CREATEOBJECT", FUNC_CREATEOBJECT },
	{ "VOLATILE", FUNC_VOLATILE },
	{ "LASTERROR", FUNC_LASTERROR },
	{ "CUSTOMUNDO", FUNC_CUSTOMUNDO },
	{ "CUSTOMREPEAT", FUNC_CUSTOMREPEAT },
	{ "FORMULACONVERT", FUNC_FORMULACONVERT },
	{ "GETLINKINFO", FUNC_GETLINKINFO },
	{ "TEXTBOX", FUNC_TEXTBOX },
	{ "INFO", FUNC_INFO },
	{ "GROUP", FUNC_GROUP },
	{ "GETOBJECT", FUNC_GETOBJECT },
	{ "DB", FUNC_DB },
	{ "PAUSE", FUNC_PAUSE },
	{ "RESUME", FUNC_RESUME },
	{ "FREQUENCY", FUNC_FREQUENCY },
	{ "ADDTOOLBAR", FUNC_ADDTOOLBAR },
	{ "DELETETOOLBAR", FUNC_DELETETOOLBAR },
	{ "RESETTOOLBAR", FUNC_RESETTOOLBAR },
	{ "EVALUATE", FUNC_EVALUATE },
	{ "GETTOOLBAR", FUNC_GETTOOLBAR },
	{ "GETTOOL", FUNC_GETTOOL },
	{ "SPELLINGCHECK", FUNC_SPELLINGCHECK },
	{ "ERRORTYPE", FUNC_ERRORTYPE },
	{ "APPTITLE", FUNC_APPTITLE },
	{ "WINDOWTITLE", FUNC_WINDOWTITLE },
	{ "SAVETOOLBAR", FUNC_SAVETOOLBAR },
	{ "ENABLETOOL", FUNC_ENABLETOOL },
	{ "PRESSTOOL", FUNC_PRESSTOOL },
	{ "REGISTERID", FUNC_REGISTERID },
	{ "GETWORKBOOK", FUNC_GETWORKBOOK },
	{ "AVEDEV", FUNC_AVEDEV },
	{ "BETADIST", FUNC_BETADIST },
	{ "GAMMALN", FUNC_GAMMALN },
	{ "BETAINV", FUNC_BETAINV },
	{ "BINOMDIST", FUNC_BINOMDIST },
	{ "CHIDIST", FUNC_CHIDIST },
	{ "CHIINV", FUNC_CHIINV },
	{ "COMBIN", FUNC_COMBIN },
	{ "CONFIDENCE", FUNC_CONFIDENCE },
	{ "CRITBINOM", FUNC_CRITBINOM },
	{ "EVEN", FUNC_EVEN },
	{ "EXPONDIST", FUNC_EXPONDIST },
	{ "FDIST", FUNC_FDIST },
	{ "FINV", FUNC_FINV },
	{ "FISHER", FUNC_FISHER },
	{ "FISHERINV", FUNC_FISHERINV },
	{ "FLOOR", FUNC_FLOOR },
	{ "GAMMADIST", FUNC_GAMMADIST },
	{ "GAMMAINV", FUNC_GAMMAINV },
	{ "CEILING", FUNC_CEILING },
	{ "HYPGEOMDIST", FUNC_HYPGEOMDIST },
	{ "LOGNORMDIST", FUNC_LOGNORMDIST },
	{ "LOGINV", FUNC_LOGINV },
	{ "NEGBINOMDIST", FUNC_NEGBINOMDIST },
	{ "NORMDIST", FUNC_NORMDIST },
	{ "NORMSDIST", FUNC_NORMSDIST },
	{ "NORMINV", FUNC_NORMINV },
	{ "NORMSINV", FUNC_NORMSINV },
	{ "STANDARDIZE", FUNC_STANDARDIZE },
	{ "ODD", FUNC_ODD },
	{ "PERMUT", FUNC_PERMUT },
	{ "POISSON", FUNC_POISSON },
	{ "TDIST", FUNC_TDIST },
	{ "WEIBULL", FUNC_WEIBULL },
	{ "SUMXMY2", FUNC_SUMXMY2 },
	{ "SUMX2MY2", FUNC_SUMX2MY2 },
	{ "SUMX2PY2", FUNC_SUMX2PY2 },
	{ "CHITEST", FUNC_CHITEST },
	{ "CORREL", FUNC_CORREL },
	{ "COVAR", FUNC_COVAR },
	{ "FORECAST", FUNC_FORECAST },
	{ "FTEST", FUNC_FTEST },
	{ "INTERCEPT", FUNC_INTERCEPT },
	{ "PEARSON", FUNC_PEARSON },
	{ "RSQ", FUNC_RSQ },
	{ "STEYX", FUNC_STEYX },
	{ "SLOPE", FUNC_SLOPE },
	{ "TTEST", FUNC_TTEST },
	{ "PROB", FUNC_PROB },
	{ "DEVSQ", FUNC_DEVSQ },
	{ "GEOMEAN", FUNC_GEOMEAN },
	{ "HARMEAN", FUNC_HARMEAN },
	{ "SUMSQ", FUNC_SUMSQ },
	{ "KURT", FUNC_KURT },
	{ "SKEW", FUNC_SKEW },
	{ "ZTEST", FUNC_ZTEST },
	{ "LARGE", FUNC_LARGE },
	{ "SMALL", FUNC_SMALL },
	{ "QUARTILE", FUNC_QUARTILE },
	{ "PERCENTILE", FUNC_PERCENTILE },
	{ "PERCENTRANK", FUNC_PERCENTRANK },
	{ "MODE", FUNC_MODE },
	{ "TRIMMEAN", FUNC_TRIMMEAN },
	{ "TINV", FUNC_TINV },
	{ "MOVIECOMMAND", FUNC_MOVIECOMMAND },
	{ "GETMOVIE", FUNC_GETMOVIE },
	{ "CONCATENATE", FUNC_CONCATENATE },
	{ "POWER", FUNC_POWER },
	{ "PIVOTADDDATA", FUNC_PIVOTADDDATA },
	{ "GETPIVOTTABLE", FUNC_GETPIVOTTABLE },
	{ "GETPIVOTFIELD", FUNC_GETPIVOTFIELD },
	{ "GETPIVOTITEM", FUNC_GETPIVOTITEM },
	{ "RADIANS", FUNC_RADIANS },
	{ "DEGREES", FUNC_DEGREES },
	{ "SUBTOTAL", FUNC_SUBTOTAL },
	{ "SUMIF", FUNC_SUMIF },
	{ "COUNTIF", FUNC_COUNTIF },
	{ "COUNTBLANK", FUNC_COUNTBLANK },
	{ "SCENARIOGET", FUNC_SCENARIOGET },
	{ "OPTIONSLISTSGET", FUNC_OPTIONSLISTSGET },
	{ "ISPMT", FUNC_ISPMT },
	{ "DATEDIF", FUNC_DATEDIF },
	{ "DATESTRING", FUNC_DATESTRING },
	{ "NUMBERSTRING", FUNC_NUMBERSTRING },
	{ "ROMAN", FUNC_ROMAN },
	{ "OPENDIALOG", FUNC_OPENDIALOG },
	{ "SAVEDIALOG", FUNC_SAVEDIALOG },
	{ "VIEWGET", FUNC_VIEWGET },
	{ "GETPIVOTDATA", FUNC_GETPIVOTDATA },
	{ "HYPERLINK", FUNC_HYPERLINK },
	{ "PHONETIC", FUNC_PHONETIC },
	{ "AVERAGEA", FUNC_AVERAGEA },
	{ "MAXA", FUNC_MAXA },
	{ "MINA", FUNC_MINA },
	{ "STDEVPA", FUNC_STDEVPA },
	{ "VARPA", FUNC_VARPA },
	{ "STDEVA", FUNC_STDEVA },
	{ "VARA", FUNC_VARA },
	{ "BAHTTEXT", FUNC_BAHTTEXT },
	{ "THAIDAYOFWEEK", FUNC_THAIDAYOFWEEK },
	{ "THAIDIGIT", FUNC_THAIDIGIT },
	{ "THAIMONTHOFYEAR", FUNC_THAIMONTHOFYEAR },
	{ "THAINUMSOUND", FUNC_THAINUMSOUND },
	{ "THAINUMSTRING", FUNC_THAINUMSTRING },
	{ "THAISTRINGLENGTH", FUNC_THAISTRINGLENGTH },
	{ "ISTHAIDIGIT", FUNC_ISTHAIDIGIT },
	{ "ROUNDBAHTDOWN", FUNC_ROUNDBAHTDOWN },
	{ "ROUNDBAHTUP", FUNC_ROUNDBAHTUP },
	{ "THAIYEAR", FUNC_THAIYEAR },
	{ "RTD", FUNC_RTD },
	{ "CUBEVALUE", FUNC_CUBEVALUE },
	{ "CUBEMEMBER", FUNC_CUBEMEMBER },
	{ "CUBEMEMBERPROPERTY", FUNC_CUBEMEMBERPROPERTY },
	{ "CUBERANKEDMEMBER", FUNC_CUBERANKEDMEMBER },
	{ "HEX2BIN", FUNC_HEX2BIN },
	{ "HEX2DEC", FUNC_HEX2DEC },
	{ "HEX2OCT", FUNC_HEX2OCT },
	{ "DEC2BIN", FUNC_DEC2BIN },
	{ "DEC2HEX", FUNC_DEC2HEX },
	{ "DEC2OCT", FUNC_DEC2OCT },
	{ "OCT2BIN", FUNC_OCT2BIN },
	{ "OCT2HEX", FUNC_OCT2HEX },
	{ "OCT2DEC", FUNC_OCT2DEC },
	{ "BIN2DEC", FUNC_BIN2DEC },
	{ "BIN2OCT", FUNC_BIN2OCT },
	{ "BIN2HEX", FUNC_BIN2HEX },
	{ "IMSUB", FUNC_IMSUB },
	{ "IMDIV", FUNC_IMDIV },
	{ "IMPOWER", FUNC_IMPOWER },
	{ "IMABS", FUNC_IMABS },
	{ "IMSQRT", FUNC_IMSQRT },
	{ "IMLN", FUNC_IMLN },
	{ "IMLOG2", FUNC_IMLOG2 },
	{ "IMLOG10", FUNC_IMLOG10 },
	{ "IMSIN", FUNC_IMSIN },
	{ "IMCOS", FUNC_IMCOS },
	{ "IMEXP", FUNC_IMEXP },
	{ "IMARGUMENT", FUNC_IMARGUMENT },
	{ "IMCONJUGATE", FUNC_IMCONJUGATE },
	{ "IMAGINARY", FUNC_IMAGINARY },
	{ "IMREAL", FUNC_IMREAL },
	{ "COMPLEX", FUNC_COMPLEX },
	{ "IMSUM", FUNC_IMSUM },
	{ "IMPRODUCT", FUNC_IMPRODUCT },
	{ "SERIESSUM", FUNC_SERIESSUM },
	{ "FACTDOUBLE", FUNC_FACTDOUBLE },
	{ "SQRTPI", FUNC_SQRTPI },
	{ "QUOTIENT", FUNC_QUOTIENT },
	{ "DELTA", FUNC_DELTA },
	{ "GESTEP", FUNC_GESTEP },
	{ "ISEVEN", FUNC_ISEVEN },
	{ "ISODD", FUNC_ISODD },
	{ "MROUND", FUNC_MROUND },
	{ "ERF", FUNC_ERF },
	{ "ERFC", FUNC_ERFC },
	{ "BESSELJ", FUNC_BESSELJ },
	{ "BESSELK", FUNC_BESSELK },
	{ "BESSELY", FUNC_BESSELY },
	{ "BESSELI", FUNC_BESSELI },
	{ "XIRR", FUNC_XIRR },
	{ "XNPV", FUNC_XNPV },
	{ "PRICEMAT", FUNC_PRICEMAT },
	{ "YIELDMAT", FUNC_YIELDMAT },
	{ "INTRATE", FUNC_INTRATE },
	{ "RECEIVED", FUNC_RECEIVED },
	{ "DISC", FUNC_DISC },
	{ "PRICEDISC", FUNC_PRICEDISC },
	{ "YIELDDISC", FUNC_YIELDDISC },
	{ "TBILLEQ", FUNC_TBILLEQ },
	{ "TBILLPRICE", FUNC_TBILLPRICE },
	{ "TBILLYIELD", FUNC_TBILLYIELD },
	{ "PRICE", FUNC_PRICE },
	{ "YIELD", FUNC_YIELD },
	{ "DOLLARDE", FUNC_DOLLARDE },
	{ "DOLLARFR", FUNC_DOLLARFR },
	{ "NOMINAL", FUNC_NOMINAL },
	{ "EFFECT", FUNC_EFFECT },
	{ "CUMPRINC", FUNC_CUMPRINC },
	{ "CUMIPMT", FUNC_CUMIPMT },
	{ "EDATE", FUNC_EDATE },
	{ "EOMONTH", FUNC_EOMONTH },
	{ "YEARFRAC", FUNC_YEARFRAC },
	{ "COUPDAYBS", FUNC_COUPDAYBS },
	{ "COUPDAYS", FUNC_COUPDAYS },
	{ "COUPDAYSNC", FUNC_COUPDAYSNC },
	{ "COUPNCD", FUNC_COUPNCD },
	{ "COUPNUM", FUNC_COUPNUM },
	{ "COUPPCD", FUNC_COUPPCD },
	{ "DURATION", FUNC_DURATION },
	{ "MDURATION", FUNC_MDURATION },
	{ "ODDLPRICE", FUNC_ODDLPRICE },
	{ "ODDLYIELD", FUNC_ODDLYIELD },
	{ "ODDFPRICE", FUNC_ODDFPRICE },
	{ "ODDFYIELD", FUNC_ODDFYIELD },
	{ "RANDBETWEEN", FUNC_RANDBETWEEN },
	{ "WEEKNUM", FUNC_WEEKNUM },
	{ "AMORDEGRC", FUNC_AMORDEGRC },
	{ "AMORLINC", FUNC_AMORLINC },
	{ "CONVERT", FUNC_CONVERT },
	{ "ACCRINT", FUNC_ACCRINT },
	{ "ACCRINTM", FUNC_ACCRINTM },
	{ "WORKDAY", FUNC_WORKDAY },
	{ "NETWORKDAYS", FUNC_NETWORKDAYS },
	{ "GCD", FUNC_GCD },
	{ "MULTINOMIAL", FUNC_MULTINOMIAL },
	{ "LCM", FUNC_LCM },
	{ "FVSCHEDULE", FUNC_FVSCHEDULE },
	{ "CUBEKPIMEMBER", FUNC_CUBEKPIMEMBER },
	{ "CUBESET", FUNC_CUBESET },
	{ "CUBESETCOUNT", FUNC_CUBESETCOUNT },
	{ "IFERROR", FUNC_IFERROR },
	{ "COUNTIFS", FUNC_COUNTIFS },
	{ "SUMIFS", FUNC_SUMIFS },
	{ "AVERAGEIF", FUNC_AVERAGEIF },
	{ "AVERAGEIFS", FUNC_AVERAGEIFS },
	{ "AGGREGATE", FUNC_AGGREGATE },
	{ "BINOM_DIST", FUNC_BINOM_DIST },
	{ "BINOM_INV", FUNC_BINOM_INV },
	{ "CONFIDENCE_NORM", FUNC_CONFIDENCE_NORM },
	{ "CONFIDENCE_T", FUNC_CONFIDENCE_T },
	{ "CHISQ_TEST", FUNC_CHISQ_TEST },
	{ "F_TEST", FUNC_F_TEST },
	{ "COVARIANCE_P", FUNC_COVARIANCE_P },
	{ "COVARIANCE_S", FUNC_COVARIANCE_S },
	{ "EXPON_DIST", FUNC_EXPON_DIST },
	{ "GAMMA_DIST", FUNC_GAMMA_DIST },
	{ "GAMMA_INV", FUNC_GAMMA_INV },
	{ "MODE_MULT", FUNC_MODE_MULT },
	{ "MODE_SNGL", FUNC_MODE_SNGL },
	{ "NORM_DIST", FUNC_NORM_DIST },
	{ "NORM_INV", FUNC_NORM_INV },
	{ "PERCENTILE_EXC", FUNC_PERCENTILE_EXC },
	{ "PERCENTILE_INC", FUNC_PERCENTILE_INC },
	{ "PERCENTRANK_EXC", FUNC_PERCENTRANK_EXC },
	{ "PERCENTRANK_INC", FUNC_PERCENTRANK_INC },
	{ "POISSON_DIST", FUNC_POISSON_DIST },
	{ "QUARTILE_EXC", FUNC_QUARTILE_EXC },
	{ "QUARTILE_INC", FUNC_QUARTILE_INC },
	{ "RANK_AVG", FUNC_RANK_AVG },
	{ "RANK_EQ", FUNC_RANK_EQ },
	{ "STDEV_S", FUNC_STDEV_S },
	{ "STDEV_P", FUNC_STDEV_P },
	{ "T_DIST", FUNC_T_DIST },
	{ "T_DIST_2T", FUNC_T_DIST_2T },
	{ "T_DIST_RT", FUNC_T_DIST_RT },
	{ "T_INV", FUNC_T_INV },
	{ "T_INV_2T", FUNC_T_INV_2T },
	{ "VAR_S", FUNC_VAR_S },
	{ "VAR_P", FUNC_VAR_P },
	{ "WEIBULL_DIST", FUNC_WEIBULL_DIST },
	{ "NETWORKDAYS_INTL", FUNC_NETWORKDAYS_INTL },
	{ "WORKDAY_INTL", FUNC_WORKDAY_INTL },
	{ "ECMA_CEILING", FUNC_ECMA_CEILING },
	{ "ISO_CEILING", FUNC_ISO_CEILING },
	{ "BETA_DIST", FUNC_BETA_DIST },
	{ "BETA_INV", FUNC_BETA_INV },
	{ "CHISQ_DIST", FUNC_CHISQ_DIST },
	{ "CHISQ_DIST_RT", FUNC_CHISQ_DIST_RT },
	{ "CHISQ_INV", FUNC_CHISQ_INV },
	{ "CHISQ_INV_RT", FUNC_CHISQ_INV_RT },
	{ "F_DIST", FUNC_F_DIST },
	{ "F_DIST_RT", FUNC_F_DIST_RT },
	{ "F_INV", FUNC_F_INV },
	{ "F_INV_RT", FUNC_F_INV_RT },
	{ "HYPGEOM_DIST", FUNC_HYPGEOM_DIST },
	{ "LOGNORM_DIST", FUNC_LOGNORM_DIST },
	{ "LOGNORM_INV", FUNC_LOGNORM_INV },
	{ "NEGBINOM_DIST", FUNC_NEGBINOM_DIST },
	{ "NORM_S_DIST", FUNC_NORM_S_DIST },
	{ "NORM_S_INV", FUNC_NORM_S_INV },
	{ "T_TEST", FUNC_T_TEST },
	{ "Z_TEST", FUNC_Z_TEST },
	{ "ERF_PRECISE", FUNC_ERF_PRECISE },
	{ "ERFC_PRECISE", FUNC_ERFC_PRECISE },
	{ "GAMMALN_PRECISE", FUNC_GAMMALN_PRECISE },
	{ "CEILING_PRECISE", FUNC_CEILING_PRECISE },
	{ "FLOOR_PRECISE", FUNC_FLOOR_PRECISE },
};


char *FormulaFunctionsTest(const char *md5_checksum)
{
	workbook wb;
	worksheet* sh[4];
	
	sh[0] = wb.sheet("2003 and before");
	sh[1] = wb.sheet("2007");
	sh[2] = wb.sheet("2010");
	sh[3] = wb.sheet("Formulas_04");

	const unsigned32_t number_of_builtin_functions = sizeof(function_arr) / sizeof(function_arr[0]);

	unsigned32_t row = 4;
	worksheet* cur_sh = sh[0];
	for (unsigned32_t r = 0; r < number_of_builtin_functions; r++)
	{
		expr_function_code_t fn = function_arr[r].code;
		if (FUNC_BAHTTEXT == fn)
		{
			cur_sh = sh[1];
			row = 4;
#if 01 /* set to 0 to create an XLS which includes Excel 2007/2010 functions in the sample formulas */
			break;
#endif
		}
		else if (FUNC_AGGREGATE == fn)
		{
			cur_sh = sh[2];
			row = 4;
		}

		expression_node_factory_t& maker = wb.GetFormulaFactory();

		unsigned16_t argmask = NumberOfArgsForExcelFunction(fn);
		if (argmask != 0 && !(argmask & 0x8000U) /* A_MACRO */) 
		{
			if (argmask & 0x0001)
			{
				expression_node_t *z_ary_root = maker.f(fn);
				cur_sh->formula(row, 1, z_ary_root, true); 
			}
			if (argmask & 0x0002)
			{
				expression_node_t *unary_root = maker.f(fn, maker.floating_point(1.002));
				cur_sh->formula(row, 2, unary_root, true); 
			}
			if (argmask & 0x0004)
			{
				expression_node_t *binary_root = maker.f(fn, maker.floating_point(1.002), maker.floating_point(2.075));
				cur_sh->formula(row, 3, binary_root, true); 
			}

			size_t argcnt;
			for (argcnt = 3; argcnt < 12; argcnt++)
			{
				if (argmask & (1U << argcnt))
				{
					n_ary_func_node_t *n_ary_root = maker.f(fn, argcnt, NULL);

					int a;
					for (a = 0; a < (int)argcnt; a++)
					{
						expression_node_t *num = maker.integer(a + 1);
						n_ary_root->PushArg(num);
					}
					cur_sh->formula(row, argcnt + 1, n_ary_root, true); 
				}
			}
		}

		cur_sh->label(row, 0, function_arr[r].name);

		row++;
	}

	for (unsigned32_t i = 0; i < 4; i++)
	{
		sh[i]->label(1, 0, "function name");
		sh[i]->label(0, 1, "argument count");
		for (size_t argcnt = 0; argcnt < 12; argcnt++)
		{
			sh[i]->number(1, (unsigned32_t)(1 + argcnt), (signed32_t)argcnt);
		}
	}

	int err = wb.Dump("formulas.xls");

    char *checkP = file_err;
	if (err != NO_ERRORS)
	{
		cerr << "FormulaFunctionsTest failed: I/O failure: " << err << std::endl;
		return checkP;
	}
	if ((checkP = check_file("formulas.xls", md5_checksum)))
	{
		cerr << "FormulaFunctionsTest failed: MD5 of generated XLS mismatch or I/O failure." << std::endl;
	}
	return checkP;
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

