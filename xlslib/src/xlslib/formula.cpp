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

signed8_t formula_t::PushCellReference(const cell_t& cell, cell_addr_mode_t opt)
{
	signed8_t ret;
	unsigned32_t idx;

	idx = cell.GetWorksheet() ? cell.GetWorksheet()->GetIndex() : invalidIndex;
	ret = PushReference(cell.GetRow(), cell.GetCol(), idx, opt);
	return ret;
}

signed8_t formula_t::PushReference(unsigned32_t row, unsigned32_t col, unsigned32_t idx, cell_addr_mode_t opt)
{
    signed8_t errcode = NO_ERRORS;

    if (m_Worksheet == NULL || idx == invalidIndex || idx == m_Worksheet->GetIndex()) {
        errcode |= main_data->AddValue8(OP_REFV);
        col &= 0x3FFF;
    } else {
        errcode |= main_data->AddValue8(OP_REF3DV);
        errcode |= main_data->AddValue16(static_cast<unsigned16_t>(idx));
        col &= 0x00FF;
    }
    errcode |= main_data->AddValue16(static_cast<unsigned16_t>(row));

    XL_ASSERT((opt & ~0xC000) == 0);
    col |= opt & 0xC000;
    errcode |= main_data->AddValue16(static_cast<unsigned16_t>(col));

	return errcode;
}

signed8_t formula_t::PushCellAreaReference(const cell_t& upper_left_cell, const cell_t& lower_right_cell, cell_addr_mode_t opt)
{
	signed8_t ret;
	unsigned32_t ul_idx, lr_idx;

	ul_idx = upper_left_cell.GetWorksheet() ? upper_left_cell.GetWorksheet()->GetIndex() : invalidIndex;
	lr_idx = lower_right_cell.GetWorksheet() ? lower_right_cell.GetWorksheet()->GetIndex() : invalidIndex;

	ret = PushAreaReference(upper_left_cell.GetRow(), upper_left_cell.GetCol(), ul_idx, lower_right_cell.GetRow(), lower_right_cell.GetCol(), lr_idx, opt);
	return ret;
}

signed8_t formula_t::PushAreaReference(unsigned32_t ul_row, unsigned32_t ul_col, unsigned32_t ul_idx, unsigned32_t lr_row, unsigned32_t lr_col, unsigned32_t lr_idx, cell_addr_mode_t opt)
{
    signed8_t errcode = NO_ERRORS;
	(void)lr_idx;	// prevent warning

    if (m_Worksheet == NULL || ul_idx == invalidIndex || ul_idx == m_Worksheet->GetIndex()) {
        errcode |= main_data->AddValue8(OP_AREAA); // OP_AREA. OP_AREAV, OP_AREAA
        ul_col &= 0x3FFF;
        lr_col &= 0x3FFF;
    } else {
        errcode |= main_data->AddValue8(OP_AREA3DA); // OP_AREA. OP_AREAV, OP_AREAA
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

signed8_t formula_t::PushFunction(expr_function_code_t func) {
	function_property func_prop = PropertyForForExcelFunction(func);
	unsigned32_t argcntmask = func_prop.num_args;
	unsigned8_t op = (OP_FUNC & ~CELLOP_AS_REFER) | func_prop.op_class;
if(func == FUNC_IF) op |= CELLOP_AS_ARRAY;
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

signed8_t formula_t::PushFunction(expr_function_code_t func, size_t argcount) {
	function_property func_prop = PropertyForForExcelFunction(func);
	unsigned32_t argcntmask = func_prop.num_args;
	unsigned8_t op = (OP_FUNCVAR & ~CELLOP_AS_REFER) | func_prop.op_class;
if(func == FUNC_IF) op |= CELLOP_AS_ARRAY;
    signed8_t errcode = NO_ERRORS;
    if (argcntmask == A_UNKNOWN || (argcntmask & ~(1U << argcount))) {
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


static const uint32_t maxProperties = 368; // 548 if you enable newer functions than were defined since BIFF8.

// Notes: the function_properties table was build by combining Ger's original switch statement for the number of arguments with
//    a second table mapping the 'FUNC_...' values to the Excel English function name (search for "function_arr"). The CELLOP_AS_
//    values come mostly from the OpenOffice "MIcrosoft Excel File Format" Table 3.11.1 (I could not find the same thing within
//    MS's own document!). I disagreed with several of the OpenOffice values and overrode them (for instance, IF does not return
//    a reference!)

#define XLNAME(x)	// this can be turned on to add a name to the table, when used for diagnostics
static function_property function_properties[maxProperties] = {
    /* FUNC_COUNT */            CELLOP_AS_VALUE, A_1_OR_MORE,           XLNAME("COUNT")
    /* FUNC_IF */               CELLOP_AS_VALUE, A_2_OR_3,              XLNAME("IF")
    /* FUNC_ISNA */             CELLOP_AS_VALUE, A_1,                   XLNAME("ISNA")
    /* FUNC_ISERROR */          CELLOP_AS_VALUE, A_1,                   XLNAME("ISERROR")
    /* FUNC_SUM */              CELLOP_AS_VALUE, A_1_OR_MORE,           XLNAME("SUM")
    /* FUNC_AVERAGE */          CELLOP_AS_VALUE, A_1_OR_MORE,           XLNAME("AVERAGE")
    /* FUNC_MIN */              CELLOP_AS_VALUE, A_1_OR_MORE,           XLNAME("MIN")
    /* FUNC_MAX */              CELLOP_AS_VALUE, A_1_OR_MORE,           XLNAME("MAX")
    /* FUNC_ROW */              CELLOP_AS_VALUE, A_0_OR_1,              XLNAME("ROW")
    /* FUNC_COLUMN */           CELLOP_AS_VALUE, A_0_OR_1,              XLNAME("COLUMN")
    /* FUNC_NA */               CELLOP_AS_VALUE, A_0,                   XLNAME("NA")
    /* FUNC_NPV */              CELLOP_AS_VALUE, A_2_OR_MORE,           XLNAME("NPV")
    /* FUNC_STDEV */            CELLOP_AS_VALUE, A_1_OR_MORE,           XLNAME("STDEV")
    /* FUNC_DOLLAR */           CELLOP_AS_VALUE, A_1_OR_2,              XLNAME("DOLLAR")
    /* FUNC_FIXED */            CELLOP_AS_VALUE, A_1_TO_3,              XLNAME("FIXED")
    /* FUNC_SIN */              CELLOP_AS_VALUE, A_1,                   XLNAME("SIN")
    /* FUNC_COS */              CELLOP_AS_VALUE, A_1,                   XLNAME("COS")
    /* FUNC_TAN */              CELLOP_AS_VALUE, A_1,                   XLNAME("TAN")
    /* FUNC_ATAN */             CELLOP_AS_VALUE, A_1,                   XLNAME("ATAN")
    /* FUNC_PI */               CELLOP_AS_VALUE, A_0,                   XLNAME("PI")
    /* FUNC_SQRT */             CELLOP_AS_VALUE, A_1,                   XLNAME("SQRT")
    /* FUNC_EXP */              CELLOP_AS_VALUE, A_1,                   XLNAME("EXP")
    /* FUNC_LN */               CELLOP_AS_VALUE, A_1,                   XLNAME("LN")
    /* FUNC_LOG10 */            CELLOP_AS_VALUE, A_1,                   XLNAME("LOG10")
    /* FUNC_ABS */              CELLOP_AS_VALUE, A_1,                   XLNAME("ABS")
    /* FUNC_INT */              CELLOP_AS_VALUE, A_1,                   XLNAME("INT")
    /* FUNC_SIGN */             CELLOP_AS_VALUE, A_1,                   XLNAME("SIGN")
    /* FUNC_ROUND */            CELLOP_AS_VALUE, A_2,                   XLNAME("ROUND")
    /* FUNC_LOOKUP */           CELLOP_AS_VALUE, A_2_OR_3,              XLNAME("LOOKUP")
    /* FUNC_INDEX */            CELLOP_AS_VALUE, A_2_TO_4,              XLNAME("INDEX")
    /* FUNC_REPT */             CELLOP_AS_VALUE, A_2,                   XLNAME("REPT")
    /* FUNC_MID */              CELLOP_AS_VALUE, A_3,                   XLNAME("MID")
    /* FUNC_LEN */              CELLOP_AS_VALUE, A_1,                   XLNAME("LEN")
    /* FUNC_VALUE */            CELLOP_AS_VALUE, A_1,                   XLNAME("VALUE")
    /* FUNC_TRUE */             CELLOP_AS_VALUE, A_0,                   XLNAME("TRUE")
    /* FUNC_FALSE */            CELLOP_AS_VALUE, A_0,                   XLNAME("FALSE")
    /* FUNC_AND */              CELLOP_AS_VALUE, A_1_OR_MORE,           XLNAME("AND")
    /* FUNC_OR */               CELLOP_AS_VALUE, A_1_OR_MORE,           XLNAME("OR")
    /* FUNC_NOT */              CELLOP_AS_VALUE, A_1,                   XLNAME("NOT")
    /* FUNC_MOD */              CELLOP_AS_VALUE, A_2,                   XLNAME("MOD")
    /* FUNC_DCOUNT */           CELLOP_AS_VALUE, A_3,                   XLNAME("DCOUNT")
    /* FUNC_DSUM */             CELLOP_AS_VALUE, A_3,                   XLNAME("DSUM")
    /* FUNC_DAVERAGE */         CELLOP_AS_VALUE, A_3,                   XLNAME("DAVERAGE")
    /* FUNC_DMIN */             CELLOP_AS_VALUE, A_3,                   XLNAME("DMIN")
    /* FUNC_DMAX */             CELLOP_AS_VALUE, A_3,                   XLNAME("DMAX")
    /* FUNC_DSTDEV */           CELLOP_AS_VALUE, A_3,                   XLNAME("DSTDEV")
    /* FUNC_VAR */              CELLOP_AS_VALUE, A_1_OR_MORE,           XLNAME("VAR")
    /* FUNC_DVAR */             CELLOP_AS_VALUE, A_3,                   XLNAME("DVAR")
    /* FUNC_TEXT */             CELLOP_AS_VALUE, A_2,                   XLNAME("TEXT")
    /* FUNC_LINEST */           CELLOP_AS_VALUE, A_1_TO_4,              XLNAME("LINEST")
    /* FUNC_TREND */            CELLOP_AS_VALUE, A_1_TO_4,              XLNAME("TREND")
    /* FUNC_LOGEST */           CELLOP_AS_VALUE, A_1_TO_4,              XLNAME("LOGEST")
    /* FUNC_GROWTH */           CELLOP_AS_VALUE, A_1_TO_4,              XLNAME("GROWTH")
    /* FUNC_GOTO */             CELLOP_AS_VALUE, A_1 | A_MACRO,         XLNAME("GOTO")
    /* FUNC_HALT */             CELLOP_AS_VALUE, A_0_OR_1 | A_MACRO,    XLNAME("HALT")
    /* FUNC_PV */               CELLOP_AS_VALUE, A_3_TO_5,              XLNAME("PV")
    /* FUNC_FV */               CELLOP_AS_VALUE, A_3_TO_5,              XLNAME("FV")
    /* FUNC_NPER */             CELLOP_AS_VALUE, A_3_TO_5,              XLNAME("NPER")
    /* FUNC_PMT */              CELLOP_AS_VALUE, A_3_TO_5,              XLNAME("PMT")
    /* FUNC_RATE */             CELLOP_AS_VALUE, A_3_TO_6,              XLNAME("RATE")
    /* FUNC_MIRR */             CELLOP_AS_VALUE, A_3,                   XLNAME("MIRR")
    /* FUNC_IRR */              CELLOP_AS_VALUE, A_1_OR_2,              XLNAME("IRR")
    /* FUNC_RAND */             CELLOP_AS_VALUE, A_0,                   XLNAME("RAND")
    /* FUNC_MATCH */            CELLOP_AS_VALUE, A_2_OR_3,              XLNAME("MATCH")
    /* FUNC_DATE */             CELLOP_AS_VALUE, A_3,                   XLNAME("DATE")
    /* FUNC_TIME */             CELLOP_AS_VALUE, A_3,                   XLNAME("TIME")
    /* FUNC_DAY */              CELLOP_AS_VALUE, A_1,                   XLNAME("DAY")
    /* FUNC_MONTH */            CELLOP_AS_VALUE, A_1,                   XLNAME("MONTH")
    /* FUNC_YEAR */             CELLOP_AS_VALUE, A_1,                   XLNAME("YEAR")
    /* FUNC_WEEKDAY */          CELLOP_AS_VALUE, A_1_OR_2,              XLNAME("WEEKDAY")
    /* FUNC_HOUR */             CELLOP_AS_VALUE, A_1,                   XLNAME("HOUR")
    /* FUNC_MINUTE */           CELLOP_AS_VALUE, A_1,                   XLNAME("MINUTE")
    /* FUNC_SECOND */           CELLOP_AS_VALUE, A_1,                   XLNAME("SECOND")
    /* FUNC_NOW */              CELLOP_AS_VALUE, A_0,                   XLNAME("NOW")
    /* FUNC_AREAS */            CELLOP_AS_VALUE, A_1,                   XLNAME("AREAS")
    /* FUNC_ROWS */             CELLOP_AS_VALUE, A_1,                   XLNAME("ROWS")
    /* FUNC_COLUMNS */          CELLOP_AS_VALUE, A_1,                   XLNAME("COLUMNS")
    /* FUNC_OFFSET */           CELLOP_AS_VALUE, A_3_TO_5,              XLNAME("OFFSET")
    /* FUNC_ABSREF */           CELLOP_AS_VALUE, A_2 | A_MACRO,         XLNAME("ABSREF")
    /* FUNC_RELREF */           CELLOP_AS_VALUE, A_2 | A_MACRO,         XLNAME("RELREF")
    /* FUNC_ARGUMENT */         CELLOP_AS_VALUE, A_0_TO_3 | A_MACRO,    XLNAME("ARGUMENT")
    /* FUNC_SEARCH */           CELLOP_AS_VALUE, A_2_OR_3,              XLNAME("SEARCH")
    /* FUNC_TRANSPOSE */        CELLOP_AS_VALUE, A_1,                   XLNAME("TRANSPOSE")
    /* FUNC_ERROR */            CELLOP_AS_VALUE, A_0_TO_2 | A_MACRO,    XLNAME("ERROR")
    /* FUNC_STEP */             CELLOP_AS_VALUE, A_0 | A_MACRO,         XLNAME("STEP")
    /* FUNC_TYPE */             CELLOP_AS_VALUE, A_1,                   XLNAME("TYPE")
    /* FUNC_ECHO */             CELLOP_AS_VALUE, A_0_OR_1 | A_MACRO,    XLNAME("ECHO")
    /* FUNC_SETNAME */          CELLOP_AS_VALUE, A_1_OR_2 | A_MACRO,    XLNAME("SETNAME")
    /* FUNC_CALLER */           CELLOP_AS_VALUE, A_0 | A_MACRO,         XLNAME("CALLER")
    /* FUNC_DEREF */            CELLOP_AS_VALUE, A_1 | A_MACRO,         XLNAME("DEREF")
    /* FUNC_WINDOWS */          CELLOP_AS_VALUE, A_0_TO_2 | A_MACRO,    XLNAME("WINDOWS")
    /* FUNC_SERIES */           CELLOP_AS_VALUE, A_4_OR_5,              XLNAME("SERIES")
    /* FUNC_DOCUMENTS */        CELLOP_AS_VALUE, A_0_TO_2 | A_MACRO,    XLNAME("DOCUMENTS")
    /* FUNC_ACTIVECELL */       CELLOP_AS_VALUE, A_0 | A_MACRO,         XLNAME("ACTIVECELL")
    /* FUNC_SELECTION */        CELLOP_AS_VALUE, A_0 | A_MACRO,         XLNAME("SELECTION")
    /* FUNC_RESULT */           CELLOP_AS_VALUE, A_0_OR_1 | A_MACRO,    XLNAME("RESULT")
    /* FUNC_ATAN2 */            CELLOP_AS_VALUE, A_2,                   XLNAME("ATAN2")
    /* FUNC_ASIN */             CELLOP_AS_VALUE, A_1,                   XLNAME("ASIN")
    /* FUNC_ACOS */             CELLOP_AS_VALUE, A_1,                   XLNAME("ACOS")
    /* FUNC_CHOOSE */           CELLOP_AS_VALUE, A_2_OR_MORE,           XLNAME("CHOOSE")
    /* FUNC_HLOOKUP */          CELLOP_AS_VALUE, A_3_OR_4,              XLNAME("HLOOKUP")
    /* FUNC_VLOOKUP */          CELLOP_AS_VALUE, A_3_OR_4,              XLNAME("VLOOKUP")
    /* FUNC_LINKS */            CELLOP_AS_VALUE, A_0_TO_2 | A_MACRO,    XLNAME("LINKS")
    /* FUNC_INPUT */            CELLOP_AS_VALUE, A_1_TO_7 | A_MACRO,    XLNAME("INPUT")
    /* FUNC_ISREF */            CELLOP_AS_VALUE, A_1,                   XLNAME("ISREF")
    /* FUNC_GETFORMULA */       CELLOP_AS_VALUE, A_1 | A_MACRO,         XLNAME("GETFORMULA")
    /* FUNC_GETNAME */          CELLOP_AS_VALUE, A_1_OR_2 | A_MACRO,    XLNAME("GETNAME")
    /* FUNC_SETVALUE */         CELLOP_AS_VALUE, A_2 | A_MACRO,         XLNAME("SETVALUE")
    /* FUNC_LOG */              CELLOP_AS_VALUE, A_1_OR_2,              XLNAME("LOG")
    /* FUNC_EXEC */             CELLOP_AS_VALUE, A_1_TO_4 | A_MACRO,    XLNAME("EXEC")
    /* FUNC_CHAR */             CELLOP_AS_VALUE, A_1,                   XLNAME("CHAR")
    /* FUNC_LOWER */            CELLOP_AS_VALUE, A_1,                   XLNAME("LOWER")
    /* FUNC_UPPER */            CELLOP_AS_VALUE, A_1,                   XLNAME("UPPER")
    /* FUNC_PROPER */           CELLOP_AS_VALUE, A_1,                   XLNAME("PROPER")
    /* FUNC_LEFT */             CELLOP_AS_VALUE, A_1_OR_2,              XLNAME("LEFT")
    /* FUNC_RIGHT */            CELLOP_AS_VALUE, A_1_OR_2,              XLNAME("RIGHT")
    /* FUNC_EXACT */            CELLOP_AS_VALUE, A_2,                   XLNAME("EXACT")
    /* FUNC_TRIM */             CELLOP_AS_VALUE, A_1,                   XLNAME("TRIM")
    /* FUNC_REPLACE */          CELLOP_AS_VALUE, A_4,                   XLNAME("REPLACE")
    /* FUNC_SUBSTITUTE */       CELLOP_AS_VALUE, A_3_OR_4,              XLNAME("SUBSTITUTE")
    /* FUNC_CODE */             CELLOP_AS_VALUE, A_1,                   XLNAME("CODE")
    /* FUNC_NAMES */            CELLOP_AS_VALUE, A_0_TO_3 | A_MACRO,    XLNAME("NAMES")
    /* FUNC_DIRECTORY */        CELLOP_AS_VALUE, A_0_OR_1 | A_MACRO,    XLNAME("DIRECTORY")
    /* FUNC_FIND */             CELLOP_AS_VALUE, A_2_OR_3,              XLNAME("FIND")
    /* FUNC_CELL */             CELLOP_AS_VALUE, A_1_OR_2,              XLNAME("CELL")
    /* FUNC_ISERR */            CELLOP_AS_VALUE, A_1,                   XLNAME("ISERR")
    /* FUNC_ISTEXT */           CELLOP_AS_VALUE, A_1,                   XLNAME("ISTEXT")
    /* FUNC_ISNUMBER */         CELLOP_AS_VALUE, A_1,                   XLNAME("ISNUMBER")
    /* FUNC_ISBLANK */          CELLOP_AS_VALUE, A_1,                   XLNAME("ISBLANK")
    /* FUNC_T */                CELLOP_AS_VALUE, A_1,                   XLNAME("T")
    /* FUNC_N */                CELLOP_AS_VALUE, A_1,                   XLNAME("N")
    /* FUNC_FOPEN */            CELLOP_AS_VALUE, A_1_OR_2 | A_MACRO,    XLNAME("FOPEN")
    /* FUNC_FCLOSE */           CELLOP_AS_VALUE, A_1 | A_MACRO,         XLNAME("FCLOSE")
    /* FUNC_FSIZE */            CELLOP_AS_VALUE, A_1 | A_MACRO,         XLNAME("FSIZE")
    /* FUNC_FREADLN */          CELLOP_AS_VALUE, A_1 | A_MACRO,         XLNAME("FREADLN")
    /* FUNC_FREAD */            CELLOP_AS_VALUE, A_2 | A_MACRO,         XLNAME("FREAD")
    /* FUNC_FWRITELN */         CELLOP_AS_VALUE, A_2 | A_MACRO,         XLNAME("FWRITELN")
    /* FUNC_FWRITE */           CELLOP_AS_VALUE, A_2 | A_MACRO,         XLNAME("FWRITE")
    /* FUNC_FPOS */             CELLOP_AS_VALUE, A_1_OR_2 | A_MACRO,    XLNAME("FPOS")
    /* FUNC_DATEVALUE */        CELLOP_AS_VALUE, A_1,                   XLNAME("DATEVALUE")
    /* FUNC_TIMEVALUE */        CELLOP_AS_VALUE, A_1,                   XLNAME("TIMEVALUE")
    /* FUNC_SLN */              CELLOP_AS_VALUE, A_3,                   XLNAME("SLN")
    /* FUNC_SYD */              CELLOP_AS_VALUE, A_4,                   XLNAME("SYD")
    /* FUNC_DDB */              CELLOP_AS_VALUE, A_4_OR_5,              XLNAME("DDB")
    /* FUNC_GETDEF */           CELLOP_AS_VALUE, A_1_TO_3 | A_MACRO,    XLNAME("GETDEF")
    /* FUNC_REFTEXT */          CELLOP_AS_VALUE, A_1_OR_2 | A_MACRO,    XLNAME("REFTEXT")
    /* FUNC_TEXTREF */          CELLOP_AS_VALUE, A_1_OR_2 | A_MACRO,    XLNAME("TEXTREF")
    /* FUNC_INDIRECT */         CELLOP_AS_VALUE, A_1_OR_2,              XLNAME("INDIRECT")
    /* FUNC_REGISTER */         CELLOP_AS_VALUE, A_1_OR_MORE | A_MACRO, XLNAME("REGISTER")
    /* FUNC_CALL */             CELLOP_AS_VALUE, A_1_OR_MORE | A_MACRO, XLNAME("CALL")
    /* FUNC_ADDBAR */           CELLOP_AS_VALUE, A_0_OR_1 | A_MACRO,    XLNAME("ADDBAR")
    /* FUNC_ADDMENU */          CELLOP_AS_VALUE, A_2_TO_4 | A_MACRO,    XLNAME("ADDMENU")
    /* FUNC_ADDCOMMAND */       CELLOP_AS_VALUE, A_3_TO_5 | A_MACRO,    XLNAME("ADDCOMMAND")
    /* FUNC_ENABLECOMMAND */    CELLOP_AS_VALUE, A_4_OR_5 | A_MACRO,    XLNAME("ENABLECOMMAND")
    /* FUNC_CHECKCOMMAND */     CELLOP_AS_VALUE, A_4_OR_5 | A_MACRO,    XLNAME("CHECKCOMMAND")
    /* FUNC_RENAMECOMMAND */    CELLOP_AS_VALUE, A_4_OR_5 | A_MACRO,    XLNAME("RENAMECOMMAND")
    /* FUNC_SHOWBAR */          CELLOP_AS_VALUE, A_0_OR_1 | A_MACRO,    XLNAME("SHOWBAR")
    /* FUNC_DELETEMENU */       CELLOP_AS_VALUE, A_2_OR_3 | A_MACRO,    XLNAME("DELETEMENU")
    /* FUNC_DELETECOMMAND */    CELLOP_AS_VALUE, A_3_OR_4 | A_MACRO,    XLNAME("DELETECOMMAND")
    /* FUNC_GETCHARTITEM */     CELLOP_AS_VALUE, A_1_TO_3 | A_MACRO,    XLNAME("GETCHARTITEM")
    /* FUNC_DIALOGBOX */        CELLOP_AS_VALUE, A_1 | A_MACRO,         XLNAME("DIALOGBOX")
    /* FUNC_CLEAN */            CELLOP_AS_VALUE, A_1,                   XLNAME("CLEAN")
    /* FUNC_MDETERM */          CELLOP_AS_VALUE, A_1,                   XLNAME("MDETERM")
    /* FUNC_MINVERSE */         CELLOP_AS_VALUE, A_1,                   XLNAME("MINVERSE")
    /* FUNC_MMULT */            CELLOP_AS_VALUE, A_2,                   XLNAME("MMULT")
    /* FUNC_FILES */            CELLOP_AS_VALUE, A_0_TO_2 | A_MACRO,    XLNAME("FILES")
    /* FUNC_IPMT */             CELLOP_AS_VALUE, A_4_TO_6,              XLNAME("IPMT")
    /* FUNC_PPMT */             CELLOP_AS_VALUE, A_4_TO_6,              XLNAME("PPMT")
    /* FUNC_COUNTA */           CELLOP_AS_VALUE, A_1_OR_MORE,           XLNAME("COUNTA")
    /* FUNC_CANCELKEY */        CELLOP_AS_VALUE, A_0_TO_2 | A_MACRO,    XLNAME("CANCELKEY")
    /* FUNC_INITIATE */         CELLOP_AS_VALUE, A_2 | A_MACRO,         XLNAME("INITIATE")
    /* FUNC_REQUEST */          CELLOP_AS_VALUE, A_2 | A_MACRO,         XLNAME("REQUEST")
    /* FUNC_POKE */             CELLOP_AS_VALUE, A_3 | A_MACRO,         XLNAME("POKE")
    /* FUNC_EXECUTE */          CELLOP_AS_VALUE, A_2 | A_MACRO,         XLNAME("EXECUTE")
    /* FUNC_TERMINATE */        CELLOP_AS_VALUE, A_1 | A_MACRO,         XLNAME("TERMINATE")
    /* FUNC_RESTART */          CELLOP_AS_VALUE, A_0_OR_1 | A_MACRO,    XLNAME("RESTART")
    /* FUNC_HELP */             CELLOP_AS_VALUE, A_0_OR_1 | A_MACRO,    XLNAME("HELP")
    /* FUNC_GETBAR */           CELLOP_AS_VALUE, A_0_TO_4 | A_MACRO,    XLNAME("GETBAR")
    /* FUNC_PRODUCT */          CELLOP_AS_VALUE, A_1_OR_MORE,           XLNAME("PRODUCT")
    /* FUNC_FACT */             CELLOP_AS_VALUE, A_1,                   XLNAME("FACT")
    /* FUNC_GETCELL */          CELLOP_AS_VALUE, A_1_OR_2 | A_MACRO,    XLNAME("GETCELL")
    /* FUNC_GETWORKSPACE */     CELLOP_AS_VALUE, A_1 | A_MACRO,         XLNAME("GETWORKSPACE")
    /* FUNC_GETWINDOW */        CELLOP_AS_VALUE, A_1_OR_2 | A_MACRO,    XLNAME("GETWINDOW")
    /* FUNC_GETDOCUMENT */      CELLOP_AS_VALUE, A_1_OR_2 | A_MACRO,    XLNAME("GETDOCUMENT")
    /* FUNC_DPRODUCT */         CELLOP_AS_VALUE, A_3,                   XLNAME("DPRODUCT")
    /* FUNC_ISNONTEXT */        CELLOP_AS_VALUE, A_1,                   XLNAME("ISNONTEXT")
    /* FUNC_GETNOTE */          CELLOP_AS_VALUE, A_0_TO_3 | A_MACRO,    XLNAME("GETNOTE")
    /* FUNC_NOTE */             CELLOP_AS_VALUE, A_0_TO_4 | A_MACRO,    XLNAME("NOTE")
    /* FUNC_STDEVP */           CELLOP_AS_VALUE, A_1_OR_MORE,           XLNAME("STDEVP")
    /* FUNC_VARP */             CELLOP_AS_VALUE, A_1_OR_MORE,           XLNAME("VARP")
    /* FUNC_DSTDEVP */          CELLOP_AS_VALUE, A_3,                   XLNAME("DSTDEVP")
    /* FUNC_DVARP */            CELLOP_AS_VALUE, A_3,                   XLNAME("DVARP")
    /* FUNC_TRUNC */            CELLOP_AS_VALUE, A_1_OR_2,              XLNAME("TRUNC")
    /* FUNC_ISLOGICAL */        CELLOP_AS_VALUE, A_1,                   XLNAME("ISLOGICAL")
    /* FUNC_DCOUNTA */          CELLOP_AS_VALUE, A_3,                   XLNAME("DCOUNTA")
    /* FUNC_DELETEBAR */        CELLOP_AS_VALUE, A_1 | A_MACRO,         XLNAME("DELETEBAR")
    /* FUNC_UNREGISTER */       CELLOP_AS_VALUE, A_1 | A_MACRO,         XLNAME("UNREGISTER")
    /* FUNC_USDOLLAR */         CELLOP_AS_VALUE, A_1_OR_2,              XLNAME("USDOLLAR")
    /* FUNC_FINDB */            CELLOP_AS_VALUE, A_2_OR_3,              XLNAME("FINDB")
    /* FUNC_SEARCHB */          CELLOP_AS_VALUE, A_2_OR_3,              XLNAME("SEARCHB")
    /* FUNC_REPLACEB */         CELLOP_AS_VALUE, A_4,                   XLNAME("REPLACEB")
    /* FUNC_LEFTB */            CELLOP_AS_VALUE, A_1_OR_2,              XLNAME("LEFTB")
    /* FUNC_RIGHTB */           CELLOP_AS_VALUE, A_1_OR_2,              XLNAME("RIGHTB")
    /* FUNC_MIDB */             CELLOP_AS_VALUE, A_3,                   XLNAME("MIDB")
    /* FUNC_LENB */             CELLOP_AS_VALUE, A_1,                   XLNAME("LENB")
    /* FUNC_ROUNDUP */          CELLOP_AS_VALUE, A_2,                   XLNAME("ROUNDUP")
    /* FUNC_ROUNDDOWN */        CELLOP_AS_VALUE, A_2,                   XLNAME("ROUNDDOWN")
    /* FUNC_ASC */              CELLOP_AS_VALUE, A_1,                   XLNAME("ASC")
    /* FUNC_DBCS */             CELLOP_AS_VALUE, A_1,                   XLNAME("DBCS")
    /* FUNC_RANK */             CELLOP_AS_VALUE, A_2_OR_3,              XLNAME("RANK")
    /* FUNC_ADDRESS */          CELLOP_AS_VALUE, A_2_TO_5,              XLNAME("ADDRESS")
    /* FUNC_DAYS360 */          CELLOP_AS_VALUE, A_2_OR_3,              XLNAME("DAYS360")
    /* FUNC_TODAY */            CELLOP_AS_VALUE, A_0,                   XLNAME("TODAY")
    /* FUNC_VDB */              CELLOP_AS_VALUE, A_5_TO_7,              XLNAME("VDB")
    /* FUNC_MEDIAN */           CELLOP_AS_VALUE, A_1_OR_MORE,           XLNAME("MEDIAN")
    /* FUNC_SUMPRODUCT */       CELLOP_AS_VALUE, A_1_OR_MORE,           XLNAME("SUMPRODUCT")
    /* FUNC_SINH */             CELLOP_AS_VALUE, A_1,                   XLNAME("SINH")
    /* FUNC_COSH */             CELLOP_AS_VALUE, A_1,                   XLNAME("COSH")
    /* FUNC_TANH */             CELLOP_AS_VALUE, A_1,                   XLNAME("TANH")
    /* FUNC_ASINH */            CELLOP_AS_VALUE, A_1,                   XLNAME("ASINH")
    /* FUNC_ACOSH */            CELLOP_AS_VALUE, A_1,                   XLNAME("ACOSH")
    /* FUNC_ATANH */            CELLOP_AS_VALUE, A_1,                   XLNAME("ATANH")
    /* FUNC_DGET */             CELLOP_AS_VALUE, A_3,                   XLNAME("DGET")
    /* FUNC_CREATEOBJECT */     CELLOP_AS_VALUE, A_2_OR_MORE | A_MACRO, XLNAME("CREATEOBJECT")
    /* FUNC_VOLATILE */         CELLOP_AS_VALUE, A_0_OR_1 | A_MACRO,    XLNAME("VOLATILE")
    /* FUNC_LASTERROR */        CELLOP_AS_VALUE, A_0 | A_MACRO,         XLNAME("LASTERROR")
    /* FUNC_CUSTOMUNDO */       CELLOP_AS_VALUE, A_0_TO_2 | A_MACRO,    XLNAME("CUSTOMUNDO")
    /* FUNC_CUSTOMREPEAT */     CELLOP_AS_VALUE, A_0_TO_3 | A_MACRO,    XLNAME("CUSTOMREPEAT")
    /* FUNC_FORMULACONVERT */   CELLOP_AS_VALUE, A_2_TO_5 | A_MACRO,    XLNAME("FORMULACONVERT")
    /* FUNC_GETLINKINFO */      CELLOP_AS_VALUE, A_2_TO_4 | A_MACRO,    XLNAME("GETLINKINFO")
    /* FUNC_TEXTBOX */          CELLOP_AS_VALUE, A_1_TO_4 | A_MACRO,    XLNAME("TEXTBOX")
    /* FUNC_INFO */             CELLOP_AS_VALUE, A_1,                   XLNAME("INFO")
    /* FUNC_GROUP */            CELLOP_AS_VALUE, A_0 | A_MACRO,         XLNAME("GROUP")
    /* FUNC_GETOBJECT */        CELLOP_AS_VALUE, A_1_TO_5 | A_MACRO,    XLNAME("GETOBJECT")
    /* FUNC_DB */               CELLOP_AS_VALUE, A_4_OR_5,              XLNAME("DB")
    /* FUNC_PAUSE */            CELLOP_AS_VALUE, A_0_OR_1 | A_MACRO,    XLNAME("PAUSE")
    /* FUNC_RESUME */           CELLOP_AS_VALUE, A_0_OR_1 | A_MACRO,    XLNAME("RESUME")
    /* FUNC_FREQUENCY */        CELLOP_AS_VALUE, A_2,                   XLNAME("FREQUENCY")
    /* FUNC_ADDTOOLBAR */       CELLOP_AS_VALUE, A_0_TO_2 | A_MACRO,    XLNAME("ADDTOOLBAR")
    /* FUNC_DELETETOOLBAR */    CELLOP_AS_VALUE, A_1 | A_MACRO,         XLNAME("DELETETOOLBAR")
    /* FUNC_UDF */              CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("")
    /* FUNC_RESETTOOLBAR */     CELLOP_AS_VALUE, A_1 | A_MACRO,         XLNAME("RESETTOOLBAR")
    /* FUNC_EVALUATE */         CELLOP_AS_VALUE, A_1 | A_MACRO,         XLNAME("EVALUATE")
    /* FUNC_GETTOOLBAR */       CELLOP_AS_VALUE, A_1_OR_2 | A_MACRO,    XLNAME("GETTOOLBAR")
    /* FUNC_GETTOOL */          CELLOP_AS_VALUE, A_1_TO_3 | A_MACRO,    XLNAME("GETTOOL")
    /* FUNC_SPELLINGCHECK */    CELLOP_AS_VALUE, A_1_TO_3 | A_MACRO,    XLNAME("SPELLINGCHECK")
    /* FUNC_ERRORTYPE */        CELLOP_AS_VALUE, A_1,                   XLNAME("ERRORTYPE")
    /* FUNC_APPTITLE */         CELLOP_AS_VALUE, A_0_OR_1 | A_MACRO,    XLNAME("APPTITLE")
    /* FUNC_WINDOWTITLE */      CELLOP_AS_VALUE, A_0_OR_1 | A_MACRO,    XLNAME("WINDOWTITLE")
    /* FUNC_SAVETOOLBAR */      CELLOP_AS_VALUE, A_0_TO_2 | A_MACRO,    XLNAME("SAVETOOLBAR")
    /* FUNC_ENABLETOOL */       CELLOP_AS_VALUE, A_3 | A_MACRO,         XLNAME("ENABLETOOL")
    /* FUNC_PRESSTOOL */        CELLOP_AS_VALUE, A_3 | A_MACRO,         XLNAME("PRESSTOOL")
    /* FUNC_REGISTERID */       CELLOP_AS_VALUE, A_2_OR_3 | A_MACRO,    XLNAME("REGISTERID")
    /* FUNC_GETWORKBOOK */      CELLOP_AS_VALUE, A_1_OR_2 | A_MACRO,    XLNAME("GETWORKBOOK")
    /* FUNC_AVEDEV */           CELLOP_AS_VALUE, A_1_OR_MORE,           XLNAME("AVEDEV")
    /* FUNC_BETADIST */         CELLOP_AS_VALUE, A_3_TO_5,              XLNAME("BETADIST")
    /* FUNC_GAMMALN */          CELLOP_AS_VALUE, A_1,                   XLNAME("GAMMALN")
    /* FUNC_BETAINV */          CELLOP_AS_VALUE, A_3_TO_5,              XLNAME("BETAINV")
    /* FUNC_BINOMDIST */        CELLOP_AS_VALUE, A_4,                   XLNAME("BINOMDIST")
    /* FUNC_CHIDIST */          CELLOP_AS_VALUE, A_2,                   XLNAME("CHIDIST")
    /* FUNC_CHIINV */           CELLOP_AS_VALUE, A_2,                   XLNAME("CHIINV")
    /* FUNC_COMBIN */           CELLOP_AS_VALUE, A_2,                   XLNAME("COMBIN")
    /* FUNC_CONFIDENCE */       CELLOP_AS_VALUE, A_3,                   XLNAME("CONFIDENCE")
    /* FUNC_CRITBINOM */        CELLOP_AS_VALUE, A_3,                   XLNAME("CRITBINOM")
    /* FUNC_EVEN */             CELLOP_AS_VALUE, A_1,                   XLNAME("EVEN")
    /* FUNC_EXPONDIST */        CELLOP_AS_VALUE, A_3,                   XLNAME("EXPONDIST")
    /* FUNC_FDIST */            CELLOP_AS_VALUE, A_3,                   XLNAME("FDIST")
    /* FUNC_FINV */             CELLOP_AS_VALUE, A_3,                   XLNAME("FINV")
    /* FUNC_FISHER */           CELLOP_AS_VALUE, A_1,                   XLNAME("FISHER")
    /* FUNC_FISHERINV */        CELLOP_AS_VALUE, A_1,                   XLNAME("FISHERINV")
    /* FUNC_FLOOR */            CELLOP_AS_VALUE, A_2,                   XLNAME("FLOOR")
    /* FUNC_GAMMADIST */        CELLOP_AS_VALUE, A_4,                   XLNAME("GAMMADIST")
    /* FUNC_GAMMAINV */         CELLOP_AS_VALUE, A_3,                   XLNAME("GAMMAINV")
    /* FUNC_CEILING */          CELLOP_AS_VALUE, A_2,                   XLNAME("CEILING")
    /* FUNC_HYPGEOMDIST */      CELLOP_AS_VALUE, A_4,                   XLNAME("HYPGEOMDIST")
    /* FUNC_LOGNORMDIST */      CELLOP_AS_VALUE, A_3,                   XLNAME("LOGNORMDIST")
    /* FUNC_LOGINV */           CELLOP_AS_VALUE, A_3,                   XLNAME("LOGINV")
    /* FUNC_NEGBINOMDIST */     CELLOP_AS_VALUE, A_3,                   XLNAME("NEGBINOMDIST")
    /* FUNC_NORMDIST */         CELLOP_AS_VALUE, A_4,                   XLNAME("NORMDIST")
    /* FUNC_NORMSDIST */        CELLOP_AS_VALUE, A_1,                   XLNAME("NORMSDIST")
    /* FUNC_NORMINV */          CELLOP_AS_VALUE, A_3,                   XLNAME("NORMINV")
    /* FUNC_NORMSINV */         CELLOP_AS_VALUE, A_1,                   XLNAME("NORMSINV")
    /* FUNC_STANDARDIZE */      CELLOP_AS_VALUE, A_3,                   XLNAME("STANDARDIZE")
    /* FUNC_ODD */              CELLOP_AS_VALUE, A_1,                   XLNAME("ODD")
    /* FUNC_PERMUT */           CELLOP_AS_VALUE, A_2,                   XLNAME("PERMUT")
    /* FUNC_POISSON */          CELLOP_AS_VALUE, A_3,                   XLNAME("POISSON")
    /* FUNC_TDIST */            CELLOP_AS_VALUE, A_3,                   XLNAME("TDIST")
    /* FUNC_WEIBULL */          CELLOP_AS_VALUE, A_4,                   XLNAME("WEIBULL")
    /* FUNC_SUMXMY2 */          CELLOP_AS_VALUE, A_2,                   XLNAME("SUMXMY2")
    /* FUNC_SUMX2MY2 */         CELLOP_AS_VALUE, A_2,                   XLNAME("SUMX2MY2")
    /* FUNC_SUMX2PY2 */         CELLOP_AS_VALUE, A_2,                   XLNAME("SUMX2PY2")
    /* FUNC_CHITEST */          CELLOP_AS_VALUE, A_2,                   XLNAME("CHITEST")
    /* FUNC_CORREL */           CELLOP_AS_VALUE, A_2,                   XLNAME("CORREL")
    /* FUNC_COVAR */            CELLOP_AS_VALUE, A_2,                   XLNAME("COVAR")
    /* FUNC_FORECAST */         CELLOP_AS_VALUE, A_3,                   XLNAME("FORECAST")
    /* FUNC_FTEST */            CELLOP_AS_VALUE, A_2,                   XLNAME("FTEST")
    /* FUNC_INTERCEPT */        CELLOP_AS_VALUE, A_2,                   XLNAME("INTERCEPT")
    /* FUNC_PEARSON */          CELLOP_AS_VALUE, A_2,                   XLNAME("PEARSON")
    /* FUNC_RSQ */              CELLOP_AS_VALUE, A_2,                   XLNAME("RSQ")
    /* FUNC_STEYX */            CELLOP_AS_VALUE, A_2,                   XLNAME("STEYX")
    /* FUNC_SLOPE */            CELLOP_AS_VALUE, A_2,                   XLNAME("SLOPE")
    /* FUNC_TTEST */            CELLOP_AS_VALUE, A_4,                   XLNAME("TTEST")
    /* FUNC_PROB */             CELLOP_AS_VALUE, A_3_OR_4,              XLNAME("PROB")
    /* FUNC_DEVSQ */            CELLOP_AS_VALUE, A_1_OR_MORE,           XLNAME("DEVSQ")
    /* FUNC_GEOMEAN */          CELLOP_AS_VALUE, A_1_OR_MORE,           XLNAME("GEOMEAN")
    /* FUNC_HARMEAN */          CELLOP_AS_VALUE, A_1_OR_MORE,           XLNAME("HARMEAN")
    /* FUNC_SUMSQ */            CELLOP_AS_VALUE, A_1_OR_MORE,           XLNAME("SUMSQ")
    /* FUNC_KURT */             CELLOP_AS_VALUE, A_1_OR_MORE,           XLNAME("KURT")
    /* FUNC_SKEW */             CELLOP_AS_VALUE, A_1_OR_MORE,           XLNAME("SKEW")
    /* FUNC_ZTEST */            CELLOP_AS_VALUE, A_2_OR_3,              XLNAME("ZTEST")
    /* FUNC_LARGE */            CELLOP_AS_VALUE, A_2,                   XLNAME("LARGE")
    /* FUNC_SMALL */            CELLOP_AS_VALUE, A_2,                   XLNAME("SMALL")
    /* FUNC_QUARTILE */         CELLOP_AS_VALUE, A_2,                   XLNAME("QUARTILE")
    /* FUNC_PERCENTILE */       CELLOP_AS_VALUE, A_2,                   XLNAME("PERCENTILE")
    /* FUNC_PERCENTRANK */      CELLOP_AS_VALUE, A_2_OR_3,              XLNAME("PERCENTRANK")
    /* FUNC_MODE */             CELLOP_AS_VALUE, A_1_OR_MORE,           XLNAME("MODE")
    /* FUNC_TRIMMEAN */         CELLOP_AS_VALUE, A_2,                   XLNAME("TRIMMEAN")
    /* FUNC_TINV */             CELLOP_AS_VALUE, A_2,                   XLNAME("TINV")
    /* FUNC_MOVIECOMMAND */     CELLOP_AS_VALUE, A_3_OR_4 | A_MACRO,    XLNAME("MOVIECOMMAND")
    /* FUNC_GETMOVIE */         CELLOP_AS_VALUE, A_3_OR_4 | A_MACRO,    XLNAME("GETMOVIE")
    /* FUNC_CONCATENATE */      CELLOP_AS_VALUE, A_1_OR_MORE,           XLNAME("CONCATENATE")
    /* FUNC_POWER */            CELLOP_AS_VALUE, A_2,                   XLNAME("POWER")
    /* FUNC_PIVOTADDDATA */     CELLOP_AS_VALUE, A_2_TO_9 | A_MACRO,    XLNAME("PIVOTADDDATA")
    /* FUNC_GETPIVOTTABLE */    CELLOP_AS_VALUE, A_1_OR_2 | A_MACRO,    XLNAME("GETPIVOTTABLE")
    /* FUNC_GETPIVOTFIELD */    CELLOP_AS_VALUE, A_1_TO_3 | A_MACRO,    XLNAME("GETPIVOTFIELD")
    /* FUNC_GETPIVOTITEM */     CELLOP_AS_VALUE, A_1_TO_4 | A_MACRO,    XLNAME("GETPIVOTITEM")
    /* FUNC_RADIANS */          CELLOP_AS_VALUE, A_1,                   XLNAME("RADIANS")
    /* FUNC_DEGREES */          CELLOP_AS_VALUE, A_1,                   XLNAME("DEGREES")
    /* FUNC_SUBTOTAL */         CELLOP_AS_VALUE, A_2_OR_MORE,           XLNAME("SUBTOTAL")
    /* FUNC_SUMIF */            CELLOP_AS_VALUE, A_2_OR_3,              XLNAME("SUMIF")
    /* FUNC_COUNTIF */          CELLOP_AS_VALUE, A_2,                   XLNAME("COUNTIF")
    /* FUNC_COUNTBLANK */       CELLOP_AS_VALUE, A_1,                   XLNAME("COUNTBLANK")
    /* FUNC_SCENARIOGET */      CELLOP_AS_VALUE, A_1_OR_2 | A_MACRO,    XLNAME("SCENARIOGET")
    /* FUNC_OPTIONSLISTSGET */  CELLOP_AS_VALUE, A_1 | A_MACRO,         XLNAME("OPTIONSLISTSGET")
    /* FUNC_ISPMT */            CELLOP_AS_VALUE, A_4,                   XLNAME("ISPMT")
    /* FUNC_DATEDIF */          CELLOP_AS_VALUE, A_3,                   XLNAME("DATEDIF")
    /* FUNC_DATESTRING */       CELLOP_AS_VALUE, A_1,                   XLNAME("DATESTRING")
    /* FUNC_NUMBERSTRING */     CELLOP_AS_VALUE, A_2,                   XLNAME("NUMBERSTRING")
    /* FUNC_ROMAN */            CELLOP_AS_VALUE, A_1_OR_2,              XLNAME("ROMAN")
    /* FUNC_OPENDIALOG */       CELLOP_AS_VALUE, A_0_TO_4 | A_MACRO,    XLNAME("OPENDIALOG")
    /* FUNC_SAVEDIALOG */       CELLOP_AS_VALUE, A_0_TO_5 | A_MACRO,    XLNAME("SAVEDIALOG")
    /* FUNC_VIEWGET */          CELLOP_AS_VALUE, A_1_OR_2 | A_MACRO,    XLNAME("VIEWGET")
    /* FUNC_GETPIVOTDATA */     CELLOP_AS_VALUE, A_2_OR_MORE,           XLNAME("")
    /* FUNC_HYPERLINK */        CELLOP_AS_VALUE, A_1_OR_2,              XLNAME("HYPERLINK")
    /* FUNC_PHONETIC */         CELLOP_AS_VALUE, A_1,                   XLNAME("PHONETIC")
    /* FUNC_AVERAGEA */         CELLOP_AS_VALUE, A_1_OR_MORE,           XLNAME("AVERAGEA")
    /* FUNC_MAXA */             CELLOP_AS_VALUE, A_1_OR_MORE,           XLNAME("MAXA")
    /* FUNC_MINA */             CELLOP_AS_VALUE, A_1_OR_MORE,           XLNAME("MINA")
    /* FUNC_STDEVPA */          CELLOP_AS_VALUE, A_1_OR_MORE,           XLNAME("STDEVPA")
    /* FUNC_VARPA */            CELLOP_AS_VALUE, A_1_OR_MORE,           XLNAME("VARPA")
    /* FUNC_STDEVA */           CELLOP_AS_VALUE, A_1_OR_MORE,           XLNAME("STDEVA")
    /* FUNC_VARA */             CELLOP_AS_VALUE, A_1_OR_MORE,           XLNAME("VARA")
#if 0
    /* FUNC_BAHTTEXT */         CELLOP_AS_VALUE, A_1,                   XLNAME("BAHTTEXT")
    /* FUNC_THAIDAYOFWEEK */    CELLOP_AS_VALUE, A_1,                   XLNAME("THAIDAYOFWEEK")
    /* FUNC_THAIDIGIT */        CELLOP_AS_VALUE, A_1,                   XLNAME("THAIDIGIT")
    /* FUNC_THAIMONTHOFYEAR */  CELLOP_AS_VALUE, A_1,                   XLNAME("THAIMONTHOFYEAR")
    /* FUNC_THAINUMSOUND */     CELLOP_AS_VALUE, A_1,                   XLNAME("THAINUMSOUND")
    /* FUNC_THAINUMSTRING */    CELLOP_AS_VALUE, A_1,                   XLNAME("THAINUMSTRING")
    /* FUNC_THAISTRINGLENGTH */ CELLOP_AS_VALUE, A_1,                   XLNAME("THAISTRINGLENGTH")
    /* FUNC_ISTHAIDIGIT */      CELLOP_AS_VALUE, A_1,                   XLNAME("ISTHAIDIGIT")
    /* FUNC_ROUNDBAHTDOWN */    CELLOP_AS_VALUE, A_1,                   XLNAME("ROUNDBAHTDOWN")
    /* FUNC_ROUNDBAHTUP */      CELLOP_AS_VALUE, A_1,                   XLNAME("ROUNDBAHTUP")
    /* FUNC_THAIYEAR */         CELLOP_AS_VALUE, A_1,                   XLNAME("THAIYEAR")
    /* FUNC_RTD */              CELLOP_AS_VALUE, A_3_OR_MORE,           XLNAME("RTD")
    /* FUNC_CUBEVALUE */        CELLOP_AS_VALUE, A_1_OR_MORE | A_MACRO, XLNAME("CUBEVALUE")
    /* FUNC_CUBEMEMBER */       CELLOP_AS_VALUE, A_2_OR_3 | A_MACRO,    XLNAME("CUBEMEMBER")
    /* FUNC_CUBEMEMBERPROPERTY */CELLOP_AS_VALUE, A_3 | A_MACRO,        XLNAME("CUBEMEMBERPROPERTY")
    /* FUNC_CUBERANKEDMEMBER */ CELLOP_AS_VALUE, A_3_OR_4 | A_MACRO,    XLNAME("CUBERANKEDMEMBER")
    /* FUNC_HEX2BIN */          CELLOP_AS_VALUE, A_1_OR_2,              XLNAME("HEX2BIN")
    /* FUNC_HEX2DEC */          CELLOP_AS_VALUE, A_1,                   XLNAME("HEX2DEC")
    /* FUNC_HEX2OCT */          CELLOP_AS_VALUE, A_1_OR_2,              XLNAME("HEX2OCT")
    /* FUNC_DEC2BIN */          CELLOP_AS_VALUE, A_1_OR_2,              XLNAME("DEC2BIN")
    /* FUNC_DEC2HEX */          CELLOP_AS_VALUE, A_1_OR_2,              XLNAME("DEC2HEX")
    /* FUNC_DEC2OCT */          CELLOP_AS_VALUE, A_1_OR_2,              XLNAME("DEC2OCT")
    /* FUNC_OCT2BIN */          CELLOP_AS_VALUE, A_1_OR_2,              XLNAME("OCT2BIN")
    /* FUNC_OCT2HEX */          CELLOP_AS_VALUE, A_1_OR_2,              XLNAME("OCT2HEX")
    /* FUNC_OCT2DEC */          CELLOP_AS_VALUE, A_1,                   XLNAME("OCT2DEC")
    /* FUNC_BIN2DEC */          CELLOP_AS_VALUE, A_1,                   XLNAME("BIN2DEC")
    /* FUNC_BIN2OCT */          CELLOP_AS_VALUE, A_1_OR_2,              XLNAME("BIN2OCT")
    /* FUNC_BIN2HEX */          CELLOP_AS_VALUE, A_1_OR_2,              XLNAME("BIN2HEX")
    /* FUNC_IMSUB */            CELLOP_AS_VALUE, A_2,                   XLNAME("IMSUB")
    /* FUNC_IMDIV */            CELLOP_AS_VALUE, A_2,                   XLNAME("IMDIV")
    /* FUNC_IMPOWER */          CELLOP_AS_VALUE, A_2,                   XLNAME("IMPOWER")
    /* FUNC_IMABS */            CELLOP_AS_VALUE, A_1,                   XLNAME("IMABS")
    /* FUNC_IMSQRT */           CELLOP_AS_VALUE, A_1,                   XLNAME("IMSQRT")
    /* FUNC_IMLN */             CELLOP_AS_VALUE, A_1,                   XLNAME("IMLN")
    /* FUNC_IMLOG2 */           CELLOP_AS_VALUE, A_1,                   XLNAME("IMLOG2")
    /* FUNC_IMLOG10 */          CELLOP_AS_VALUE, A_1,                   XLNAME("IMLOG10")
    /* FUNC_IMSIN */            CELLOP_AS_VALUE, A_1,                   XLNAME("IMSIN")
    /* FUNC_IMCOS */            CELLOP_AS_VALUE, A_1,                   XLNAME("IMCOS")
    /* FUNC_IMEXP */            CELLOP_AS_VALUE, A_1,                   XLNAME("IMEXP")
    /* FUNC_IMARGUMENT */       CELLOP_AS_VALUE, A_1,                   XLNAME("IMARGUMENT")
    /* FUNC_IMCONJUGATE */      CELLOP_AS_VALUE, A_1,                   XLNAME("IMCONJUGATE")
    /* FUNC_IMAGINARY */        CELLOP_AS_VALUE, A_1,                   XLNAME("IMAGINARY")
    /* FUNC_IMREAL */           CELLOP_AS_VALUE, A_1,                   XLNAME("IMREAL")
    /* FUNC_COMPLEX */          CELLOP_AS_VALUE, A_2_OR_3,              XLNAME("COMPLEX")
    /* FUNC_IMSUM */            CELLOP_AS_VALUE, A_1_OR_MORE,           XLNAME("IMSUM")
    /* FUNC_IMPRODUCT */        CELLOP_AS_VALUE, A_1_OR_MORE,           XLNAME("IMPRODUCT")
    /* FUNC_SERIESSUM */        CELLOP_AS_VALUE, A_4,                   XLNAME("SERIESSUM")
    /* FUNC_FACTDOUBLE */       CELLOP_AS_VALUE, A_1,                   XLNAME("FACTDOUBLE")
    /* FUNC_SQRTPI */           CELLOP_AS_VALUE, A_1,                   XLNAME("SQRTPI")
    /* FUNC_QUOTIENT */         CELLOP_AS_VALUE, A_2,                   XLNAME("QUOTIENT")
    /* FUNC_DELTA */            CELLOP_AS_VALUE, A_1_OR_2,              XLNAME("DELTA")
    /* FUNC_GESTEP */           CELLOP_AS_VALUE, A_1_OR_2,              XLNAME("GESTEP")
    /* FUNC_ISEVEN */           CELLOP_AS_VALUE, A_1,                   XLNAME("ISEVEN")
    /* FUNC_ISODD */            CELLOP_AS_VALUE, A_1,                   XLNAME("ISODD")
    /* FUNC_MROUND */           CELLOP_AS_VALUE, A_2,                   XLNAME("MROUND")
    /* FUNC_ERF */              CELLOP_AS_VALUE, A_1_OR_2,              XLNAME("ERF")
    /* FUNC_ERFC */             CELLOP_AS_VALUE, A_1,                   XLNAME("ERFC")
    /* FUNC_BESSELJ */          CELLOP_AS_VALUE, A_2,                   XLNAME("BESSELJ")
    /* FUNC_BESSELK */          CELLOP_AS_VALUE, A_2,                   XLNAME("BESSELK")
    /* FUNC_BESSELY */          CELLOP_AS_VALUE, A_2,                   XLNAME("BESSELY")
    /* FUNC_BESSELI */          CELLOP_AS_VALUE, A_2,                   XLNAME("BESSELI")
    /* FUNC_XIRR */             CELLOP_AS_VALUE, A_2_OR_3,              XLNAME("XIRR")
    /* FUNC_XNPV */             CELLOP_AS_VALUE, A_3,                   XLNAME("XNPV")
    /* FUNC_PRICEMAT */         CELLOP_AS_VALUE, A_5_OR_6,              XLNAME("PRICEMAT")
    /* FUNC_YIELDMAT */         CELLOP_AS_VALUE, A_5_OR_6,              XLNAME("YIELDMAT")
    /* FUNC_INTRATE */          CELLOP_AS_VALUE, A_4_OR_5,              XLNAME("INTRATE")
    /* FUNC_RECEIVED */         CELLOP_AS_VALUE, A_4_OR_5,              XLNAME("RECEIVED")
    /* FUNC_DISC */             CELLOP_AS_VALUE, A_4_OR_5,              XLNAME("DISC")
    /* FUNC_PRICEDISC */        CELLOP_AS_VALUE, A_4_OR_5,              XLNAME("PRICEDISC")
    /* FUNC_YIELDDISC */        CELLOP_AS_VALUE, A_4_OR_5,              XLNAME("YIELDDISC")
    /* FUNC_TBILLEQ */          CELLOP_AS_VALUE, A_3,                   XLNAME("TBILLEQ")
    /* FUNC_TBILLPRICE */       CELLOP_AS_VALUE, A_3,                   XLNAME("TBILLPRICE")
    /* FUNC_TBILLYIELD */       CELLOP_AS_VALUE, A_3,                   XLNAME("TBILLYIELD")
    /* FUNC_PRICE */            CELLOP_AS_VALUE, A_6_OR_7,              XLNAME("PRICE")
    /* FUNC_YIELD */            CELLOP_AS_VALUE, A_6_OR_7,              XLNAME("YIELD")
    /* FUNC_DOLLARDE */         CELLOP_AS_VALUE, A_2,                   XLNAME("DOLLARDE")
    /* FUNC_DOLLARFR */         CELLOP_AS_VALUE, A_2,                   XLNAME("DOLLARFR")
    /* FUNC_NOMINAL */          CELLOP_AS_VALUE, A_2,                   XLNAME("NOMINAL")
    /* FUNC_EFFECT */           CELLOP_AS_VALUE, A_2,                   XLNAME("EFFECT")
    /* FUNC_CUMPRINC */         CELLOP_AS_VALUE, A_6,                   XLNAME("CUMPRINC")
    /* FUNC_CUMIPMT */          CELLOP_AS_VALUE, A_6,                   XLNAME("CUMIPMT")
    /* FUNC_EDATE */            CELLOP_AS_VALUE, A_2,                   XLNAME("EDATE")
    /* FUNC_EOMONTH */          CELLOP_AS_VALUE, A_2,                   XLNAME("EOMONTH")
    /* FUNC_YEARFRAC */         CELLOP_AS_VALUE, A_2_OR_3,              XLNAME("YEARFRAC")
    /* FUNC_COUPDAYBS */        CELLOP_AS_VALUE, A_3_OR_4,              XLNAME("COUPDAYBS")
    /* FUNC_COUPDAYS */         CELLOP_AS_VALUE, A_3_OR_4,              XLNAME("COUPDAYS")
    /* FUNC_COUPDAYSNC */       CELLOP_AS_VALUE, A_3_OR_4,              XLNAME("COUPDAYSNC")
    /* FUNC_COUPNCD */          CELLOP_AS_VALUE, A_3_OR_4,              XLNAME("COUPNCD")
    /* FUNC_COUPNUM */          CELLOP_AS_VALUE, A_3_OR_4,              XLNAME("COUPNUM")
    /* FUNC_COUPPCD */          CELLOP_AS_VALUE, A_3_OR_4,              XLNAME("COUPPCD")
    /* FUNC_DURATION */         CELLOP_AS_VALUE, A_5_OR_6,              XLNAME("DURATION")
    /* FUNC_MDURATION */        CELLOP_AS_VALUE, A_5_OR_6,              XLNAME("MDURATION")
    /* FUNC_ODDLPRICE */        CELLOP_AS_VALUE, A_7_OR_8,              XLNAME("ODDLPRICE")
    /* FUNC_ODDLYIELD */        CELLOP_AS_VALUE, A_7_OR_8,              XLNAME("ODDLYIELD")
    /* FUNC_ODDFPRICE */        CELLOP_AS_VALUE, A_8_OR_9,              XLNAME("ODDFPRICE")
    /* FUNC_ODDFYIELD */        CELLOP_AS_VALUE, A_8_OR_9,              XLNAME("ODDFYIELD")
    /* FUNC_RANDBETWEEN */      CELLOP_AS_VALUE, A_2,                   XLNAME("RANDBETWEEN")
    /* FUNC_WEEKNUM */          CELLOP_AS_VALUE, A_1_OR_2,              XLNAME("WEEKNUM")
    /* FUNC_AMORDEGRC */        CELLOP_AS_VALUE, A_6_OR_7,              XLNAME("AMORDEGRC")
    /* FUNC_AMORLINC */         CELLOP_AS_VALUE, A_6_OR_7,              XLNAME("AMORLINC")
    /* FUNC_CONVERT */          CELLOP_AS_VALUE, A_3,                   XLNAME("CONVERT")
    /* FUNC_ACCRINT */          CELLOP_AS_VALUE, A_6_TO_8,              XLNAME("ACCRINT")
    /* FUNC_ACCRINTM */         CELLOP_AS_VALUE, A_4_OR_5,              XLNAME("ACCRINTM")
    /* FUNC_WORKDAY */          CELLOP_AS_VALUE, A_2_OR_3,              XLNAME("WORKDAY")
    /* FUNC_NETWORKDAYS */      CELLOP_AS_VALUE, A_2_OR_3,              XLNAME("NETWORKDAYS")
    /* FUNC_GCD */              CELLOP_AS_VALUE, A_1_OR_MORE,           XLNAME("GCD")
    /* FUNC_MULTINOMIAL */      CELLOP_AS_VALUE, A_1_OR_MORE,           XLNAME("MULTINOMIAL")
    /* FUNC_LCM */              CELLOP_AS_VALUE, A_1_OR_MORE,           XLNAME("LCM")
    /* FUNC_FVSCHEDULE */       CELLOP_AS_VALUE, A_2,                   XLNAME("FVSCHEDULE")
    /* FUNC_CUBEKPIMEMBER */    CELLOP_AS_VALUE, A_3_OR_4 | A_MACRO,    XLNAME("CUBEKPIMEMBER")
    /* FUNC_CUBESET */          CELLOP_AS_VALUE, A_2_TO_5 | A_MACRO,    XLNAME("CUBESET")
    /* FUNC_CUBESETCOUNT */     CELLOP_AS_VALUE, A_1,                   XLNAME("CUBESETCOUNT")
    /* FUNC_IFERROR */          CELLOP_AS_VALUE, A_2,                   XLNAME("IFERROR")
    /* FUNC_COUNTIFS */         CELLOP_AS_VALUE, A_2_OR_MORE,           XLNAME("COUNTIFS")
    /* FUNC_SUMIFS */           CELLOP_AS_VALUE, A_3_OR_MORE,           XLNAME("SUMIFS")
    /* FUNC_AVERAGEIF */        CELLOP_AS_VALUE, A_2_OR_3,              XLNAME("AVERAGEIF")
    /* FUNC_AVERAGEIFS */       CELLOP_AS_VALUE, A_3_OR_MORE,           XLNAME("AVERAGEIFS")
    /* FUNC_AGGREGATE */        CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("AGGREGATE")
    /* FUNC_BINOM_DIST */       CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("BINOM_DIST")
    /* FUNC_BINOM_INV */        CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("BINOM_INV")
    /* FUNC_CONFIDENCE_NORM */  CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("CONFIDENCE_NORM")
    /* FUNC_CONFIDENCE_T */     CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("CONFIDENCE_T")
    /* FUNC_CHISQ_TEST */       CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("CHISQ_TEST")
    /* FUNC_F_TEST */           CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("F_TEST")
    /* FUNC_COVARIANCE_P */     CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("COVARIANCE_P")
    /* FUNC_COVARIANCE_S */     CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("COVARIANCE_S")
    /* FUNC_EXPON_DIST */       CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("EXPON_DIST")
    /* FUNC_GAMMA_DIST */       CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("GAMMA_DIST")
    /* FUNC_GAMMA_INV */        CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("GAMMA_INV")
    /* FUNC_MODE_MULT */        CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("MODE_MULT")
    /* FUNC_MODE_SNGL */        CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("MODE_SNGL")
    /* FUNC_NORM_DIST */        CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("NORM_DIST")
    /* FUNC_NORM_INV */         CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("NORM_INV")
    /* FUNC_PERCENTILE_EXC */   CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("PERCENTILE_EXC")
    /* FUNC_PERCENTILE_INC */   CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("PERCENTILE_INC")
    /* FUNC_PERCENTRANK_EXC */  CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("PERCENTRANK_EXC")
    /* FUNC_PERCENTRANK_INC */  CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("PERCENTRANK_INC")
    /* FUNC_POISSON_DIST */     CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("POISSON_DIST")
    /* FUNC_QUARTILE_EXC */     CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("QUARTILE_EXC")
    /* FUNC_QUARTILE_INC */     CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("QUARTILE_INC")
    /* FUNC_RANK_AVG */         CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("RANK_AVG")
    /* FUNC_RANK_EQ */          CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("RANK_EQ")
    /* FUNC_STDEV_S */          CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("STDEV_S")
    /* FUNC_STDEV_P */          CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("STDEV_P")
    /* FUNC_T_DIST */           CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("T_DIST")
    /* FUNC_T_DIST_2T */        CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("T_DIST_2T")
    /* FUNC_T_DIST_RT */        CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("T_DIST_RT")
    /* FUNC_T_INV */            CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("T_INV")
    /* FUNC_T_INV_2T */         CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("T_INV_2T")
    /* FUNC_VAR_S */            CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("VAR_S")
    /* FUNC_VAR_P */            CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("VAR_P")
    /* FUNC_WEIBULL_DIST */     CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("WEIBULL_DIST")
    /* FUNC_NETWORKDAYS_INTL */ CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("NETWORKDAYS_INTL")
    /* FUNC_WORKDAY_INTL */     CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("WORKDAY_INTL")
    /* FUNC_ECMA_CEILING */     CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("ECMA_CEILING")
    /* FUNC_ISO_CEILING */      CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("ISO_CEILING")
    /* FUNC_BETA_DIST */        CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("BETA_DIST")
    /* FUNC_BETA_INV */         CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("BETA_INV")
    /* FUNC_CHISQ_DIST */       CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("CHISQ_DIST")
    /* FUNC_CHISQ_DIST_RT */    CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("CHISQ_DIST_RT")
    /* FUNC_CHISQ_INV */        CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("CHISQ_INV")
    /* FUNC_CHISQ_INV_RT */     CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("CHISQ_INV_RT")
    /* FUNC_F_DIST */           CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("F_DIST")
    /* FUNC_F_DIST_RT */        CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("F_DIST_RT")
    /* FUNC_F_INV */            CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("F_INV")
    /* FUNC_F_INV_RT */         CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("F_INV_RT")
    /* FUNC_HYPGEOM_DIST */     CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("HYPGEOM_DIST")
    /* FUNC_LOGNORM_DIST */     CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("LOGNORM_DIST")
    /* FUNC_LOGNORM_INV */      CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("LOGNORM_INV")
    /* FUNC_NEGBINOM_DIST */    CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("NEGBINOM_DIST")
    /* FUNC_NORM_S_DIST */      CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("NORM_S_DIST")
    /* FUNC_NORM_S_INV */       CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("NORM_S_INV")
    /* FUNC_T_TEST */           CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("T_TEST")
    /* FUNC_Z_TEST */           CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("Z_TEST")
    /* FUNC_ERF_PRECISE */      CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("ERF_PRECISE")
    /* FUNC_ERFC_PRECISE */     CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("ERFC_PRECISE")
    /* FUNC_GAMMALN_PRECISE */  CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("GAMMALN_PRECISE")
    /* FUNC_CEILING_PRECISE */  CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("CEILING_PRECISE")
    /* FUNC_FLOOR_PRECISE */    CELLOP_AS_VALUE, A_UNKNOWN,             XLNAME("FLOOR_PRECISE")
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


