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

///
/// OperatorConsole.cpp : Defines the class behaviors for the application.
///

#include "config.h"
#include "imatest_library.h"
#include "imatest_acquisition.h"
#include "imatestdefines.h"
#include "inifile.h"
#include "operatorconsole.h"
#include "operatorconsoledialog.h"
#include "setupdialog.h"
#include "passfail.h"
#include "passfaildialog.h"
#include "passfailsettings.h"
#include "passworddialog.h"
#include "imatestsourceids.h"
#include "waitdialog.h"
#include <sstream>
#include <QColor>
#include <QCursor>
#include <QFile>
#include <QFileInfo>
#include <QGuiApplication>
#include <QMessageBox>
#include <QTextStream>
#include <QWaitCondition>

// This REDIRECT_STDIO causes crashes on 'quit', and I'm not seeing what extra stdout data is displayed in the GUI when it is switched on.
//#define REDIRECT_STDIO				// this redirects stdout and stderr to log window using pipes and threads
#define STDIO_DEBUG				// turns on some calls printf and cout (for debugging)
#define STDOUT_BUFSIZE	32768		// this is the desired buffer size for the redirected stdout
#define START_TEST_FROM_FRAME_READY	// test (blemish or sfrplus) will be started in OnFrameReady() rather than OnStart()

//#define NO_LOG					// turns off log messages generated by the OperatorConsole object
#define LOG_FILENAME ".\\log.txt"	// contents of log window will be saved to this file when the program exits


/// OperatorConsole construction

OperatorConsole::OperatorConsole()
{
    m_cameraImage   = nullptr;
    m_camera        = nullptr;
    m_cameraControl = nullptr;
    m_test          = nullptr;
    m_status        = idle;
    m_width         = CAMERA_WIDTH;     // placeholder value until a camera has been selected
    m_height        = CAMERA_HEIGHT;    // placeholder value until a camera has been selected

	memset(&m_flags, 0, sizeof(m_flags));
    m_PFSettings.m_ini_file = INI_FILENAME;
    m_password = ADMIN_PASSWORD;
#ifdef NO_AUTHORIZATION
	m_passFailIsUnlocked = true; // Keep this initialized to false unless you want to completely disable password-protection for pass/fail settings!
#else
	m_passFailIsUnlocked = false; // Keep this initialized to false unless you want to completely disable password-protection for pass/fail settings!
#endif
    m_image_source = no_source;
//    m_image_source = imatest_source;
//    m_image_source = file_source;   // during development, load image from a file instead of a camera
    m_logFileName = LOG_FILENAME;

    SetupSlots();
}

OperatorConsole::~OperatorConsole()
{
    Quit();         // kill all the threads
    CloseLibs();    // close libs (do this after threads are killed, in case a thread is using a lib function)

    if (m_cameraImage != nullptr)
	{
		delete [] m_cameraImage;
	}
}


/// OperatorConsole initialization

bool OperatorConsole::Init1(OperatorConsoleDialog *dialog)
{
    bool    success = true;
    m_dlg = dialog;

    cout << "Hello from Init1!" << endl;
    printf("Hello from printf\n");


//	InitOutput();	// this sets up a pipe for handling stdout results from the DLLs

    if (!InitLibs()) // InitLibs must be called before any calls to Imatest library functions
    {
        cerr << "Unable to initialize the Imatest library." << endl;
    }

    if (Inifile::ReadSettings(m_setup))
    {
        // change the image source if needed
        if (m_setup.sourceID != SOURCE_OpConsoleDirectShow)
        {
            m_image_source=imatest_source;
            SetImatestCamera();
        }
        else
        {
            m_image_source=file_source;
            SetQCamera();
        }
    }
    else     // if we're unable to read the ini file, we must generate image dimensions from user input before continuing
    {
        m_setup.width = 640;
        m_setup.height = 480;
        cout << "INI settings for Operator Console not found." << endl;
        cout << "Please click the 'Setup' button and fill in the" << endl;
        cout << "appropriate fields before proceeding." << endl;
    }

    if (!PassFail::ReadFile(m_PFSettings))  // this currently always returns false (that's how it was in the original MFC version, too)
    {

	}

    //
    // Initialize the Config object.  These values are used in the calls to blemish_shell(), sfrplus_shell(), an arbitrary_charts_shell()
    // Right now they're all hard coded (they're defined in OperatorConsole.h)
    //
    m_config.Init(INI_FILENAME, "", PROGRAMPATH, RGB_EXTENSION, FILE_ROOT, SERIAL_NUMBER, PART_NUMBER, 3);

    //
    // Copy the values into m_setup (this is passed to the setup dialog).
    // TODO: combine m_config and m_setup into a single object
    //
    m_setup.ini_file        = m_config.m_iniFilePathName.c_str();
    m_setup.chart_def_file  = m_config.m_chartDefFilePathName.c_str();
    m_setup.program_path    = m_config.m_programPath.c_str();
    m_setup.serial_number   = m_config.m_serialNumber.c_str();
    m_setup.part_number     = m_config.m_partNumber.c_str();


    if (m_image_source != no_source)
    {
        success = Init2();
    }

    if (!success)
    {
        cout << "Application Initialization Failed" << endl;
    }

    cout << "Init1 returning " << (success ? "true" : "false");
    return success;
}


//////////////////////////////////////////////
///
/// Init2         perform application-specific initialization
///
/// @params        none
///
/// @return value true if successful, false otherwise
///
/// Description
///    Init does the following things:
///    It checks all of the hard coded filenames to be sure that the files exist
///    It initializes the blemish and sfrplus file acquisition objects (loads the raw files into buffers)
///    It initializes the camera object
///    Allocates a buffer to hold a single image frame
///    Creates and starts the blemish and sfrplus threads
///    Initializes the MATLAB and ImatestIT libraries
///    Initializes the blemish and sfrplus modules
///
/// If any of the initialization steps fails, an appropriate message is displayed in an Alert Box.
/////////////////////////////////////////////
bool OperatorConsole::Init2()
{
    QString	errMsg;
	bool	success = false;

    if (m_camera == nullptr)
    {
        return false;
    }

    if (!CheckFiles(errMsg))	// make sure that all of the hard coded files exist
	{
		success = false;
	}
    else if (!InitCamera())
	{
        if (m_camera != nullptr)
        {
            errMsg = m_camera->GetInfo();
        }
	}
	else if (!AllocateImageBuf())	// allocate a buffer large enough to hold an image [must be done after call to m_blemishAcq.Open()]
	{
		errMsg = "Unable to allocate image buffers.";
	}
	else if (!InitBlemishThread())
	{
		errMsg = "Unable to create Blemish thread.";
	}
	else if (!InitSFRplusThread())
	{
		errMsg = "Unable to create SFRplus thread.";
	}
	else if (!InitArbitraryChartThread())
	{
		errMsg = "Unable to create SFRplus thread.";
	}
	else if (!InitCameraThread())
	{
		errMsg = "Unable to create Capture thread.";
	}
	else
	{
        OnSetSFRplus();	// the default test is set to SFRplus
		m_status = idle;
		success = true;
	}

    if (!errMsg.isEmpty())
	{
        QMessageBox::critical(nullptr, "Error", errMsg, QMessageBox::Close);
	}

	return success;
}

bool OperatorConsole::InitCamera()
{
	bool	success = false;


	if (m_image_source==imatest_source)
	{
		if (m_camera->Init(m_setup.width, m_setup.height, 4))
		{
			success = m_camera->Open();
            m_imatest_cam.m_device_ID = m_setup.epiphan_deviceID;
            m_imatest_cam.m_source_ID = m_setup.sourceID;
            m_imatest_cam.m_ini_file  = m_setup.ini_file.toStdString();
		}
        m_camera = &m_imatest_cam;
	}
    else if (m_image_source==qcam_source)
    {
        success = m_qcam.Open(m_setup.qcam_deviceID);   // this calls Init for us
        m_camera = &m_qcam;
    }
	else if (m_image_source==file_source)
	{
        success = m_file_cam.Open(IMAGE_NAME);  // Open calls Init, so we don't need to do that separately
        m_camera = &m_file_cam;
    }

    if (m_camera != nullptr)
    {
        m_width   = m_camera->GetWidth();
        m_height  = m_camera->GetHeight();
        m_setup.width = m_width;
        m_setup.height = m_height;
    }

    QSize size(m_width, m_height);
    m_dlg->set_image_size(size);

    return success;
}

bool OperatorConsole::InitBlemishThread()
{
    m_blemish.Init(m_cameraImage, m_width, m_height, &m_config);
    m_flags.blemishThread = m_blemishControl.Init(m_blemish.ThreadProc, &m_blemish);

	return m_flags.blemishThread;
}


bool OperatorConsole::InitSFRplusThread()
{
    m_sfrPlus.Init(m_cameraImage, m_width, m_height, &m_config);
    m_flags.sfrplusThread = m_sfrPlusControl.Init(m_sfrPlus.ThreadProc, &m_sfrPlus);

	return m_flags.sfrplusThread;
}


bool OperatorConsole::InitArbitraryChartThread()
{
    m_arbitraryChart.Init(m_cameraImage, m_setup.width, m_setup.height, &m_config);
    m_flags.arbitraryChartThread = this->m_arbitraryChartControl.Init(m_arbitraryChart.ThreadProc, &m_arbitraryChart);

	return m_flags.sfrplusThread;
}

bool OperatorConsole::InitCameraThread()
{
    m_flags.ImatestCameraThread = m_ImatestCameraControl.Init(m_imatest_cam.ThreadProc, &m_imatest_cam);
    m_flags.FileCameraThread    = m_FileCameraControl.Init(m_file_cam.ThreadProc, &m_file_cam);
    m_flags.QCameraThread       = m_QCameraControl.Init(m_qcam.ThreadProc, &m_qcam);

    if (m_image_source == imatest_source)
	{
        SetImatestCamera();
	}
    else if (m_image_source == file_source)
	{
        SetQCamera();
	}
    else if (m_image_source == file_source)
    {
        SetFileCamera();
    }
    return (m_flags.ImatestCameraThread && m_flags.QCameraThread);
}



bool OperatorConsole::InitLibs()
{
    QString	str;

    m_flags.matlab = mclInitializeApplication(nullptr, 0);	// try to initialize the MATLAB library

	if (!m_flags.matlab)
	{
		str = "Unable to initialize MATLAB library";
	}
	else
	{
		//
		// Try to initialize Blemish and SFRplus libraries
		//
		m_flags.imatestIT = imatest_libraryInitialize();

		if (!m_flags.imatestIT)
		{
			str = "Unable to initialize imatest library.";
		}

		m_flags.imatestAcq = imatest_acquisitionInitialize();

		if (!m_flags.imatestAcq)
		{
			str = "Unable to initialize imatest acquisition library.";
		}
	}

    if (!str.isEmpty())
	{
        QMessageBox::critical(nullptr, "InitLibs Error", str, QMessageBox::Close);
	}

	return m_flags.matlab && m_flags.imatestIT && m_flags.imatestAcq;
}

void OperatorConsole::CloseLibs()
{
	if (m_flags.imatestIT)
	{
		imatest_libraryTerminate();
		m_flags.imatestIT = false;
	}

	if (m_flags.imatestAcq)
	{
		imatest_acquisitionTerminate();
		m_flags.imatestAcq = false;
	}

	if (m_flags.matlab)
	{
		mclTerminateApplication();	// terminate MATLAB runtime
		m_flags.matlab = false;
	}
}


bool OperatorConsole::InitOutput()
{
	m_flags.stdOut = false;
	m_flags.stdErr = false;

#if defined REDIRECT_STDIO
	m_stdoutThread.InitThread(STDOUT_BUFSIZE, MSG_STDOUT, m_nThreadID);
	m_stderrThread.InitThread(STDOUT_BUFSIZE, MSG_STDERR, m_nThreadID);
	m_flags.stdOut = true;
	m_flags.stdErr = true;
#endif

	return m_flags.stdOut && m_flags.stdErr;
}


bool OperatorConsole::AllocateImageBuf()
{
    bool    success = true;

    if (m_camera != nullptr)
    {
        if (m_cameraImage != nullptr)
        {
            delete [] m_cameraImage;
        }

        m_cameraImage = new byte[m_camera->BytesPerFrame()];
        success = m_cameraImage != nullptr;
    }
    return success;
}


void OperatorConsole::OnSetBlemish()
{
	m_test = &m_blemishControl;
    m_dlg->qsoShow(false);
}

void OperatorConsole::OnSetSFRplus()
{
	m_test = &m_sfrPlusControl;
}

void OperatorConsole::OnSetArbitraryChart()
{
	m_test = &m_arbitraryChartControl;
    m_dlg->qsoShow(false);
}


void OperatorConsole::OnStart()	// the Start button was pressed in the dialog
{
    if (m_status == idle)
	{
		m_status = runningTest;
        OnRunTest();
	}
}

void OperatorConsole::OnStop()
{
    if (m_status == runningTest)
    {
        m_status = stopping;
        m_wait.start();
    }
    else
    {
        m_status = idle;	// any test in progress will finish, but a new one won't be started
    }
}

void OperatorConsole::OnShowJSON()
{
    if (m_jsonDialog.isVisible())
    {
        m_jsonDialog.hide();
    }
    else
    {
        m_jsonDialog.setText(m_results.json);
        m_jsonDialog.setTitle(m_results.name);
        m_jsonDialog.show();
    }
}

void OperatorConsole::Quit()
{
	///
	/// This function gets called when the user wants to Quit the application (either by closing the window or
    /// pressing the Quit button).  Before we quit, we need to wait for the other threads to finish.
	/// 
    /// We tell the threads to finish by sending a Quit message to them.  However, if a test is in
    /// progress, that thread won't process the Quit message until the current test has finished.
    /// This can take a while, so we post a Log message and turn on the Wait cursor until the threads finish.
	///
    QCursor waitcursor(Qt::WaitCursor);

	m_status = quitting;		// this tells us not to run another test

    if (m_flags.QCameraThread)
    {
        m_QCameraControl.Quit(); // wait for the QCamera thread to quit
        m_flags.QCameraThread = false;
    }

    if (m_flags.ImatestCameraThread)
    {
        m_ImatestCameraControl.Quit(); // wait for the Imatest library camera thread to quit
        m_flags.ImatestCameraThread = false;
    }

    if (m_flags.FileCameraThread)
    {
        m_FileCameraControl.Quit(); // wait for the file camera thread to quit
        m_flags.FileCameraThread = false;
    }

    if (m_flags.sfrplusThread)
	{
		m_sfrPlusControl.Quit();	// wait for the sfrplus thread to quit
		m_flags.sfrplusThread = false;
	}

	if (m_flags.blemishThread)
	{
		m_blemishControl.Quit();	// wait for the blemish thread to quit
		m_flags.blemishThread = false;
	}

    if (m_flags.arbitraryChartThread)
    {
        m_arbitraryChartControl.Quit();
        m_flags.arbitraryChartThread = false;
    }

#if defined(REDIRECT_STDIO)
	if (m_flags.stdOut)
	{
		m_stdoutThread.Quit();		// close the stdout pipe and wait for the thread to quit
		m_flags.stdOut = false;
	}
	if (m_flags.stdErr)
	{
		m_stderrThread.Quit();		// close the stderr pipe and wait for the thread to quit
		m_flags.stdErr = false;
	}
#endif
	// TDC 2014/08/07 Calling SaveLog() anywhere in Quit() will NOT copy the contents of 
	// c_log as c_log.GetWindowText returns an empty string instead of the contents of c_log. Additionally,
	// if you use SetSel to set to the entire edit box, the values it returns have nStart > nStop.
    SaveLog(LOG_FILENAME);
	
	
}

void OperatorConsole::OnRunTest()
{
#if !defined NO_LOG
    QString str;
    QTextStream text(&str);

    text << GetTestName() << " started";
    LogMessage(text);
#endif

    m_cameraControl->Run();	// capture 1 frame from the camera (we'll run the test after the frame is captured)
}

void OperatorConsole::OnFrameReady()
{
    //
    // There appears to be a potential problem here when the application is quitting:  if the FrameReady
    // signal is received after m_camera's destructor has been called, this function would be trying to
    // access an object that no longer exists.  Therefore we only try to access the camera buffer if
    // we're atill running a test (or should we check the "quitting" flag instead?)
    //
#if 1
    if (m_status == runningTest)
    {
        m_camera->GetFrame(m_cameraImage);  // copy image data from camnera object's shared buffer
        m_test->Run();                      // tell the thread to run 1 test (it will send our thread a message when it's done)
    }
#else
    //
    // Display the image and start another capture.  This bypasses the test (useful during development)
    //
    m_camera->GetFrame(m_cameraImage);  // copy image data from camnera object's shared buffer
    QImage image(m_cameraImage, m_width, m_height, QImage::Format_RGB32);   // convert camera data to an image
    m_dlg->update_image(image);                                             // display the image in the dialog
    m_cameraControl->Run();
#endif
}

void OperatorConsole::OnBlemishDone()
{
	if (m_status != quitting)
	{
		UpdateResults(&m_blemish);	// right now blemish doesn't have any special results, so we can just call UpdateResults() directly
        TestDone();
	}
}

void OperatorConsole::OnSFRplusDone()
{
	if (m_status != quitting)
	{
		UpdateResultsSFRplus(&m_sfrPlus);
        TestDone();
    }
}


void OperatorConsole::OnAribtraryChartDone()
{
	if (m_status != quitting)
	{
		UpdateResults(&m_arbitraryChart);	// right now arbitrary chart doesn't have any special results, so we can just call UpdateResults() directly
        TestDone();
    }
}

void OperatorConsole::OnUpdateStdout()
{
//	GetStdoutMsg(m_stdoutThread, m_stdoutStr);
}

void OperatorConsole::OnUpdateStderr()
{
//	GetStdoutMsg(m_stderrThread, m_stderrStr);
}


//void OperatorConsole::GetStdoutMsg(StdoutThread &data, QString &str)
//{

//	data.Get(str, true);			// get the message

//    if (!str.isEmpty())
//	{
//        str.replace("\n", "\r\n");		// convert \n to \r\n
//        LogMessage(str);                // copy the message to the log display in the dialog
//	}
//}

void OperatorConsole::UpdateResultsSFRplus(ImageTest *test)
{
    const bool  *qso = 	m_sfrPlus.GetQSO(); // QSO overlay (tic-tac-toe)

	UpdateResults(test);

    m_dlg->update_quadrants(qso);
    m_dlg->qsoShow(true);
}


void OperatorConsole::UpdateResults(ImageTest *test)
{
    QString		str;
    QTextStream text(&str);
    bool		passed = test->Passed();


#if !defined NO_LOG
    text << test->m_name << " took " << test->m_elapsedStr << " sec";
    LogMessage(text);
#endif

#if defined START_TEST_FROM_FRAME_READY
    QImage image(m_cameraImage, m_width, m_height, QImage::Format_RGB32);   // convert camera data to an image
    m_dlg->update_image(image);                                             // display the image in the dialog
#endif

	//
	// Copy strings from the test object into our application
	// object so that we can display them in the dialog.
	//
	GetResults(test);

	//
    // Update the dialog with the test results.  The functions below just call functions in the
    // dialog, so we can probably just call the dialog functions here.
	//
    LogMessage(m_results.log, false);			// update any log messages from the test
    UpdateStatus(passed, m_results.failInfo);	// status (pass/fail)
    UpdateSummary(m_results.summary);			// summary (results)
    UpdateFPS(test->m_elapsedStr);				// frame rate

	//
	// Update the values in the json string, replacing \n with \r\n so
	// that the line breaks will work when displayed in a dialog.
	// 
    m_results.json = m_results.json.replace("\n", "\r\n");
}

void OperatorConsole::GetResults(ImageTest *test)
{
	test->GetSummary(m_results.summary);	// the summary results of the test (these get displayed in the dialog)
	test->GetFailInfo(m_results.failInfo);	// reasons that image failed the test
	test->GetLog(m_results.log);			// log message(s)
	test->GetName(m_results.name);			// the name of the test that was run
	test->GetJSON(m_results.json);
}

void OperatorConsole::LogTime()
{
    QTime	t   = QTime::currentTime();
    QString	str = t.toString(Qt::DefaultLocaleShortDate);
    LogMessage(str);
}

bool OperatorConsole::CheckFiles(QString &msg)
{
    const char	*files[] = {LOGO_NAME, INI_FILENAME};
	int			numFiles = sizeof(files) / sizeof (*files);
	int			i;
    QString     filename;
	bool		success = true;

    msg.clear();

	//
	// Try to open all of the hard-coded files.  
	//
	for (i = 0; i < numFiles; i++)
	{
        filename = files[i];
        if (!QFile::exists(filename))
		{
            msg.append(filename);
            msg.append(" not found");
			success = false;
		}
	}

	return success;
}

void OperatorConsole::SaveLog(void)
{
	SaveLog(m_logFileName);
}


void OperatorConsole::SaveLog(const QString& filePathName)
{
    if (m_dlg != nullptr)
    {
        m_dlg->save_log(filePathName);
    }
}

void OperatorConsole::OnSetup()
{
    int oldWidth = m_setup.width;
    int oldHeight = m_setup.height;
    int oldSourceID = m_setup.sourceID;
    QString oldQCamID = m_setup.qcam_deviceID;


    SetupDialog dialog(m_setup, m_dlg);

    int result = dialog.exec();

    if (result == QDialog::Accepted)
    {
        m_setup = dialog.GetSettings();

        m_imatest_cam.m_device_ID = m_setup.epiphan_deviceID;
        m_imatest_cam.m_source_ID = m_setup.sourceID;
        m_imatest_cam.m_ini_file  = m_setup.ini_file.toStdString();

        // transfer the configuration details to the other classes
        m_config.m_iniFilePathName = m_setup.ini_file.toStdString();
        m_config.m_chartDefFilePathName = m_setup.chart_def_file.toStdString();
        m_config.m_serialNumber = m_setup.serial_number.toStdString();
        m_config.m_partNumber = m_setup.part_number.toStdString();
        m_config.m_programPath = m_setup.program_path.toStdString();

        Inifile::WriteSettings(m_setup, m_config.m_iniFilePathName); // store new settings

        if (oldWidth != m_setup.width || oldHeight != m_setup.height
            || ((oldSourceID != SOURCE_OpConsoleDirectShow) && (m_setup.sourceID == SOURCE_OpConsoleDirectShow))
            || ((oldSourceID == SOURCE_OpConsoleDirectShow) && (m_setup.sourceID != SOURCE_OpConsoleDirectShow))
            || ((oldSourceID == SOURCE_OpConsoleDirectShow) && (oldQCamID != m_setup.qcam_deviceID)))
        {

            if (m_setup.sourceID != SOURCE_OpConsoleDirectShow)
            {
                m_image_source = imatest_source;

                if (oldSourceID == SOURCE_OpConsoleDirectShow)
                {
                    SetImatestCamera();
                }
            }
            else
            {
                m_image_source = qcam_source;

                if (oldSourceID != SOURCE_OpConsoleDirectShow || (oldQCamID != m_setup.qcam_deviceID))
                {
                    SetQCamera();
                }
            }
            // image dimensions have changed, so we must reallocate
            try
            {
                if (!ReInit())
                {
                    cout << "Unable to reinitialize." << endl;
                }
            }
            catch (std::exception & ex)
            {
                cout << "Error occurred during reinitialization." << endl;
                cerr << ex.what() << endl;
            }
        }
    }
}

//
// This function allows for reallocation of the various image buffers when we change the image size
//
bool OperatorConsole::ReInit(void)
{

    QString	errMsg;
	bool	success = false;

	m_sfrPlusControl.Quit();
	m_blemishControl.Quit();
    m_arbitraryChartControl.Quit();

	if (!CheckFiles(errMsg))	// make sure that all of the hard coded files exist
	{
		success = false;
	}
    else if (!InitCamera())
	{
		errMsg = m_camera->GetInfo();
	}
    else if (!AllocateImageBuf())	// allocate a buffer large enough to hold an image [must be done after call to m_blemishAcq.Open()]
	{
		errMsg = "Unable to allocate image buffers.";
	}
	else if (!InitBlemishThread())
	{
		errMsg = "Unable to create Blemish thread.";
	}
	else if (!InitSFRplusThread())
	{
		errMsg = "Unable to create SFRplus thread.";
	}
    else if (!InitArbitraryChartThread())
    {
        errMsg = "Unable to create SFRplus thread.";
    }
    else if (!InitCameraThread())
    {
        errMsg = "Unable to create Capture thread.";
    }
    else
	{
//        OnSetSFRplus();	// the default test is set to SFRplus
		m_status = idle;
		success = true;
//        ((OperatorConsoleDlg *)m_pMainWnd)->ReInitDialog();
	}


    if (!errMsg.isEmpty())
	{
        QMessageBox::critical(nullptr, "Init Error", errMsg);
	}

	return success;

}

bool OperatorConsole::GetPassword()
{
    PasswordDialog  pwdDialog(m_dlg);
    bool            success = false;

    if (pwdDialog.exec() == QDialog::Accepted)
    {
        if (m_password.compare(pwdDialog.getResponse()) ==0)
        {
            m_passFailIsUnlocked = pwdDialog.getUnlockStatus();
            success = true;
        }
        else
        {
            cout << "Incorrect password." << endl;
        }
    }

    return success;
}

void OperatorConsole::OnPassFail()
{
    if (m_passFailIsUnlocked || GetPassword())
    {
        PassFailDialog passfail(m_PFSettings, nullptr);

        if (passfail.exec() == QDialog::Accepted)
        {
            if (!m_PFSettings.b_isReadOnly)
            {
                m_PFSettings = passfail.GetSettings();
                PassFail::Write(m_PFSettings);
            }
            else
            {
                cout << "Pass/Fail file is read-only. No changes were saved." << endl;
            }
        }
    }
}

void OperatorConsole::SetImatestCamera()
{
	m_camera = &m_imatest_cam;
	m_cameraControl = &m_ImatestCameraControl;
}


void OperatorConsole::SetFileCamera()
{
    m_camera = &m_file_cam;
    m_cameraControl = &m_FileCameraControl;

}

void OperatorConsole::SetQCamera()
{
    m_qcam.Close();
    m_qcam.Open(m_setup.qcam_deviceID);
    m_camera        = &m_qcam;
    m_cameraControl = &m_QCameraControl;
}

bool OperatorConsole::ReadyForTesting()
{
	bool	ready = true;

	//
	// If we're doing an Arbitrary Chart test, make sure that a chart definition file has been selected
	//
	if (m_test == &m_arbitraryChartControl)
	{
        if (!m_arbitraryChart.HaveChartDef())
		{
            QMessageBox::information(m_dlg, "Error", "You must select a Chart Definition file from the Setup Dialog before running the test.");
			ready = false;
		}
	}

	return ready;
}

void OperatorConsole::LogMessage(QString &str, bool timestamp)
{
    m_dlg->log_message(str, timestamp);
}

void OperatorConsole::LogMessage(QTextStream &text, bool timestamp)
{
    QString str = text.readAll();

    LogMessage(str, timestamp);
}

void OperatorConsole::UpdateSummary(QString &results)
{
    m_dlg->update_summary(results);
}


void OperatorConsole::UpdateStatus(bool passed, QString &failInfo)
{
    m_dlg->update_status(passed, failInfo);
}

void OperatorConsole::UpdateFPS(QString &fps)
{
    m_dlg->update_fps(fps);
}

void OperatorConsole::SetupSlots()
{
    //
    // These slots allow other threads to send a signal to our thread when data is ready
    //
    connect(&m_blemishControl,        &ThreadControl::data_ready, this, &OperatorConsole::OnBlemishDone);
    connect(&m_sfrPlusControl,        &ThreadControl::data_ready, this, &OperatorConsole::OnSFRplusDone);
    connect(&m_arbitraryChartControl, &ThreadControl::data_ready, this, &OperatorConsole::OnAribtraryChartDone);
    connect(&m_ImatestCameraControl,  &ThreadControl::data_ready, this, &OperatorConsole::OnFrameReady);
    connect(&m_QCameraControl,        &ThreadControl::data_ready, this, &OperatorConsole::OnFrameReady);
    connect(&m_FileCameraControl,     &ThreadControl::data_ready, this, &OperatorConsole::OnFrameReady);
}

void OperatorConsole::TestDone()
{
    if (m_status == runningTest)
    {
        OnRunTest();
    }
    else if (m_status == stopping)
    {
        m_wait.stop();
        m_status = idle;
    }
}
