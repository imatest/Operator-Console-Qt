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

#ifndef TIMER_H
#define TIMER_H

#include <sys/types.h>
#include <sys/timeb.h>
#include <QString>

///
/// a timer class for getting the elapsed time between events
///
class Timer
{
public:
	Timer(void);
	virtual ~Timer(void);

	void	Start();
	void	Stop();
    void	Get(long long* ms);
    void	Get(double* sec);
    void	Get(QString* str);

protected:
	void Elapsed();


protected:
#if defined(Q_OS_LINUX)
    struct timeb	m_start;
    struct timeb	m_stop;
#else
    struct _timeb	m_start;
    struct _timeb	m_stop;
#endif
    long long		m_elapsed;		//!< elapsed time in ms
};

#endif
