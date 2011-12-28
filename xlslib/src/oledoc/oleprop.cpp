/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * This file is part of xlslib -- A multiplatform, C/C++ library
 * for dynamic generation of Excel(TM) files.
 *
 * Copyright 2004 Yeico S. A. de C. V. All Rights Reserved.
 * Copyright 2008-2011 David Hoerl All Rights Reserved.
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
#include "oledoc/oleprop.h"
#include "xlslib/datast.h"

/*
 ***********************************
 *  COleProp class Implementation
 ***********************************
 */

using namespace xlslib_core;

COleProp::COleProp() :
	m_sName(),
	m_nNameSize(0),
	m_nPropType(0),
	m_nNodeColor(0),

	m_nPrevPropIndex(0),
	m_nNextPropIndex(0),
	m_nChildPropIndex(0),

	m_nTSCreatedSeconds(0),
	m_nTSCreatedDays(0),
	m_nTSModifiedSeconds(0),
	m_nTSModifiedDays(0),

	m_nStartBlock(0),
	m_nSize(0),
	m_nIndex(0),
	m_pData(NULL),
	m_Child_List()
{
}

COleProp::COleProp(signed32_t node_index, const std::string& name, CDataStorage* data) :
	m_sName(name),
	m_nNameSize(0),
	m_nPropType(0),
	m_nNodeColor(0),

	m_nPrevPropIndex(0),
	m_nNextPropIndex(0),
	m_nChildPropIndex(0),

	m_nTSCreatedSeconds(0),
	m_nTSCreatedDays(0),
	m_nTSModifiedSeconds(0),
	m_nTSModifiedDays(0),

	m_nStartBlock(0),
	m_nSize(0),
	m_nIndex(node_index),
	m_pData(data),
	m_Child_List()
{
}

COleProp::COleProp(signed32_t node_index, const char *name, CDataStorage* data) :
	m_sName(name),
	m_nNameSize(0),
	m_nPropType(0),
	m_nNodeColor(0),

	m_nPrevPropIndex(0),
	m_nNextPropIndex(0),
	m_nChildPropIndex(0),

	m_nTSCreatedSeconds(0),
	m_nTSCreatedDays(0),
	m_nTSModifiedSeconds(0),
	m_nTSModifiedDays(0),

	m_nStartBlock(0),
	m_nSize(0),
	m_nIndex(node_index),
	m_pData(data),
	m_Child_List()
{
}

COleProp::~COleProp()
{
	for(Tree_Level_Itor_t chld = m_Child_List.begin(); chld != m_Child_List.end(); chld++) {
		delete *chld;
	}
}

int COleProp::Init(signed32_t node_index, const std::string& name, CDataStorage* data)
{
	int errcode = NO_ERRORS;

	SetName(name);
	m_pData = data;
	m_nIndex = node_index;

	return errcode;
}

int COleProp::SetName(const std::string& name)
{
	int errcode = NO_ERRORS;

	m_nNameSize = static_cast<unsigned16_t>(name.size() + 1);
	m_sName = name;

	return errcode;
}
const std::string& COleProp::GetName(void) const
{
	return m_sName;
}

int COleProp::SetIndex(signed32_t newindex)
{
	int errcode = NO_ERRORS;

	m_nIndex = newindex;

	return errcode;
}

int COleProp::SetType(unsigned8_t newtype)
{
	int errcode = NO_ERRORS;

	m_nPropType = newtype;

	return errcode;
}
unsigned8_t COleProp::GetType(void) const
{
	return m_nPropType;
}

int COleProp::SetColor(unsigned8_t newcolor)
{
	int errcode = NO_ERRORS;

	m_nNodeColor = newcolor;

	return errcode;
}
unsigned8_t COleProp::GetColor(void) const
{
	return m_nNodeColor;
}

int COleProp::SetPreviousIndex(signed32_t prev)
{
	int errcode = NO_ERRORS;

	m_nPrevPropIndex = prev;

	return errcode;
}
signed32_t COleProp::GetPreviousIndex(void) const
{
	return m_nPrevPropIndex;
}

int COleProp::SetNextIndex(signed32_t next)
{
	int errcode = NO_ERRORS;

	m_nNextPropIndex = next;

	return errcode;
}
signed32_t COleProp::GetNextIndex(void) const
{
	return m_nNextPropIndex;
}

int COleProp::SetChildIndex(signed32_t child)
{
	int errcode = NO_ERRORS;

	m_nChildPropIndex = child;

	return errcode;
}
signed32_t COleProp::GetChildIndex(void) const
{
	return m_nChildPropIndex;
}

int COleProp::SetStartBlock(signed32_t sb)
{
	int errcode = NO_ERRORS;

	m_nStartBlock = sb;

	return errcode;
}
signed32_t COleProp::GetStartBlock(void) const
{
	return m_nStartBlock;
}

int COleProp::SetSize(size_t size)
{
	int errcode = NO_ERRORS;

	m_nSize = size;

	return errcode;
}
size_t COleProp::GetSize(void) const
{
	return m_nSize;
}

void COleProp::SetDataPointer(CDataStorage* pdata)
{
	m_pData = pdata;
}
CDataStorage* COleProp::GetDataPointer(void) const
{
	return m_pData;
}

void COleProp::SetCreatedSecs(signed32_t secs1)
{
	m_nTSCreatedSeconds = secs1;
}
signed32_t COleProp::GetCreatedSecs(void) const
{
	return m_nTSCreatedSeconds;
}

void COleProp::SetCreatedDays(signed32_t days1)
{
	m_nTSCreatedDays = days1;
}
signed32_t COleProp::GetCreatedDays(void) const
{
	return m_nTSCreatedDays;
}

void COleProp::SetModifiedSecs(signed32_t secs2)
{
	m_nTSModifiedSeconds = secs2;
}
signed32_t COleProp::GetModifiedSecs(void) const
{
	return m_nTSModifiedSeconds;
}

void COleProp::SetModifiedDays(signed32_t days2)
{
	m_nTSModifiedDays = days2;
}
signed32_t COleProp::GetModifiedDays(void) const
{
	return m_nTSModifiedDays;
}
