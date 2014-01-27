/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * This file is part of xlslib -- A multiplatform, C/C++ library
 * for dynamic generation of Excel(TM) files.
 *
 * Copyright 2004 Yeico S. A. de C. V. All Rights Reserved.
 * Copyright 2008-2014 David Hoerl All Rights Reserved.
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
#include "xlslib/note.h"
#include "xlslib/globalrec.h"
#include "xlslib/datast.h"
#include "xlslib/rectypes.h"

using namespace xlslib_core;
using namespace xlslib_strings;

static unsigned char draw001[18] = { 0xbf, 0x00, 0x08, 0x00, 0x08, 0x00, 0x81,
    0x01, 0x09, 0x00, 0x00, 0x08, 0xc0, 0x01, 0x40, 0x00, 0x00, 0x08, 
  };
static unsigned char draw002[16] = { 0x0d, 0x00, 0x00, 0x08, 0x0c, 0x00, 0x00,
    0x08, 0x17, 0x00, 0x00, 0x08, 0xf7, 0x00, 0x00, 0x10, 
  };

static unsigned char draw101[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  };
static unsigned char draw102[6] = { 0xBF, 0x00, 0x20, 0x00, 0x20, 0x00,
  };

/*
 *********************************
 *  note_t class implementation
 *********************************
 */
note_t::note_t(CGlobalRecords& gRecords, unsigned32_t rowval, unsigned32_t colval, const std::string& auth, const std::string& msg, xf_t* pxfval) :
	cell_t(gRecords, rowval, colval, pxfval),
	author(),
	text(),
	noteRec(),
	fillColor(0x08000050),
	idx(0)
{
	gRecords.char2str16(auth, this->author);
	gRecords.char2str16(msg, this->text);
	Finalize((unsigned16_t)rowval, (unsigned16_t)colval);
}

note_t::note_t(CGlobalRecords& gRecords, unsigned32_t rowval, unsigned32_t colval, const ustring& auth, const ustring& msg, xf_t* pxfval) :
	cell_t(gRecords, rowval, colval, pxfval),
	author(),
	text(),
	noteRec(),
	fillColor(0x08000050),
	idx(0)
{
	gRecords.wide2str16(auth, this->author);
	gRecords.wide2str16(msg, this->text);
	Finalize((unsigned16_t)rowval, (unsigned16_t)colval);
}

#ifndef __FRAMEWORK__
note_t::note_t(CGlobalRecords& gRecords, unsigned32_t rowval, unsigned32_t colval, const u16string& auth, const u16string& msg, xf_t* pxfval) :
	cell_t(gRecords, rowval, colval, pxfval),
	author(auth),
	text(msg),
	noteRec(),
	fillColor(0x08000050),
	idx(0)
{
	Finalize((unsigned16_t)rowval, (unsigned16_t)colval);
}
#endif

void note_t::Finalize(unsigned16_t rowval, unsigned16_t colval)
{
	size_t alen = author.length();
	if(alen == 0 || alen > 54) {
		xchar16_t warning[] = { 'A', 'u', 't', 'h', 'o', 'r', ' ', 'M', 'u', 's', 't', ' ', 'B', 'e', ' ', '1', ' ', 't', 'o', ' ', '5', '4', 'c', 'h', 'a', 'r', 's', ' ', 'l', 'o', 'n', 'g', 0};
		author.assign(warning);
		alen = author.length();
	}
	size_t tlen = text.length();
	if(tlen == 0 || tlen > 2048) {
		xchar16_t warning[] = { 'T', 'e', 'x', 't',  ' ', 'M', 'u', 's', 't', ' ', 'B', 'e', ' ', '1', ' ', 't', 'o', ' ', '2', '0', '4', '8', 'c', 'h', 'a', 'r', 's', ' ', 'l', 'o', 'n', 'g', 0};
		text.assign(warning);
		tlen = text.length();
	}

	size_t totalLen = alen + tlen + 2;
	size_t count = totalLen/11;
	bool lastIsNL = false;
	for(size_t i=0; i<tlen; ++i) {
		if(text[i] == '\n') {
			count += lastIsNL ? 2 : 1;
			lastIsNL = true;
		} else {
			lastIsNL = false;
		}
	}
	unsigned16_t lines = (unsigned16_t)(1 /* for author */ + (count+1)/2 /* measured average chars/column width */ + 1 /* final partial line */);
	
	unsigned16_t cols = 2;
	while(lines > 20) {
		cols *= 2;
		lines /= 2;
	}
	
	noteRec.Flag	= (unsigned16_t)0x3;
	noteRec.Col1	= colval+(unsigned16_t)1;
	noteRec.DX1		= (unsigned16_t)0xCD;
	noteRec.Row1	= rowval;
	noteRec.DY1		= (unsigned16_t)0x27;
	noteRec.Col2	= colval+(unsigned16_t)1 + cols;
	noteRec.DX2		= (unsigned16_t)0x7B;
	noteRec.Row2	= rowval+lines;
	noteRec.DY2		= (unsigned16_t)0x3B;
}

static size_t dumpDrawData(CRecord *data, unsigned16_t rec, unsigned16_t instance, unsigned16_t type, unsigned32_t len, size_t arr_len, const unsigned8_t *arr)
{
	unsigned16_t val = rec & 0x000F;
	val |= instance << 4;
	data->AddValue16(val);
	data->AddValue16(type);

	size_t len_position = data->GetDataSize();
	data->AddValue32(len);
	
	if(arr_len) {
		data->AddDataArray(arr, arr_len);
	}
	
	return len_position;
}
static void drawDataOPID(CRecord *data, unsigned16_t opid, unsigned32_t val)
{
	data->AddValue16(opid);
	data->AddValue32(val);
}


static const uint32_t drawingSize = 8 + 128;	// header, 120 is the main blob, 8 is the trailing blob

void note_t::dumpDrawingContainer(CRecord *data, unsigned16_t sheetIndex, unsigned32_t& currentSPIDidx, uint32_t noteCount) const
{
	uint32_t thisDrawingExtra = 40 + 8;	//
	uint32_t completeDrawingSize =  8/*f002*/ + 16/*f008*/ + thisDrawingExtra/*first one*/ + drawingSize*noteCount/*every note*/;
	dumpDrawData(data,  0xf, 0x0, 0xf002, completeDrawingSize,  0, NULL /* len=0 */ ) ;  // OfficeArtDgContainer
		dumpDrawData(data,  0x0, 0x1, 0xf008, 8,  0, NULL /* len=0 */ ) ;  // OfficeArtFDG - spidCur needs to be set
		data->AddValue32(noteCount+1);	// csp (4 bytes): An unsigned integer that specifies the number of shapes in this drawing
		data->AddValue32(CGlobalRecords::MakeSPID(sheetIndex, currentSPIDidx)+noteCount);	// spidCur (4 bytes): An MSOSPID structure, as defined in section 2.1.2, that specifies the shape identifier of the last shape in this drawing
		dumpDrawData(data,  0xF, 0x0, 0xF003, thisDrawingExtra+drawingSize*noteCount, 0, NULL /* len=0 */ ) ;  // OfficeArtSpgrContainer
			dumpDrawData(data,  0xF, 0x0, 0xF004, 40,  0, NULL /* len=0 */ ) ;  // OfficeArtSpContainer
				dumpDrawData(data,  0x1, 0x0, 0xF009, 16,  16, draw101 /* len=16 */ ) ;  // OfficeArtFSPGR
				dumpDrawData(data,  0x2, 0x0, 0xF00A, 8,  0, NULL /* len=0 */ ) ;  // OfficeArtFSP
					data->AddValue32(CGlobalRecords::MakeSPID(sheetIndex, currentSPIDidx++));
					data->AddValue32(0x05);
}

void note_t::dumpDrawingText(CRecord *data, unsigned16_t sheetIndex, unsigned32_t& currentSPIDidx) const
{
	dumpDrawData(data,  0xF, 0x0, 0xF004, drawingSize-8,  0, NULL /* len=0 */ ) ;  // OfficeArtSpContainer
		dumpDrawData(data,  0x2, 0xCA, 0xF00A, 8,  0, NULL /* len=0 */ ) ;  // OfficeArtFSP
			data->AddValue32(CGlobalRecords::MakeSPID(sheetIndex, currentSPIDidx++));
			data->AddValue32(0x0A00);
		dumpDrawData(data,  0x3, 0x8, 0xF00B, 48,  0, NULL /* len=48 */ ) ;  // OfficeArtFOPT
		drawDataOPID(data, 0x0085, 0x00000001);
			drawDataOPID(data, 0x00BF, 0x000A0008);
			drawDataOPID(data, 0x0158, 0x00000000);
			drawDataOPID(data, 0x0181, fillColor);
			drawDataOPID(data, 0x0183, 0x08000050);
			drawDataOPID(data, 0x01BF, 0x00100010);
			drawDataOPID(data, 0x023F, 0x00030003);
			drawDataOPID(data, 0x03BF, 0x00020002);
		dumpDrawData(data,  0x3, 0x1, 0xF122, 6,  6, draw102 /* len=6 */ ) ;  // OfficeArtTertiaryFOPT
		dumpDrawData(data,  0x0, 0x0, 0xF010, 18,  0, NULL /* len=18 */ ) ;  // msofbtClientAnchor (was draw104)
			data->AddValue16(noteRec.Flag);
			data->AddValue16(noteRec.Col1);
			data->AddValue16(noteRec.DX1);
			data->AddValue16(noteRec.Row1);
			data->AddValue16(noteRec.DY1);
			data->AddValue16(noteRec.Col2);
			data->AddValue16(noteRec.DX2);
			data->AddValue16(noteRec.Row2);
			data->AddValue16(noteRec.DY2);
		dumpDrawData(data,  0x0, 0x0, 0xF011, 0,  0, NULL /* len=0 */ ) ;  // msofbtClientData
}

void note_t::MakeDrawing(CRecord *data, unsigned32_t& currentSPID, unsigned16_t sheetIndex, unsigned16_t notesInThisSheet) const
{
	xlslib_strings::u16string fullStr = author;
	fullStr.append(1, (unsigned16_t)':');
	fullStr.append(1, (unsigned16_t)'\n');
	unsigned16_t authorLen = (unsigned16_t)fullStr.length();
	fullStr.append(text);
	unsigned16_t totalLen = (unsigned16_t)fullStr.length();
	
	// All the records below
	data->Inflate(4 + (idx == 0 ? 90 : 0) + drawingSize +   30 + 16 + 22 +  4+2+totalLen*(CGlobalRecords::IsASCII(fullStr)?1:2) + 28);

	data->SetRecordType(RECTYPE_MSODRAWING);
	
	if(idx == 0) {
		dumpDrawingContainer(data, sheetIndex, currentSPID, notesInThisSheet);
	}
	dumpDrawingText(data, sheetIndex, currentSPID);

	size_t offset = data->GetDataSize();
	data->SetRecordLength(offset - RECORD_HEADER_SIZE);

#if 0
	OBJ from Apple 'Numbers', so we dont need to create a GUID for the item
	ID: 005Dh OBJ (Describes a Graphic Object)
	Size: 26
	OBJ ft=0x15 cb=0x12 ot=0x19 idx=0x3 flags= len=4  0x00 0x00
#endif

	data->AddValue16(RECTYPE_OBJ);
	data->AddValue16(26);
	
	data->AddValue16(0x15);		// ft
	data->AddValue16(0x12);		// cb
	data->AddValue16(0x19);		// ot
	data->AddValue16(idx+1);	// id
	data->AddValue16(0x6011);	// flag bitfield
	
	data->AddValue32(0);		// must be 0
	data->AddValue32(0);		// must be 0
	data->AddValue32(0);		// must be 0

	data->AddValue32(0);		// must be 0


	data->AddValue16(RECTYPE_MSODRAWING);
	data->AddValue16(8);
	dumpDrawData(data,  0x0, 0x0, 0xF00D, 0,  0, NULL /* len=0 */ ) ;  // msofbtClientTextbox

#if 0
	ID: 01B6h TXO (Text Object)
	Size: 18
	TXO: grbit=0x0212 rot=0x0000 chText=0x0013 cbRuns=0x0018
#endif
		
	data->AddValue16(RECTYPE_TXO);
	data->AddValue16(18);

	data->AddValue16(0x0212);		// grbit
	data->AddValue16(0);			// rot
	data->AddFixedDataArray(0, 6);	// reserved
	data->AddValue16(totalLen);		// chText - len of text, first CONTINUE
	data->AddValue16(8*3);			// cbRuns - len or runs, second CONTINUE. Always 3
	data->AddFixedDataArray(0, 4);	// reserved

	data->AddValue16(RECTYPE_CONTINUE);
	offset = data->GetDataSize();
	data->AddValue16(0);

	size_t stringStart = data->GetDataSize();
	data->AddUnicodeString(fullStr, CRecord::NOLEN_FLAGS_UNICODE);
	data->SetValueAt16((unsigned16_t)(data->GetDataSize() - stringStart), (unsigned)offset);
	
	data->AddValue16(RECTYPE_CONTINUE);
	data->AddValue16(3*8);
	
	data->AddValue16(0);					// text index
	data->AddValue16(0x1);					// text font
	data->AddValue32(0);					// unknown
	
	data->AddValue16(authorLen);			// text index
	data->AddValue16(0x0);					// text font
	data->AddValue32(0);					// unknown
	
	data->AddValue16(totalLen);					// text index
	data->AddValue16(0);					// text font
	data->AddValue32(0);					// unknown
}

note_t::~note_t()
{
}

size_t note_t::GetSize(void) const
{
	return 12;
}

CUnit* note_t::GetData(CDataStorage &datastore) const
{
	return datastore.MakeCNote(*this);  // NOTE: this pointer HAS to be deleted elsewhere.
}

/*
 *********************************
 *  CNote class implementation
 *********************************
 *
 */
void CNote::MakeDrawingGroup(CRecord *data, unsigned32_t count, const sheet_notes *notes)
{
	size_t curr_offset;
	
	data->Inflate(82 + count*8);
	data->SetRecordType(RECTYPE_MSODRAWINGGROUP);

	size_t f000_len = dumpDrawData(data, 0xf, 0x0, 0xf000, 0, 0, NULL /* len=0 */ );  // OfficeArtDggContainer

	// Need to get he max spid, which is the last sheet combined with the number of notes
#if 0
		spidMax (4 bytes): An MSOSPID structure, as defined in section 2.1.2, specifying the current maximum shape identifier that is used in any drawing. This value MUST be less than 0x03FFD7FF.
		cidcl (4 bytes): An unsigned integer that specifies the number of OfficeArtIDCL records, as defined in section 2.2.46, + 1. This value MUST be less than 0x0FFFFFFF.
		cspSaved (4 bytes): An unsigned integer specifying the total number of shapes that have been saved in all of the drawings.
		cdgSaved (4 bytes): An unsigned integer specifying the total number of drawings that have been saved in the file.
#endif
	
		size_t f006_offset = data->GetDataSize();
		size_t f006_len = dumpDrawData(data,  0x0, 0x0, 0xf006, 0,  0, NULL /* len=0 */ );

		struct sheet_notes last_sheet = notes[count-1];		// last one
		unsigned32_t spid = CGlobalRecords::MakeSPID(last_sheet.sheet_idx, last_sheet.sheet_notes);
		data->AddValue32(spid);								// spidMax
		data->AddValue32(count+1);							// cidcl
		
		size_t cspSaved_offset = data->GetDataSize();
		data->AddValue32(0);								// cspSaved
		
		data->AddValue32(count);							// cdgSaved
		
		unsigned32_t total = 0;
		for(unsigned32_t i=0; i<count; ++i) {
			struct sheet_notes sn = notes[i];

			total += sn.sheet_notes;

			data->AddValue32(i+1);							// dgid
			data->AddValue32(1 + sn.sheet_notes);			// cspidCur
		}
		data->SetValueAt32(count + total, (unsigned)cspSaved_offset);

		curr_offset = data->GetDataSize();
		data->SetValueAt32((unsigned)(curr_offset - f006_offset - 8), (unsigned)f006_len);

		dumpDrawData(data,  0x3, 0x3, 0xf00b, 18,  18, draw001 /* len=18 */ ) ;  // OfficeArtFOPT
		dumpDrawData(data,  0x0, 0x4, 0xf11e, 16,  16, draw002 /* len=16 */ ) ;  // OfficeArtSplitMenuColorContainer
	
	curr_offset = data->GetDataSize();
	data->SetValueAt32((unsigned)(curr_offset - 8 - RECORD_HEADER_SIZE), (unsigned)f000_len);
	data->SetRecordLength(curr_offset-RECORD_HEADER_SIZE);
}

CNote::CNote(CDataStorage &datastore, const note_t& notedef) :
	CRecord(datastore)
{
	bool isAscii = CGlobalRecords::IsASCII(notedef.GetAuthor());
	Inflate(4 + 2 + 2 + 2 + 2 + 2 + 1 + notedef.GetAuthor().size() * (isAscii ? 1 : 2) );

	SetRecordType(RECTYPE_NOTE);
	AddValue16((unsigned16_t)notedef.GetRow());
	AddValue16((unsigned16_t)notedef.GetCol());
	AddValue16(0); // grBit
	AddValue16(notedef.GetIndex()+1);
	AddUnicodeString(notedef.GetAuthor(), LEN2_FLAGS_UNICODE);
	AddValue8(0);

	SetRecordLength(GetDataSize()-RECORD_HEADER_SIZE);
}
CNote::~CNote()
{
}

