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
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <wchar.h>
#include <sys/types.h>

wchar_t foo;

#include <xlslib.h>

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
