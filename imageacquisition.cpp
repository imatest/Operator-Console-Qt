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

#include "imageacquisition.h"
#include "threadcontrol.h"


ImageAcquisition::ImageAcquisition(void)
{
	m_width     = 0;
	m_height    = 0;
	m_numBytes  = 0;
	m_numPixels = 0;
    m_buf       = nullptr;
}


ImageAcquisition::~ImageAcquisition(void)
{
    if (m_buf != nullptr)
	{
        delete [] m_buf;
	}
}

bool ImageAcquisition::Init(int width, int height, int bytesPerPixel)
{
	bool	success = false;

	m_width     = width;
	m_height    = height;
    m_numPixels = static_cast<unsigned int>(m_width * m_height);
    m_numBytes  = m_numPixels * static_cast<unsigned int>(bytesPerPixel);

    if ( m_buf != nullptr) // allow for resizing of buffer by re-initialization
	{
		delete[] m_buf;
	}

    m_buf = new unsigned char[m_numBytes];

    if (m_buf == nullptr)
	{
        m_logMsg = QString("%1: Unable to allocate image buffer (%2 bytes)").arg(__FUNCTION__, static_cast<int>(m_numPixels));
	}
	else
	{
		success = m_frame.Init(m_numBytes);
	}

	return success;
}


void __cdecl ImageAcquisition::ThreadProc(void *param)
{
    ImageAcquisition	*acq = static_cast<ImageAcquisition *>(param);

    acq->CaptureFrame();
    acq->m_frame.Set(acq->m_buf);

}
