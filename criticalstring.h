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

#ifndef CRITICALSTRING_H
#define CRITICALSTRING_H

#include <string>
#include <QMutex>
#include <QString>
#include <QTextStream>

///
/// A null-terminated string class for which all access is controlled by critical sections
///
class CriticalString
{
public:
    CriticalString(void);
    virtual ~CriticalString(void);
    bool	Append(QString &str);
    bool    Append(QTextStream &text);
    bool	Append(const char *str);
    bool	Append(const char *str, int len);
    void	Clear();
    void	Get(QString &str, bool clear=false);	//!< copy m_string into str, then optionally clear m_string
    void	Set(QString &str);
    void	Set(QTextStream &text);
    void	Set(std::string &str);
	void	Set(const char *str);
	void	Set(const char *str, int len);

private:
    QString	m_string;
    QMutex  m_mutex;
};

#endif
