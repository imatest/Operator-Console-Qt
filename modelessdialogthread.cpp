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

#include "modelessdialogthread.h"
#include "textdialog.h"


// ModelessDialogThread



// ModelessDialogThread construction

ModelessDialogThread::ModelessDialogThread()
{
    m_dialog = nullptr;
}

ModelessDialogThread::~ModelessDialogThread()
{
    if (m_dialog != nullptr)
    {
        delete m_dialog;
    }
}


// ModelessDialogThread initialization

bool ModelessDialogThread::InitInstance()
{

    m_dialog  = new TextDialog;

    return true;
}


void ModelessDialogThread::Show(bool visible)
{
    if (m_dialog != nullptr)
	{
        if (visible)
        {
            m_dialog->show();
        }
        else
        {
            m_dialog->hide();
        }
	}
}

void ModelessDialogThread::SetTitle(QString &title)
{
    if (m_dialog != nullptr)
	{
        m_dialog->setTitle(title);
	}
}

void ModelessDialogThread::SetText(QString &text)
{
    if (m_dialog != nullptr)
	{
        m_dialog->setText(text);
	}
}

