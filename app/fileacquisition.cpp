/****************************************************************************	
*	Operator Console - an extensible user interface for the Imatest IT 		*
*	library																	*
*	Copyright (C) 2013 Imatest LLC.											*
*																			*
*	This program is free software: you can redistribute it and/or modify	*
*	it under the terms of the GNU General Public License as published by	*
*	the Free Software Foundation, either version 3 of the License, or		*
*	(at your option) any later version.										*
*																			*
*	This program is distributed in the hope that it will be useful,			*
*	but WITHOUT ANY WARRANTY; without even the implied warranty of			*
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the			*
*	GNU General Public License for more details.							*
*																			*
*	You should have received a copy of the GNU General Public License		*
*	along with this program.  If not, see <http://www.gnu.org/licenses/>. 	*
****************************************************************************/

#include "fileacquisition.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <QTextStream>

using namespace std;


FileAcquisition::FileAcquisition(void)
{
	m_numPixels = 0;
    m_buf       = nullptr;
    m_filename.clear();
}


FileAcquisition::~FileAcquisition(void)
{
}

///
/// Initializes the buffers inherited from class ImageAcquisition with 2 bytes per pixel.
/// @return Returns TRUE if allocation is successful.
///
bool FileAcquisition::Init(int width, int height, const char *filename)
{
	bool	result;

	m_filename = filename;
    result     = reinterpret_cast<ImageAcquisition *>(this)->Init(width, height, 2);

	return result;
}

bool FileAcquisition::Open()
{
	bool result = LoadFile();

	return result;
}

bool FileAcquisition::Close()
{
	return true;
}
///
/// Loads an image file through a filestream and then copies into m_buf.
/// @return Returns TRUE if the file is loaded into m_buf, FALSE otherwise.
///
bool FileAcquisition::LoadFile()
{
	bool				success = false;
	ifstream::pos_type	size;
    ifstream			filestream(m_filename.toStdString().c_str(), ios::binary);
    QTextStream         msg(&m_logMsg);

	if (!filestream.is_open()) 
	{
      msg << __FUNCTION__ << ": Unable to open image file '" << m_filename << "'";
	}
	else
	{
		filestream.seekg(0, ios::end);	// Determine length of file
		size = filestream.tellg();
		filestream.seekg(0, ios::beg);	// rewind

		//
		// Make sure that our buffer is large enough to hold the file
		//
        if (m_numBytes != static_cast<unsigned int>(size))
		{
            msg << __FUNCTION__ << ": '" << m_filename << "' is too big for buffer";
		}
        else if (m_buf == nullptr)
		{
            msg << __FUNCTION__ << "1: m_buf is NULL";
		}
		else
		{
			//
			// Read the contents of the file into m_buf, closing the file when done
			//
            filestream.read(reinterpret_cast<char *>(m_buf), size);
			filestream.close();
            msg << __FUNCTION__ << ":  Read in " << size <<" bytes";
#if 0
			
            m_frame.Set(m_buf);	// this copies m_buf to shared memory that gets accessed by GetFrame() <-- this is handled in threadProc
#endif
			success = true;
		}
	}

	return success;
}
