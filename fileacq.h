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

#ifndef FILEACQ_H
#define FILEACQ_H


#include "imageacquisition.h"
#include <QPixmap>

///
/// A class for acquiring from a file with the data in RGB format
///
class FileAcq :	public ImageAcquisition
{
public:
	FileAcq(void);
    FileAcq(QString &filepathname);
	~FileAcq(void);
	
	bool	CaptureFrame()  {return true;}	//!< image is already in m_frame, so there's nothing to do
	bool	Close() {return true;}
    bool	Init(int width, int height, const char *filename);
    bool	Open();
    bool    Open(const char *filename);

protected:
    QString	m_filename;		//!< fully-qualified image file name
};

#endif
