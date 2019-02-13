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
#include <string.h>

#include <QTextStream>

#include "arbitrarycharttest.h"
#ifdef _WIN32
#include "imatest_library.h"
#else
#include "libImatest.h"
#endif
#include "mclcppclass.h"

using namespace std;

#define TEST_NAME			"Arbitrary Chart"
#define ALL_TESTS			"all_tests_passed"
#define INI_RAW_JSON_MODE	"-17"
#define PIXEL_ERROR_REGIONS 9   // for blemish counts, the frame of the image is split into nine areas

#define NUM_TESTS	(sizeof(gTestID) / sizeof(gTestID[0]))


static const TestID gTestID[] = 
{
#if 0
    {"Dead_pixels_passed", "Dead Pixels"},
	{"Hot_pixels_passed", "Hot Pixels"},
	{"Optical_center_offset_X_passed", "Center Offset X" },
	{"Optical_center_offset_Y_passed", "Center Offset Y" },
	{"Optical_center_offset_passed", "Center Offset"},
	{"Relative_illumination_worst_corner_passed", "Corner Darkness"},
	{"Relative_illumination_corner_diff_pct_passed", "Corner Delta"},
	{"Uniformity_RoverG_corners_pct_passed", "R over G "},
	{"Uniformity_BoverG_corners_pct_passed", "B over G "},
#endif
    {"ArbitraryChart_all_passed", "ArbitraryChart"},
};


ArbitraryChartTest::ArbitraryChartTest(void)
{
	m_name        = TEST_NAME;
	m_tests       = gTestID;
	m_numTests    = NUM_TESTS;
	m_imatestFunc = arbitrary_charts_shell;
}


ArbitraryChartTest::~ArbitraryChartTest(void)
{
}

void prepareTestSettings(int width, int height, int ncolors, const char* extension, const char* fileroot,
                         const char* serial_number, const char* part_number, int crop_borders[4], json_string &test_settings);

///
/// Runs the test pointed to by m_imatestFunc
///
void ArbitraryChartTest::Run()	// we need to override the parent method because the params for arbitrary_charts_shell are different than blemish and sfrplus
{
	json_string		test_settings;
	int				crop_borders[4] = { 0,0,0,0 };
	char			buf[1024];

    dynamic_cast<ImageTest*>(this)->InitResults();		// clear out strings, set m_passed to false
    dynamic_cast<ImageTest*>(this)->PlanesFromRGB();	// convert m_rawPixels from rgba format to planar format for MATLAB

	//
	// I'm not sure if this is needed for arbitaray charts.  Once the test is working and producing results, we can see if we need this anymore
	//
    prepareTestSettings(m_width, m_height, m_config->m_ncolors, m_config->m_extension.c_str(), m_config->m_fileRoot.c_str(), m_config->m_serialNumber.c_str(), m_config->m_partNumber.c_str(), crop_borders, test_settings);

	//
	// Set up the JSON options param.  For data coming from a buffer (rather than an image), these are the required fields:
	//
	//{
	//	"fileroot": m_fileroot
	//	"extension": "rgb",
	//	"encoding": "sRGB", 
	//	"width": m_width,
	//	"height": m_height
	//}
	//
    snprintf(buf, sizeof(buf), "{\"fileroot\": \"%s\", \"extension\": \"rgb\", \"encoding\": \"sRGB\", \"width\": %d, \"height\": %d }", m_config->m_fileRoot.c_str(), m_width, m_height);
	

	//
	// According to the arbitrary charts sample code (C:\Program Files\Imatest\v5.0\IT\samples\cpp\arbitrary_charts\main.cpp)
	// optionsJson may contain fields:
	//    serial_number: A string containing the serial number
	//    part_number:   A string containing the part number
	//    crop_borders:  A 1 x 4 double array indicating the crop borders ( [L T R B] )
	//    lens_to_chart_distance_cm: A double indicating the lens to chart distance in cm
	//    chart_height_cm: A scalar double indicating the chart height in cm

	try
	{
        std::string empty("");
        mwArray fileParam(empty.c_str());
        mwArray pathParam(m_config->m_programPath.c_str());
        mwArray iniFileParam(m_config->m_iniFilePathName.c_str());

//#define USE_IMAGE_FILE
#if defined(USE_IMAGE_FILE)
		//
		// I used an image file during development because the dll had trouble processing the image from my webcam
		//
		mwArray inputDataParam("C:\\Program Files\\Imatest\\v5.0\\IT\\samples\\cpp\\arbitrary_charts\\P1858_combination_chart_example.jpg");
		mwArray optionsJsonParam("{\"encoding\":\"sRGB\"}");
#else
        mwArray inputDataParam(static_cast<mwSize>(m_width * m_height * 3), 1, mxUINT8_CLASS, mxREAL);
        inputDataParam.SetData(m_rgb, static_cast<mwSize>(m_width * m_height * 3));
		mwArray	optionsJsonParam(buf);
#endif
        std::string zero("0");
        mwArray averageModeParam(zero.c_str());
        mwArray chartFileParam(m_config->m_chartDefFilePathName.c_str());
		mwArray	out;


        arbitrary_charts_shell(1, out, inputDataParam, chartFileParam, iniFileParam, averageModeParam, optionsJsonParam);

		//
		// Copy the output of the module to a System::String object
		//
		stringstream ss(stringstream::in | stringstream::out);
		ss << out;
		string jsonResults = ss.str();
		ParseResults(jsonResults);
        dynamic_cast<ImageTest*>(this)->SetJSON(jsonResults);	// copy results into m_jsonResults (m_jsonResults is shared between threads, so we don't want to access it directly)
	}

	catch (mwException &e)
	{
        QString     str;
        QTextStream text(&str);
        text << m_timestamp.Get() << " Error running " << m_name << " test" << endl << e.what();
        dynamic_cast<ImageTest*>(this)->AppendLog(text);
		cout << "Run Error!" << endl;
		cerr << e.what() << endl;
		e.print_stack_trace();
		m_passed = false;
	}
}

///
/// Parses the JSON output of an ArbitraryChart test to determine the pass/fail status of the various metrics. 
///
void ArbitraryChartTest::ParseResults(string &results)
{
	JSONNode	passfail;
    JSONNode	data;

    dynamic_cast<ImageTest*>(this)->InitResults();

	//
	// Currently (August 2018), there is no Pass/Fail section in the results, so the test will always fail.
	// This will need to be revisited once the output has the proper sections.
	//
    if (dynamic_cast<ImageTest*>(this)->GetDataNode(results, data) && dynamic_cast<ImageTest*>(this)->GetPassFailNode(data, passfail))// this will copy any errors into m_log
	{
		//
        // Get the pass/fail results.  Depending on the format of the results, this may need to be reworked.
		// 
		try 
		{
			m_passed = passfail.at("all_tests_passed").as_array()[0].as_bool();

			if (!m_passed)
			{
				//
				// This may need to be overridden, depending on the format of the results
				//
                dynamic_cast<ImageTest*>(this)->ParseFailures(&passfail);
			}
		}
		catch(...) 
		{
            const char *str = "Unable to find pass/fail result";
			m_failInfo.Set(str);
			m_log.Append(str);
			m_passed = false;
		}

		//
		// Get the results of interest from the data and passfail node
		//
		//GetResults(data, passfail);
	}
}
