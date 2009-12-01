/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright (C) 2004-2006 Yeico S. A. de C. V.
 * Copyright (C) 2008 David Hoerl
 *
 * $Source: /cvsroot/xlslib/xlslib/targets/test/mainC.c,v $
 * $Revision: 1.5 $
 * $Author: dhoerl $
 * $Date: 2009/03/08 21:17:45 $
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * File description:
 *
 *
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifdef HAVE_CONFIG_H
#include <xlconfig.h>
#elif defined(_MSC_VER) && defined(WIN32)
#include "ac-config.win32.h"
#endif


#include <stdio.h>
#include <config.h>
#ifdef HAVE_STDINT_H	// [i_a] 
#include <stdint.h>
#endif
#ifdef HAVE_STDBOOL_H	// [i_a] 
#include <stdbool.h>
#else
typedef enum
{
	false = 0,
	true = 1
} bool;
#endif
#include <wchar.h>
#include <sys/types.h>

#define RANGE_FEATURE
#include <xlslib.h>


wchar_t foo;


int main(int argc, char *argv[]) {
	workbook *w;
	worksheet *ws;
	int ret;

	w = xlsNewWorkbook();
	ws =  xlsWorkbookSheet(w, "xlslib C");

	xlsWorksheetNumberDbl(ws, (unsigned16_t)1, (unsigned16_t)1, 1.0, NULL);  
	xlsWorksheetNumberDbl(ws, (unsigned16_t)2, (unsigned16_t)1, 2.0, NULL);
	xlsWorksheetNumberDbl(ws, (unsigned16_t)3, (unsigned16_t)1, 3.0, NULL);
	ret =  xlsWorkbookDump(w, "testC.xls");

	printf("saved it ret=%d!\n", ret);
	xlsDeleteWorkbook(w);

	return 0;
}
