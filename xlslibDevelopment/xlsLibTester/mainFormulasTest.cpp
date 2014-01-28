//
//  mainFormulasTest.cpp
//  xlsLibTester
//
//  Created by David Hoerl on 1/28/14.
//
//

#include "xlslib.h"

#include "common/timespan.h"

#include "md5.h"

#include <string>
#include <sstream>
#include <iostream>

#include <stdio.h>
#include <limits.h>
#ifdef _X_DEBUG_
#include <unistd.h>
#endif


const char formFile[] = "/Volumes/Data/Users/dhoerl/Public/formulas.xls";

using namespace std;
using namespace xlslib_core;

// set to 'true' if you want cell format print-out while generating the XLS
//static const bool PRINT_CELL_FORMAT	= 0;

static char file_err[] = "00000000000000000000000000000000";

char *FormulaFunctionsTest(const char *md5_checksum);

int main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;

	char check[40];
	
	FormulaFunctionsTest(check);
	return 0;
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
//	{ "GETPIVOTDATA", FUNC_GETPIVOTDATA },	// this causes a Excel file error
	{ "HYPERLINK", FUNC_HYPERLINK },
	{ "PHONETIC", FUNC_PHONETIC },
	{ "AVERAGEA", FUNC_AVERAGEA },
	{ "MAXA", FUNC_MAXA },
	{ "MINA", FUNC_MINA },
	{ "STDEVPA", FUNC_STDEVPA },
	{ "VARPA", FUNC_VARPA },
	{ "STDEVA", FUNC_STDEVA },
	{ "VARA", FUNC_VARA },
	// 2007
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
	{ "IF", FUNC_IF },
	{ "IFERROR", FUNC_IFERROR },
	{ "COUNTIFS", FUNC_COUNTIFS },
	{ "SUMIFS", FUNC_SUMIFS },
	{ "AVERAGEIF", FUNC_AVERAGEIF },
	{ "AVERAGEIFS", FUNC_AVERAGEIFS },
#if 0
	// These not in "old" Excel, only the new xml based excel 2010
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
#endif
};


char *FormulaFunctionsTest(const char *md5_checksum)
{
	workbook wb;
	const unsigned32_t numSheets = 2;
	worksheet* sh[numSheets];
	
	sh[0] = wb.sheet("2003 and before");
	sh[1] = wb.sheet("2007");
	//sh[2] = wb.sheet("2010");
	//sh[3] = wb.sheet("Formulas_04");

	const unsigned32_t number_of_builtin_functions = 350 ; // sizeof(function_arr) / sizeof(function_arr[0]);
printf("Howdie=%zd\n", sizeof(function_arr) / sizeof(function_arr[0]));
	unsigned32_t row = 4;
	worksheet* cur_sh = sh[0];
	for (unsigned32_t r = 0; r < number_of_builtin_functions; r++)
	{
		expr_function_code_t fn = function_arr[r].code;
		printf("FUNC: %s\n", function_arr[r].name);
	
// These just won't work for older excel programs
		if (FUNC_BAHTTEXT == fn)
		{
#if 0
			/* set #if to 0 to create an XLS which includes Excel 2007/2010 functions in the sample formulas */
			break;
#else
			row = 4;
			cur_sh = sh[1];
#endif
		}
#if 0
		else if (FUNC_AGGREGATE == fn)
		{
			row = 4;
			cur_sh = sh[2];
		}
#endif
		expression_node_factory_t& maker = wb.GetFormulaFactory();

		unsigned32_t argmask = NumberOfArgsForExcelFunction(fn);
		if (argmask != 0 && !(argmask & 0x80000000U) /* A_MACRO */)
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

			unsigned32_t argcnt;
			for (argcnt = 3; argcnt <= 30; argcnt++)
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

	for (unsigned32_t i = 0; i < numSheets; i++)
	{
		sh[i]->label(1, 0, "function name");
		sh[i]->label(0, 1, "argument count");
		for (size_t argcnt = 0; argcnt <= 30; argcnt++)
		{
			sh[i]->number(1, (unsigned32_t)(1 + argcnt), (signed32_t)argcnt);
		}
	}

	int err = wb.Dump(formFile);

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
