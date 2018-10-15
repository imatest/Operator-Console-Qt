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

#ifndef CONFIG_H
#define CONFIG_H

#include<string>

///
/// A class that contains a few configuration details.
///

class Config
{
public:
	Config(void);
	virtual ~Config(void);
    void Init(const char* iniFileName, const char* chartDefFileName, const char* programPath, const char* ext,
        const char* fileRoot, const char* serialNum, const char* partNum, int nColors);
	
protected:
	bool	Load();
	bool	Save();

public:
    std::string m_programPath;			//!< Path to Imatest
    std::string m_iniFilePathName;		//!< Fully-qualified ini file name (including full path)
    std::string m_chartDefFilePathName;	//!< Fully-qualified chart definition file name (including full path)
    std::string m_extension;			//!< RAW file extension
    std::string m_fileRoot;				//!< File root
    std::string m_serialNumber;			//!< Camera serial number
    std::string m_partNumber;			//!< Camera part number
    int     	m_ncolors;				// 1 = raw data 3 = rgb data
};

#endif

