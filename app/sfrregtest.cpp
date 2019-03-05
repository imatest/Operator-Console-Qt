#include "sfrregtest.h"
#include <string>

using namespace std;

#define TEST_NAME   "SFRreg"

#define ALL_TESTS	"all_tests_passed"


static const TestID gTestID[] =
{
    {"Horizontal_bars_OK_passed",                          "Horizontal bars"},
    {"All_Edge_IDs_detected_passed",                       "Edge IDs"},
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

constexpr size_t NUM_TESTS = (sizeof(gTestID) / sizeof(gTestID[0]));

SFRregTest::SFRregTest()
{
    m_name        = TEST_NAME;
    m_tests       = gTestID;
    m_numTests    = NUM_TESTS;
    m_imatestFunc = sfrreg_shell;
}

SFRregTest::~SFRregTest()
{

}

void SFRregTest::InitResults()
{
    ImageTest::InitResults();
}

///
/// Gets the pass/fail results for a range of pass/fail metrics to be displayed in summary dialog
///
void SFRregTest::GetResults(JSONNode &passfail)
{
    QString      str;
    QTextStream  summary(&str);
    json_string	center1, outer1, center2, outer2;

    //
    // Get the data values (they're stored in the pass/fail section)
    // GetString() gets the value and handles any exceptions.
    //
    dynamic_cast<ImageTest*>(this)->GetString(passfail, "Secondary_readout_1_center_mean", center1);
    dynamic_cast<ImageTest*>(this)->GetString(passfail, "Secondary_readout_1_outer_mean", outer1);
    dynamic_cast<ImageTest*>(this)->GetString(passfail, "Secondary_readout_2_center_mean", center2);
    dynamic_cast<ImageTest*>(this)->GetString(passfail, "Secondary_readout_2_outer_mean", outer2);

    summary << "MTF#1 Center " << center1.c_str() << endl << "MTF#1 Outer " << outer1.c_str() << endl;
    summary << "MTF#2 Center " << center2.c_str() << endl << "MTF#2 Outer " << outer2.c_str() << endl;
    m_summary.Set(summary);
}


///
/// Parses the JSON output of an SFRreg test to determine the pass/fail status of the various metrics.
///
void SFRregTest::ParseResults(string &results)
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
    }
}
