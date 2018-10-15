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


#include "criticalstring.h"
#include <QChar>

CriticalString::CriticalString(void)
{
}


CriticalString::~CriticalString(void)
{
}

void CriticalString::Clear()
{
    m_mutex.lock();
    m_string.clear();
    m_mutex.unlock();
}


void CriticalString::Get(QString &dest, bool clear)
{
    m_mutex.lock();
	dest = m_string;					// copy m_string into dest str

	if (clear)
	{
        m_string.clear();
	}
    m_mutex.unlock();
}

void CriticalString::Set(QString &src)
{
    m_mutex.lock();
    m_string = src;						// copy src into m_string
    m_mutex.unlock();
}
void CriticalString::Set(QTextStream &src)
{
    QString str = src.readAll();
    m_mutex.lock();
    m_string = str;						// copy src into m_string
    m_mutex.unlock();
}

void CriticalString::Set(std::string &src)
{
    m_mutex.lock();
    m_string = QString::fromStdString(src);	// copy src into m_string
    m_mutex.unlock();
}

void CriticalString::Set(const char *src)
{
    if (src != nullptr)
	{
        m_mutex.lock();
        m_string = QString::fromLatin1(src);	// copy src into m_string
        m_mutex.unlock();
	}
}

void CriticalString::Set(const char *str, int len)
{
    if (str != nullptr)
	{
        m_mutex.lock();
        m_string = QString::fromLatin1(str, len);		// copy len characters of str into m_string
//        m_string.append('\0');
        m_mutex.unlock();
	}
}

bool CriticalString::Append(QString &str)
{
	bool wasEmpty;

    m_mutex.lock();
    wasEmpty = m_string.isEmpty();

    if (!str.isEmpty())
	{
        m_string.append(str);			// concatenate str onto the end of m_string
	}

    m_mutex.unlock();

	return wasEmpty;
}

bool CriticalString::Append(QTextStream &text)
{
    bool result;

    QString str = text.readAll();
    result = Append(str);

    return result;
}

bool CriticalString::Append(const char *str)
{
    bool wasEmpty;
    int len = static_cast<int>(strlen(str));

    m_mutex.lock();
    wasEmpty = m_string.isEmpty();

    if (str != nullptr)
    {
        m_string = QString::fromLatin1(str, len);
    }

    m_mutex.unlock();

    return wasEmpty;
}

bool CriticalString::Append(const char *str, int len)
{
    bool wasEmpty;

    m_mutex.lock();
    wasEmpty = m_string.isEmpty();

    if (str != nullptr)
    {
        m_string = QString::fromLatin1(str, len);
    }

    m_mutex.unlock();

    return wasEmpty;
}
