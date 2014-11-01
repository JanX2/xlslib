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

#include "xlslib/record.h"
#include "xlslib/formula.h"
#include "xlslib/globalrec.h"
#include "xlslib/datast.h"
#include "xlslib/cell.h"
#include "xlslib/rectypes.h"
#include "xlslib/formula_cell.h"
#include "xlslib/formula_expr.h"
#include "xlslib/formula_estimate.h"

#ifdef __BCPLUSPLUS__
#include <malloc.h>
// malloc.h needed for calloc. RLN 111208
#include <memory.h>
// memory.h needed for memset. RLN 111215
// These may be needed for other compilers as well.
#endif

using namespace xlslib_core;
using namespace xlslib_strings;

/* For information on the XLS binary format, see
 * http://msdn.microsoft.com/en-us/library/cc313154(v=office.12).aspx
 *
 * The section on formulas is entitled "Microsoft Excel Formulas"
 */

formula_cell_t::formula_cell_t(CGlobalRecords& gRecords, unsigned32_t rowval, unsigned32_t colval, expression_node_t* ast_val, bool a_formula, bool autodes, xf_t* pxfval) :
	cell_t(gRecords, rowval, colval, pxfval),
	ast(ast_val),
	auto_destruct_expression_tree(autodes),
	array_formula(a_formula),
	stack(NULL)
{
	XL_ASSERT(ast_val);

#if !defined (HAVE_PRAGMA_PACK) && !defined (HAVE_PRAGMA_PACK_PUSH_POP)
	XL_ASSERTS("Must Have Pragma Pack to use formulas");
#endif
}

formula_cell_t::formula_cell_t(CGlobalRecords& gRecords, unsigned32_t rowval, unsigned32_t colval, 
        formula_t *stack_val, bool a_formula, xf_t* pxfval) :
	cell_t(gRecords, rowval, colval, pxfval),
    ast(NULL),
	auto_destruct_expression_tree(true),
	array_formula(a_formula),
	stack(stack_val)
{
	XL_ASSERT(stack_val);

#if !defined (HAVE_PRAGMA_PACK) && !defined (HAVE_PRAGMA_PACK_PUSH_POP)
	XL_ASSERTS("Must Have Pragma Pack to use formulas");
#endif
}

formula_cell_t::~formula_cell_t()
{
	if (ast && auto_destruct_expression_tree) {
		ast->DestroyAST();
	}
}

void formula_cell_t::GetResultEstimate(estimated_formula_result_t &dst) const
{
	if (ast) {
		ast->GetResultEstimate(dst);
	} else if (stack) {
        stack->GetResultEstimate(dst);
    }
}

size_t formula_cell_t::GetSize(void) const
{
	estimated_formula_result_t estimate(m_GlobalRecords);
	const expression_node_t *expr = GetAST();
    XL_ASSERT(expr != NULL || stack != NULL);
	size_t len = 4+2+2+2+8+2+4+2;
    if (expr) {
        len += expr->GetSize();
		//printf("ESize: %ld\n", expr->GetSize());
    } else if (stack) {
        len += stack->GetSize();
		//printf("SSize: %ld\n", stack->GetSize());
    }
    GetResultEstimate(estimate);
	// TODO: Estimate size does not seem to work very well - quite low
	if (estimate.EncodedValueIsString()) {
		// FORMULA BIFF8 is immediately followed by a STRING BIFF8 record!
		const u16string* str = estimate.GetStringValue();

		XL_ASSERT(str);
		len += 4 + str->length() * (CGlobalRecords::IsASCII(*str) ? sizeof(unsigned8_t) : sizeof(unsigned16_t));
	}
	//printf("Size: %ld\n", len);
	return len;
}

CUnit* formula_cell_t::GetData(CDataStorage &datastore) const
{
	return datastore.MakeCFormula(*this);   // NOTE: this pointer HAS to be deleted elsewhere.
}

void formula_cell_t::DumpData(CUnit &dst) const
{
    if (ast) {
        formula_t *fs = new formula_t(m_GlobalRecords, this->GetWorksheet());
        ast->DumpData(*fs, true); // rgce dump, length_of_parsed_expr
        fs->DumpData(dst);
        delete fs;
    } else if (stack) {
        stack->DumpData(dst);
    }
}

/*
 *********************************
 *  CFormula class implementation
 *********************************
 */

CFormula::CFormula(CDataStorage &datastore, const formula_cell_t& expr) :
	CRecord(datastore)
{
	size_t basepos = 0;

	SetRecordType(RECTYPE_FORMULA);  // followed by the RECTYPE_STRING record when the formula evaluates to a string!
	AddValue16((unsigned16_t)expr.GetRow());
	AddValue16((unsigned16_t)expr.GetCol());
	AddValue16(expr.GetXFIndex());

	estimated_formula_result_t estimate(expr.GetGlobalRecords());
	expr.GetResultEstimate(estimate);
	AddValue64(estimate.GetEncodedValue());	// current_value_of_formula
	AddValue16(estimate.GetOptionFlags());	// flags: grbit

	AddValue32(0); // chn

	if(expr.IsArrayFormula()) {
		AddValue16(1+2+2);							// len
		AddValue8(1);								// tExpr
		AddValue16((unsigned16_t)expr.GetRow());
		AddValue16((unsigned16_t)expr.GetCol());
		SetRecordLength(GetDataSize()-RECORD_HEADER_SIZE);

		// fake it by appending it at the tail of the current record!
		basepos = GetDataSize();

		AddValue16(RECTYPE_ARRAY);  // followed by the RECTYPE_STRING record when the formula evaluates to a string!
		AddValue16(0);				// placeholder for len

		AddValue16((unsigned16_t)expr.GetRow());
		AddValue16((unsigned16_t)expr.GetRow());
		AddValue8((unsigned8_t)expr.GetCol());
		AddValue8((unsigned8_t)expr.GetCol());
		AddValue16(estimate.GetOptionFlags()); // flags: grbit

		AddValue32(0); // chn
	}

	size_t len_position = GetDataSize();
	AddValue16(0 /* expr.GetSize() */ ); // length_of_parsed_expr
	
	expr.DumpData(*this);
	size_t end = GetDataSize();
	SetValueAt16((unsigned16_t)(end - len_position - 2), (unsigned)len_position);	// go back and set real value for token length

	SetValueAt16((unsigned16_t)(GetDataSize() - basepos - RECORD_HEADER_SIZE), (unsigned)basepos + 2);	// SetRecordLength on either FORMULA or the ARRAY

	if (estimate.EncodedValueIsString()) {
		// FORMULA BIFF8 is immediately followed by a STRING BIFF8 record!
		//
		// fake it by appending it at the tail of the current record!
		basepos = GetDataSize();

		AddValue16(RECTYPE_STRING);
		AddValue16(0);				// placeholder for len

		const u16string* str = estimate.GetStringValue();

		XL_ASSERT(str);
		XL_ASSERT(str->length() < 256); // dbg
		AddUnicodeString(*str, LEN2_FLAGS_UNICODE);

		SetValueAt16((unsigned16_t)(GetDataSize() - basepos - RECORD_HEADER_SIZE), (unsigned)basepos + 2); // SetRecordLength
	}
}

CFormula::~CFormula()
{
}
