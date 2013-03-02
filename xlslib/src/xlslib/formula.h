
#ifndef FORMULAST_H
#define FORMULAST_H

#include "common/xlsys.h"
#include "common/systype.h"

#include "xlslib/err.h"			// need enum values
#include "xlslib/formula_const.h"

namespace xlslib_core
{
	class CDataStorage;
    class CUnit;
    class estimated_formula_result_t;

	/*
	 *  @return a bit for each number of arguments supported by this function.
	 *
	 *  @note
	 *  Bit 0 (0x0001U) indicates whether the function accepts @e zero (0) arguments.
	 *
	 *  Bit 1..14 indicate whether the function accepts 1..14 arguments.
	 *
	 *  Bit 15 (0x8000U) indicates the function accepts more than 14 arguments.
	 */
	unsigned16_t NumberOfArgsForExcelFunction(expr_function_code_t func);


    class formula_t
    {
		friend class cell_deref_node_t;
		friend class cellarea_deref_node_t;

    public:
        formula_t(CGlobalRecords& glbl, worksheet* ws);
        virtual ~formula_t();

        void DumpData(CUnit& dst) const;
        size_t GetSize() const;
        void GetResultEstimate(estimated_formula_result_t &dst) const;

        signed8_t PushBoolean(bool value);
        signed8_t PushMissingArgument();
        signed8_t PushError(unsigned8_t value);
        signed8_t PushInteger(signed32_t value);
        signed8_t PushFloatingPoint(double value);
        signed8_t PushOperator(expr_operator_code_t op);
        signed8_t PushCellReference(const cell_t& cell, cell_addr_mode_t opt);
        signed8_t PushCellAreaReference(const cell_t& upper_left_cell, const cell_t& lower_right_cell, cell_addr_mode_t opt);
        signed8_t PushFunction(expr_function_code_t func);
        signed8_t PushFunction(expr_function_code_t func, size_t argcount);
        signed8_t PushText(const std::string& v);
        signed8_t PushText(const u16string& value);
        signed8_t PushTextArray(const std::vector<std::string>& vec);
        signed8_t PushFloatingPointArray(const std::vector<double>& vec);

    protected:
        CDataStorage *data_storage;
		CGlobalRecords& m_GlobalRecords;
        worksheet *m_Worksheet;

        CUnit *main_data;
        CUnit *aux_data;

	private:
		signed8_t PushReference(unsigned32_t row, unsigned32_t col, unsigned32_t idx, cell_addr_mode_t opt);
        signed8_t PushAreaReference(unsigned32_t ul_row, unsigned32_t ul_col, unsigned32_t ul_idx, unsigned32_t lr_row, unsigned32_t lr_col, unsigned32_t lr_idx, cell_addr_mode_t opt);
    };
}

#endif
