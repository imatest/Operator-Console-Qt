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
#include "imatestdefines.h"
#include "sfrplustest.h"
#include "config.h"
#include "imatestmainlibrary.h"

IMA_WARNING_PUSH
IMA_DISABLE_UNUSED_PARAM_WARNING
#include "mclcppclass.h"
IMA_DISABLE_REORDER_PARAM_WARNING
#include "libjson.h"
#include "jsonhelpers.h"
#include "JSON/JSONDefs.h"
IMA_WARNING_POP

using namespace std;

#define TEST_NAME			"SFRplus"
#define INI_RAW_JSON_MODE	"-17"


#define ALL_TESTS	"all_tests_passed"
#define NUM_TESTS	(sizeof(gTestID) / sizeof(gTestID[0]))


static const TestID gTestID[] = 
{
	{"Horizontal_bars_OK_passed",                          "Horizontal bars"},
	{"All_Edge_IDs_detected_passed",                       "Edge IDs"},
	{"Mirrored_chart_passed",                              "Mirrored chart"},
	{"Bayer_decode_passed",                                "Bayer decode"},
	{"Rotation_degrees_passed",                            "Rotation degrees"},
	{"FOV_passed",                                         "FOV"},
	{"Horizontal_convergence_angle_passed",                "Horizontal convergence angle"},
	{"Vertical_convergence_angle_passed",                  "Vertical convergence angle"},
	{"Secondary_readout_1_center_mean_passed",             "Secondary readout 1 center mean"},
	{"Secondary_readout_1_outer_mean_passed",              "Secondary readout 1 outer mean"},
	{"Secondary_readout_2_center_mean_passed",             "Secondary readout 2 center mean"},
	{"Secondary_readout_2_outer_mean_passed",              "Secondary readout 2 outer mean"},
	{"Secondary_readout_1_outer_quadrant_delta_passed",    "Secondary readout 1 outer quadrant delta"}, 
	{"Secondary_readout_1_outer_quadrant_mean_min_passed", "Secondary readout 1 outer quadrant mean min"},
	{"Secondary_readout_2_outer_quadrant_delta_passed",    "Secondary readout 2 outer quadrant delta"},
	{"Secondary_readout_2_outer_quadrant_mean_min_passed", "Secondary readout 2 outer quadrant mean min"},
	{"High_pixel_saturation_fraction_passed",              "High pixel saturation fraction"},
	{"Low_pixel_saturation_fraction_passed",               "Low pixel saturation fraction"},
	{"Chart_mean_pixel_level_passed",                      "Chart mean pixel level"},
	{"Chart_radial_pixel_shift_passed",                    "Chart radial pixel shift"},
};


SFRplusTest::SFRplusTest(void)
{
	m_name        = TEST_NAME;
	m_tests       = gTestID;
	m_numTests    = NUM_TESTS;
	m_imatestFunc = sfrplus_shell;
}


SFRplusTest::~SFRplusTest(void)
{
}

void SFRplusTest::InitResults()
{
    memset(m_qso, 0, sizeof(m_qso));
    ImageTest::InitResults();
}


///
/// Parses the JSON output of an sfrPlus test to determine the pass/fail status of the various metrics. 
///
void SFRplusTest::ParseResults(std::string &results)
{
	JSONNode	passfail;
    JSONNode	data;

    if (dynamic_cast<ImageTest*>(this)->GetDataNode(results, data) && dynamic_cast<ImageTest*>(this)->GetPassFailNode(data, passfail))
	{
		try
		{
            passfail.begin();   // I was getting an exception that the iterator was already at the end, so I'm resetting it here.
            m_passed = passfail.at(ALL_TESTS).as_array()[0].as_bool();

			if (!m_passed)
			{
                dynamic_cast<ImageTest*>(this)->ParseFailures(&passfail);
			}
		}

		catch(...) 
		{
			m_failInfo.Set("Couldn't find pass/fail result");
			m_passed = false;
		}

		GetResults(passfail);			// get the results (they're stored in the pass fail node)
		ParseOverlay(data, passfail);
	}
}

///
/// Gets the pass/fail results for a range of pass/fail metrics to be displayed in summary dialog
///
void SFRplusTest::GetResults(JSONNode &passfail)
{
    QString      str;
    QTextStream  summary(&str);
	json_string	center1, outer1, center2, outer2, fov, hConverge, vConverge, rotation;
	
	//
	// Get the data values (they're stored in the pass/fail section)
	// GetString() gets the value and handles any exceptions.
    //
    dynamic_cast<ImageTest*>(this)->GetString(passfail, "Secondary_readout_1_center_mean", center1);
    dynamic_cast<ImageTest*>(this)->GetString(passfail, "Secondary_readout_1_outer_mean", outer1);
    dynamic_cast<ImageTest*>(this)->GetString(passfail, "Secondary_readout_2_center_mean", center2);
    dynamic_cast<ImageTest*>(this)->GetString(passfail, "Secondary_readout_2_outer_mean", outer2);
    dynamic_cast<ImageTest*>(this)->GetString(passfail, "FOV_degrees_diagonal", fov);
    dynamic_cast<ImageTest*>(this)->GetString(passfail, "Horizontal_convergence_angle_degrees", hConverge);
    dynamic_cast<ImageTest*>(this)->GetString(passfail, "Vertical_convergence_angle_degrees", vConverge);
    dynamic_cast<ImageTest*>(this)->GetString(passfail, "Rotation_degrees", rotation);

    summary << "MTF#1 Center " << center1.c_str() << endl << "MTF#1 Outer " << outer1.c_str() << endl << "MTF#2 Center " << center2.c_str() << endl << "MTF#2 Outer " << outer2.c_str() << endl << "FOV " << fov.c_str() << endl << "H Tilt " << hConverge.c_str() << endl << "V Tilt" << vConverge.c_str() << endl << "Rotation " << rotation.c_str() << endl;
    m_summary.Set(summary);
}

///
/// Determines the pass/fail result for each quadrant
///
void SFRplusTest::ParseOverlay(JSONNode &data, JSONNode &passfail)
{
	int		i;
	double	readout1[eNumQuadrants];
	double	readout2[eNumQuadrants];
	double	minCenter1, minCenter2;		// minimum allowed values for center quadrant
	double	minOuter1,  minOuter2;		// minimum allowed values for outer (non-center) quadrants
	double	min1, min2;

	memset(m_qso, 0, sizeof(m_qso));

	try
	{
        JSONNode node;

		//
		// The readout data values are stored in the data section of the JSON buffer
		//
        getFloatArray(node=data.at("secondary_1_quadrant_mean").as_array(), readout1, static_cast<int>(eNumQuadrants));	// get the readout1 data values
        getFloatArray(node=data.at("secondary_2_quadrant_mean").as_array(), readout2, static_cast<int>(eNumQuadrants));	// get the readout2 data values

		//
		// The min allowed values are stored in the pass/fail section of the JSON buffer
		//
		minCenter1 = passfail.at("Secondary_readout_1_center_mean_min").as_array()[0].as_float();	// get the readout1 center min value
		minCenter2 = passfail.at("Secondary_readout_2_center_mean_min").as_array()[0].as_float();	// get the readout2 center min value

		try 
		{
			minOuter1  = passfail.at("Secondary_readout_1_outer_quadrant_mean_min_min").as_array()[0].as_float();	// get the readout1 outer quadrant min value
			minOuter2  = passfail.at("Secondary_readout_2_outer_quadrant_mean_min_min").as_array()[0].as_float();	// get the readout2 outer quadrant min value
		}
		catch (const std::exception &ex)
		{
			cerr << ex.what() << endl;
			cout << "Unable to locate 'Secondary_readout_1_outer_quadrant_mean_min_min' or" <<endl;
			cout << "'Secondary_readout_2_outer_quadrant_mean_min_min'. Will use 'Secondary_readout_1_outer_mean_min'" << endl;
			cout << " and 'Secondary_readout_2_outer_mean_min' instead. Note that there may be discrepancy between" << endl;
			cout << " the overall pass/fail result and the Quadrant-status overlays." << endl;
			minOuter1  = passfail.at("Secondary_readout_1_outer_mean_min").as_array()[0].as_float();	// get the readout1 outer min value
			minOuter2  = passfail.at("Secondary_readout_2_outer_mean_min").as_array()[0].as_float();	// get the readout2 outer min value
		}

		//
		// See whether or not each quadrant passed the test.
		//
		for (i = 0; i < eNumQuadrants; i++)
		{
			if (i == eCenter)
			{
				min1 = minCenter1;
				min2 = minCenter2;
			}
			else
			{
				min1 = minOuter1;
				min2 = minOuter2;
			}

			m_qso[i] = (readout1[i] >= min1) && (readout2[i] >= min2);
		}
	}

	catch (...)
	{
        QString     str;
        QTextStream text(&str);
        text << m_timestamp.Get() << " Error parsing quadrant results" << endl;
        m_log.Append(text);
	} 
}


void SFRplusTest::ClearQSO(void)
{
	for (int i1 = 0; i1 < eNumQuadrants; ++i1)
	{
		m_qso[i1] = false;
	}
}
