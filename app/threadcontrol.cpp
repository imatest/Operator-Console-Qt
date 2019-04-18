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

#include "threadcontrol.h"
#include <QMutexLocker>

ThreadControl::ThreadControl(QObject *parent) : QThread(parent)
{
    data = nullptr;
    quit    = true;    // set quit to true until Init is called
    waiting = false;
}


ThreadControl::~ThreadControl(void)
{
    if (!isFinished())
    {
        Quit();
    }
}

bool ThreadControl::Init(ThreadFunc func, void *data)
{
    bool	success = true;

    this->func = func;
    this->data = data;
    quit    = false;
    waiting = false;
    start();
    return success;
}

void ThreadControl::Run() // signal the thread to run its function one time
{
    waitCondition.wakeAll();
}

void ThreadControl::run()
{
    forever
    {
        if (quit)
            break;
        //
        // Wait until we get a signal to run
        //
        {
            QMutexLocker locker(&mutex);
            waiting = true;
            waitCondition.wait(&mutex);
            waiting = false;
        }

        //
        // See if we should quit
        //
        if (quit)
            break;

        //
        // Run the function
        //
        func(data);

        //
        // See if we should quit
        //
        if (quit)
            break;

        //
        // Tell the parent thread that data is ready
        //
        emit data_ready();
    }
}


void ThreadControl::Quit()
{
    if (isRunning())
    {
        quit = true;  // tell the thread to quit

        //
        // If the thread is currently in a wait state, we need to wake it up so it can quit
        //
        if (waiting)
        {
            waitCondition.wakeAll();
        }

        //
        // Wait for thread to finish
        //
        wait();
    }
}
