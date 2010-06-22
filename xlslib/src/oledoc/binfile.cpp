/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * This file is part of xlslib -- A multiplatform, C/C++ library
 * for dynamic generation of Excel(TM) files.
 *
 * xlslib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * xlslib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with xlslib.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * Copyright 2004 Yeico S. A. de C. V.
 * Copyright 2008 David Hoerl
 *  
 * $Source: /cvsroot/xlslib/xlslib/src/oledoc/binfile.cpp,v $
 * $Revision: 1.5 $
 * $Author: dhoerl $
 * $Date: 2009/03/02 04:08:43 $
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 * File description:
 *
 *
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


#include <xlsys.h>

#include <binfile.h>

using namespace std;
using namespace xlslib_core;

/* 
******************************
CBinFile class Implementation
******************************
*/

CBinFile::CBinFile() :
	m_File()
{
}
CBinFile::~CBinFile()
{
   Close();

}

/* 
******************************
******************************
*/
int CBinFile::Open(const string& file_name)
{
   Close();
   m_File.open(file_name.c_str(),ios_base::binary|ios_base::out);

   return m_File.good() ? NO_ERRORS : FILE_ERROR;
}

/* 
******************************
******************************
*/
int CBinFile::Close (  )
{
   if(m_File.is_open())
      m_File.close();

   return NO_ERRORS;
}
/* 
******************************
******************************
*/

unsigned32_t CBinFile::Position (  )
{
	unsigned32_t pt = 0;
   if(m_File.is_open()) {
      pt = (unsigned32_t)m_File.tellp();
	}

   return pt;
}

/* 
******************************
******************************
*/
int CBinFile::Write(unsigned8_t * data, size_t size)
{
   int errcode;

   errcode = write_service((const char*)data, size);

   return errcode;
}


/* 
******************************
******************************
*/
int CBinFile::Write(CUnit& data_unit)
{
   int errcode;

   errcode = write_service((const char*)data_unit.GetBuffer(), data_unit.GetDataSize());

   return errcode;
}

/* 
******************************
******************************
*/
int CBinFile::WriteByte(unsigned8_t byte)
{
   int errcode;

   errcode = write_service((const char*)&byte, 1);

   return errcode;
}

/* 
******************************
******************************
*/
int CBinFile::WriteUnsigned16(unsigned16_t data)
{
   int errcode = NO_ERRORS;

   WriteByte(BYTE_0(data));
   WriteByte(BYTE_1(data));

   return errcode;
}

/* 
******************************
******************************
*/
int CBinFile::WriteUnsigned32(unsigned32_t data)
{
   int errcode = NO_ERRORS;

   WriteByte(BYTE_0(data));
   WriteByte(BYTE_1(data));
   WriteByte(BYTE_2(data));
   WriteByte(BYTE_3(data));

   return errcode;
}

/* 
******************************
******************************
*/
int CBinFile::WriteSigned16(signed16_t data)
{
   int errcode = NO_ERRORS;

   WriteByte(BYTE_0(data));
   WriteByte(BYTE_1(data));

   return errcode;
}

/* 
******************************
******************************
*/
int CBinFile::WriteSigned32(signed32_t data)
{
   int errcode = NO_ERRORS;
   WriteByte(BYTE_0(data));
   WriteByte(BYTE_1(data));
   WriteByte(BYTE_2(data));
   WriteByte(BYTE_3(data));

   return errcode;
}

int CBinFile::WriteByteArray(const unsigned8_t *data, size_t size)
{
   int errcode;
   
   errcode = write_service((const char*)data, size);
   
   return errcode;
}

/* 
******************************
******************************
*/

int CBinFile::SerializeFixedArray(const unsigned8_t data, size_t size)
{
   int errcode = NO_ERRORS;
   
   for (size_t i = 0; i<size; i++)
      WriteByte(data);

   return errcode;
}

/* 
******************************
******************************
*/

int CBinFile::write_service(const char *buffer, size_t size)
{
//   if(!is_stroke)
   {
      if(m_File.is_open())
      {
         if(size > 1)
            m_File.write(buffer, static_cast<streamsize>(size));
         else if(size == 1)
            m_File.put(*buffer);
      }
   }
   return NO_ERRORS;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * $Log: binfile.cpp,v $
 * Revision 1.5  2009/03/02 04:08:43  dhoerl
 * Code is now compliant to gcc  -Weffc++
 *
 * Revision 1.4  2008/12/06 01:42:57  dhoerl
 * John Peterson changes along with lots of tweaks. Many bugs that causes Excel crashes fixed.
 *
 * Revision 1.3  2008/10/27 01:12:20  dhoerl
 * Remove PHP
 *
 * Revision 1.2  2008/10/25 18:39:53  dhoerl
 * 2008
 *
 * Revision 1.1.1.1  2004/08/27 16:31:43  darioglz
 * Initial Import.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

