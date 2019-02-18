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

#include <sstream>
#include <string>
//#include <string.h>
#include "imagetest.h"
#include "operatorconsole.h"


//
// This defines the delimiter to use when formatting the string of failed tests
//
#define FAIL_DELIMITER	"\r\n"

ImageTest::ImageTest(void)
{
	m_width           = 0;
	m_height          = 0;
#if 0
    m_extension       = nullptr;
    m_fileroot        = nullptr;
    m_serialNumber    = nullptr;
    m_partNumber      = nullptr;
    m_iniFilePathName = nullptr;
    m_programPath     = nullptr;
    m_name            = nullptr;
#endif
    m_imatestFunc     = nullptr;
    m_rawPixels		  = nullptr;
    m_rgb             = nullptr;
    m_config          = nullptr;
    m_tests           = nullptr;
    m_elapsed         = 0;
}



ImageTest::~ImageTest(void)
{
	DeleteRGB();
}

#if 0
static const char *tocstr(QString &qstr)
{
    const char *cstr = qstr.toStdString().c_str();

    return cstr;
}
#endif

void ImageTest::Init(void *raw_pixels, int width, int height, const Config *config)
{
    m_rawPixels = raw_pixels;
    m_width     = width;
    m_height    = height;
    m_config    = config;

	AllocateRGB();
}

bool ImageTest::AllocateRGB()
{
	//
	// If the source data is 3-channel (i.e. rgb) in rgba format, then
	// we will need to convert it into a planar format for MATLAB.  
	//
    if (m_config->m_ncolors == 3)
	{
		DeleteRGB();
        m_rgb = new unsigned char[static_cast<unsigned long long>(m_width * m_height * 3)];
	}

    return m_rgb != nullptr;
}

void ImageTest::DeleteRGB()
{
    if (m_rgb != nullptr)
	{
		delete [] m_rgb;
        m_rgb = nullptr;
	}
}

/// converts from m_rawPixels to m_rgb
void ImageTest::PlanesFromRGB()
{
	int				i, numPixels;
	unsigned char	*r, *g, *b;		// pointers into m_rgb
    const unsigned char	*R, *G, *B;		// pointers into m_rawPixels


	//
	// m_rawPixels is an array of RGBQUAD structs (32-bit ints), where each byte represents one color component (r, g, b), plus
	// one unused byte.  MATLAB stores an RGB image sequentially, with all of the red values followed by all of the green values, 
	// followed by all of the blue values.  This function converts an arry of RGBQUAD values into MATLAB's color plane format.
	//
	numPixels = m_width * m_height;
	r = m_rgb;
	g = r + numPixels;
	b = g + numPixels;

    R = static_cast<const unsigned char*>(m_rawPixels);
	G = R+1;
	B = G+1;

	// TODO, vectorize this loop?
	for (i = 0; i < numPixels; i++)
	{
		*r = *R;
		*g = *G;
		*b = *B;

		r++;
		g++;
		b++;
		R += 4;
		G += 4;
		B += 4;
	}
}

/// converts from m_rgb to rgb
void ImageTest::RGBFromPlanes(uint32_t *rgb)
{
	int				i, numPixels;
	unsigned char	*r, *g, *b;		// pointers into m_rgb (MATLAB format)
    unsigned char	*R, *G, *B;		// pointers into rgb (Windows RGBQUAD format)


	//
	// rgb is an array of 32-bit ints, arranged in rgba order (i.e. an array of RGBQUAD structs).
	// MATLAB stores an RGB image sequentally, with all of the red values followed
	// by all of the green values, followed by all of the blue values.
	// 
	// This function converts from MATLAB's color plane format into rgb format
	//
	numPixels = m_width * m_height;
	r = m_rgb;
	g = r + numPixels;
	b = g + numPixels;

    R = reinterpret_cast<unsigned char *>(rgb);
	G = R+1;
	B = G+1;

	for (i = 0; i < numPixels; i++)
	{
		*R = *r;
		*G = *g;
		*B = *b;

		r++;
		g++;
		b++;
		R += 4;
		G += 4;
		B += 4;
	}
}

void ImageTest::Run(void *raw_pixels, int width, int height, const Config *config)
{
	Init(raw_pixels, width, height, config);
	Run();
}

///
/// Runs the test pointed to by m_imatestFunc
///
void ImageTest::Run()
{
	json_string		test_settings;
	int				crop_borders[4] = {0,0,0,0};

    InitResults();  	// clear out strings, set m_passed to false
    PlanesFromRGB();    // convert m_rawPixels from rgba format to planar format for MATLAB

    prepareTestSettings(m_width, m_height, m_config->m_ncolors, m_config->m_extension.c_str(), m_config->m_fileRoot.c_str(), m_config->m_serialNumber.c_str(), m_config->m_partNumber.c_str(), crop_borders, test_settings);

	try
	{
#if 0
        //
        // To create mwArrays using the constructor prototype:
        // mwArray(mwSize num_strings, const char** str)
        //
        mwArray fileParam(1, file_name);										// Argument 1 - File name (Not applicable for raw files)
		mwArray pathParam(1, program_path);										// Argument 2 -	Program Path
        mwArray iniFileParam(1, iniFile);										// Argument 5 INI file
        mwArray	json_options_param(1,json_options);								// JSON options describing the RAW file;
        mwArray keysParam(1, input_keys);										// Argument 3 - Mode of Control
        mwArray modeParam(1, operation_flag);									// Argument 4 - Operation Flag
#else
        //
        // When I try to init mwArrays using a literal string, an exception is getting thrown, so I'm using std::strings
        // instead.  This may be an artifact of running the debugger with a mix of debug and release libs
        //
        std::string empty("");
        std::string json("JSON");
        std::string rawMode(INI_RAW_JSON_MODE);
        mwArray fileParam(empty.c_str());
        mwArray pathParam(m_config->m_programPath.c_str());
        mwArray iniFileParam(m_config->m_iniFilePathName.c_str());
        mwArray	json_options_param(test_settings.c_str());						// JSON options describing the RAW file;
        mwArray keysParam(json.c_str());           								// Argument 3 - Mode of Control
        mwArray modeParam(rawMode.c_str());         							// Argument 4 - Operation Flag
#endif
        mwArray rawFileParam(static_cast<mwSize>(m_width * m_height * 3), 1, mxUINT8_CLASS, mxREAL);	// Argument 6 - RGB Data
		mwArray	vararginParam(1,3,mxCELL_CLASS);								// The remaining arguments go in this param
		mwArray	out;															// this will hold returned data

        rawFileParam.SetData(m_rgb, static_cast<mwSize>(m_width * m_height * 3));

        // Our remaining arguments go into the Varargin parameter
		vararginParam.Get(1,1).Set(iniFileParam);			// Path to INI file
		vararginParam.Get(1,2).Set(rawFileParam);			// data description
		vararginParam.Get(1,3).Set(json_options_param);		// options file

		//
		// Call the Imatest IT library function
		//
        if (m_imatestFunc == nullptr)
		{
		}
		else
		{
			m_imatestFunc(1, out, fileParam, pathParam, keysParam, modeParam, vararginParam);
		}

        //
        // Copy the output of the module to a System::String object
        //
        stringstream ss (stringstream::in | stringstream::out);
		ss << out;
		string jsonResults = ss.str();
        ParseResults(jsonResults);
		SetJSON(jsonResults);	// copy results into m_jsonResults (m_jsonResults is shared between threads, so we don't want to access it directly)
	}

	catch (mwException &e)
	{
        QString str;
        QTextStream text(&str);
        QString lf("\n");
        QString crlf("\r\n");

        text << m_timestamp.Get() << " Error running " << m_name << " test" << endl << e.what();
        AppendLog(str);
		cout << "Run Error!" << endl;
		cerr << e.what() << endl;
		e.print_stack_trace();
		m_passed = false;
	}
}


bool ImageTest::GetDataNode(string &results, JSONNode &data, bool logErrors)
{
	bool		success = false;
	JSONNode	node;

	try
	{
		node = libjson::parse(results.c_str());
		data = node.at(0);
		success = true;
	}
	catch(...)
	{
		if (logErrors)
		{
            QString     str;
            QTextStream text(&str);
            text << m_timestamp.Get() << " Results have invalid format" << endl;
            m_log.Append(text);
        }

		success = false;
	}

	return success;
}


bool ImageTest::GetPassFailNode(JSONNode &data, JSONNode &passFail, bool logErrors)
{
	bool	validNode;

	try 
	{
		passFail = data.at("passfail");
		validNode = true;
	}
	
	catch(...) 
	{
		validNode = false;
		
		if (logErrors)
		{
            QString     str;
            QTextStream text(&str);
            text << m_timestamp.Get() << " Couldn't find pass/fail section in results" << endl;
            m_log.Append(text);
        }
	}


	return validNode;
}

void ImageTest::GetString(JSONNode &node, const char *name, json_string &string, bool logErrors)
{
	try
	{
		string = node.at(name).as_array()[0].as_string();
	}

	catch(...)
	{
        if (logErrors)
        {
            QString     str;
            QTextStream text(&str);
            text << m_timestamp.Get() << " Couldn't find " << name << " value" << endl;
            m_log.Append(text);
        }
	}
}

void ImageTest::ParseFailures(const JSONNode *passfail)
{
    QString	fail;
	int		i;
	bool	first  = true;
	bool	passed = false;


	for (i = 0; i < m_numTests; i++)
	{
		try 
		{
			passed = passfail->at(m_tests[i].key).as_array()[0].as_bool();
			
			if (!passed)
			{
				if (!first)
				{
                    fail.append(FAIL_DELIMITER);	// add a separator before adding the next test name
				}

                fail.append(m_tests[i].name);	// add the name of the test that failed to the m_failInfo string
				first = false;
			}
		}
		
		catch(...) 
		{
            QString     str;
            QTextStream text(&str);

            text << "Couldn't find " << m_tests[i].key << " in results" << endl;
            m_log.Append(text);
        }
	}

	m_failInfo.Set(fail);
}

void IMA_CALL_CONV ImageTest::ThreadProc(void *param)
{
    ImageTest   *test = static_cast<ImageTest *>(param);

    test->m_time.Start();
    test->Run();
    test->m_time.Stop();
    test->m_time.Get(&test->m_elapsedStr);
    test->m_time.Get(&test->m_elapsed);
}

void ImageTest::InitResults()
{
	m_jsonResults.Clear();
	m_summary.Clear();
	m_failInfo.Clear();
	m_log.Clear();
	m_passed = false;
}
