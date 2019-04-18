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
#include <QMutexLocker>

CriticalString::CriticalString(void)
{
}


CriticalString::~CriticalString(void)
{
}

void CriticalString::Clear()
{
    QMutexLocker locker(&m_mutex);
    m_string.clear();

}


void CriticalString::Get(QString &dest, bool clear)
{
    QMutexLocker locker(&m_mutex);
    dest = m_string;					// copy m_string into dest str

    if (clear)
    {
        m_string.clear();
    }

}

void CriticalString::Set(QString &src)
{
    QMutexLocker locker(&m_mutex);
    m_string = src;						// copy src into m_string
}
void CriticalString::Set(QTextStream &src)
{
    QString str = src.readAll();
    QMutexLocker locker(&m_mutex);
    m_string = str;						// copy src into m_string
}

void CriticalString::Set(std::string &src)
{
    QMutexLocker locker(&m_mutex);
    m_string = QString::fromStdString(src);	// copy src into m_string
}

void CriticalString::Set(const char *src)
{
    if (src != nullptr)
    {
        QMutexLocker locker(&m_mutex);
        m_string = QString::fromLatin1(src);	// copy src into m_string
    }
}

void CriticalString::Set(const char *str, int len)
{
    if (str != nullptr)
    {
        QMutexLocker locker(&m_mutex);
        m_string = QString::fromLatin1(str, len);		// copy len characters of str into m_string
        //        m_string.append('\0');
    }
}

bool CriticalString::Append(QString &str)
{
    bool wasEmpty;

    QMutexLocker locker(&m_mutex);
    wasEmpty = m_string.isEmpty();

    if (!str.isEmpty())
    {
        m_string.append(str);			// concatenate str onto the end of m_string
    }

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

    QMutexLocker locker(&m_mutex);
    wasEmpty = m_string.isEmpty();

    if (str != nullptr)
    {
        m_string = QString::fromLatin1(str, len);
    }

    return wasEmpty;
}

bool CriticalString::Append(const char *str, int len)
{
    bool wasEmpty;

    QMutexLocker locker(&m_mutex);
    wasEmpty = m_string.isEmpty();

    if (str != nullptr)
    {
        m_string = QString::fromLatin1(str, len);
    }

    return wasEmpty;
}
