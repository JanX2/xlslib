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
 * Copyright 2010 Ger Hobbelt
 *  
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * File description:
 *
 *
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <xlsys.h>
#include <systype.h>
#include <tostr.h>


static void exception_throwing_assertion_reporter(const char *expr, const char *filename, int lineno, const char *funcname)
{
	str_stream s;

	s << "Assertion failed: ";
	s << (expr ? expr : "???");
	s << " at line ";
	s << lineno;
	if (funcname)
	{
		s << " (" << funcname << ")";
	}
	if (filename)
	{
		s << " in " << filename;
	}
	else
	{
		s << " in [unidentified source file]";
	}
	throw std::string(s);
}


extern "C"
{

static void xlslib_default_assertion_reporter(const char *expr, const char *fname, int lineno, const char *funcname)
{
	exception_throwing_assertion_reporter(expr, fname, lineno, funcname);
}


static xlslib_userdef_assertion_reporter *callback = &xlslib_default_assertion_reporter;


void xlslib_report_failed_assertion(const char *expr, const char *fname, int lineno, const char *funcname)
{
	if (callback)
	{
		callback(expr, fname, lineno, funcname);
	}
}

void xlslib_register_assert_reporter(xlslib_userdef_assertion_reporter *user_func)
{
	callback = user_func;
}

};


