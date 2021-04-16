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
#include "blemishtest.h"
#include "imatestmainlibrary.h"
#include "mclcppclass.h"

using namespace std;

#define TEST_NAME			"Blemish"
#define ALL_TESTS			"all_tests_passed"
#define INI_RAW_JSON_MODE	"-17"
#define PIXEL_ERROR_REGIONS 9   // for blemish counts, the frame of the image is split into nine areas

#define NUM_TESTS	(sizeof(gTestID) / sizeof(gTestID[0]))


static const TestID gTestID[] = 
{
	{"Dead_pixels_passed", "Dead Pixels"},
	{"Hot_pixels_passed", "Hot Pixels"},
	{"Optical_center_offset_X_passed", "Center Offset X" },
	{"Optical_center_offset_Y_passed", "Center Offset Y" },
	{"Optical_center_offset_passed", "Center Offset"},
	{"Relative_illumination_worst_corner_passed", "Corner Darkness"},
	{"Relative_illumination_corner_diff_pct_passed", "Corner Delta"},
	{"Uniformity_RoverG_corners_pct_passed", "R over G "},
	{"Uniformity_BoverG_corners_pct_passed", "B over G "},
	{"Blemishes_all_passed", "Blemishes"},
};


BlemishTest::BlemishTest(void)
{
	m_name        = TEST_NAME;
	m_tests       = gTestID;
	m_numTests    = NUM_TESTS;
	m_imatestFunc = blemish_shell;
}


BlemishTest::~BlemishTest(void)
{
}


///
/// Parses the JSON output of an sfrPlus test to determine the pass/fail status of the various metrics. 
///
void BlemishTest::ParseResults(std::string &results)
{
	JSONNode	passfail;
    JSONNode	data;

    dynamic_cast<ImageTest*>(this)->InitResults();

    if (dynamic_cast<ImageTest*>(this)->GetDataNode(results, data) && dynamic_cast<ImageTest*>(this)->GetPassFailNode(data, passfail))// this will copy any errors into m_log
	{
		//
		// Get the pass/fail results
		// 
		try 
		{
			m_passed = passfail.at("all_tests_passed").as_array()[0].as_bool();

			if (!m_passed)
			{
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
		GetResults(data, passfail);
	}
}


void BlemishTest::GetResults(const JSONNode& data, const JSONNode &passfail)
{
	JSONNode	pixelErrors;
	JSONNode	blemishSize;
	json_string	sizes;
	int			total;
	int			dead;
	int			hot;
	int			blemishes;
    char        str[100] = {"N_blemish_count"};
    QString     textString;
    QTextStream text(&textString);

	try
	{
		pixelErrors = data.at("pixelErrors").as_array();
		total       = sumArrayInt(pixelErrors, PIXEL_ERROR_REGIONS);
        blemishes   = getIntFromArray(data, str);
		
		// Note: 'blemishSizePxls' is only written to JSON output if N_blemish_count > 0

		if (blemishes > 0) {
			blemishSize = data.at("blemishSizePxls").as_array();


			// Note: blemishSize is limited to 50 entries in the Imatest library and does 
			// not necessarily equal the JSON entry "N_blemish_count" = blemishes
            if (blemishes < static_cast<int>(blemishSize.size()))
			{
				getStringArray(blemishSize, blemishes, &sizes);
			}
			else
			{
                getStringArray(blemishSize, static_cast<int>(blemishSize.size()), &sizes);
			}
		}

		dead = passfail.at("Dead_pixels").as_array()[0].as_int();
		hot = passfail.at("Hot_pixels").as_array()[0].as_int();

        text << "Total pixel errors " << total << endl << "Dead pixels " << dead << endl << "Hot pixels " << hot << endl << "Blemishes " << blemishes << endl << "Blemish size (pixels) " << sizes.c_str();
        m_summary.Set(text);
	}

	catch(...)
	{
        text << m_timestamp.Get() << " Error parsing results" << endl;
        dynamic_cast<ImageTest*>(this)->AppendLog(text);
    }
}
