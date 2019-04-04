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

#ifndef IMAGE_TEST_H_
#define IMAGE_TEST_H_

#include <QLatin1String>
#include <QString>
#include <QTextStream>
#include <string>
#include <string.h>
#include "config.h"
#include "criticalstring.h"
#include "imatestmainlibrary.h"
#include "imatestdefines.h"
#include "timer.h"
#include "timestamp.h"

#include "libjson.h"
#include "jsonhelpers.h"
#include "libjson.h"
#include "jsonhelpers.h"
#include "JSON/JSONDefs.h"

using namespace std;

#define INI_RAW_JSON_MODE	"-17"

#define CSTR(qstr)  (qstr.toStdString().c_str())


///
/// This struct is used for figuring out which test(s) failed in class ImageTest.
///
typedef struct TestID
{
	const char	*key;	//!< JSON key value for the test
	const char	*name;	//!< our own name for the test (displayed to user)
} TestID;

//
// This typedef defines ImatestShellFunc, which is a pointer to an Imatest Library shell function (sfrplus_shell, blemish_shell, etc.)
//
#ifdef _WIN32
typedef LIB_imatest_library_CPP_API void (MW_CALL_CONV *ImatestShellFunc)(int nargout, mwArray& nret, const mwArray& inputFile, const mwArray& rootDir, const mwArray& inputKeys, const mwArray& opMode, const mwArray& varargin);
#else
typedef LIB_libImatest_CPP_API void (MW_CALL_CONV *ImatestShellFunc)(int nargout, mwArray& nret, const mwArray& inputFile, const mwArray& rootDir, const mwArray& inputKeys, const mwArray& opMode, const mwArray& varargin);
#endif


///////////////////////////////////////////////////////////////////////
/// An abstract class that defines an image test to be performed.
///
/// ImageTest is an abstract class that defines an image test to be performed (e.g. Blemish, SFRrplus,
/// Arbitrary Chart).  To use this class, create a subclass and implement the ParseResults() method.
///
/// Some member variables are used by more than 1 thread.  To prevent these variables from being accessed
/// simultaneously from multiple threads, they are protected by critical sections, which limit access to one
/// thread at a time.
/// 
/// In theory there shouldn't be a case where both threads are trying to access the variable simultaneously,
/// but theory is often different than the real world.  The sequence of events in the threads is as follows:
///
/// The main thread signals the ImageTest thread to run its test once.
///
/// The test runs once
/// It sets the values of the shared member variables
/// It sends a "done" message to the main thread 
/// It waits for a signal to run again
/// Repeat 
///
/// The main thread receives the "done" message
/// It makes copies of the shared member variables
/// It signals the ImageTest thread to run once
/// Repeat
///
/// In this scenario, the main thread only accesses the shared data when it has received the "done" 
/// message (i.e. after the ImageTest thread is finished using the shared data), and the ImageTest
/// thread only accesses the shared data while the main thread is doing other things.
///////////////////////////////////////////////////////////////////////
class ImageTest
{
public:
	ImageTest(void);
	virtual ~ImageTest(void);

	virtual void	ParseResults(string &results) = 0;	//!< This must be implemented by subclass
	virtual void	Run();
	virtual void	Run(void *raw_pixels, int width, int height, const Config *config);

    void			GetLog(QString &dst) {m_log.Get(dst);}
    void			GetFailInfo(QString &dst) {m_failInfo.Get(dst);}
    void			GetJSON(QString &dst) {m_jsonResults.Get(dst);}
    QString         &GetName() {return m_name;}
    void            GetName(QString &name) {name = m_name;}
    void			GetString(JSONNode &node, const char *name, json_string &string, bool logErrors=true);
    void			GetSummary(QString &dst) {m_summary.Get(dst);}
	void			Init(void *raw_pixels, int width, int height, const Config *config);
	bool			Passed() {return m_passed;}
	void			SetBuffer(void *buf) {m_rawPixels = buf;}
    void			SetJSON(string &src) {m_jsonResults.Set(src);}

    static void IMA_CALL_CONV ThreadProc(void *param);	//!< param must be a pointer to an ImageTest object

    void			AppendLog(QString &log) {m_log.Append(log);}
    void			AppendLog(QTextStream &log) {m_log.Append(log);}
    void			AppendLog(QTextStream &log, QString &before, QString &after) {m_log.Append(log.readAll().replace(before, after));}
    void			AppendLog(QTextStream &log, QLatin1String &before, QLatin1String &after) {m_log.Append(log.readAll().replace(before, after));}
    bool			GetDataNode(string &results, JSONNode &data, bool logErrors=true);
    bool			GetPassFailNode(JSONNode &data, JSONNode &passFail, bool logErrors=true);
    virtual void	InitResults();
    void			ParseFailures(const JSONNode *data);
    void			PlanesFromRGB();
    void			RGBFromPlanes(uint32_t *rgb);

protected:
	bool			AllocateRGB();
    void			ClearLog() {m_log.Clear();}
	void			ClearJSON() {m_jsonResults.Clear();}
	void			ClearFailInfo() {m_failInfo.Clear();}
	void			ClearSummary() {m_summary.Clear();}
	void			DeleteRGB();
    void			SetSummary(QString &src) {m_summary.Set(src);}

public:
    int				m_width;
    int				m_height;
    QString         m_name;     				//<! the test name (e.g. Blemish or SFRplus)
    long long		m_elapsed;					//<! number of milliseconds it took to run the test
    QString			m_elapsedStr;
    Timestamp		m_timestamp;

protected:
    const Config        *m_config;
	const void			*m_rawPixels;				//<! pointer to buffer containing raw pixels on which to perform the test
	unsigned char		*m_rgb;
	bool				m_passed;					//<! did image pass the test
	const TestID		*m_tests;					//<! pointer to array of TestID values (pointer to static array)
	int					m_numTests;					//<! number of elements in m_tests array
	Timer				m_time;
	ImatestShellFunc	m_imatestFunc;				//<! pointer to imatest library function to call

	///////////////////////////////
	//
	// Some member variables are used by more than 1 thread.  To prevent these variables from being accessed
	// simultaneously from multiple threads, they are protected by critical sections, which limit access to one
	// thread at a time.
	// 
	// In theory there shouldn't be a case where both threads are trying to access the variable simultaneously,
	// but theory is often different than the real world.  The sequence of events in the threads is as follows:
	//
	// The main thread signals the ImageTest thread to run its test once.
	//
	// The test runs once
	// It sets the values of the shared member variables
	// It sends a "done" message to the main thread 
	// It waits for a signal to run again
	// Repeat 
	//
	// The main thread receives the "done" message
	// It makes copies of the shared member variables
	// It signals the ImageTest thread to run once
	// Repeat
	//
	// In this scenario, the main thread only accesses the shared data when it has received the "done" 
	// message (i.e. after the ImageTest thread is finished using the shared data), and the ImageTest
	// thread only accesses the shared data while the main thread is doing other things.
	//
	/////////////////////////////////
    CriticalString		m_jsonResults;	//!< the full results of the test in JSON format
    CriticalString		m_summary;		//!< the summary results of the test (these get displayed in the dialog)
    CriticalString		m_failInfo;		//!< reasons that image failed the test
    CriticalString		m_log;			//!< log message(s)
};

#endif
