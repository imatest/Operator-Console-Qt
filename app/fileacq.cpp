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

#include "fileacq.h"
#include <QColor>


FileAcq::FileAcq(void)
{
}

FileAcq::FileAcq(QString &filepathname)
{
    m_filename = filepathname;
}

FileAcq::~FileAcq(void)
{
}

///
/// Initializes the buffers inherited from class ImageAcquisition with 2 bytes per pixel.
/// @return Returns TRUE if allocation is successful.
///
bool FileAcq::Init(int width, int height, const char *filename)
{
    bool	result;

    m_filename = filename;
    result     = reinterpret_cast<ImageAcquisition *>(this)->Init(width, height, 2);

    return result;
}

bool FileAcq::Open(const char *filename)
{
   m_filename   = filename;
   bool success =  Open();

   return success;
}

/// 
/// Loads a file first into the QImage object m_image, and then into m_buf inherited from class ImageAcquisition if allocation is successful.
/// @return Returns TRUE if m_buf is successfully allocated, FALSE otherwise
///
bool FileAcq::Open()
{
	bool	success = false;
	int		width, height, bytesPerPixel;
    QImage  image;

    if (image.load(m_filename))
    {
        bytesPerPixel = image.depth() / 8;
        width         = image.width();
        height        = image.height();
        success       = dynamic_cast<ImageAcquisition *>(this)->Init(width, height, bytesPerPixel); // allocates m_buf and m_frame

		if (success)
		{
//            QRgb color = image.pixel(0, 0);
            //
            // Copy the pixels into m_buf.
			//
            // Each line of pixels in a QImage is aligned on a 32-bit boundary.  Since we're dealing with
            // ARGB images (which are 32 bits per pixel) we can just copy the buffer in one fell swoop.
            //
            uchar   *src = image.bits();
            memcpy(m_buf, src, m_numBytes);
        }
	}
	
	return success;
}
