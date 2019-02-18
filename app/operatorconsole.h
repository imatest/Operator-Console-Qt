#ifndef OPERATORCONSOLE_H
#define OPERATORCONSOLE_H

#include <QFile>
#include <QString>
#include <QTextStream>
#include <QWaitCondition>
#include "arbitrarycharttest.h"
#include "blemishtest.h"
#include "cameraacquisition.h"
#include "config.h"
#include "fileacq.h"
#include "imageacquisition.h"
#include "imagetest.h"
#include "imatestdefines.h"
#include "imatestlibacq.h"
#include "passfailsettings.h"
#include "setup_settings.h"
#include "sfrplustest.h"
//#include "stderrredirect.h"
//#include "stderrthread.h"
//#include "stdoutredirect.h"
//#include "stdoutthread.h"
#include "textdialog.h"
#include "threadcontrol.h"
#include "waitdialog.h"

//
// Hard coded values go here.  Most or all will eventually come from a Config or Setup dialog.
//
// IMAGE_NAME is the name of the static file to be displayed in the application window (until live acquisition is implemented)
//
#define LOGO_NAME			"Data\\imatest_logo.png"	// imatest logo to be displayed in dialog
#define IMAGE_NAME			"Data\\sfrplus.raw.jpg"			// image file to be used for acquisition from a file (mainly used during development)
// #define ADMIN_PASSWORD		"Imatest%840cX"						// password for Administrator access to the pass/fail settings
#define ADMIN_PASSWORD		"a"						// password for Administrator access to the pass/fail settings


//
// Size of an image from the camera (in pixels).  Ultimately the RAW_ definitions above will go
// away and we'll only be using the camera for images, but right now we're not there yet.
//
// TODO: Define these to the proper size for your camera
//
//#define RES_XGA
//#define RES_VGA
#define RES_WVGA

#if defined RES_VGA
#define CAMERA_WIDTH	640
#define CAMERA_HEIGHT	480
#elif defined RES_WVGA
#define CAMERA_WIDTH	800
#define CAMERA_HEIGHT	600
#elif defined RES_SVGA
#define CAMERA_WIDTH	800
#define CAMERA_HEIGHT	480
#elif defined RES_XGA
#define CAMERA_WIDTH	1024
#define CAMERA_HEIGHT	768
#endif

#define RGB_EXTENSION		"rgb2"
#define FILE_ROOT			"NoFile"
#define SERIAL_NUMBER		"123"
#define PART_NUMBER			"720PCam"


//
// These are messages that get sent to our application object in the MFC version of the Operator Console.
// They're just here as a reference during development.
//
#define MSG_STDOUT				(WM_APP + 0x20)	// gets sent from StdoutThread when console output is available
#define MSG_STDERR				(WM_APP + 0x21)	// gets sent from StderrThread when console output is available

typedef enum AppStatus
{
    runningTest,	// an image test is running
    idle,			// no test is running
    quitting,		// the user pressed Quit or closed the dialog window, but we need to wait for threads to finish
    stopping        // the user pressed Stop, but we need to wait for the current test to finish
} AppStatus;

///
/// A struct for holding all results (including JSON) of a test
///
typedef struct TestResults
{
    QString	name;		//!< the name of the test that was run
    QString	summary;	//!< the summary results of the test
    QString	json;		//!< holds the JSON output of a test
    QString	failInfo;	//!< reasons that image failed the test
    QString	log;		//!< log message(s)
} TestResults;

///
/// A struct for holding the various flags used throughout the application
///
typedef struct AppFlags
{
    unsigned int	blemishThread:1;
    unsigned int	sfrplusThread : 1;
    unsigned int	arbitraryChartThread : 1;
    unsigned int	matlab:1;			//!< MATLAB lib has been initialized
    unsigned int	imatestIT:1;		//!< imatestIT lib has been initialized
    unsigned int    imatestAcq:1;		//!< Imatest IT acquisition lib has been initialized
    unsigned int	stdOut:1;			//!< stdout pipe is open
    unsigned int	stdErr:1;			//!< stderr pipe is open
    unsigned int	ImatestCameraThread:1;
    unsigned int	QCameraThread:1;
    unsigned int    FileCameraThread:1;
} AppFlags;

///
/// An enum type for indicating the image source
///
typedef enum image_source_t
{
    no_source = -1,     //!< no image acquisition source has been selected yet
    imatest_source, 	//!< image acquisition using the Imatest library
    qcam_source,        //!< imaging from a QCamera
    file_source         //!< loading a file for the image source
} image_source_t;

class OperatorConsoleDialog;

class OperatorConsole : public QObject
{
    Q_OBJECT

public:
    OperatorConsole();
    virtual ~OperatorConsole();

    bool                Init1(OperatorConsoleDialog *dialog);
    void                Quit();
    bool				ReadyForTesting();						//!< This function gets called before a test is run to amke sure everything is ready

public slots:
    //
    // The slots are analagous to the message handlers in the MFC version of the Operator Console.
    // The operator console object will get signals from other threads as well as from the GUI.
    //

    //
    // These slots receive signals from other threads (the camera thread, and the ImageTest threads)
    //
    void				OnAribtraryChartDone(); //!< Called when data_ready signal is received from m_arbitraryChartControl
    void				OnBlemishDone();        //!< Called when data_ready signal is received from m_blemishControl
    void				OnFrameReady();         //!< Called when data-ready signal is received from m_cameraControl
    void				OnSFRplusDone();        //!< Called when data_ready signal is received from m_sfrPlusControl

    void				OnUpdateStderr();		//!< Called after MSG_STDERR is received
    void				OnUpdateStdout();		//!< Called after MSG_STDOUT is received

    //
    // These slots receive signals sent from the Operator Console Dialog (generally after a button has been clicked)
    //
    void				OnPassFail();           //!< Called after MSG_PASS_FAIL is received
    void				OnRunTest();            //!< Called when "Start" button is clicked in Operator Console Dialog
    void				OnSetBlemish();         //!< Called when "Blemish" radio button is clicked in Operator Console Dialog
    void				OnSetSFRplus();         //!< Called when "SFRplus" radio button is clicked in Operator Console Dialog
    void				OnSetArbitraryChart();	//!< Called when  "Arbitrary Charts" radio button is clicked in Operator Console Dialog
    void				OnShowJSON();       	//!< Called after MSG_JSON is received
    void				OnStop();               //!< Called after MSG_STOP is received
    void				OnStart();              //!< Called after MSG_START is received
    void				OnSetup();              //!< Called after MSG_SETUP is received

protected:
    bool				AllocateImageBuf();
    bool				CheckFiles(QString &msg);
    void				CloseLibs();								//!< Function that closes the Imatest library. Note that once closed, the library cannot be reinitialized.
    bool                GetPassword();
    void				GetResults(ImageTest *test);
//    void				GetStdoutMsg(StdoutThread &data, QString &str);
    const char  		*GetTestName() { return (m_test == nullptr) ? "" : (reinterpret_cast<ImageTest *>(m_test->data)->GetName().toStdString().c_str());}
    bool				Init2();
    bool				InitArbitraryChartAcq();
    bool				InitArbitraryChartThread();
    bool				InitBlemishThread();
    bool				InitCamera();
    bool				InitCameraThread();
    bool				InitLibs();									//!< Function that initializes the Imatest library. Must be called before any Imatest library functions are used.
    bool				InitOutput();
    bool				InitSFRplusThread();
    bool				LoadConfig();
    void				MakeHandles();

    bool				ReInit(void);							//!< This function allows for reallocation of the various image buffers when we change the image size

    void				SetImatestCamera();
    void                SetQCamera();
    void				SetFileCamera();

    bool				SaveConfig();
    void				SaveLog(const QString& filePathName);

    void				LogTime();
    void                TestDone();
    void				UpdateResults(ImageTest *test);
    void				UpdateResultsSFRplus(ImageTest *test);
    void				UpdateStderr();
    void				UpdateStdout();

    void                WaitForDone();           //!< displays info box and wait cursor until current text finishes
    void				SaveLog(void);			//!< A public function to save the contents of the log CEdit to m_logFileName.

    void                SetupSlots();

    void                UpdateStatus(bool passed, QString &failInfo);
    void                UpdateFPS(QString &fps);
    void                LogMessage(QTextStream &text, bool timestamp=true);
    void                LogMessage(QString &str, bool timestamp = true);
    void                LogMessage(const char *str, bool timestamp = true);
    void                UpdateSummary(QString &results);

protected:
    AppStatus				m_status;
    Config					m_config;   		//!< for using rgb data:   will eventually come from a dialog (uses hard coded values for now)
    uint8_t					*m_cameraImage;		//!< buffer to hold a single image frame from the camera (will eventually be combined with m_fileImage)
    ImatestLibAcq			m_imatest_cam;		//!< live acquisition using Imatest acquire_image()
    CameraAcquisition       m_qcam;             //!< live acquisition using a camera controlled by QCamera interface
    FileAcq					m_file_cam;			//!< acquisition comes from a file, with data in ARGB format (0xffrrggbb)
    ImageAcquisition		*m_camera;

    int						m_width;
    int						m_height;

    setup_settings			m_setup;			//!< this contains settings from/for the setup dialog

    BlemishTest				m_blemish;			//!< this will run the blemish tests
    SFRplusTest				m_sfrPlus;			//!< this will run the SFRplus tests
    ArbitraryChartTest		m_arbitraryChart;		//!< this will run the arbitrary chart tests

    ThreadControl			m_blemishControl;	//!< this is the thread control for running Blemish tests
    ThreadControl			m_sfrPlusControl;	//!< this is the thread control for running SFRplus tests
    ThreadControl			m_arbitraryChartControl;	//!< this is the thread control for running Arbitrary Chart tests

    ThreadControl			m_ImatestCameraControl;
    ThreadControl			m_QCameraControl;
    ThreadControl			m_FileCameraControl;

    ThreadControl			*m_cameraControl;	//!< points to the current image source's control thread
    ThreadControl			*m_test;			//!< the current test being run (&m_arbitraryChartControl, &m_blemishControl, or &sfrPlusControl)

    TextDialog              m_jsonDialog;

    TestResults				m_results;
//    StdoutThread			m_stdoutThread;
//    StderrThread			m_stderrThread;
    QString					m_stdoutStr;		//!< our copy of the contents of m_stdout
    QString					m_stderrStr;		//!< our copy of the contents of m_stderr
    AppFlags				m_flags;
    OperatorConsoleDialog   *m_dlg;
    WaitDialog              m_wait;

public:
    PassFailSettings m_PFSettings;				//!< Contains the pass/fail criteria given by the pass/fail file listed in imatest.ini
private:
    QString m_password;							//!< This contains the administrator password
    bool m_passFailIsUnlocked;					//!< Indicates whether the administrator has chosen to unlock access to pass/fail settings for the duration that the executable runs
    image_source_t m_image_source;
    QString m_logFileName;						//!< Contains the file name for the log file (log file is filled with the contents of the log edit box upon app closure).
};

#endif // OperatorConsole_H
