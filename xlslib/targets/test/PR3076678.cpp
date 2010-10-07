/*
 * SF issue #3083003 (! NOT 3076678 !)
*/

//#define RANGE_FEATURE
#include <xlslib.h>

#include <iostream>
#include <fstream>

#include "md5.h"

using namespace std;
using namespace xlslib_core;



/*
https://sourceforge.net/projects/xlslib/forums/forum/402380/topic/3879738
By: john443

I have the following problem: If I set a cell border to anything else than
BORDER_NONE, then in Excel the "Format Cell" dialog cannot be opened for that
cell. Is this a bug in Excel, in xlslib or is there something special I have
to take care of? I observed that behaviour in Excel 2002, 2003 and 2010. The
problem does not occur in Calc.
*/
int BorderTest(const char *md5_checksum)
{
	workbook wb;

	worksheet* sh = wb.sheet("Test1");
	cell_t * pCell = sh->label(1, 1, "Test border; Excel should be able to open the Format Cell dialog...");
	pCell->borderstyle(BORDER_LEFT, BORDER_THIN);

	sh = wb.sheet("Test2");
	xf_t * pxf = wb.xformat();
	pxf->SetBorderStyle(BORDER_LEFT, BORDER_THIN);
	pxf->SetLocked(false);
	pCell = sh->label(1, 1, "Test #2", pxf);

	// PR3082021: rowheight doesn't work [for empty rows, and then the next ones that /are/ filled also miss out on the rowheight]
	sh->rowheight(0, 30);
	sh->rowheight(1, 76);		// in points (Excel uses twips, 1/20th of a point, but we dont)
	
	int err = wb.Dump("PR3083003.xls");
	// test multiple Dump() calls: PR3083160
	err |= wb.Dump("PR3083003-2.xls");

	if (err != NO_ERRORS)
	{
		cerr << "BorderTest failed: I/O failure: " << err << std::endl;
		return -1;
	}
	if (0 != check_file("PR3083003.xls", md5_checksum))
	{
		cerr << "BorderTest failed: MD5 of generated XLS mismatch or I/O failure." << std::endl;
		return -1;
	}
	if (0 != check_file("PR3083003-2.xls", md5_checksum))
	{
		cerr << "BorderTest failed: MD5 of generated XLS mismatch or I/O failure." << std::endl;
		return -1;
	}
	return 0;
}



int main(int argc, char *argv[]) 
{
	int rv = 0;

	try
	{
		rv |= BorderTest("52c4432092351283d09453bdf13847ab");
	}
	catch (std::string &errmsg)
	{
		std::cerr << errmsg << std::endl;
		rv = 1;
	}
	
	return (rv == 0 ? EXIT_SUCCESS : EXIT_FAILURE);
}

