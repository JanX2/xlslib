/*
 * SF issue #2859188
 * 
 * 

I was needed to define specific format for date cell. So with help of intuition I did something like this:

workbook book;
worksheet* sheet = book.sheet("test");
sheet->number(0, 1, 40065.0, FMT_DATE1, 0)->format(book.format("yyyy-mm-dd"));
book.Dump("test.xls");

All looks OK. Now if i add some more columns, like this:

workbook book;
worksheet* sheet = book.sheet("test");
sheet->number(0, 0, 3.14);
sheet->number(0, 1, 40065.0, FMT_DATE1, 0)->format(book.format("yyyy-mm-dd"));
book.Dump("test.xls");

I got XLS file with both columns as date - format were set to all cells there.

Then I noticed that it's enough to use workbook::format() to set current format, so:

workbook book;
worksheet* sheet = book.sheet("test");
book.format("0.00");
sheet->number(0, 0, 1000);
sheet->number(0, 1, 3.14);

book.format("yyyy-mm-dd");
sheet->number(0, 2, 40065.0, FMT_DATE1, 0);

book.Dump("test.xls");

Does the trick, but it's inconvenient to explicitly set format cells if custom format were used at least (only) once. Also I should take attention that custom date format didn't override "3.14" format, as it did in earlier example.

It's a question what cell_t::format() is needed for, or how to create format without setting it globally (reminding that format_t ctor is private).

So either I do not know something, it's a specific design decision or a bug in workbook::format(). Or a mix of all of that.
*/

//#define RANGE_FEATURE
#include <xlslib.h>

#include <iostream>
#include <fstream>

#include "md5.h"

using namespace std;
using namespace xlslib_core;

static char file_err[] = "00000000000000000000000000000000";

#define NUM_TESTS	3

char check[NUM_TESTS][40];

char *test1(const char *md5_checksum) 
{
	workbook book;
	worksheet* sheet = book.sheet("test1");
	sheet->number(0, 1, 40065.0, FMT_DATE1, 0)->format(book.format("yyyy-mm-dd"));
	int err = book.Dump("PR2859188-1.xls");

	char *checkP = file_err;
	if (err != NO_ERRORS)
	{
		cerr << "test1 failed: I/O failure: " << err << std::endl;
		return checkP;
	}
	if ((checkP = check_file("PR2859188-1.xls", md5_checksum)))
	{
		cerr << "test1 failed: MD5 of generated XLS mismatch or I/O failure." << std::endl;
	}

	return checkP;
}

char *test2(const char *md5_checksum) 
{
	/* both cells formatted as date ??? */
	workbook book;
	worksheet* sheet = book.sheet("test");
	sheet->number(0, 0, 3.14);
	sheet->number(0, 1, 40065.0, FMT_DATE1, 0)->format(book.format("yyyy-mm-dd"));
	int err = book.Dump("PR2859188-2.xls");

	char *checkP = file_err;
	if (err != NO_ERRORS)
	{
		cerr << "test2 failed: I/O failure: " << err << std::endl;
	}
	if ((checkP = check_file("PR2859188-2.xls", md5_checksum)))
	{
		cerr << "test2 failed: MD5 of generated XLS mismatch or I/O failure." << std::endl;
	}

	return checkP;
}

char *test3(const char *md5_checksum) 
{
	workbook book;
	worksheet* sheet = book.sheet("test2");
	book.format("0.00"); /* this one will be lost to us for ever */
	sheet->number(0, 0, 1000); /* must be show as "1000" */
	sheet->number(0, 1, 3.14);

	book.format("yyyy-mm-dd"); /* this one will be lost to us for ever */
	sheet->number(0, 2, 40065.0, FMT_DATE1, 0); /* must be show as "d/m/y" or whatever the local default date format is */

	format_t *datefmt = book.format("yyyy-mm-dd");
	sheet->number(0, 3, 40066.0, FMT_GENERAL, 0)->format(datefmt); /* must be show in custom date format */

	int err = book.Dump("PR2859188-3.xls");

	char *checkP = file_err;
	if (err != NO_ERRORS)
	{
		cerr << "test3 failed: I/O failure: " << err << std::endl;
	}
	if ((checkP = check_file("PR2859188-3.xls", md5_checksum)))
	{
		cerr << "test3 failed: MD5 of generated XLS mismatch or I/O failure." << std::endl;
	}

	return checkP;
}

int main(int argc, char *argv[]) 
{
	int rv = 0;
	
#ifdef _X_DEBUG_
	// Used for internal testing
	if(argc == 2) {
		int ret = chdir(argv[1]);
		XL_ASSERT(!ret);
	}
#endif
	{
		FILE *fp = fopen("PR2859188.md5", "r");
		for(int i=0; i<NUM_TESTS; ++i) {
			char *checkP = check[i];
			if(fp) {
				fscanf(fp, "%s", checkP);
			} else {
				strcpy(checkP, "00000000000000000000000000000000");
			}
			printf("MD5 = %s\n", checkP);
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
			if((checkP = test1(checkP)))
			{
				rv |= failed;
#ifdef _X_DEBUG_
				strcpy(check[idx], checkP);
#endif
			}
		}
#endif
		failed <<= 1, ++idx;

#if 1
		{
			checkP = check[idx];
			if((checkP = test2(checkP)))
			{
				rv |= failed;
#ifdef _X_DEBUG_
				strcpy(check[idx], checkP);
#endif
			}
		}
#endif
		failed <<= 1, ++idx;

#if 1
		{
			checkP = check[idx];
			if((checkP = test3(checkP)))
			{
				rv |= failed;
#ifdef _X_DEBUG_
				strcpy(check[idx], checkP);
#endif
			}
		}
#endif
		failed <<= 1, ++idx;
	}
	catch (std::string &errmsg)
	{
		std::cerr << errmsg << std::endl;
		rv = 1;
	}

#ifdef _X_DEBUG_
		if(rv && argc == 2) {
			FILE *fp = fopen("PR2859188.md5", "w");
			if(fp) {
				for(int i=0; i<NUM_TESTS; ++i) {
					char *checkP = check[i];
					if(fp) {
						fprintf(fp, "%s\n", checkP);
					}
					printf("MD5 = %s\n", checkP);
				}
				fclose(fp);
			}
		}
#endif
	
	return (rv == 0 ? EXIT_SUCCESS : EXIT_FAILURE);
}

