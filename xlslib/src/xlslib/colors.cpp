/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * This file is part of xlslib -- A multiplatform, C/C++ library
 * for dynamic generation of Excel(TM) files.
 *
 * Copyright 2009 David Hoerl All Rights Reserved.
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
#include "xlslib/colors.h"
#include "xlslib/datast.h"
#include "xlslib/rectypes.h"


using namespace std;
using namespace xlslib_core;

#define NUM_PALETTE		56

static color_entry_t default_palette[NUM_PALETTE] =
{
	{0x00, 0x00, 0x00, 0x00}, {0xff, 0xff, 0xff, 0x00}, { 0xff, 0x00, 0x00, 0x00}, {0x00, 0xff, 0x00, 0x00},
	{0x00, 0x00, 0xff, 0x00}, {0xff, 0xff, 0x00, 0x00}, { 0xff, 0x00, 0xff, 0x00}, {0x00, 0xff, 0xff, 0x00},
	{0x80, 0x00, 0x00, 0x00}, {0x00, 0x80, 0x00, 0x00}, { 0x00, 0x00, 0x80, 0x00}, {0x80, 0x80, 0x00, 0x00},
	{0x80, 0x00, 0x80, 0x00}, {0x00, 0x80, 0x80, 0x00}, { 0xc0, 0xc0, 0xc0, 0x00}, {0x80, 0x80, 0x80, 0x00},
	{0x99, 0x99, 0xff, 0x00}, {0x99, 0x33, 0x66, 0x00}, { 0xff, 0xff, 0xcc, 0x00}, {0xcc, 0xff, 0xff, 0x00},
	{0x66, 0x00, 0x66, 0x00}, {0xff, 0x80, 0x80, 0x00}, { 0x00, 0x66, 0xcc, 0x00}, {0xcc, 0xcc, 0xff, 0x00},
	{0x00, 0x00, 0x80, 0x00}, {0xff, 0x00, 0xff, 0x00}, { 0xff, 0xff, 0x00, 0x00}, {0x00, 0xff, 0xff, 0x00},
	{0x80, 0x00, 0x80, 0x00}, {0x80, 0x00, 0x00, 0x00}, { 0x00, 0x80, 0x80, 0x00}, {0x00, 0x00, 0xff, 0x00},
	{0x00, 0xcc, 0xff, 0x00}, {0xcc, 0xff, 0xff, 0x00}, { 0xcc, 0xff, 0xcc, 0x00}, {0xff, 0xff, 0x99, 0x00},
	{0x99, 0xcc, 0xff, 0x00}, {0xff, 0x99, 0xcc, 0x00}, { 0xcc, 0x99, 0xff, 0x00}, {0xe3, 0xe3, 0xe3, 0x00},
	{0x33, 0x66, 0xff, 0x00}, {0x33, 0xcc, 0xcc, 0x00}, { 0x99, 0xcc, 0x00, 0x00}, {0xff, 0xcc, 0x00, 0x00},
	{0xff, 0x99, 0x00, 0x00}, {0xff, 0x66, 0x00, 0x00}, { 0x66, 0x66, 0x99, 0x00}, {0x96, 0x96, 0x96, 0x00},
	{0x00, 0x33, 0x66, 0x00}, {0x33, 0x99, 0x66, 0x00}, { 0x00, 0x33, 0x00, 0x00}, {0x33, 0x33, 0x00, 0x00},
	{0x99, 0x33, 0x00, 0x00}, {0x99, 0x33, 0x66, 0x00}, { 0x33, 0x33, 0x99, 0x00}, {0x33, 0x33, 0x33, 0x00}
};

/*
 *********************************
 * blank_t class implementation
 *********************************
 */
colors_t::colors_t() :
	colors(NULL)
{
}

bool colors_t::setColor(unsigned8_t r, unsigned8_t g, unsigned8_t b, unsigned8_t idx)
{
	color_entry_t	*clr;

	if(idx < 0x8 || idx >= (NUM_PALETTE+8)) {
		return false;   // illegal value
	}
	if(!colors) {
		colors = new color_entry_t[NUM_PALETTE];
		memcpy(colors, default_palette, NUM_PALETTE*sizeof(color_entry_t));
	}
	clr = colors + idx - 8;

	clr->r		= r;
	clr->g		= g;
	clr->b		= b;
	clr->nuttin	= 0;

	return true;
}

CUnit* colors_t::GetData(CDataStorage &datastore) const
{
	return datastore.MakeCPalette(colors ? colors : default_palette);
}

colors_t::~colors_t()
{
	delete[] colors;
}

/*
 **********************************
 * CPalette class implementation
 **********************************
 */
CPalette::CPalette(CDataStorage &datastore, const color_entry_t *colors) :
	CRecord(datastore)
{
	SetRecordType(RECTYPE_PALETTE);

	AddValue16(NUM_PALETTE);
	AddDataArray((unsigned8_t *)colors, NUM_PALETTE*sizeof(color_entry_t));

	SetRecordLength(GetDataSize()-RECORD_HEADER_SIZE);
}

CPalette::~CPalette()
{
}
