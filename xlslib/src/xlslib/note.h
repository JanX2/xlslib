/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * This file is part of xlslib -- A multiplatform, C/C++ library
 * for dynamic generation of Excel(TM) files.
 *
 * Copyright 2004 Yeico S. A. de C. V. All Rights Reserved.
 * Copyright 2008-2013 David Hoerl All Rights Reserved.
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

#ifndef NOTE_H
#define NOTE_H

#include "common/xlsys.h"
#include "common/systype.h"

#include "xlslib/cell.h"	// superclass

// #include "common/xls_pshpack2.h"

namespace xlslib_core
{
	// Used internally
	struct sheet_notes {
		uint16_t	sheet_idx;
		uint16_t	sheet_notes;		// can be a total count, or an index less than that
	};
	
	// Google "[MS-XLS]", look at section 2.5.191 OfficeArtClientAnchorChart
	struct NoteRec {
		uint16_t	Flag;
		uint16_t	Col1;
		uint16_t	DX1;
		uint16_t	Row1;
		uint16_t	DY1;
		uint16_t	Col2;
		uint16_t	DX2;
		uint16_t	Row2;
		uint16_t	DY2;
	};

	class note_t : public cell_t
	{
		friend class worksheet;

	private:
		note_t(CGlobalRecords& gRecords, unsigned32_t rowval, unsigned32_t colval, const std::string& author, const std::string& text, xf_t* pxfval = NULL);
		note_t(CGlobalRecords& gRecords, unsigned32_t rowval, unsigned32_t colval, const xlslib_strings::ustring& author, const xlslib_strings::ustring& text, xf_t* pxfval = NULL);
#ifndef __FRAMEWORK__
		note_t(CGlobalRecords& gRecords, unsigned32_t rowval, unsigned32_t colval, const xlslib_strings::u16string& author, const xlslib_strings::u16string& text, xf_t* pxfval = NULL);
#endif
		void MakeDrawing(CRecord *data, unsigned32_t& currentSPID, unsigned16_t sheetIndex, unsigned16_t notesInThisSheet) const;
		void Finalize(unsigned16_t rowval, unsigned16_t colval);
		virtual ~note_t();

	private:
		xlslib_strings::u16string author;
		xlslib_strings::u16string text;
		NoteRec noteRec;
		unsigned32_t fillColor;
		unsigned16_t idx;

	public:
		virtual size_t GetSize(void) const;
		virtual CUnit* GetData(CDataStorage &datastore) const;
		unsigned16_t GetIndex(void) const { return idx; }
		NoteRec GetNoteRect(void) const { return noteRec; }
		void SetNoteRect(const NoteRec nRect) { noteRec = nRect; }
		void SetFillColor(uint8_t red, uint8_t green, uint8_t blue) { fillColor = ((unsigned32_t)blue << 16) | ((unsigned32_t)green << 8) | (unsigned32_t)red; }

		const xlslib_strings::u16string& GetAuthor(void) const {return author; }
		const xlslib_strings::u16string& GetNote(void) const {return text; }
	
	private:
		void SetIndex(unsigned16_t index) { idx = index; }
		void dumpDrawingContainer(CRecord *data, unsigned16_t sheetIndex, unsigned32_t& currentSPID, uint32_t noteCount) const;
		void dumpDrawingText(CRecord *data, unsigned16_t sheetIndex, unsigned32_t& currentSPID) const;
	};


	// forward ref
	class CDataStorage;

	class CNote : public CRecord
	{
		friend class CDataStorage;

	public:
		static void MakeDrawingGroup(CRecord *data, unsigned32_t count, const sheet_notes *notes);

	protected:
		CNote(CDataStorage &datastore, const note_t& notedef);

	private:
		virtual ~CNote();
	};
}

// #include "common/xls_poppack.h"

#endif
