
#include <string>
#include <sstream>
#include <iostream>
#include <xlslib.h>
#include "formulas.h"

using namespace std;
using namespace xlslib_core;

extern void formula_database(workbook &wb);
extern void formula_numbers(workbook &wb);
extern void formula_numbers_sheets(workbook &wb);
extern void formula_text(workbook &wb);
extern void formula_date_time(workbook &wb);

#if 0
		boolean_value_node_t *boolean(bool value);
		integer_value_node_t *integer(signed32_t value);
		float_value_node_t *floating_point(double value);
		error_value_node_t *error_value(errcode_t value);
		missing_arg_node_t *missing_arg(void);
		text_value_node_t *text(const std::string& value);
		text_value_node_t *text(const xlslib_strings::u16string& value);
		
		cell_deref_node_t *cell(const cell_t& value, cell_addr_mode_t attr, cell_op_class_t opclass = CELLOP_AS_VALUE);
		cell_deref_node_t *cell(const cell_t& value, const worksheet* ws, cell_addr_mode_t attr, cell_op_class_t opclass = CELLOP_AS_VALUE);
		cellarea_deref_node_t *area(const cell_t& upper_left_corner, const cell_t& lower_right_corner, cell_addr_mode_t attr, cell_op_class_t opclass = CELLOP_AS_VALUE);
		cellarea_deref_node_t *area(const cell_t& upper_left_corner, const cell_t& lower_right_corner, const worksheet* ws, cell_addr_mode_t attr, cell_op_class_t opclass = CELLOP_AS_VALUE);
		unary_op_node_t *op(expr_operator_code_t op, expression_node_t* arg);
		binary_op_node_t *op(expr_operator_code_t op, expression_node_t* arg1, expression_node_t* arg2);
		z_ary_func_node_t *f(expr_function_code_t func, cell_op_class_t op_class = CELL_DEFAULT);
		unary_func_node_t *f(expr_function_code_t func, expression_node_t* arg, cell_op_class_t op_class = CELL_DEFAULT);
		binary_func_node_t *f(expr_function_code_t func, expression_node_t* arg1, expression_node_t* arg2, cell_op_class_t op_class = CELL_DEFAULT);
		n_ary_func_node_t *f(expr_function_code_t func, size_t argcount, expression_node_t** arg_arr, cell_op_class_t op_class = CELL_DEFAULT);
		userdef_func_node_t *udf(int expr_user_function, size_t argcount = 0, expression_node_t** arg_arr = NULL, cell_op_class_t op_class = CELL_DEFAULT);
#endif

void formulas(void)
{
	workbook wb;
	
	formula_text(wb);
	formula_numbers(wb);
	formula_numbers_sheets(wb);
	formula_database(wb);
	formula_date_time(wb);
	formula_date_time(wb);
	
	const char *fName = "/Volumes/Data/Users/dhoerl/Public/formula.xls";
	fprintf(stderr, "%s VERSION 2 \n", fName);
	fprintf(stderr, "Dump \n");
	wb.Dump(fName);
	fprintf(stderr, "Dump End \n");
}

void formula_date_time(workbook &wb)
{
	worksheet* sh = wb.sheet("Date and Time");
	expression_node_factory_t& maker = wb.GetFormulaFactory();

	sh->colwidth(0, 256*8);
	sh->colwidth(1, 256*15);
	sh->colwidth(2, 256*22);

	// NOW() and TEXT(cell, "mmm dd, yyyy  hh:mm")
	unsigned int row = 0;
	{
		sh->label(row, 0, "Now->");
		expression_node_t *f = maker.f(FUNC_NOW, CELL_DEFAULT);
		cell_t *c = sh->formula(row, 1, f, true);

		expression_node_t *today = maker.cell(*c, CELL_RELATIVE_A1, CELLOP_AS_VALUE);
		expression_node_t *format = maker.text("mmm dd, yyyy  hh:mm");

		expression_node_t *f1 = maker.f(FUNC_TEXT, today, format, CELL_DEFAULT);
		sh->formula(row, 2, f1, true);
	}
	
	// TODAY() and TEXT(cell, "mmm dd, yyyy")
	++row;
	{
		sh->label(row, 0, "Today->");
		expression_node_t *f = maker.f(FUNC_TODAY, CELL_DEFAULT);
		cell_t *c = sh->formula(row, 1, f, true);

		expression_node_t *today = maker.cell(*c, CELL_RELATIVE_A1, CELLOP_AS_VALUE);
		expression_node_t *format = maker.text("mmm dd, yyyy");

		expression_node_t *f1 = maker.f(FUNC_TEXT, today, format, CELL_DEFAULT);
		sh->formula(row, 2, f1, true);
	}

	// TODAY() and YEAR(cell)
	++row;
	{
		sh->label(row, 0, "Year->");
		expression_node_t *f = maker.f(FUNC_TODAY, CELL_DEFAULT);
		cell_t *c = sh->formula(row, 1, f, true);

		expression_node_t *today = maker.cell(*c, CELL_RELATIVE_A1, CELLOP_AS_VALUE);

		expression_node_t *f1 = maker.f(FUNC_YEAR, today, CELL_DEFAULT);
		sh->formula(row, 2, f1, true);
	}

}


void formula_text(workbook &wb)
{
	worksheet* sh = wb.sheet("TEXT");
	expression_node_factory_t& maker = wb.GetFormulaFactory();
	
	unsigned int len = 3;
	unsigned int row = 1;
	unsigned int formula_col = len + 1;

	{
		char buf[256];
		sprintf(buf, "Remark item %d/%d/%d", 1,2,3);

		sh->number(0,0, (signed32_t)99);
		//sh->note(0,0, buf, "GHO");
	}
	
	sh->colwidth(formula_col, 256*64);
	sh->label(0, formula_col, "FORMULAS");
	
	// CONCATENATE(cell, " ", cell, " ", cell)
	expression_node_t *cells[len*2];
	cell_t *real_cells[len];
	unsigned int idx = 0;
	for(unsigned int i=0; i<len; ++i) {
		char msg[64];
		sprintf(msg, "%c", '!' + row * i + i);
		cell_t *c = sh->label(row, i, msg);
		if(i) cells[idx++] = maker.text(" ");
		cells[idx++] = maker.cell(*c, CELL_RELATIVE_A1, CELLOP_AS_VALUE);
		
		// use later below
		real_cells[i] = c;
	}
	{
		expression_node_t *f = maker.f(FUNC_CONCATENATE, idx, cells, CELL_DEFAULT); // CELL_DEFAULT CELLOP_AS_ARRAY
		sh->formula(row, formula_col, f, true);
	}
	
	
	// CONCATENATE(TEXT(cell), " ", TEXT(cell), " ", TEXT(cell))
	++row;
	idx = 0;
	expression_node_t *funcs[len];
	for(unsigned int i=0; i<len; ++i) {
		cell_t *c = sh->number(row, i, 1+i);
		expression_node_t *cr = maker.cell(*c, CELL_RELATIVE_A1, CELLOP_AS_VALUE);
		if(i) funcs[idx++] = maker.text(" ");
		funcs[idx++] = maker.f(FUNC_TEXT, cr, CELL_DEFAULT); // CELL_DEFAULT CELLOP_AS_ARRAY
	}
	{
		expression_node_t *f = maker.f(FUNC_CONCATENATE, idx, funcs, CELL_DEFAULT); // CELL_DEFAULT CELLOP_AS_ARRAY
		sh->formula(row, formula_col, f, true);
	}
}

void formula_numbers(workbook &wb)
{
	worksheet* sh = wb.sheet("NUMBERS");
	expression_node_factory_t& maker = wb.GetFormulaFactory();
	
	unsigned int len = 4;
	unsigned int row = 1;
	unsigned int formula_col = len + 1;
	
	sh->label(0, formula_col, "FORMULAS");
	
	// SUM(cell, cell, cell, cell)
	expression_node_t *cells[len];
	for(unsigned int i=0; i<len; ++i) {
		cell_t *c = sh->number(row, i, 1+i);
		cells[i] = maker.cell(*c, CELL_RELATIVE_A1, CELLOP_AS_VALUE);
	}
	{
		expression_node_t *f = maker.f(FUNC_SUM, len, cells, CELL_DEFAULT); // CELL_DEFAULT CELLOP_AS_ARRAY
		sh->formula(row, formula_col, f, true);
	}
	
	// SUM(cell:cell)
	++row;
	cell_t *real_cells[len];
	for(unsigned int i=0; i<len; ++i) {
		real_cells[i] = sh->number(row, i, (1+i)*row);
		//cells[i] = maker.cell(*c, CELL_RELATIVE_A1, CELLOP_AS_VALUE);
	}
	{
		expression_node_t *area = maker.area((cell_t&)*(real_cells[0]), (cell_t&)*(real_cells[len-1]), CELL_RELATIVE_A1, CELLOP_AS_REFER);
		expression_node_t *areas[1];
		areas[0] = area;
		expression_node_t *f = maker.f(FUNC_SUM, 1, areas, CELL_DEFAULT);
		sh->formula(row, formula_col, f, true);
	}
	// SUM(cell:cell, 5.101)
	++row;
	for(unsigned int i=0; i<len; ++i) {
		real_cells[i] = sh->number(row, i, (1+i)*row);
		//cells[i] = maker.cell(*c, CELL_RELATIVE_A1, CELLOP_AS_VALUE);
	}
	{
		expression_node_t *area = maker.area((cell_t&)*(real_cells[0]), (cell_t&)*(real_cells[len-1]), CELL_RELATIVE_A1, CELLOP_AS_REFER);
		expression_node_t *args[2];
		args[0] = area;
		args[1] = maker.floating_point(5.101);
		
		expression_node_t *f = maker.f(FUNC_SUM, 2, args, CELL_DEFAULT);
		sh->formula(row, formula_col, f, true);
	}
}

void formula_numbers_sheets(workbook &wb)
{
	worksheet* sh1 = wb.sheet("Number 1");
	worksheet* sh2 = wb.sheet("Number 2");
	worksheet* shT = wb.sheet("InterSheetTotal");
	expression_node_factory_t& maker = wb.GetFormulaFactory();
	
	sh1->label(0, 0, "Number");
	cell_t *c1 = sh1->number(0, 1, 10);
	sh2->label(0, 0, "Number");
	cell_t *c2 = sh2->number(0, 1, 22);

	
	// SUM(cell, cell, cell, cell)
	expression_node_t *cells[2];
	cells[0] = maker.cell(*c1, CELL_RELATIVE_A1, CELLOP_AS_VALUE);
	cells[1] = maker.cell(*c2, CELL_RELATIVE_A1, CELLOP_AS_VALUE);

	expression_node_t *f = maker.f(FUNC_SUM, 2, cells, CELL_DEFAULT); // CELL_DEFAULT CELLOP_AS_ARRAY
	
	shT->label(0, 0, "TOTAL");
	shT->formula(0, 1, f, true);
}

void formula_database(workbook &wb)
{
	fprintf(stderr, "Start \n");
	worksheet* sh = wb.sheet("DATABASE");
	expression_node_factory_t& maker = wb.GetFormulaFactory();

	cell_t *topLeft, *botRight;
	uint32_t row, col /*, offset */;
	//offset = 0;
	
	// Headers
	for(col=1; col<=8; ++col) {
		char year[32];
		sprintf(year, "%d", 1999 + col);
		cell_t *c = sh->label(0, col, year);
		if(col==1) topLeft = c;
	}

	// Criteria
	cell_t *c1 = sh->label(10, 2, "2001");
	cell_t *c2 = sh->label(11, 2, ">9");
	cellarea_deref_node_t *a2 = maker.area(*c1, *c2, CELL_RELATIVE_A1, CELLOP_AS_REFER); // Note CELLOP_AS_REFER
	
	// Data
	for(row=1; row<=8; ++row) {
		for(col=1; col<=8; ++col) {
			cell_t *c = sh->number(row, col, row*col);
			if(col==8 && row == 8) botRight = c;
		}
	}
	
	cellarea_deref_node_t *a1 = maker.area(*topLeft, *botRight, CELL_RELATIVE_A1, CELLOP_AS_REFER); // Note CELLOP_AS_REFER
	integer_value_node_t *i1 = maker.integer(2);

	// How to pass multiple args
	expression_node_t *args[3];
	args[0] = a1;
	args[1] = i1;
	args[2] = a2;

	expression_node_t *f1 = maker.f(FUNC_DAVERAGE, (size_t)3, args);
	sh->label(10, 0, "DAVERAGE");
	sh->formula(10, 1, f1, true);
	
	cellarea_deref_node_t *b1	= new cellarea_deref_node_t(*a1);
	cellarea_deref_node_t *b2	= new cellarea_deref_node_t(*a2);
	integer_value_node_t *j1	= new integer_value_node_t(*i1);
	args[0] = b1;
	args[1] = j1;
	args[2] = b2;
	expression_node_t *f = maker.f(FUNC_DMAX, (size_t)3, args);
	sh->label(11, 0, "DMAX");
	sh->formula(11, 1, f, true);
}

#if 0
void formulas(void)
{

	fprintf(stderr, "Start \n");
	workbook wb;
	worksheet* sh = wb.sheet("FORMULAS");
	expression_node_factory_t& maker = wb.GetFormulaFactory();

	// add two numbers
	if(0){
		cell_t *c1 = sh->number(0, 0, 1);
		cell_deref_node_t *cd1 = maker.cell(*c1, CELL_RELATIVE_A1);
		cell_t *c2 = sh->number(0, 1, 1);
		cell_deref_node_t *cd2 = maker.cell(*c2, CELL_RELATIVE_A1);

		expression_node_t *en = maker.f(FUNC_SUM, cd1, cd2);
		sh->formula(0, 32, en, true);
	}

	// add from 1 to 30 numbers
	uint32_t row, col, offset;
	offset = 0;
	for(row=0; row<32; ++row) {
		expression_node_t *cells[row];
		for(col=0; col<row; ++col) {
			cell_t *c = sh->number(row+offset, col, 1);
			expression_node_t *cdn = maker.cell(*c, CELL_RELATIVE_A1);
			cells[col] = cdn;
		}
		if(row == 0) continue;

		expression_node_t *f = maker.f(FUNC_SUM, (size_t)col, cells); // (expression_node_t **)
if(row==31) {
	sh->formula(row, 32, f);
	printf("WRITE FORMULA\n");
}
	}
	
	// concatenate from 1 to 30 strings
	offset = 40;
	if(0)for(row=0; row<31; ++row) {
		expression_node_t *cells[row];
		for(col=0; col<row; ++col) {
			char foo[2] = { static_cast<char>('A' + (char)col), (char)0 };
			cell_t *c = sh->label(row+offset, col, foo);
			expression_node_t *cdn = maker.cell(*c, CELL_RELATIVE_A1);
			cells[col] = cdn;
		}
		if(row == 0) continue;

		expression_node_t *f = maker.f(FUNC_CONCATENATE, (size_t)col, cells); // (expression_node_t **)
		sh->formula(row+offset, 32, f);
	}


#if 0
	// row & col
	cell_t *a1t = sh->number(0, 0, -10);
				  sh->number(1, 0,  20);
	cell_t *a1b = sh->number(2, 0, -30);
	
	cell_t *a2t = sh->number(0, 1,  10);
				  sh->number(1, 1, -20);
	cell_t *a2b = sh->number(2, 1,  30);

	cellarea_deref_node_t *a1 = maker.area(*a1t, *a1b, CELL_RELATIVE_A1, CELLOP_AS_ARRAY);
	cellarea_deref_node_t *a2 = maker.area(*a2t, *a2b, CELL_RELATIVE_A1, CELLOP_AS_ARRAY);
	expression_node_t *i1 = maker.op(OP_GT, a1, a2);


	cellarea_deref_node_t *aa1 = maker.area(*a1t, *a1b, CELL_RELATIVE_A1, CELLOP_AS_ARRAY);
	cellarea_deref_node_t *aa2 = maker.area(*a2t, *a2b, CELL_RELATIVE_A1, CELLOP_AS_ARRAY);
	expression_node_t *args[3] = { i1, aa1, aa2 };
	expression_node_t *i2 = maker.f(FUNC_IF, 3, args, CELLOP_AS_ARRAY);
	expression_node_t *pExpFormula = maker.f(FUNC_SUM, i2, CELLOP_AS_ARRAY);

	sh->formula(0, 2, true, pExpFormula, true);
#endif

	const char *fName = "/Volumes/Data/Users/dhoerl/Public/formula.xls";
	fprintf(stderr, "%s VERSION 2 \n", fName);
	fprintf(stderr, "Dump \n");
	wb.Dump(fName);
	fprintf(stderr, "Dump End \n");
}
#endif



#if 0
	{
		fprintf(stderr, "Start \n");
		workbook wb;
		worksheet* sh = wb.sheet("ARRAY FORMULA");
		expression_node_factory_t& maker = wb.GetFormulaFactory();



#if 1
		// row & col
		cell_t *a1t = sh->number(0, 0, -10);
					  sh->number(1, 0,  20);
		cell_t *a1b = sh->number(2, 0, -30);
		
		cell_t *a2t = sh->number(0, 1,  10);
					  sh->number(1, 1, -20);
		cell_t *a2b = sh->number(2, 1,  30);

		cellarea_deref_node_t *a1 = maker.area(*a1t, *a1b, CELL_RELATIVE_A1, CELLOP_AS_ARRAY);
		cellarea_deref_node_t *a2 = maker.area(*a2t, *a2b, CELL_RELATIVE_A1, CELLOP_AS_ARRAY);
		expression_node_t *i1 = maker.op(OP_GT, a1, a2);


		cellarea_deref_node_t *aa1 = maker.area(*a1t, *a1b, CELL_RELATIVE_A1, CELLOP_AS_ARRAY);
		cellarea_deref_node_t *aa2 = maker.area(*a2t, *a2b, CELL_RELATIVE_A1, CELLOP_AS_ARRAY);
		expression_node_t *args[3] = { i1, aa1, aa2 };
		expression_node_t *i2 = maker.f(FUNC_IF, 3, args, CELLOP_AS_ARRAY);
		expression_node_t *pExpFormula = maker.f(FUNC_SUM, i2, CELLOP_AS_ARRAY);

		sh->formula(0, 2, true, pExpFormula, true);
#else
		// row & col
		cell_t *a1t = sh->number(0, 0, -10);
		cell_t *a2t = sh->number(0, 1,  10);

		cell_deref_node_t *c1 = maker.cell(*a1t, CELL_RELATIVE_A1);
		cell_deref_node_t *c2 = maker.cell(*a2t, CELL_RELATIVE_A1);
		expression_node_t *i1 = maker.op(OP_GT, c1, c2);

		cell_deref_node_t *cc1 = maker.cell(*a1t, CELL_RELATIVE_A1);
		cell_deref_node_t *cc2 = maker.cell(*a2t, CELL_RELATIVE_A1);
		expression_node_t *args[3] = { i1, cc1, cc2 };
	
		expression_node_t *pExpFormula = maker.f(FUNC_IF, 3, args);

		sh->formula(0, 2, pExpFormula, true);
#endif


#if 0
		boolean_value_node_t *boolean(bool value);
		integer_value_node_t *integer(signed32_t value);
		float_value_node_t *floating_point(double value);
		error_value_node_t *error_value(errcode_t value);
		missing_arg_node_t *missing_arg(void);
		text_value_node_t *text(const std::string& value);
		text_value_node_t *text(const xlslib_strings::u16string& value);
		
		cell_deref_node_t *cell(const cell_t& value, cell_addr_mode_t attr, cell_op_class_t opclass = CELLOP_AS_VALUE);
		cell_deref_node_t *cell(const cell_t& value, const worksheet* ws, cell_addr_mode_t attr, cell_op_class_t opclass = CELLOP_AS_VALUE);
		cellarea_deref_node_t *area(const cell_t& upper_left_corner, const cell_t& lower_right_corner, cell_addr_mode_t attr, cell_op_class_t opclass = CELLOP_AS_VALUE);
		cellarea_deref_node_t *area(const cell_t& upper_left_corner, const cell_t& lower_right_corner, const worksheet* ws, cell_addr_mode_t attr, cell_op_class_t opclass = CELLOP_AS_VALUE);
		unary_op_node_t *op(expr_operator_code_t op, expression_node_t* arg);
		binary_op_node_t *op(expr_operator_code_t op, expression_node_t* arg1, expression_node_t* arg2);
		z_ary_func_node_t *f(expr_function_code_t func);
		unary_func_node_t *f(expr_function_code_t func, expression_node_t* arg);
		binary_func_node_t *f(expr_function_code_t func, expression_node_t* arg1, expression_node_t* arg2);
		n_ary_func_node_t *f(expr_function_code_t func, size_t argcount, expression_node_t** arg_arr = NULL);
		userdef_func_node_t *udf(int expr_user_function, size_t argcount = 0, expression_node_t** arg_arr = NULL);
#endif
		// FUNC_IF

		
		
#endif
