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

#if !defined(THREADCONTROL_H)
#define THREADCONTROL_H

////////////////////////////////////////////////////////////////////////
///
///@class ThreadControl 
/// Thread synchronization and communication class.
///
/// This class simplifies the task of having 2 threads communicate with each other.
///
///
/// A pointer to a ThreadControl object should be passed as the thread function's single parameter
///
////////////////////////////////////////////////////////////////////////

#include <QMutex>
#include <QThread>
#include <QWaitCondition>
#include "imatestdefines.h"

typedef void (IMA_CALL_CONV *ThreadFunc)(void *);


class ThreadControl : public QThread
{
    Q_OBJECT

public:
    ThreadControl(QObject *parent = nullptr);
    virtual ~ThreadControl(void) override;
    bool	Init(ThreadFunc func, void *data);
    void    Run();
	void	Quit();

signals:
    virtual void data_ready();

protected:
    void    run() override;
    bool	InitThread(void *threadProc);

public:
    void            *data;		//!< pointer to whatever is useful

protected:
    QString *name;   // for debugging
private:
    QWaitCondition  waitCondition;  // the thread will block until woken up
    QMutex          mutex;          // the mutex to use with waitCondition
    bool            waiting;        // indicates thread is waiting for waitCondition
    bool            quit;
    ThreadFunc      func;           // the function to call (usually ImageAcquisition or ImageTest)
};

#endif
