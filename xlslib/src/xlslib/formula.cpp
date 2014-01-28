/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * This file is part of xlslib -- A multiplatform, C/C++ library
 * for dynamic generation of Excel(TM) files.
 *
 * Copyright 2010-2013 Ger Hobbelt All Rights Reserved.
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
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifdef __BCPLUSPLUS__
#include <memory.h>
// string.h needed for memcpy(). RLN 111215
// This may be applicable to other compilers as well.
#endif

#include "xlslib/unit.h"
#include "xlslib/globalrec.h"
#include "xlslib/formula.h"
#include "xlslib/formula_estimate.h"
#include "xlslib/formula_cell.h"
#include "xlslib/formula_const.h"
#include "xlslib/datast.h"
#include "xlslib/common.h"
#include "xlslib/sheetrec.h"

using namespace xlslib_core;
using namespace xlslib_strings;

struct function_property {
	cell_op_class_t		op_class;
	unsigned32_t		num_args;
	// const char		*name;	// same structure used elsewhere
};

static function_property PropertyForForExcelFunction(expr_function_code_t code);

formula_t::formula_t(CGlobalRecords& glbl, worksheet *ws) :
    m_GlobalRecords(glbl),
    m_Worksheet(ws)
{
    data_storage = new CDataStorage();

    main_data = data_storage->MakeCUnit();
    aux_data = data_storage->MakeCUnit();

    main_data->Inflate(10);
    aux_data->Inflate(10);
}

formula_t::~formula_t()
{
    delete data_storage;
}

signed8_t formula_t::PushBoolean(bool value) 
{
    signed8_t errcode = NO_ERRORS;

    errcode |= main_data->AddValue8(OP_BOOL);
    errcode |= main_data->AddValue8((unsigned8_t) !!value);

    return errcode;
}

signed8_t formula_t::PushMissingArgument()
{
    signed8_t errcode = NO_ERRORS;

    errcode |= main_data->AddValue8(OP_MISSARG);

    return errcode;
}

signed8_t formula_t::PushError(unsigned8_t value)
{
    signed8_t errcode = NO_ERRORS;

    errcode |= main_data->AddValue8(OP_ERR);
    errcode |= main_data->AddValue8(value);

    return errcode;
}

signed8_t formula_t::PushInteger(signed32_t value)
{
    signed8_t errcode = NO_ERRORS;

    if (value >= 0 && value <= 65535) {
        errcode |= main_data->AddValue8(OP_INT);
        errcode |= main_data->AddValue16((unsigned16_t)value);
    } else {
        errcode |= main_data->AddValue8(OP_NUM);
        errcode |= main_data->AddValue64FP(value);
    }

    return errcode;
}

signed8_t formula_t::PushFloatingPoint(double value)
{
    signed8_t errcode = NO_ERRORS;

    errcode |= main_data->AddValue8(OP_NUM);
    errcode |= main_data->AddValue64FP(value);

    return errcode;
}

signed8_t formula_t::PushOperator(expr_operator_code_t op)
{
    signed8_t errcode = NO_ERRORS;

    errcode |= main_data->AddValue8(op);

    return errcode;
}

signed8_t formula_t::PushCellReference(const cell_t& cell, cell_addr_mode_t opt, cell_op_class_t op_class)
{
	signed8_t ret;
	unsigned32_t idx;

	idx = cell.GetWorksheet() ? cell.GetWorksheet()->GetIndex() : invalidIndex;
	ret = PushReference(cell.GetRow(), cell.GetCol(), idx, opt, op_class);
	return ret;
}

signed8_t formula_t::PushReference(unsigned32_t row, unsigned32_t col, unsigned32_t idx, cell_addr_mode_t opt, cell_op_class_t op_class)
{
    signed8_t errcode = NO_ERRORS;

	(void)op_class;

    if (m_Worksheet == NULL || idx == invalidIndex || idx == m_Worksheet->GetIndex()) {
		unsigned8_t op = (OP_REF & ~CELLOP_AS_ARRAY) | op_class;
        errcode |= main_data->AddValue8(op);
        col &= 0x3FFF;
    } else {
		unsigned8_t op = (OP_REF3D & ~CELLOP_AS_ARRAY) | op_class;
        errcode |= main_data->AddValue8(op);
        errcode |= main_data->AddValue16(static_cast<unsigned16_t>(idx));
        col &= 0x00FF;
    }
    errcode |= main_data->AddValue16(static_cast<unsigned16_t>(row));

    XL_ASSERT((opt & ~0xC000) == 0);
    col |= opt & 0xC000;
    errcode |= main_data->AddValue16(static_cast<unsigned16_t>(col));

	return errcode;
}

signed8_t formula_t::PushCellAreaReference(const cell_t& upper_left_cell, const cell_t& lower_right_cell, cell_addr_mode_t opt, cell_op_class_t op_class)
{
	signed8_t ret;
	unsigned32_t ul_idx, lr_idx;

	ul_idx = upper_left_cell.GetWorksheet() ? upper_left_cell.GetWorksheet()->GetIndex() : invalidIndex;
	lr_idx = lower_right_cell.GetWorksheet() ? lower_right_cell.GetWorksheet()->GetIndex() : invalidIndex;

	ret = PushAreaReference(upper_left_cell.GetRow(), upper_left_cell.GetCol(), ul_idx, lower_right_cell.GetRow(), lower_right_cell.GetCol(), lr_idx, opt, op_class);
	return ret;
}

signed8_t formula_t::PushAreaReference(unsigned32_t ul_row, unsigned32_t ul_col, unsigned32_t ul_idx, unsigned32_t lr_row, unsigned32_t lr_col, unsigned32_t lr_idx, cell_addr_mode_t opt, cell_op_class_t op_class)
{
    signed8_t errcode = NO_ERRORS;
	(void)lr_idx;	// prevent warning

    if (m_Worksheet == NULL || ul_idx == invalidIndex || ul_idx == m_Worksheet->GetIndex()) {
		unsigned8_t op = (OP_AREA & ~CELLOP_AS_ARRAY) | op_class;
        errcode |= main_data->AddValue8(op);
        ul_col &= 0x3FFF;
        lr_col &= 0x3FFF;
    } else {
		unsigned8_t op = (OP_AREA3D & ~CELLOP_AS_ARRAY) | op_class;
        errcode |= main_data->AddValue8(op);
        errcode |= main_data->AddValue16(static_cast<unsigned16_t>(ul_idx));
        ul_col &= 0x00FF;
        lr_col &= 0x00FF;
    }

	// BIFF8 format!
	errcode |= main_data->AddValue16(static_cast<unsigned16_t>(ul_row));
	errcode |= main_data->AddValue16(static_cast<unsigned16_t>(lr_row));

	XL_ASSERT((opt & ~0xC000) == 0);
	ul_col |= opt & 0xC000;
	errcode |= main_data->AddValue16(static_cast<unsigned16_t>(ul_col));

	XL_ASSERT((opt & ~0xC000) == 0);
	lr_col |= opt & 0xC000;
	errcode |= main_data->AddValue16(static_cast<unsigned16_t>(lr_col));

	return errcode;
}

signed8_t formula_t::PushFunction(expr_function_code_t func, cell_op_class_t op_class) {
	function_property func_prop = PropertyForForExcelFunction(func);
	unsigned32_t argcntmask = func_prop.num_args;
	cell_op_class_t op_cl = op_class == CELL_DEFAULT ? func_prop.op_class : op_class;
	unsigned8_t op = (OP_FUNC & ~CELLOP_AS_ARRAY) | op_cl;
    signed8_t errcode = NO_ERRORS;
    if (argcntmask == A_0 || argcntmask == A_1 || argcntmask == A_2 ||
            argcntmask == A_3 || argcntmask == A_4 || argcntmask == A_5 ||
            argcntmask == A_UNKNOWN) {
		errcode |= main_data->AddValue8(op);
		errcode |= main_data->AddValue16(func);
    } else {
        errcode = GENERAL_ERROR;
    }
    return errcode;
}

signed8_t formula_t::PushFunction(expr_function_code_t func, size_t argcount, cell_op_class_t op_class) {
	function_property func_prop = PropertyForForExcelFunction(func);
	unsigned32_t argcntmask = func_prop.num_args;
	cell_op_class_t op_cl = op_class == CELL_DEFAULT ? func_prop.op_class : op_class;
	unsigned8_t op = (OP_FUNCVAR & ~CELLOP_AS_ARRAY) | op_cl;
    signed8_t errcode = NO_ERRORS;
	// argcntmask test is: is there just one bit of options, and the current count is not it
	// printf("func=%d argbcmask=%x argcount=%ld mask=%8.8x\n", func, argcntmask, argcount, (1U << argcount));
    if (argcntmask == A_UNKNOWN || (argcount <= 30 && (argcntmask & (1U << argcount)))) {
		errcode |= main_data->AddValue8(op);
		errcode |= main_data->AddValue8((unsigned8_t)argcount & 0x7F); // no prompt for user: 0x80 not set
		errcode |= main_data->AddValue16((unsigned16_t)func & 0x7FFF); 
    } else {
        errcode = GENERAL_ERROR;
    }
    return errcode;
}

signed8_t formula_t::PushText(const std::string& v) {
    u16string value;
	m_GlobalRecords.char2str16(v, value);

    return PushText(value);
}

signed8_t formula_t::PushText(const ustring& v) {
    u16string value;
	m_GlobalRecords.wide2str16(v, value);

    return PushText(value);
}

#if !defined(__FRAMEWORK__)
signed8_t formula_t::PushText(const u16string& value) {
	signed8_t errcode = NO_ERRORS;

	errcode |= main_data->AddValue8(OP_STR);
	// TODO: clip string to 255 chars max!
	errcode |= main_data->AddUnicodeString(value, CUnit::LEN1_FLAGS_UNICODE);

	return errcode;
}
#endif

signed8_t formula_t::PushTextArray(const std::vector<std::string>& vec) {
	signed8_t errcode = NO_ERRORS;
    errcode |= main_data->AddValue8(OP_ARRAYA);
    errcode |= main_data->AddFixedDataArray(0, 7);
    errcode |= aux_data->AddValue8(1);
    errcode |= aux_data->AddValue16((unsigned16_t)vec.size());
    for(unsigned int i=0; i<vec.size(); i++) {
        errcode |= aux_data->AddValue8(0x01);
        std::string str = vec[i];
        u16string value;
        m_GlobalRecords.char2str16(str, value);

        errcode |= aux_data->AddUnicodeString(value, CUnit::LEN1_FLAGS_UNICODE);
    }
    return errcode;
}

signed8_t formula_t::PushTextArray(const std::vector<ustring>& vec) {
	signed8_t errcode = NO_ERRORS;
    errcode |= main_data->AddValue8(OP_ARRAYA);
    errcode |= main_data->AddFixedDataArray(0, 7);
    errcode |= aux_data->AddValue8(1);
    errcode |= aux_data->AddValue16((unsigned16_t)vec.size());
    for(unsigned int i=0; i<vec.size(); i++) {
        errcode |= aux_data->AddValue8(0x01);
        ustring str = vec[i];
        u16string value;
        m_GlobalRecords.wide2str16(str, value);

        errcode |= aux_data->AddUnicodeString(value, CUnit::LEN1_FLAGS_UNICODE);
    }
    return errcode;
}

signed8_t formula_t::PushFloatingPointArray(const std::vector<double>& vec) {
	signed8_t errcode = NO_ERRORS;
    errcode |= main_data->AddValue8(OP_ARRAYA);
    errcode |= aux_data->AddValue8(1);
    errcode |= aux_data->AddValue16(static_cast<unsigned16_t>(vec.size()));
    for(unsigned int i=0; i<vec.size(); i++) {
        errcode |= aux_data->AddValue8(0x02);
        errcode |= aux_data->AddValue64FP(vec[i]);
    }
    return errcode;
}

void formula_t::DumpData(CUnit &dst) const
{
    dst.Append(*main_data);
    dst.Append(*aux_data);
}

size_t formula_t::GetSize(void) const
{
    return main_data->GetDataSize() + aux_data->GetDataSize();
}

void formula_t::GetResultEstimate(estimated_formula_result_t &dst) const
{
    dst.SetCalcOnLoad();
    dst.SetErrorCode(XLERR_VALUE);
}


static const uint32_t maxProperties = 485 ; // 368 in 2004, 484 in 2007, and 548 if you enable newer functions than were defined since BIFF8.

// Notes: the function_properties table was build by combining Ger's original switch statement for the number of arguments with
//    a second table mapping the 'FUNC_...' values to the Excel English function name (search for "function_arr"). The CELLOP_AS_
//    values come mostly from the OpenOffice "Microsoft Excel File Format" Table 3.11.1 (I could not find the same thing within
//    MS's own document!). I disagreed with several of the OpenOffice values and overrode them (for instance, IF does not return
//    a reference!)
// Also, I have no idea (yet) what the purpose of A_MACRO is...

#define XLNAME(x)	// this can be turned on to add a name to the table, when used for diagnostics
static function_property function_properties[maxProperties] = {
    /* FUNC_COUNT=0 */                  CELLOP_AS_VALUE, A_1_OR_MORE,           XLNAME("COUNT")
    /* FUNC_IF=1 */                     CELLOP_AS_VALUE, A_2_OR_3,              XLNAME("IF")
    /* FUNC_ISNA=2 */                   CELLOP_AS_VALUE, A_1,                   XLNAME("ISNA")
    /* FUNC_ISERROR=3 */                CELLOP_AS_VALUE, A_1,                   XLNAME("ISERROR")
    /* FUNC_SUM=4 */                    CELLOP_AS_VALUE, A_1_OR_MORE,           XLNAME("SUM")
    /* FUNC_AVERAGE=5 */                CELLOP_AS_VALUE, A_1_OR_MORE,           XLNAME("AVERAGE")
    /* FUNC_MIN=6 */                    CELLOP_AS_VALUE, A_1_OR_MORE,           XLNAME("MIN")
    /* FUNC_MAX=7 */                    CELLOP_AS_VALUE, A_1_OR_MORE,           XLNAME("MAX")
    /* FUNC_ROW=8 */                    CELLOP_AS_VALUE, A_0_OR_1,              XLNAME("ROW")
    /* FUNC_COLUMN=9 */                 CELLOP_AS_VALUE, A_0_OR_1,              XLNAME("COLUMN")
    /* FUNC_NA=10 */                    CELLOP_AS_VALUE, A_0,                   XLNAME("NA")
    /* FUNC_NPV=11 */                   CELLOP_AS_VALUE, A_2_OR_MORE,           XLNAME("NPV")
    /* FUNC_STDEV=12 */                 CELLOP_AS_VALUE, A_1_OR_MORE,           XLNAME("STDEV")
    /* FUNC_DOLLAR=13 */                CELLOP_AS_VALUE, A_1_OR_2,              XLNAME("DOLLAR")
    /* FUNC_FIXED=14 */                 CELLOP_AS_VALUE, A_1_TO_3,              XLNAME("FIXED")
    /* FUNC_SIN=15 */                   CELLOP_AS_VALUE, A_1,                   XLNAME("SIN")
    /* FUNC_COS=16 */                   CELLOP_AS_VALUE, A_1,                   XLNAME("COS")
    /* FUNC_TAN=17 */                   CELLOP_AS_VALUE, A_1,                   XLNAME("TAN")
    /* FUNC_ATAN=18 */                  CELLOP_AS_VALUE, A_1,                   XLNAME("ATAN")
    /* FUNC_PI=19 */                    CELLOP_AS_VALUE, A_0,                   XLNAME("PI")
    /* FUNC_SQRT=20 */                  CELLOP_AS_VALUE, A_1,                   XLNAME("SQRT")
    /* FUNC_EXP=21 */                   CELLOP_AS_VALUE, A_1,                   XLNAME("EXP")
    /* FUNC_LN=22 */                    CELLOP_AS_VALUE, A_1,                   XLNAME("LN")
    /* FUNC_LOG10=23 */                 CELLOP_AS_VALUE, A_1,                   XLNAME("LOG10")
    /* FUNC_ABS=24 */                   CELLOP_AS_VALUE, A_1,                   XLNAME("ABS")
    /* FUNC_INT=25 */                   CELLOP_AS_VALUE, A_1,                   XLNAME("INT")
    /* FUNC_SIGN=26 */                  CELLOP_AS_VALUE, A_1,                   XLNAME("SIGN")
    /* FUNC_ROUND=27 */                 CELLOP_AS_VALUE, A_2,                   XLNAME("ROUND")
    /* FUNC_LOOKUP=28 */                CELLOP_AS_VALUE, A_2_OR_3,              XLNAME("LOOKUP")
    /* FUNC_INDEX=29 */                 CELLOP_AS_REFER, A_2_TO_4,              XLNAME("INDEX")
    /* FUNC_REPT=30 */                  CELLOP_AS_VALUE, A_2,                   XLNAME("REPT")
    /* FUNC_MID=31 */                   CELLOP_AS_VALUE, A_3,                   XLNAME("MID")
    /* FUNC_LEN=32 */                   CELLOP_AS_VALUE, A_1,                   XLNAME("LEN")
    /* FUNC_VALUE=33 */                 CELLOP_AS_VALUE, A_1,                   XLNAME("VALUE")
    /* FUNC_TRUE=34 */                  CELLOP_AS_VALUE, A_0,                   XLNAME("TRUE")
    /* FUNC_FALSE=35 */                 CELLOP_AS_VALUE, A_0,                   XLNAME("FALSE")
    /* FUNC_AND=36 */                   CELLOP_AS_VALUE, A_1_OR_MORE,           XLNAME("AND")
    /* FUNC_OR=37 */                    CELLOP_AS_VALUE, A_1_OR_MORE,           XLNAME("OR")
    /* FUNC_NOT=38 */                   CELLOP_AS_VALUE, A_1,                   XLNAME("NOT")
    /* FUNC_MOD=39 */                   CELLOP_AS_VALUE, A_2,                   XLNAME("MOD")
    /* FUNC_DCOUNT=40 */                CELLOP_AS_VALUE, A_3,                   XLNAME("DCOUNT")
    /* FUNC_DSUM=41 */                  CELLOP_AS_VALUE, A_3,                   XLNAME("DSUM")
    /* FUNC_DAVERAGE=42 */              CELLOP_AS_VALUE, A_3,                   XLNAME("DAVERAGE")
    /* FUNC_DMIN=43 */                  CELLOP_AS_VALUE, A_3,                   XLNAME("DMIN")
    /* FUNC_DMAX=44 */                  CELLOP_AS_VALUE, A_3,                   XLNAME("DMAX")
    /* FUNC_DSTDEV=45 */                CELLOP_AS_VALUE, A_3,                   XLNAME("DSTDEV")
    /* FUNC_VAR=46 */                   CELLOP_AS_VALUE, A_1_OR_MORE,           XLNAME("VAR")
    /* FUNC_DVAR=47 */                  CELLOP_AS_VALUE, A_3,                   XLNAME("DVAR")
    /* FUNC_TEXT=48 */                  CELLOP_AS_VALUE, A_2,                   XLNAME("TEXT")
    /* FUNC_LINEST=49 */                CELLOP_AS_ARRAY, A_1_TO_4,              XLNAME("LINEST")
    /* FUNC_TREND=50 */                 CELLOP_AS_ARRAY, A_1_TO_4,              XLNAME("TREND")
    /* FUNC_LOGEST=51 */                CELLOP_AS_ARRAY, A_1_TO_4,              XLNAME("LOGEST")
    /* FUNC_GROWTH=52 */                CELLOP_AS_ARRAY, A_1_TO_4,              XLNAME("GROWTH")
    /* FUNC_GOTO=53 */                  CELLOP_AS_VALUE, A_1 | A_MACRO,         XLNAME("GOTO")
    /* FUNC_HALT=54 */                  CELLOP_AS_VALUE, A_0_OR_1 | A_MACRO,    XLNAME("HALT")
    /* UNUSED=55 */						CELLOP_AS_ARRAY, 0,						XLNAME("")
    /* FUNC_PV=56 */                    CELLOP_AS_VALUE, A_3_TO_5,              XLNAME("PV")
    /* FUNC_FV=57 */                    CELLOP_AS_VALUE, A_3_TO_5,              XLNAME("FV")
    /* FUNC_NPER=58 */                  CELLOP_AS_VALUE, A_3_TO_5,              XLNAME("NPER")
    /* FUNC_PMT=59 */                   CELLOP_AS_VALUE, A_3_TO_5,              XLNAME("PMT")
    /* FUNC_RATE=60 */                  CELLOP_AS_VALUE, A_3_TO_6,              XLNAME("RATE")
    /* FUNC_MIRR=61 */                  CELLOP_AS_VALUE, A_3,                   XLNAME("MIRR")
    /* FUNC_IRR=62 */                   CELLOP_AS_VALUE, A_1_OR_2,              XLNAME("IRR")
    /* FUNC_RAND=63 */                  CELLOP_AS_VALUE, A_0,                   XLNAME("RAND")
    /* FUNC_MATCH=64 */                 CELLOP_AS_VALUE, A_2_OR_3,              XLNAME("MATCH")
    /* FUNC_DATE=65 */                  CELLOP_AS_VALUE, A_3,                   XLNAME("DATE")
    /* FUNC_TIME=66 */                  CELLOP_AS_VALUE, A_3,                   XLNAME("TIME")
    /* FUNC_DAY=67 */                   CELLOP_AS_VALUE, A_1,                   XLNAME("DAY")
    /* FUNC_MONTH=68 */                 CELLOP_AS_VALUE, A_1,                   XLNAME("MONTH")
    /* FUNC_YEAR=69 */                  CELLOP_AS_VALUE, A_1,                   XLNAME("YEAR")
    /* FUNC_WEEKDAY=70 */               CELLOP_AS_VALUE, A_1_OR_2,              XLNAME("WEEKDAY")
    /* FUNC_HOUR=71 */                  CELLOP_AS_VALUE, A_1,                   XLNAME("HOUR")
    /* FUNC_MINUTE=72 */                CELLOP_AS_VALUE, A_1,                   XLNAME("MINUTE")
    /* FUNC_SECOND=73 */                CELLOP_AS_VALUE, A_1,                   XLNAME("SECOND")
    /* FUNC_NOW=74 */                   CELLOP_AS_VALUE, A_0,                   XLNAME("NOW")
    /* FUNC_AREAS=75 */                 CELLOP_AS_VALUE, A_1,                   XLNAME("AREAS")
    /* FUNC_ROWS=76 */                  CELLOP_AS_VALUE, A_1,                   XLNAME("ROWS")
    /* FUNC_COLUMNS=77 */               CELLOP_AS_VALUE, A_1,                   XLNAME("COLUMNS")
    /* FUNC_OFFSET=78 */                CELLOP_AS_REFER, A_3_TO_5,              XLNAME("OFFSET")
    /* FUNC_ABSREF=79 */                CELLOP_AS_VALUE, A_2 | A_MACRO,         XLNAME("ABSREF")
    /* FUNC_RELREF=80 */                CELLOP_AS_VALUE, A_2 | A_MACRO,         XLNAME("RELREF")
    /* FUNC_ARGUMENT=81 */              CELLOP_AS_VALUE, A_0_TO_3 | A_MACRO,    XLNAME("ARGUMENT")
    /* FUNC_SEARCH=82 */                CELLOP_AS_VALUE, A_2_OR_3,              XLNAME("SEARCH")
    /* FUNC_TRANSPOSE=83 */             CELLOP_AS_ARRAY, A_1,                   XLNAME("TRANSPOSE")
    /* FUNC_ERROR=84 */                 CELLOP_AS_VALUE, A_0_TO_2 | A_MACRO,    XLNAME("ERROR")
    /* FUNC_STEP=85 */                  CELLOP_AS_VALUE, A_0 | A_MACRO,         XLNAME("STEP")
    /* FUNC_TYPE=86 */                  CELLOP_AS_VALUE, A_1,                   XLNAME("TYPE")
    /* FUNC_ECHO=87 */                  CELLOP_AS_VALUE, A_0_OR_1 | A_MACRO,    XLNAME("ECHO")
    /* FUNC_SETNAME=88 */               CELLOP_AS_VALUE, A_1_OR_2 | A_MACRO,    XLNAME("SETNAME")
    /* FUNC_CALLER=89 */                CELLOP_AS_VALUE, A_0 | A_MACRO,         XLNAME("CALLER")
    /* FUNC_DEREF=90 */                 CELLOP_AS_VALUE, A_1 | A_MACRO,         XLNAME("DEREF")
    /* FUNC_WINDOWS=91 */               CELLOP_AS_VALUE, A_0_TO_2 | A_MACRO,    XLNAME("WINDOWS")
    /* FUNC_SERIES=92 */                CELLOP_AS_VALUE, A_4_OR_5,              XLNAME("SERIES")
    /* FUNC_DOCUMENTS=93 */             CELLOP_AS_VALUE, A_0_TO_2 | A_MACRO,    XLNAME("DOCUMENTS")
    /* FUNC_ACTIVECELL=94 */            CELLOP_AS_VALUE, A_0 | A_MACRO,         XLNAME("ACTIVECELL")
    /* FUNC_SELECTION=95 */             CELLOP_AS_VALUE, A_0 | A_MACRO,         XLNAME("SELECTION")
    /* FUNC_RESULT=96 */                CELLOP_AS_VALUE, A_0_OR_1 | A_MACRO,    XLNAME("RESULT")
    /* FUNC_ATAN2=97 */                 CELLOP_AS_VALUE, A_2,                   XLNAME("ATAN2")
    /* FUNC_ASIN=98 */                  CELLOP_AS_VALUE, A_1,                   XLNAME("ASIN")
    /* FUNC_ACOS=99 */                  CELLOP_AS_VALUE, A_1,                   XLNAME("ACOS")
    /* FUNC_CHOOSE=100 */               CELLOP_AS_REFER, A_2_OR_MORE,           XLNAME("CHOOSE")
    /* FUNC_HLOOKUP=101 */              CELLOP_AS_VALUE, A_3_OR_4,              XLNAME("HLOOKUP")
    /* FUNC_VLOOKUP=102 */              CELLOP_AS_VALUE, A_3_OR_4,              XLNAME("VLOOKUP")
    /* FUNC_LINKS=103 */                CELLOP_AS_VALUE, A_0_TO_2 | A_MACRO,    XLNAME("LINKS")
    /* FUNC_INPUT=104 */                CELLOP_AS_VALUE, A_1_TO_7 | A_MACRO,    XLNAME("INPUT")
    /* FUNC_ISREF=105 */                CELLOP_AS_VALUE, A_1,                   XLNAME("ISREF")
    /* FUNC_GETFORMULA=106 */           CELLOP_AS_VALUE, A_1 | A_MACRO,         XLNAME("GETFORMULA")
    /* FUNC_GETNAME=107 */              CELLOP_AS_VALUE, A_1_OR_2 | A_MACRO,    XLNAME("GETNAME")
    /* FUNC_SETVALUE=108 */             CELLOP_AS_VALUE, A_2 | A_MACRO,         XLNAME("SETVALUE")
    /* FUNC_LOG=109 */                  CELLOP_AS_VALUE, A_1_OR_2,              XLNAME("LOG")
    /* FUNC_EXEC=110 */                 CELLOP_AS_VALUE, A_1_TO_4 | A_MACRO,    XLNAME("EXEC")
    /* FUNC_CHAR=111 */                 CELLOP_AS_VALUE, A_1,                   XLNAME("CHAR")
    /* FUNC_LOWER=112 */                CELLOP_AS_VALUE, A_1,                   XLNAME("LOWER")
    /* FUNC_UPPER=113 */                CELLOP_AS_VALUE, A_1,                   XLNAME("UPPER")
    /* FUNC_PROPER=114 */               CELLOP_AS_VALUE, A_1,                   XLNAME("PROPER")
    /* FUNC_LEFT=115 */                 CELLOP_AS_VALUE, A_1_OR_2,              XLNAME("LEFT")
    /* FUNC_RIGHT=116 */                CELLOP_AS_VALUE, A_1_OR_2,              XLNAME("RIGHT")
    /* FUNC_EXACT=117 */                CELLOP_AS_VALUE, A_2,                   XLNAME("EXACT")
    /* FUNC_TRIM=118 */                 CELLOP_AS_VALUE, A_1,                   XLNAME("TRIM")
    /* FUNC_REPLACE=119 */              CELLOP_AS_VALUE, A_4,                   XLNAME("REPLACE")
    /* FUNC_SUBSTITUTE=120 */           CELLOP_AS_VALUE, A_3_OR_4,              XLNAME("SUBSTITUTE")
    /* FUNC_CODE=121 */                 CELLOP_AS_VALUE, A_1,                   XLNAME("CODE")
    /* FUNC_NAMES=122 */                CELLOP_AS_VALUE, A_0_TO_3 | A_MACRO,    XLNAME("NAMES")
    /* FUNC_DIRECTORY=123 */            CELLOP_AS_VALUE, A_0_OR_1 | A_MACRO,    XLNAME("DIRECTORY")
    /* FUNC_FIND=124 */                 CELLOP_AS_VALUE, A_2_OR_3,              XLNAME("FIND")
    /* FUNC_CELL=125 */                 CELLOP_AS_VALUE, A_1_OR_2,              XLNAME("CELL")
    /* FUNC_ISERR=126 */                CELLOP_AS_VALUE, A_1,                   XLNAME("ISERR")
    /* FUNC_ISTEXT=127 */               CELLOP_AS_VALUE, A_1,                   XLNAME("ISTEXT")
    /* FUNC_ISNUMBER=128 */             CELLOP_AS_VALUE, A_1,                   XLNAME("ISNUMBER")
    /* FUNC_ISBLANK=129 */              CELLOP_AS_VALUE, A_1,                   XLNAME("ISBLANK")
    /* FUNC_T=130 */                    CELLOP_AS_VALUE, A_1,                   XLNAME("T")
    /* FUNC_N=131 */                    CELLOP_AS_VALUE, A_1,                   XLNAME("N")
    /* FUNC_FOPEN=132 */                CELLOP_AS_VALUE, A_1_OR_2 | A_MACRO,    XLNAME("FOPEN")
    /* FUNC_FCLOSE=133 */               CELLOP_AS_VALUE, A_1 | A_MACRO,         XLNAME("FCLOSE")
    /* FUNC_FSIZE=134 */                CELLOP_AS_VALUE, A_1 | A_MACRO,         XLNAME("FSIZE")
    /* FUNC_FREADLN=135 */              CELLOP_AS_VALUE, A_1 | A_MACRO,         XLNAME("FREADLN")
    /* FUNC_FREAD=136 */                CELLOP_AS_VALUE, A_2 | A_MACRO,         XLNAME("FREAD")
    /* FUNC_FWRITELN=137 */             CELLOP_AS_VALUE, A_2 | A_MACRO,         XLNAME("FWRITELN")
    /* FUNC_FWRITE=138 */               CELLOP_AS_VALUE, A_2 | A_MACRO,         XLNAME("FWRITE")
    /* FUNC_FPOS=139 */                 CELLOP_AS_VALUE, A_1_OR_2 | A_MACRO,    XLNAME("FPOS")
    /* FUNC_DATEVALUE=140 */            CELLOP_AS_VALUE, A_1,                   XLNAME("DATEVALUE")
    /* FUNC_TIMEVALUE=141 */            CELLOP_AS_VALUE, A_1,                   XLNAME("TIMEVALUE")
    /* FUNC_SLN=142 */                  CELLOP_AS_VALUE, A_3,                   XLNAME("SLN")
    /* FUNC_SYD=143 */                  CELLOP_AS_VALUE, A_4,                   XLNAME("SYD")
    /* FUNC_DDB=144 */                  CELLOP_AS_VALUE, A_4_OR_5,              XLNAME("DDB")
    /* FUNC_GETDEF=145 */               CELLOP_AS_VALUE, A_1_TO_3 | A_MACRO,    XLNAME("GETDEF")
    /* FUNC_REFTEXT=146 */              CELLOP_AS_VALUE, A_1_OR_2 | A_MACRO,    XLNAME("REFTEXT")
    /* FUNC_TEXTREF=147 */              CELLOP_AS_VALUE, A_1_OR_2 | A_MACRO,    XLNAME("TEXTREF")
    /* FUNC_INDIRECT=148 */             CELLOP_AS_REFER, A_1_OR_2,              XLNAME("INDIRECT")
    /* FUNC_REGISTER=149 */             CELLOP_AS_VALUE, A_1_OR_MORE | A_MACRO, XLNAME("REGISTER")
    /* FUNC_CALL=150 */                 CELLOP_AS_VALUE, A_1_OR_MORE | A_MACRO, XLNAME("CALL")
    /* FUNC_ADDBAR=151 */               CELLOP_AS_VALUE, A_0_OR_1 | A_MACRO,    XLNAME("ADDBAR")
    /* FUNC_ADDMENU=152 */              CELLOP_AS_VALUE, A_2_TO_4 | A_MACRO,    XLNAME("ADDMENU")
    /* FUNC_ADDCOMMAND=153 */           CELLOP_AS_VALUE, A_3_TO_5 | A_MACRO,    XLNAME("ADDCOMMAND")
    /* FUNC_ENABLECOMMAND=154 */        CELLOP_AS_VALUE, A_4_OR_5 | A_MACRO,    XLNAME("ENABLECOMMAND")
    /* FUNC_CHECKCOMMAND=155 */         CELLOP_AS_VALUE, A_4_OR_5 | A_MACRO,    XLNAME("CHECKCOMMAND")
    /* FUNC_RENAMECOMMAND=156 */        CELLOP_AS_VALUE, A_4_OR_5 | A_MACRO,    XLNAME("RENAMECOMMAND")
    /* FUNC_SHOWBAR=157 */              CELLOP_AS_VALUE, A_0_OR_1 | A_MACRO,    XLNAME("SHOWBAR")
    /* FUNC_DELETEMENU=158 */           CELLOP_AS_VALUE, A_2_OR_3 | A_MACRO,    XLNAME("DELETEMENU")
    /* FUNC_DELETECOMMAND=159 */        CELLOP_AS_VALUE, A_3_OR_4 | A_MACRO,    XLNAME("DELETECOMMAND")
    /* FUNC_GETCHARTITEM=160 */         CELLOP_AS_VALUE, A_1_TO_3 | A_MACRO,    XLNAME("GETCHARTITEM")
    /* FUNC_DIALOGBOX=161 */            CELLOP_AS_VALUE, A_1 | A_MACRO,         XLNAME("DIALOGBOX")
    /* FUNC_CLEAN=162 */                CELLOP_AS_VALUE, A_1,                   XLNAME("CLEAN")
    /* FUNC_MDETERM=163 */              CELLOP_AS_VALUE, A_1,                   XLNAME("MDETERM")
    /* FUNC_MINVERSE=164 */             CELLOP_AS_ARRAY, A_1,                   XLNAME("MINVERSE")
    /* FUNC_MMULT=165 */                CELLOP_AS_ARRAY, A_2,                   XLNAME("MMULT")
    /* FUNC_FILES=166 */                CELLOP_AS_VALUE, A_0_TO_2 | A_MACRO,    XLNAME("FILES")
    /* FUNC_IPMT=167 */                 CELLOP_AS_VALUE, A_4_TO_6,              XLNAME("IPMT")
    /* FUNC_PPMT=168 */                 CELLOP_AS_VALUE, A_4_TO_6,              XLNAME("PPMT")
    /* FUNC_COUNTA=169 */               CELLOP_AS_VALUE, A_1_OR_MORE,           XLNAME("COUNTA")
    /* FUNC_CANCELKEY=170 */            CELLOP_AS_VALUE, A_0_TO_2 | A_MACRO,    XLNAME("CANCELKEY")
    /* UNUSED=171 */					CELLOP_AS_ARRAY, 0,						XLNAME("")
    /* UNUSED=172 */					CELLOP_AS_ARRAY, 0,						XLNAME("")
    /* UNUSED=173 */					CELLOP_AS_ARRAY, 0,						XLNAME("")
    /* UNUSED=174 */					CELLOP_AS_ARRAY, 0,						XLNAME("")
    /* FUNC_INITIATE=175 */             CELLOP_AS_VALUE, A_2 | A_MACRO,         XLNAME("INITIATE")
    /* FUNC_REQUEST=176 */              CELLOP_AS_VALUE, A_2 | A_MACRO,         XLNAME("REQUEST")
    /* FUNC_POKE=177 */                 CELLOP_AS_VALUE, A_3 | A_MACRO,         XLNAME("POKE")
    /* FUNC_EXECUTE=178 */              CELLOP_AS_VALUE, A_2 | A_MACRO,         XLNAME("EXECUTE")
    /* FUNC_TERMINATE=179 */            CELLOP_AS_VALUE, A_1 | A_MACRO,         XLNAME("TERMINATE")
    /* FUNC_RESTART=180 */              CELLOP_AS_VALUE, A_0_OR_1 | A_MACRO,    XLNAME("RESTART")
    /* FUNC_HELP=181 */                 CELLOP_AS_VALUE, A_0_OR_1 | A_MACRO,    XLNAME("HELP")
    /* FUNC_GETBAR=182 */               CELLOP_AS_VALUE, A_0_TO_4 | A_MACRO,    XLNAME("GETBAR")
    /* FUNC_PRODUCT=183 */              CELLOP_AS_VALUE, A_1_OR_MORE,           XLNAME("PRODUCT")
    /* FUNC_FACT=184 */                 CELLOP_AS_VALUE, A_1,                   XLNAME("FACT")
    /* FUNC_GETCELL=185 */              CELLOP_AS_VALUE, A_1_OR_2 | A_MACRO,    XLNAME("GETCELL")
    /* FUNC_GETWORKSPACE=186 */         CELLOP_AS_VALUE, A_1 | A_MACRO,         XLNAME("GETWORKSPACE")
    /* FUNC_GETWINDOW=187 */            CELLOP_AS_VALUE, A_1_OR_2 | A_MACRO,    XLNAME("GETWINDOW")
    /* FUNC_GETDOCUMENT=188 */          CELLOP_AS_VALUE, A_1_OR_2 | A_MACRO,    XLNAME("GETDOCUMENT")
    /* FUNC_DPRODUCT=189 */             CELLOP_AS_VALUE, A_3,                   XLNAME("DPRODUCT")
    /* FUNC_ISNONTEXT=190 */            CELLOP_AS_VALUE, A_1,                   XLNAME("ISNONTEXT")
    /* FUNC_GETNOTE=191 */              CELLOP_AS_VALUE, A_0_TO_3 | A_MACRO,    XLNAME("GETNOTE")
    /* FUNC_NOTE=192 */                 CELLOP_AS_VALUE, A_0_TO_4 | A_MACRO,    XLNAME("NOTE")
    /* FUNC_STDEVP=193 */               CELLOP_AS_VALUE, A_1_OR_MORE,           XLNAME("STDEVP")
    /* FUNC_VARP=194 */                 CELLOP_AS_VALUE, A_1_OR_MORE,           XLNAME("VARP")
    /* FUNC_DSTDEVP=195 */              CELLOP_AS_VALUE, A_3,                   XLNAME("DSTDEVP")
    /* FUNC_DVARP=196 */                CELLOP_AS_VALUE, A_3,                   XLNAME("DVARP")
    /* FUNC_TRUNC=197 */                CELLOP_AS_VALUE, A_1_OR_2,              XLNAME("TRUNC")
    /* FUNC_ISLOGICAL=198 */            CELLOP_AS_VALUE, A_1,                   XLNAME("ISLOGICAL")
    /* FUNC_DCOUNTA=199 */              CELLOP_AS_VALUE, A_3,                   XLNAME("DCOUNTA")
    /* FUNC_DELETEBAR=200 */            CELLOP_AS_VALUE, A_1 | A_MACRO,         XLNAME("DELETEBAR")
    /* FUNC_UNREGISTER=201 */           CELLOP_AS_VALUE, A_1 | A_MACRO,         XLNAME("UNREGISTER")
    /* UNUSED=202 */					CELLOP_AS_ARRAY, 0,						XLNAME("")
    /* UNUSED=203 */					CELLOP_AS_ARRAY, 0,						XLNAME("")
    /* FUNC_USDOLLAR=204 */             CELLOP_AS_VALUE, A_1_OR_2,              XLNAME("USDOLLAR")
    /* FUNC_FINDB=205 */                CELLOP_AS_VALUE, A_2_OR_3,              XLNAME("FINDB")
    /* FUNC_SEARCHB=206 */              CELLOP_AS_VALUE, A_2_OR_3,              XLNAME("SEARCHB")
    /* FUNC_REPLACEB=207 */             CELLOP_AS_VALUE, A_4,                   XLNAME("REPLACEB")
    /* FUNC_LEFTB=208 */                CELLOP_AS_VALUE, A_1_OR_2,              XLNAME("LEFTB")
    /* FUNC_RIGHTB=209 */               CELLOP_AS_VALUE, A_1_OR_2,              XLNAME("RIGHTB")
    /* FUNC_MIDB=210 */                 CELLOP_AS_VALUE, A_3,                   XLNAME("MIDB")
    /* FUNC_LENB=211 */                 CELLOP_AS_VALUE, A_1,                   XLNAME("LENB")
    /* FUNC_ROUNDUP=212 */              CELLOP_AS_VALUE, A_2,                   XLNAME("ROUNDUP")
    /* FUNC_ROUNDDOWN=213 */            CELLOP_AS_VALUE, A_2,                   XLNAME("ROUNDDOWN")
    /* FUNC_ASC=214 */                  CELLOP_AS_VALUE, A_1,                   XLNAME("ASC")
    /* FUNC_DBCS=215 */                 CELLOP_AS_VALUE, A_1,                   XLNAME("DBCS")
    /* FUNC_RANK=216 */                 CELLOP_AS_VALUE, A_2_OR_3,              XLNAME("RANK")
    /* UNUSED=217 */					CELLOP_AS_ARRAY, 0,						XLNAME("")
    /* UNUSED=218 */					CELLOP_AS_ARRAY, 0,						XLNAME("")
    /* FUNC_ADDRESS=219 */              CELLOP_AS_VALUE, A_2_TO_5,              XLNAME("ADDRESS")
    /* FUNC_DAYS360=220 */              CELLOP_AS_VALUE, A_2_OR_3,              XLNAME("DAYS360")
    /* FUNC_TODAY=221 */                CELLOP_AS_VALUE, A_0,                   XLNAME("TODAY")
    /* FUNC_VDB=222 */                  CELLOP_AS_VALUE, A_5_TO_7,              XLNAME("VDB")
    /* UNUSED=223 */					CELLOP_AS_ARRAY, 0,						XLNAME("")
    /* UNUSED=224 */					CELLOP_AS_ARRAY, 0,						XLNAME("")
    /* UNUSED=225 */					CELLOP_AS_ARRAY, 0,						XLNAME("")
    /* UNUSED=226 */					CELLOP_AS_ARRAY, 0,						XLNAME("")
    /* FUNC_MEDIAN=227 */               CELLOP_AS_VALUE, A_1_OR_MORE,           XLNAME("MEDIAN")
    /* FUNC_SUMPRODUCT=228 */           CELLOP_AS_VALUE, A_1_OR_MORE,           XLNAME("SUMPRODUCT")
    /* FUNC_SINH=229 */                 CELLOP_AS_VALUE, A_1,                   XLNAME("SINH")
    /* FUNC_COSH=230 */                 CELLOP_AS_VALUE, A_1,                   XLNAME("COSH")
    /* FUNC_TANH=231 */                 CELLOP_AS_VALUE, A_1,                   XLNAME("TANH")
    /* FUNC_ASINH=232 */                CELLOP_AS_VALUE, A_1,                   XLNAME("ASINH")
    /* FUNC_ACOSH=233 */                CELLOP_AS_VALUE, A_1,                   XLNAME("ACOSH")
    /* FUNC_ATANH=234 */                CELLOP_AS_VALUE, A_1,                   XLNAME("ATANH")
    /* FUNC_DGET=235 */                 CELLOP_AS_VALUE, A_3,                   XLNAME("DGET")
    /* FUNC_CREATEOBJECT=236 */         CELLOP_AS_VALUE, A_2_OR_MORE | A_MACRO, XLNAME("CREATEOBJECT")
    /* FUNC_VOLATILE=237 */             CELLOP_AS_VALUE, A_0_OR_1 | A_MACRO,    XLNAME("VOLATILE")
    /* FUNC_LASTERROR=238 */            CELLOP_AS_VALUE, A_0 | A_MACRO,         XLNAME("LASTERROR")
    /* FUNC_CUSTOMUNDO=239 */           CELLOP_AS_VALUE, A_0_TO_2 | A_MACRO,    XLNAME("CUSTOMUNDO")
    /* FUNC_CUSTOMREPEAT=240 */         CELLOP_AS_VALUE, A_0_TO_3 | A_MACRO,    XLNAME("CUSTOMREPEAT")
    /* FUNC_FORMULACONVERT=241 */       CELLOP_AS_VALUE, A_2_TO_5 | A_MACRO,    XLNAME("FORMULACONVERT")
    /* FUNC_GETLINKINFO=242 */          CELLOP_AS_VALUE, A_2_TO_4 | A_MACRO,    XLNAME("GETLINKINFO")
    /* FUNC_TEXTBOX=243 */              CELLOP_AS_VALUE, A_1_TO_4 | A_MACRO,    XLNAME("TEXTBOX")
    /* FUNC_INFO=244 */                 CELLOP_AS_VALUE, A_1,                   XLNAME("INFO")
    /* FUNC_GROUP=245 */                CELLOP_AS_VALUE, A_0 | A_MACRO,         XLNAME("GROUP")
    /* FUNC_GETOBJECT=246 */            CELLOP_AS_VALUE, A_1_TO_5 | A_MACRO,    XLNAME("GETOBJECT")
    /* FUNC_DB=247 */                   CELLOP_AS_VALUE, A_4_OR_5,              XLNAME("DB")
    /* FUNC_PAUSE=248 */                CELLOP_AS_VALUE, A_0_OR_1 | A_MACRO,    XLNAME("PAUSE")
    /* UNUSED=249 */					CELLOP_AS_ARRAY, 0,						XLNAME("")
    /* UNUSED=250 */					CELLOP_AS_ARRAY, 0,						XLNAME("")
    /* FUNC_RESUME=251 */               CELLOP_AS_VALUE, A_0_OR_1 | A_MACRO,    XLNAME("RESUME")
    /* FUNC_FREQUENCY=252 */            CELLOP_AS_ARRAY, A_2,                   XLNAME("FREQUENCY")
    /* FUNC_ADDTOOLBAR=253 */           CELLOP_AS_VALUE, A_0_TO_2 | A_MACRO,    XLNAME("ADDTOOLBAR")
    /* FUNC_DELETETOOLBAR=254 */        CELLOP_AS_VALUE, A_1 | A_MACRO,         XLNAME("DELETETOOLBAR")
    /* FUNC_UDF=255 */                  CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("")
    /* FUNC_RESETTOOLBAR=256 */         CELLOP_AS_VALUE, A_1 | A_MACRO,         XLNAME("RESETTOOLBAR")
    /* FUNC_EVALUATE=257 */             CELLOP_AS_VALUE, A_1 | A_MACRO,         XLNAME("EVALUATE")
    /* FUNC_GETTOOLBAR=258 */           CELLOP_AS_VALUE, A_1_OR_2 | A_MACRO,    XLNAME("GETTOOLBAR")
    /* FUNC_GETTOOL=259 */              CELLOP_AS_VALUE, A_1_TO_3 | A_MACRO,    XLNAME("GETTOOL")
    /* FUNC_SPELLINGCHECK=260 */        CELLOP_AS_VALUE, A_1_TO_3 | A_MACRO,    XLNAME("SPELLINGCHECK")
    /* FUNC_ERRORTYPE=261 */            CELLOP_AS_VALUE, A_1,                   XLNAME("ERRORTYPE")
    /* FUNC_APPTITLE=262 */             CELLOP_AS_VALUE, A_0_OR_1 | A_MACRO,    XLNAME("APPTITLE")
    /* FUNC_WINDOWTITLE=263 */          CELLOP_AS_VALUE, A_0_OR_1 | A_MACRO,    XLNAME("WINDOWTITLE")
    /* FUNC_SAVETOOLBAR=264 */          CELLOP_AS_VALUE, A_0_TO_2 | A_MACRO,    XLNAME("SAVETOOLBAR")
    /* FUNC_ENABLETOOL=265 */           CELLOP_AS_VALUE, A_3 | A_MACRO,         XLNAME("ENABLETOOL")
    /* FUNC_PRESSTOOL=266 */            CELLOP_AS_VALUE, A_3 | A_MACRO,         XLNAME("PRESSTOOL")
    /* FUNC_REGISTERID=267 */           CELLOP_AS_VALUE, A_2_OR_3 | A_MACRO,    XLNAME("REGISTERID")
    /* FUNC_GETWORKBOOK=268 */          CELLOP_AS_VALUE, A_1_OR_2 | A_MACRO,    XLNAME("GETWORKBOOK")
    /* FUNC_AVEDEV=269 */               CELLOP_AS_VALUE, A_1_OR_MORE,           XLNAME("AVEDEV")
    /* FUNC_BETADIST=270 */             CELLOP_AS_VALUE, A_3_TO_5,              XLNAME("BETADIST")
    /* FUNC_GAMMALN=271 */              CELLOP_AS_VALUE, A_1,                   XLNAME("GAMMALN")
    /* FUNC_BETAINV=272 */              CELLOP_AS_VALUE, A_3_TO_5,              XLNAME("BETAINV")
    /* FUNC_BINOMDIST=273 */            CELLOP_AS_VALUE, A_4,                   XLNAME("BINOMDIST")
    /* FUNC_CHIDIST=274 */              CELLOP_AS_VALUE, A_2,                   XLNAME("CHIDIST")
    /* FUNC_CHIINV=275 */               CELLOP_AS_VALUE, A_2,                   XLNAME("CHIINV")
    /* FUNC_COMBIN=276 */               CELLOP_AS_VALUE, A_2,                   XLNAME("COMBIN")
    /* FUNC_CONFIDENCE=277 */           CELLOP_AS_VALUE, A_3,                   XLNAME("CONFIDENCE")
    /* FUNC_CRITBINOM=278 */            CELLOP_AS_VALUE, A_3,                   XLNAME("CRITBINOM")
    /* FUNC_EVEN=279 */                 CELLOP_AS_VALUE, A_1,                   XLNAME("EVEN")
    /* FUNC_EXPONDIST=280 */            CELLOP_AS_VALUE, A_3,                   XLNAME("EXPONDIST")
    /* FUNC_FDIST=281 */                CELLOP_AS_VALUE, A_3,                   XLNAME("FDIST")
    /* FUNC_FINV=282 */                 CELLOP_AS_VALUE, A_3,                   XLNAME("FINV")
    /* FUNC_FISHER=283 */               CELLOP_AS_VALUE, A_1,                   XLNAME("FISHER")
    /* FUNC_FISHERINV=284 */            CELLOP_AS_VALUE, A_1,                   XLNAME("FISHERINV")
    /* FUNC_FLOOR=285 */                CELLOP_AS_VALUE, A_2,                   XLNAME("FLOOR")
    /* FUNC_GAMMADIST=286 */            CELLOP_AS_VALUE, A_4,                   XLNAME("GAMMADIST")
    /* FUNC_GAMMAINV=287 */             CELLOP_AS_VALUE, A_3,                   XLNAME("GAMMAINV")
    /* FUNC_CEILING=288 */              CELLOP_AS_VALUE, A_2,                   XLNAME("CEILING")
    /* FUNC_HYPGEOMDIST=289 */          CELLOP_AS_VALUE, A_4,                   XLNAME("HYPGEOMDIST")
    /* FUNC_LOGNORMDIST=290 */          CELLOP_AS_VALUE, A_3,                   XLNAME("LOGNORMDIST")
    /* FUNC_LOGINV=291 */               CELLOP_AS_VALUE, A_3,                   XLNAME("LOGINV")
    /* FUNC_NEGBINOMDIST=292 */         CELLOP_AS_VALUE, A_3,                   XLNAME("NEGBINOMDIST")
    /* FUNC_NORMDIST=293 */             CELLOP_AS_VALUE, A_4,                   XLNAME("NORMDIST")
    /* FUNC_NORMSDIST=294 */            CELLOP_AS_VALUE, A_1,                   XLNAME("NORMSDIST")
    /* FUNC_NORMINV=295 */              CELLOP_AS_VALUE, A_3,                   XLNAME("NORMINV")
    /* FUNC_NORMSINV=296 */             CELLOP_AS_VALUE, A_1,                   XLNAME("NORMSINV")
    /* FUNC_STANDARDIZE=297 */          CELLOP_AS_VALUE, A_3,                   XLNAME("STANDARDIZE")
    /* FUNC_ODD=298 */                  CELLOP_AS_VALUE, A_1,                   XLNAME("ODD")
    /* FUNC_PERMUT=299 */               CELLOP_AS_VALUE, A_2,                   XLNAME("PERMUT")
    /* FUNC_POISSON=300 */              CELLOP_AS_VALUE, A_3,                   XLNAME("POISSON")
    /* FUNC_TDIST=301 */                CELLOP_AS_VALUE, A_3,                   XLNAME("TDIST")
    /* FUNC_WEIBULL=302 */              CELLOP_AS_VALUE, A_4,                   XLNAME("WEIBULL")
    /* FUNC_SUMXMY2=303 */              CELLOP_AS_VALUE, A_2,                   XLNAME("SUMXMY2")
    /* FUNC_SUMX2MY2=304 */             CELLOP_AS_VALUE, A_2,                   XLNAME("SUMX2MY2")
    /* FUNC_SUMX2PY2=305 */             CELLOP_AS_VALUE, A_2,                   XLNAME("SUMX2PY2")
    /* FUNC_CHITEST=306 */              CELLOP_AS_VALUE, A_2,                   XLNAME("CHITEST")
    /* FUNC_CORREL=307 */               CELLOP_AS_VALUE, A_2,                   XLNAME("CORREL")
    /* FUNC_COVAR=308 */                CELLOP_AS_VALUE, A_2,                   XLNAME("COVAR")
    /* FUNC_FORECAST=309 */             CELLOP_AS_VALUE, A_3,                   XLNAME("FORECAST")
    /* FUNC_FTEST=310 */                CELLOP_AS_VALUE, A_2,                   XLNAME("FTEST")
    /* FUNC_INTERCEPT=311 */            CELLOP_AS_VALUE, A_2,                   XLNAME("INTERCEPT")
    /* FUNC_PEARSON=312 */              CELLOP_AS_VALUE, A_2,                   XLNAME("PEARSON")
    /* FUNC_RSQ=313 */                  CELLOP_AS_VALUE, A_2,                   XLNAME("RSQ")
    /* FUNC_STEYX=314 */                CELLOP_AS_VALUE, A_2,                   XLNAME("STEYX")
    /* FUNC_SLOPE=315 */                CELLOP_AS_VALUE, A_2,                   XLNAME("SLOPE")
    /* FUNC_TTEST=316 */                CELLOP_AS_VALUE, A_4,                   XLNAME("TTEST")
    /* FUNC_PROB=317 */                 CELLOP_AS_VALUE, A_3_OR_4,              XLNAME("PROB")
    /* FUNC_DEVSQ=318 */                CELLOP_AS_VALUE, A_1_OR_MORE,           XLNAME("DEVSQ")
    /* FUNC_GEOMEAN=319 */              CELLOP_AS_VALUE, A_1_OR_MORE,           XLNAME("GEOMEAN")
    /* FUNC_HARMEAN=320 */              CELLOP_AS_VALUE, A_1_OR_MORE,           XLNAME("HARMEAN")
    /* FUNC_SUMSQ=321 */                CELLOP_AS_VALUE, A_1_OR_MORE,           XLNAME("SUMSQ")
    /* FUNC_KURT=322 */                 CELLOP_AS_VALUE, A_1_OR_MORE,           XLNAME("KURT")
    /* FUNC_SKEW=323 */                 CELLOP_AS_VALUE, A_1_OR_MORE,           XLNAME("SKEW")
    /* FUNC_ZTEST=324 */                CELLOP_AS_VALUE, A_2_OR_3,              XLNAME("ZTEST")
    /* FUNC_LARGE=325 */                CELLOP_AS_VALUE, A_2,                   XLNAME("LARGE")
    /* FUNC_SMALL=326 */                CELLOP_AS_VALUE, A_2,                   XLNAME("SMALL")
    /* FUNC_QUARTILE=327 */             CELLOP_AS_VALUE, A_2,                   XLNAME("QUARTILE")
    /* FUNC_PERCENTILE=328 */           CELLOP_AS_VALUE, A_2,                   XLNAME("PERCENTILE")
    /* FUNC_PERCENTRANK=329 */          CELLOP_AS_VALUE, A_2_OR_3,              XLNAME("PERCENTRANK")
    /* FUNC_MODE=330 */                 CELLOP_AS_VALUE, A_1_OR_MORE,           XLNAME("MODE")
    /* FUNC_TRIMMEAN=331 */             CELLOP_AS_VALUE, A_2,                   XLNAME("TRIMMEAN")
    /* FUNC_TINV=332 */                 CELLOP_AS_VALUE, A_2,                   XLNAME("TINV")
    /* UNUSED=333 */					CELLOP_AS_ARRAY, 0,						XLNAME("")
    /* FUNC_MOVIECOMMAND=334 */         CELLOP_AS_VALUE, A_3_OR_4 | A_MACRO,    XLNAME("MOVIECOMMAND")
    /* FUNC_GETMOVIE=335 */             CELLOP_AS_VALUE, A_3_OR_4 | A_MACRO,    XLNAME("GETMOVIE")
    /* FUNC_CONCATENATE=336 */          CELLOP_AS_VALUE, A_1_OR_MORE,           XLNAME("CONCATENATE")
    /* FUNC_POWER=337 */                CELLOP_AS_VALUE, A_2,                   XLNAME("POWER")
    /* FUNC_PIVOTADDDATA=338 */         CELLOP_AS_VALUE, A_2_TO_9 | A_MACRO,    XLNAME("PIVOTADDDATA")
    /* FUNC_GETPIVOTTABLE=339 */        CELLOP_AS_VALUE, A_1_OR_2 | A_MACRO,    XLNAME("GETPIVOTTABLE")
    /* FUNC_GETPIVOTFIELD=340 */        CELLOP_AS_VALUE, A_1_TO_3 | A_MACRO,    XLNAME("GETPIVOTFIELD")
    /* FUNC_GETPIVOTITEM=341 */         CELLOP_AS_VALUE, A_1_TO_4 | A_MACRO,    XLNAME("GETPIVOTITEM")
    /* FUNC_RADIANS=342 */              CELLOP_AS_VALUE, A_1,                   XLNAME("RADIANS")
    /* FUNC_DEGREES=343 */              CELLOP_AS_VALUE, A_1,                   XLNAME("DEGREES")
    /* FUNC_SUBTOTAL=344 */             CELLOP_AS_VALUE, A_2_OR_MORE,           XLNAME("SUBTOTAL")
    /* FUNC_SUMIF=345 */                CELLOP_AS_VALUE, A_2_OR_3,              XLNAME("SUMIF")
    /* FUNC_COUNTIF=346 */              CELLOP_AS_VALUE, A_2,                   XLNAME("COUNTIF")
    /* FUNC_COUNTBLANK=347 */           CELLOP_AS_VALUE, A_1,                   XLNAME("COUNTBLANK")
    /* FUNC_SCENARIOGET=348 */          CELLOP_AS_VALUE, A_1_OR_2 | A_MACRO,    XLNAME("SCENARIOGET")
    /* FUNC_OPTIONSLISTSGET=349 */      CELLOP_AS_VALUE, A_1 | A_MACRO,         XLNAME("OPTIONSLISTSGET")
    /* FUNC_ISPMT=350 */                CELLOP_AS_VALUE, A_4,                   XLNAME("ISPMT")
    /* FUNC_DATEDIF=351 */              CELLOP_AS_VALUE, A_3,                   XLNAME("DATEDIF")
    /* FUNC_DATESTRING=352 */           CELLOP_AS_VALUE, A_1,                   XLNAME("DATESTRING")
    /* FUNC_NUMBERSTRING=353 */         CELLOP_AS_VALUE, A_2,                   XLNAME("NUMBERSTRING")
    /* FUNC_ROMAN=354 */                CELLOP_AS_VALUE, A_1_OR_2,              XLNAME("ROMAN")
    /* FUNC_OPENDIALOG=355 */           CELLOP_AS_VALUE, A_0_TO_4 | A_MACRO,    XLNAME("OPENDIALOG")
    /* FUNC_SAVEDIALOG=356 */           CELLOP_AS_VALUE, A_0_TO_5 | A_MACRO,    XLNAME("SAVEDIALOG")
    /* FUNC_VIEWGET=357 */              CELLOP_AS_VALUE, A_1_OR_2 | A_MACRO,    XLNAME("VIEWGET")
    /* FUNC_GETPIVOTDATA=358 */         CELLOP_AS_VALUE, A_2_OR_MORE | A_MACRO, XLNAME("")
    /* FUNC_HYPERLINK=359 */            CELLOP_AS_VALUE, A_1_OR_2,              XLNAME("HYPERLINK")
    /* FUNC_PHONETIC=360 */             CELLOP_AS_VALUE, A_1,                   XLNAME("PHONETIC")
    /* FUNC_AVERAGEA=361 */             CELLOP_AS_VALUE, A_1_OR_MORE,           XLNAME("AVERAGEA")
    /* FUNC_MAXA=362 */                 CELLOP_AS_VALUE, A_1_OR_MORE,           XLNAME("MAXA")
    /* FUNC_MINA=363 */                 CELLOP_AS_VALUE, A_1_OR_MORE,           XLNAME("MINA")
    /* FUNC_STDEVPA=364 */              CELLOP_AS_VALUE, A_1_OR_MORE,           XLNAME("STDEVPA")
    /* FUNC_VARPA=365 */                CELLOP_AS_VALUE, A_1_OR_MORE,           XLNAME("VARPA")
    /* FUNC_STDEVA=366 */               CELLOP_AS_VALUE, A_1_OR_MORE,           XLNAME("STDEVA")
    /* FUNC_VARA=367 */                 CELLOP_AS_VALUE, A_1_OR_MORE,           XLNAME("VARA")
	// 2007 Additions
    /* FUNC_BAHTTEXT=368 */             CELLOP_AS_VALUE, A_1,                   XLNAME("BAHTTEXT")
    /* FUNC_THAIDAYOFWEEK=369 */        CELLOP_AS_VALUE, A_1,                   XLNAME("THAIDAYOFWEEK")
    /* FUNC_THAIDIGIT=370 */            CELLOP_AS_VALUE, A_1,                   XLNAME("THAIDIGIT")
    /* FUNC_THAIMONTHOFYEAR=371 */      CELLOP_AS_VALUE, A_1,                   XLNAME("THAIMONTHOFYEAR")
    /* FUNC_THAINUMSOUND=372 */         CELLOP_AS_VALUE, A_1,                   XLNAME("THAINUMSOUND")
    /* FUNC_THAINUMSTRING=373 */        CELLOP_AS_VALUE, A_1,                   XLNAME("THAINUMSTRING")
    /* FUNC_THAISTRINGLENGTH=374 */     CELLOP_AS_VALUE, A_1,                   XLNAME("THAISTRINGLENGTH")
    /* FUNC_ISTHAIDIGIT=375 */          CELLOP_AS_VALUE, A_1,                   XLNAME("ISTHAIDIGIT")
    /* FUNC_ROUNDBAHTDOWN=376 */        CELLOP_AS_VALUE, A_1,                   XLNAME("ROUNDBAHTDOWN")
    /* FUNC_ROUNDBAHTUP=377 */          CELLOP_AS_VALUE, A_1,                   XLNAME("ROUNDBAHTUP")
    /* FUNC_THAIYEAR=378 */             CELLOP_AS_VALUE, A_1,                   XLNAME("THAIYEAR")
    /* FUNC_RTD=379 */                  CELLOP_AS_VALUE, A_3_OR_MORE,           XLNAME("RTD")
    /* FUNC_CUBEVALUE=380 */            CELLOP_AS_VALUE, A_1_OR_MORE | A_MACRO, XLNAME("CUBEVALUE")
    /* FUNC_CUBEMEMBER=381 */           CELLOP_AS_VALUE, A_2_OR_3 | A_MACRO,    XLNAME("CUBEMEMBER")
    /* FUNC_CUBEMEMBERPROPERTY=382 */   CELLOP_AS_VALUE, A_3 | A_MACRO,         XLNAME("CUBEMEMBERPROPERTY")
    /* FUNC_CUBERANKEDMEMBER=383 */     CELLOP_AS_VALUE, A_3_OR_4 | A_MACRO,    XLNAME("CUBERANKEDMEMBER")
    /* FUNC_HEX2BIN=384 */              CELLOP_AS_VALUE, A_1_OR_2,              XLNAME("HEX2BIN")
    /* FUNC_HEX2DEC=385 */              CELLOP_AS_VALUE, A_1,                   XLNAME("HEX2DEC")
    /* FUNC_HEX2OCT=386 */              CELLOP_AS_VALUE, A_1_OR_2,              XLNAME("HEX2OCT")
    /* FUNC_DEC2BIN=387 */              CELLOP_AS_VALUE, A_1_OR_2,              XLNAME("DEC2BIN")
    /* FUNC_DEC2HEX=388 */              CELLOP_AS_VALUE, A_1_OR_2,              XLNAME("DEC2HEX")
    /* FUNC_DEC2OCT=389 */              CELLOP_AS_VALUE, A_1_OR_2,              XLNAME("DEC2OCT")
    /* FUNC_OCT2BIN=390 */              CELLOP_AS_VALUE, A_1_OR_2,              XLNAME("OCT2BIN")
    /* FUNC_OCT2HEX=391 */              CELLOP_AS_VALUE, A_1_OR_2,              XLNAME("OCT2HEX")
    /* FUNC_OCT2DEC=392 */              CELLOP_AS_VALUE, A_1,                   XLNAME("OCT2DEC")
    /* FUNC_BIN2DEC=393 */              CELLOP_AS_VALUE, A_1,                   XLNAME("BIN2DEC")
    /* FUNC_BIN2OCT=394 */              CELLOP_AS_VALUE, A_1_OR_2,              XLNAME("BIN2OCT")
    /* FUNC_BIN2HEX=395 */              CELLOP_AS_VALUE, A_1_OR_2,              XLNAME("BIN2HEX")
    /* FUNC_IMSUB=396 */                CELLOP_AS_VALUE, A_2,                   XLNAME("IMSUB")
    /* FUNC_IMDIV=397 */                CELLOP_AS_VALUE, A_2,                   XLNAME("IMDIV")
    /* FUNC_IMPOWER=398 */              CELLOP_AS_VALUE, A_2,                   XLNAME("IMPOWER")
    /* FUNC_IMABS=399 */                CELLOP_AS_VALUE, A_1,                   XLNAME("IMABS")
    /* FUNC_IMSQRT=400 */               CELLOP_AS_VALUE, A_1,                   XLNAME("IMSQRT")
    /* FUNC_IMLN=401 */                 CELLOP_AS_VALUE, A_1,                   XLNAME("IMLN")
    /* FUNC_IMLOG2=402 */               CELLOP_AS_VALUE, A_1,                   XLNAME("IMLOG2")
    /* FUNC_IMLOG10=403 */              CELLOP_AS_VALUE, A_1,                   XLNAME("IMLOG10")
    /* FUNC_IMSIN=404 */                CELLOP_AS_VALUE, A_1,                   XLNAME("IMSIN")
    /* FUNC_IMCOS=405 */                CELLOP_AS_VALUE, A_1,                   XLNAME("IMCOS")
    /* FUNC_IMEXP=406 */                CELLOP_AS_VALUE, A_1,                   XLNAME("IMEXP")
    /* FUNC_IMARGUMENT=407 */           CELLOP_AS_VALUE, A_1,                   XLNAME("IMARGUMENT")
    /* FUNC_IMCONJUGATE=408 */          CELLOP_AS_VALUE, A_1,                   XLNAME("IMCONJUGATE")
    /* FUNC_IMAGINARY=409 */            CELLOP_AS_VALUE, A_1,                   XLNAME("IMAGINARY")
    /* FUNC_IMREAL=410 */               CELLOP_AS_VALUE, A_1,                   XLNAME("IMREAL")
    /* FUNC_COMPLEX=411 */              CELLOP_AS_VALUE, A_2_OR_3,              XLNAME("COMPLEX")
    /* FUNC_IMSUM=412 */                CELLOP_AS_VALUE, A_1_OR_MORE,           XLNAME("IMSUM")
    /* FUNC_IMPRODUCT=413 */            CELLOP_AS_VALUE, A_1_OR_MORE,           XLNAME("IMPRODUCT")
    /* FUNC_SERIESSUM=414 */            CELLOP_AS_VALUE, A_4,                   XLNAME("SERIESSUM")
    /* FUNC_FACTDOUBLE=415 */           CELLOP_AS_VALUE, A_1,                   XLNAME("FACTDOUBLE")
    /* FUNC_SQRTPI=416 */               CELLOP_AS_VALUE, A_1,                   XLNAME("SQRTPI")
    /* FUNC_QUOTIENT=417 */             CELLOP_AS_VALUE, A_2,                   XLNAME("QUOTIENT")
    /* FUNC_DELTA=418 */                CELLOP_AS_VALUE, A_1_OR_2,              XLNAME("DELTA")
    /* FUNC_GESTEP=419 */               CELLOP_AS_VALUE, A_1_OR_2,              XLNAME("GESTEP")
    /* FUNC_ISEVEN=420 */               CELLOP_AS_VALUE, A_1,                   XLNAME("ISEVEN")
    /* FUNC_ISODD=421 */                CELLOP_AS_VALUE, A_1,                   XLNAME("ISODD")
    /* FUNC_MROUND=422 */               CELLOP_AS_VALUE, A_2,                   XLNAME("MROUND")
    /* FUNC_ERF=423 */                  CELLOP_AS_VALUE, A_1_OR_2,              XLNAME("ERF")
    /* FUNC_ERFC=424 */                 CELLOP_AS_VALUE, A_1,                   XLNAME("ERFC")
    /* FUNC_BESSELJ=425 */              CELLOP_AS_VALUE, A_2,                   XLNAME("BESSELJ")
    /* FUNC_BESSELK=426 */              CELLOP_AS_VALUE, A_2,                   XLNAME("BESSELK")
    /* FUNC_BESSELY=427 */              CELLOP_AS_VALUE, A_2,                   XLNAME("BESSELY")
    /* FUNC_BESSELI=428 */              CELLOP_AS_VALUE, A_2,                   XLNAME("BESSELI")
    /* FUNC_XIRR=429 */                 CELLOP_AS_VALUE, A_2_OR_3,              XLNAME("XIRR")
    /* FUNC_XNPV=430 */                 CELLOP_AS_VALUE, A_3,                   XLNAME("XNPV")
    /* FUNC_PRICEMAT=431 */             CELLOP_AS_VALUE, A_5_OR_6,              XLNAME("PRICEMAT")
    /* FUNC_YIELDMAT=432 */             CELLOP_AS_VALUE, A_5_OR_6,              XLNAME("YIELDMAT")
    /* FUNC_INTRATE=433 */              CELLOP_AS_VALUE, A_4_OR_5,              XLNAME("INTRATE")
    /* FUNC_RECEIVED=434 */             CELLOP_AS_VALUE, A_4_OR_5,              XLNAME("RECEIVED")
    /* FUNC_DISC=435 */                 CELLOP_AS_VALUE, A_4_OR_5,              XLNAME("DISC")
    /* FUNC_PRICEDISC=436 */            CELLOP_AS_VALUE, A_4_OR_5,              XLNAME("PRICEDISC")
    /* FUNC_YIELDDISC=437 */            CELLOP_AS_VALUE, A_4_OR_5,              XLNAME("YIELDDISC")
    /* FUNC_TBILLEQ=438 */              CELLOP_AS_VALUE, A_3,                   XLNAME("TBILLEQ")
    /* FUNC_TBILLPRICE=439 */           CELLOP_AS_VALUE, A_3,                   XLNAME("TBILLPRICE")
    /* FUNC_TBILLYIELD=440 */           CELLOP_AS_VALUE, A_3,                   XLNAME("TBILLYIELD")
    /* FUNC_PRICE=441 */                CELLOP_AS_VALUE, A_6_OR_7,              XLNAME("PRICE")
    /* FUNC_YIELD=442 */                CELLOP_AS_VALUE, A_6_OR_7,              XLNAME("YIELD")
    /* FUNC_DOLLARDE=443 */             CELLOP_AS_VALUE, A_2,                   XLNAME("DOLLARDE")
    /* FUNC_DOLLARFR=444 */             CELLOP_AS_VALUE, A_2,                   XLNAME("DOLLARFR")
    /* FUNC_NOMINAL=445 */              CELLOP_AS_VALUE, A_2,                   XLNAME("NOMINAL")
    /* FUNC_EFFECT=446 */               CELLOP_AS_VALUE, A_2,                   XLNAME("EFFECT")
    /* FUNC_CUMPRINC=447 */             CELLOP_AS_VALUE, A_6,                   XLNAME("CUMPRINC")
    /* FUNC_CUMIPMT=448 */              CELLOP_AS_VALUE, A_6,                   XLNAME("CUMIPMT")
    /* FUNC_EDATE=449 */                CELLOP_AS_VALUE, A_2,                   XLNAME("EDATE")
    /* FUNC_EOMONTH=450 */              CELLOP_AS_VALUE, A_2,                   XLNAME("EOMONTH")
    /* FUNC_YEARFRAC=451 */             CELLOP_AS_VALUE, A_2_OR_3,              XLNAME("YEARFRAC")
    /* FUNC_COUPDAYBS=452 */            CELLOP_AS_VALUE, A_3_OR_4,              XLNAME("COUPDAYBS")
    /* FUNC_COUPDAYS=453 */             CELLOP_AS_VALUE, A_3_OR_4,              XLNAME("COUPDAYS")
    /* FUNC_COUPDAYSNC=454 */           CELLOP_AS_VALUE, A_3_OR_4,              XLNAME("COUPDAYSNC")
    /* FUNC_COUPNCD=455 */              CELLOP_AS_VALUE, A_3_OR_4,              XLNAME("COUPNCD")
    /* FUNC_COUPNUM=456 */              CELLOP_AS_VALUE, A_3_OR_4,              XLNAME("COUPNUM")
    /* FUNC_COUPPCD=457 */              CELLOP_AS_VALUE, A_3_OR_4,              XLNAME("COUPPCD")
    /* FUNC_DURATION=458 */             CELLOP_AS_VALUE, A_5_OR_6,              XLNAME("DURATION")
    /* FUNC_MDURATION=459 */            CELLOP_AS_VALUE, A_5_OR_6,              XLNAME("MDURATION")
    /* FUNC_ODDLPRICE=460 */            CELLOP_AS_VALUE, A_7_OR_8,              XLNAME("ODDLPRICE")
    /* FUNC_ODDLYIELD=461 */            CELLOP_AS_VALUE, A_7_OR_8,              XLNAME("ODDLYIELD")
    /* FUNC_ODDFPRICE=462 */            CELLOP_AS_VALUE, A_8_OR_9,              XLNAME("ODDFPRICE")
    /* FUNC_ODDFYIELD=463 */            CELLOP_AS_VALUE, A_8_OR_9,              XLNAME("ODDFYIELD")
    /* FUNC_RANDBETWEEN=464 */          CELLOP_AS_VALUE, A_2,                   XLNAME("RANDBETWEEN")
    /* FUNC_WEEKNUM=465 */              CELLOP_AS_VALUE, A_1_OR_2,              XLNAME("WEEKNUM")
    /* FUNC_AMORDEGRC=466 */            CELLOP_AS_VALUE, A_6_OR_7,              XLNAME("AMORDEGRC")
    /* FUNC_AMORLINC=467 */             CELLOP_AS_VALUE, A_6_OR_7,              XLNAME("AMORLINC")
    /* FUNC_CONVERT=468 */              CELLOP_AS_VALUE, A_3,                   XLNAME("CONVERT")
    /* FUNC_ACCRINT=469 */              CELLOP_AS_VALUE, A_6_TO_8,              XLNAME("ACCRINT")
    /* FUNC_ACCRINTM=470 */             CELLOP_AS_VALUE, A_4_OR_5,              XLNAME("ACCRINTM")
    /* FUNC_WORKDAY=471 */              CELLOP_AS_VALUE, A_2_OR_3,              XLNAME("WORKDAY")
    /* FUNC_NETWORKDAYS=472 */          CELLOP_AS_VALUE, A_2_OR_3,              XLNAME("NETWORKDAYS")
    /* FUNC_GCD=473 */                  CELLOP_AS_VALUE, A_1_OR_MORE,           XLNAME("GCD")
    /* FUNC_MULTINOMIAL=474 */          CELLOP_AS_VALUE, A_1_OR_MORE,           XLNAME("MULTINOMIAL")
    /* FUNC_LCM=475 */                  CELLOP_AS_VALUE, A_1_OR_MORE,           XLNAME("LCM")
    /* FUNC_FVSCHEDULE=476 */           CELLOP_AS_VALUE, A_2,                   XLNAME("FVSCHEDULE")
    /* FUNC_CUBEKPIMEMBER=477 */        CELLOP_AS_VALUE, A_3_OR_4 | A_MACRO,    XLNAME("CUBEKPIMEMBER")
    /* FUNC_CUBESET=478 */              CELLOP_AS_VALUE, A_2_TO_5 | A_MACRO,    XLNAME("CUBESET")
    /* FUNC_CUBESETCOUNT=479 */         CELLOP_AS_VALUE, A_1,                   XLNAME("CUBESETCOUNT")
    /* FUNC_IFERROR=480 */              CELLOP_AS_VALUE, A_2,                   XLNAME("IFERROR")
    /* FUNC_COUNTIFS=481 */             CELLOP_AS_VALUE, A_2_OR_MORE,           XLNAME("COUNTIFS")
    /* FUNC_SUMIFS=482 */               CELLOP_AS_VALUE, A_3_OR_MORE,           XLNAME("SUMIFS")
    /* FUNC_AVERAGEIF=483 */            CELLOP_AS_VALUE, A_2_OR_3,              XLNAME("AVERAGEIF")
    /* FUNC_AVERAGEIFS=484 */           CELLOP_AS_VALUE, A_3_OR_MORE,           XLNAME("AVERAGEIFS")
#if 0
	// Introduced in 2010 with the xml Excel version
    /* FUNC_AGGREGATE=485 */            CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("AGGREGATE")
    /* FUNC_BINOM_DIST=486 */           CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("BINOM_DIST")
    /* FUNC_BINOM_INV=487 */            CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("BINOM_INV")
    /* FUNC_CONFIDENCE_NORM=488 */      CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("CONFIDENCE_NORM")
    /* FUNC_CONFIDENCE_T=489 */         CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("CONFIDENCE_T")
    /* FUNC_CHISQ_TEST=490 */           CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("CHISQ_TEST")
    /* FUNC_F_TEST=491 */               CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("F_TEST")
    /* FUNC_COVARIANCE_P=492 */         CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("COVARIANCE_P")
    /* FUNC_COVARIANCE_S=493 */         CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("COVARIANCE_S")
    /* FUNC_EXPON_DIST=494 */           CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("EXPON_DIST")
    /* FUNC_GAMMA_DIST=495 */           CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("GAMMA_DIST")
    /* FUNC_GAMMA_INV=496 */            CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("GAMMA_INV")
    /* FUNC_MODE_MULT=497 */            CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("MODE_MULT")
    /* FUNC_MODE_SNGL=498 */            CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("MODE_SNGL")
    /* FUNC_NORM_DIST=499 */            CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("NORM_DIST")
    /* FUNC_NORM_INV=500 */             CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("NORM_INV")
    /* FUNC_PERCENTILE_EXC=501 */       CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("PERCENTILE_EXC")
    /* FUNC_PERCENTILE_INC=502 */       CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("PERCENTILE_INC")
    /* FUNC_PERCENTRANK_EXC=503 */      CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("PERCENTRANK_EXC")
    /* FUNC_PERCENTRANK_INC=504 */      CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("PERCENTRANK_INC")
    /* FUNC_POISSON_DIST=505 */         CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("POISSON_DIST")
    /* FUNC_QUARTILE_EXC=506 */         CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("QUARTILE_EXC")
    /* FUNC_QUARTILE_INC=507 */         CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("QUARTILE_INC")
    /* FUNC_RANK_AVG=508 */             CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("RANK_AVG")
    /* FUNC_RANK_EQ=509 */              CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("RANK_EQ")
    /* FUNC_STDEV_S=510 */              CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("STDEV_S")
    /* FUNC_STDEV_P=511 */              CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("STDEV_P")
    /* FUNC_T_DIST=512 */               CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("T_DIST")
    /* FUNC_T_DIST_2T=513 */            CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("T_DIST_2T")
    /* FUNC_T_DIST_RT=514 */            CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("T_DIST_RT")
    /* FUNC_T_INV=515 */                CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("T_INV")
    /* FUNC_T_INV_2T=516 */             CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("T_INV_2T")
    /* FUNC_VAR_S=517 */                CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("VAR_S")
    /* FUNC_VAR_P=518 */                CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("VAR_P")
    /* FUNC_WEIBULL_DIST=519 */         CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("WEIBULL_DIST")
    /* FUNC_NETWORKDAYS_INTL=520 */     CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("NETWORKDAYS_INTL")
    /* FUNC_WORKDAY_INTL=521 */         CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("WORKDAY_INTL")
    /* FUNC_ECMA_CEILING=522 */         CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("ECMA_CEILING")
    /* FUNC_ISO_CEILING=523 */          CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("ISO_CEILING")
    /* UNUSED=524 */					CELLOP_AS_ARRAY, 0,						XLNAME("")
    /* FUNC_BETA_DIST=525 */            CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("BETA_DIST")
    /* FUNC_BETA_INV=526 */             CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("BETA_INV")
    /* FUNC_CHISQ_DIST=527 */           CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("CHISQ_DIST")
    /* FUNC_CHISQ_DIST_RT=528 */        CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("CHISQ_DIST_RT")
    /* FUNC_CHISQ_INV=529 */            CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("CHISQ_INV")
    /* FUNC_CHISQ_INV_RT=530 */         CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("CHISQ_INV_RT")
    /* FUNC_F_DIST=531 */               CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("F_DIST")
    /* FUNC_F_DIST_RT=532 */            CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("F_DIST_RT")
    /* FUNC_F_INV=533 */                CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("F_INV")
    /* FUNC_F_INV_RT=534 */             CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("F_INV_RT")
    /* FUNC_HYPGEOM_DIST=535 */         CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("HYPGEOM_DIST")
    /* FUNC_LOGNORM_DIST=536 */         CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("LOGNORM_DIST")
    /* FUNC_LOGNORM_INV=537 */          CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("LOGNORM_INV")
    /* FUNC_NEGBINOM_DIST=538 */        CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("NEGBINOM_DIST")
    /* FUNC_NORM_S_DIST=539 */          CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("NORM_S_DIST")
    /* FUNC_NORM_S_INV=540 */           CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("NORM_S_INV")
    /* FUNC_T_TEST=541 */               CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("T_TEST")
    /* FUNC_Z_TEST=542 */               CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("Z_TEST")
    /* FUNC_ERF_PRECISE=543 */          CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("ERF_PRECISE")
    /* FUNC_ERFC_PRECISE=544 */         CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("ERFC_PRECISE")
    /* FUNC_GAMMALN_PRECISE=545 */      CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("GAMMALN_PRECISE")
    /* FUNC_CEILING_PRECISE=546 */      CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("CEILING_PRECISE")
    /* FUNC_FLOOR_PRECISE=547 */        CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("FLOOR_PRECISE")
#endif
};

unsigned32_t xlslib_core::NumberOfArgsForExcelFunction(expr_function_code_t code)
{
	if(code >= maxProperties) return A_UNKNOWN;
	return function_properties[code].num_args;
}

static function_property PropertyForForExcelFunction(expr_function_code_t code)
{
	if(code >= maxProperties) {
		function_property p = (function_property) {CELLOP_AS_VALUE, A_UNKNOWN};
		return p;
	}
	return function_properties[code];
}
