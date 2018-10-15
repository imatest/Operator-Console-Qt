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

#include "config.h"


Config::Config(void)
{
//    m_iniFilePathName      = nullptr;
//    m_chartDefFilePathName = nullptr;
//    m_programPath          = nullptr;
//    m_extension            = nullptr;
//    m_fileRoot             = nullptr;
//    m_serialNumber         = nullptr;
//    m_partNumber           = nullptr;
	m_ncolors              = 0;
}

void Config::Init(const char* iniFileName, const char* chartDefFileName, const char* programPath, const char* ext, const char* fileRoot, const char* serialNum, const char* partNum, int ncolors)
{
	m_iniFilePathName      = iniFileName;
	m_chartDefFilePathName = chartDefFileName;
	m_programPath          = programPath;
	m_extension            = ext;
	m_fileRoot             = fileRoot;
	m_serialNumber         = serialNum;
	m_partNumber           = partNum;
	m_ncolors              = ncolors;
}


Config::~Config(void)
{
}

bool Config::Load(void)
{
	return true;
}

bool Config::Save(void)
{
	return true;
}
