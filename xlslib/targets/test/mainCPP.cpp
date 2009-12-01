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

#include <string>
#include <sstream>
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace xlslib_core;

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
   std::cerr<<str<<" "<<span_##id.GetUsedMilliseconds()<<" ms"<<std::endl

extern void StressTest(int a,int b,int c);
extern void RandomTest(int a,int b,int c);

extern void RandomCellAndFormatTest(int sheets_sz, int rows_sz, int cols_sz);
extern void RandomFormat(cell_t* cell, bool profile = false);
extern void RandomFontName(cell_t* cell, bool profile = false);
extern void RandomFontOption(cell_t* cell, bool profile = false);

extern void RandomFormatTest(int sheets_sz, int rows_sz, int cols_sz);

extern void RandomCellAndFormatTestProf(int sheets_sz, int rows_sz, int cols_sz);

extern void StandardTest(void);
extern int GetRndNumber(int max);

/*
*********************************
********************************* 
*/

static char tmp[256];


int main(int argc, char *argv[])
{

/*
   workbook wb;
   wb.sheet("Sheet_01");
   wb.Dump("blank.xls");
   exit(0);
 */

	// comment and uncomment the below to try various tests
	StandardTest();

	//if (argc > 1) /* [i_a] */
	{
		StressTest(3,100,100);
		StressTest(3,4,4);
		RandomTest(3,200,200);
		RandomCellAndFormatTest(1,15,10); //(1,15,10)
		RandomCellAndFormatTestProf(1,15,10);
		RandomFormatTest(1,15,10);
	}

	std::cerr << "Test finished" << std::endl;
	return 0;
}

void StandardTest(void)
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
   printf("limit: "); 
   char buf[100];
   gets(buf);
   int lim = atoi(buf);
#else
   int lim = 42;
#endif
   printf("\nlimit: %d\n", lim); // >= 14 --> error to load sheet #2; <= 13 = ok

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

   sh->number(130,128,1.0);
   sh->number(131,172,2.0);
   //sh->number(132,128,3.0);
   std::string s("This tab should should show two numbers in rows 130/131 at columns 128/172 respectively");
   sh->label(1,1,s);

   wb.Dump("./testCPP.xls");
}

int GetRndNumber(int max)
{
   int rndnum;
   rndnum = ((int)(rand()*((double)(max+1)/RAND_MAX)));
   return rndnum;
}

void RandomCellAndFormatTest(int sheets_sz, int rows_sz, int cols_sz)
{
   workbook wb;
   worksheet* sh;

   srand(time(0));

   TIMESPAN_START(1);

   for(int shnum = 0; shnum < sheets_sz; shnum++)
   {
	  sprintf(tmp, "DUH_%d", shnum);
	  string snamesheet(tmp);

      sh = wb.sheet(snamesheet);

      for(int row = 0; row<rows_sz; row++)
      {
         sh->rowheight(row,GetRndNumber(10)+15);
         for(int col = 0; col<cols_sz; col++)
         {
            sh->colwidth(row,GetRndNumber(10)+15);

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

   wb.Dump("rndcellandformat.xls");
   TIMESPAN_END(1,"Random Cell and Format test:");
}

void RandomCellAndFormatTestProf(int sheets_sz, int rows_sz, int cols_sz)
{
   workbook wb;
   worksheet* sh;

   for(int shnum = 0; shnum < sheets_sz; shnum++)
   {
	  sprintf(tmp, "DUH_%d", shnum);
	  string snamesheet(tmp);

      sh = wb.sheet(snamesheet);

      for(int row = 0; row<rows_sz; row++)
      {
         sh->rowheight(row,GetRndNumber(10)+15);
         for(int col = 0; col<cols_sz; col++)
         {
            sh->colwidth(row,GetRndNumber(10)+15);

            int rndcol = GetRndNumber(rows_sz);
            int rndrow = GetRndNumber(cols_sz);

			sprintf(tmp, "S%d:%d-%d#%d-%d", shnum, row+1, col+1, rndrow, rndcol);
			string snamelabel(tmp);

            cell_t* cell = sh->label(rndrow, rndcol, snamelabel);

#if defined(__DEBUG__) || defined(_DEBUG)
			cout<<"CELL ";
            cout.fill('0');
            cout.width(2);
            cout<<rndrow<<",";
            cout.fill('0');
            cout.width(2);
            cout<<rndcol<<": ";
#endif
			int k, fmtries = GetRndNumber(OPT_MAX);

#if defined(__DEBUG__) || defined(_DEBUG)
            cout<<endl<<"**FORMAT** ";
#endif
			for(k = 0; k<fmtries; k++)
               RandomFormat(cell,
#if defined(__DEBUG__) || defined(_DEBUG)
				true
#else
				false
#endif
			   );

#if defined(__DEBUG__) || defined(_DEBUG)
            cout<<endl<<"**FONT**   ";
#endif
			fmtries = GetRndNumber(OPT_FONTMAX);
            for(k = 0; k<fmtries; k++)
               RandomFontOption(cell,
#if defined(__DEBUG__) || defined(_DEBUG)
				true
#else
				false
#endif
				);

            RandomFontName(cell,
#if defined(__DEBUG__) || defined(_DEBUG)
				true
#else
				false
#endif
				);
#if defined(__DEBUG__) || defined(_DEBUG)
            cout<<endl;
#endif
		 }
      }
   }

   wb.Dump("rndcellandformat.xls");
}


void RandomFormatTest(int sheets_sz, int rows_sz, int cols_sz)
{

   srand(time(0));

   TIMESPAN_START(1);

      workbook wb;
   worksheet* sh;

   for(int shnum = 0; shnum < sheets_sz; shnum++)
   {
	  sprintf(tmp, "DUH_%d", shnum);
	  string snamesheet(tmp);

      sh = wb.sheet(snamesheet);

      for(int row = 0; row<rows_sz; row++)
      {
         sh->rowheight(row,GetRndNumber(10)+15);
         for(int col = 0; col<cols_sz; col++)
         {
            sh->colwidth(row,GetRndNumber(10)+15);

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

   wb.Dump("rndformat.xls");
   TIMESPAN_END(1,"Random Format test:");
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

void RandomFontName(cell_t* cell, bool profile)
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

void RandomFontOption(cell_t* cell, bool profile)
{
   switch(GetRndNumber(OPT_FONTMAX))
   {
      case OPT_FONTHEIGHT:
      {
         unsigned16_t height = (unsigned16_t)GetRndNumber(20*40);
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




void RandomFormat(cell_t* cell, bool profile)
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
void StressTest(int sheets_sz, int rows_sz, int cols_sz)
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

   TIMESPAN_START(1);
   swb.Dump("stress.xls");
   TIMESPAN_END(1,"Cell-stress test:");
}

/*
*********************************
*********************************
*/
void RandomTest(int sheets_sz, int rows_sz, int cols_sz)
{
   srand(time(0));
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

            rndcol = (int)(rand()*((double)cols_sz/RAND_MAX));
            rndrow = (int)(rand()*((double)rows_sz/RAND_MAX));

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
					ssh->number(rndrow,rndcol,(int)rand(),sxf1);
				}
				break;

			case 2: // double
				{
					ssh->number(rndrow,rndcol,rand()/(double)RAND_MAX,sxf1);
				}
				break;

			case 3: // blank
				{
					ssh->blank(rndrow,rndcol,sxf1);
				}
				break;

#if 0 // not yet here...
			case 4: // bool
				{
					ssh->blank(rndrow,rndcol,(rand() >= RAND_MAX/2), sxf1);
				}
				break;

			case 5: // err
				{
					ssh->blank(rndrow,rndcol,PickErrorCode(rand()), sxf1);
				}
				break;
#endif
			}
         }
	  }
   }

   swb.Dump("random.xls");
   TIMESPAN_END(1,"Random cell test:");
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

