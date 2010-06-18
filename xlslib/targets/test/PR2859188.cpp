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

It's a question what cell_t::format() is needed for, or how to create format without setting it globaly (reminding that format_t ctor is private).

So either I do not know something, it's a specific design decision or a bug in workbook::format(). Or a mix of all of that.
*/

#define RANGE_FEATURE
#include <xlslib.h>

using namespace xlslib_core;


int test1(void) 
{
	workbook book;
	worksheet* sheet = book.sheet("test1");
	sheet->number(0, 1, 40065.0, FMT_DATE1, 0)->format(book.format("yyyy-mm-dd"));
	book.Dump("PR2859188-1.xls");

	return 0;
}

int test2(void) 
{
	/* both cells formatted as date ??? */
	workbook book;
	worksheet* sheet = book.sheet("test");
	sheet->number(0, 0, 3.14);
	sheet->number(0, 1, 40065.0, FMT_DATE1, 0)->format(book.format("yyyy-mm-dd"));
	book.Dump("PR2859188-2.xls");

	return 0;
}

int test3(void) 
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

	book.Dump("PR2859188-3.xls");

	return 0;
}

int main(int argc, char *argv[]) 
{
	test1();
	test2();
	test3();

	return 0;
}

