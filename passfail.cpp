#if 0
This is the order of the sfrplus keys as assigned in this module:
 1  All_Edge_IDs_detected
 2  Bayer_decode
 3  Chart_mean_pixel_level_bounds
 4  Chart_radial_pixel_shift_max
 5  Color_expected_detected
 6  Convergence_angle_max
 6  DeltaE_00_mean_max
 7  FOV_degrees_diagonal_min
 8  High_pixel_saturation_fraction_max
 9  Horizontal_bars_OK_min
10  Low_pixel_saturation_fraction_max
11  Mirrored_chart
12  MTF50P_CP_weighted_mean_min
13  MTF50P_ratio_min
14  passfail_ini_file_date
15  Rotation_degrees_max
16  Secondary_readout_1_center_mean_min
17  Secondary_readout_1_outer_mean_min
18  Secondary_readout_1_outer_min_min
19  Secondary_readout_1_outer_quadrant_delta_max
20  Secondary_readout_1_outer_quadrant_mean_min_min
21  Secondary_readout_2_center_mean_min
22  Secondary_readout_2_outer_mean_min
23  Secondary_readout_2_outer_min_min
24  Secondary_readout_2_outer_quadrant_delta_max
25  Secondary_readout_2_outer_quadrant_mean_min_min
26  Stepchart_expected_detected
27  upside_down

This is the order of the srfplus keys in Sepc-v2.ini:
 1    All_Edge_IDs_detected = 1
 2    Bayer_decode = 1
 3    Chart_mean_pixel_level_bounds = 0.3 0.8
 4    Chart_radial_pixel_shift_max = 18
 5    FOV_degrees_diagonal_min = 3
 6    High_pixel_saturation_fraction_max = 3
 7    Horizontal_bars_OK_min = 3
 8    Low_pixel_saturation_fraction_max = 60
 9    Mirrored_chart = 0
10    MTF50P_CP_weighted_mean_min = 1
11    passfail_ini_file_date = 1
12    Rotation_degrees_max = 45
13    Secondary_readout_1_center_mean_min = 45
14    Secondary_readout_1_outer_mean_min = 45
15    Secondary_readout_1_outer_min_min = 3
16    Secondary_readout_1_outer_quadrant_mean_min_min = 0.45
17    Secondary_readout_2_center_mean_min = 0.16
18    Secondary_readout_2_outer_mean_min = 0.16
19    Secondary_readout_2_outer_min_min = 0.5
20    Secondary_readout_2_outer_quadrant_mean_min_min = 0.18
21    Stepchart_expected_detected = 0
22    MTF50P_ratio_min = 0
23    Secondary_readout_1_outer_quadrant_delta_max = 0.75
24    Secondary_readout_2_outer_quadrant_delta_max = 0.3
25    upside_down = 0



This is the order of Blemish keys in Spec-v2.ini:
 1    Blemish_maximum_count = 3 51
 2    Blemish_size_pixels = 20 6
 3    Dead_pixels_max = 50
 4    Hot_pixels_max = 50
 5    Optical_center_offset_max = 37.14
 6    Optical_center_offset_X_max = 40
 7    Optical_center_offset_Y_max = 30
 8    Relative_illumination_corner_diff_pct_max = 21
 9    Relative_illumination_worst_corner_pct_min = 29
10    Uniformity_BoverG_corners_pct_max = 14
11    Uniformity_RoverG_corners_pct_max = 13


#endif


#include <QFileInfo>
#include "imatestdefines.h"
#include "imatest_library.h"
#include "inifile.h"
#include "passfail.h"
#include "passfailsettings.h"
#include <sstream>

using namespace std;

bool PassFail::ReadFile(PassFailSettings &settings)
{
    bool result=false;

    // First we must find the name of the Pass/Fail file as listed in imatest.ini
    mwArray vararginParam = mwArray(1,3,mxCELL_CLASS);
    mwArray readKeys = mwArray(1,5,mxCELL_CLASS);
    mwArray inifilename(settings.m_ini_file.c_str());
    mwArray mode("read");
    mwArray section("api");
    mwArray subsection_blank("");
    mwArray key_passFail("passFail");
    mwArray value_int("i"), value_string(""), value_double("d");
    int badval = -123456;
    std::string badstring = "-123456%^&%%$**#";
    mwArray default_0(badval), default_emptystring(badstring.c_str()),default_dbl(static_cast<double>(badval));
    mwSize getIndex = 1;

    getIndex = 1;
    readKeys.Get(1,getIndex++).Set(section);
#ifdef INI_INCLUDE_SUBSECTION
    readKeys.Get(1,getIndex++).Set(subsection_blank);
#endif
    readKeys.Get(1,getIndex++).Set(key_passFail);
    readKeys.Get(1,getIndex++).Set(value_string);
    readKeys.Get(1,getIndex++).Set(default_emptystring);

    vararginParam.Get(1,1).Set(inifilename);
    vararginParam.Get(1,2).Set(mode);
    vararginParam.Get(1,3).Set(readKeys);

    mwArray readSett = mwArray(1,1,mxCELL_CLASS);
    try
    {
        Inifile::inifile(1,readSett,vararginParam);
    }
    catch (mwException& e)
    {
        cout << "Run Error! Unable to read Pass/Fail file" << endl;
        cerr << e.what() << endl;
        e.print_stack_trace();
    }

    mwArray readSettContainer = readSett.Get(1, 1);

    if (readSettContainer.NumberOfElements() > 0)
    {
        remove_crlf(readSett.Get(1, 1).Get(1, 1).ToString(), settings.m_pass_fail_file);
    }
    else
    {
        /// inifile failed
        cout << "Run Error! Unable to read Pass/Fail file" << endl;
    }
    //Now we find the Pass/Fail file and check if it is read-only


    QFileInfo info(settings.m_pass_fail_file.c_str());
    if (!info.isWritable())
    {
        settings.b_isReadOnly = TRUE;
    }

    //
    // Open the pass/fail file
    //
    QFile file(settings.m_pass_fail_file.c_str());

    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        cerr << "Unable to open " << settings.m_pass_fail_file.c_str();
        qDebug() << "Unable to open " << settings.m_pass_fail_file.c_str();
        return false;
    }

    //
    // Read the entire pass/fail file into a QString
    //
    QString data(file.readAll());
    file.close();

    //
    // Get a list of sections, where a section starts with '['
    //
    QStringList sections = data.split("[");

    //
    // Process the blemish, sfrplus, and ois sections, if present
    //
    for (int i = 0; i < sections.length(); ++i)
    {
        if (sections[i].startsWith("Blemish"))
        {
            ProcessBlemish(&sections[i], settings.blemish);
            settings.blemish.b_enable = true;
        }
        else if (sections[i].startsWith("SFRplus"))
        {
            ProcessSfrplus(&sections[i], settings.sfrplus);
            settings.sfrplus.b_enable = true;
        }
        if (sections[i].startsWith("OIS"))
        {
            ProcessOIS(&sections[i], settings.ois);
            settings.other.b_enable = true;
        }
    }

    return result;
}

void PassFail::setValue(vecEntry<long> &entry, QString &value)
{
    QStringList list = value.split(' ', QString::SkipEmptyParts);
    std::size_t length = static_cast<std::size_t>(list.length());
    entry.value.resize(length);

    for (std::size_t i = 0; i < length; ++i)
    {
        entry.value[i] = list[static_cast<int>(i)].toLong();
    }

    entry.b_isUsed = length > 0;
}

void PassFail::setValue(minMaxEntry<double> &entry, QString &value)
{
    entry.min_val  = value.section(' ', 0, 0).toDouble();
    entry.max_val  = value.section(' ', 1, 1).toDouble();
    entry.b_isUsed = true;
}

void PassFail::ProcessBlemish(QString *section, Blemish &b)
{
    QTextStream stream(section);

    for (QString line = stream.readLine(); !stream.atEnd(); line = stream.readLine())
    {
        QString name  = line.section('=', 0, 0).trimmed();
        QString value = line.section('=', 1, 1).trimmed();

        // if the name matches a blemish setting, process the entry

        if (name.compare(b.Blemish_maximum_count.group_name.c_str()) == 0)
        {
            setValue(b.Blemish_maximum_count, value);
        }
        else if (name.compare(b.Blemish_size_pixels.group_name.c_str()) == 0)
        {
            setValue(b.Blemish_size_pixels, value);
        }
        else if (name.compare(b.Dead_pixel_clusters_max.name.c_str()) == 0)
        {
            setValue(b.Dead_pixel_clusters_max, value);
        }
        else if (name.compare(b.Dead_pixels_max.name.c_str()) == 0)
        {
            setValue(b.Dead_pixels_max, value);
        }
        else if (name.compare(b.Defective_pixels_max_count.name.c_str()) == 0)
        {
            setValue(b.Defective_pixels_max_count, value);
        }
        else if (name.compare(b.Hot_pixel_clusters_max.name.c_str()) == 0)
        {
            setValue(b.Hot_pixel_clusters_max, value);
        }
        else if (name.compare(b.Hot_pixels_max.name.c_str()) == 0)
        {
            setValue(b.Hot_pixels_max, value);
        }
        else if (name.compare(b.Optical_center_offset_max.name.c_str()) == 0)
        {
            setValue(b.Optical_center_offset_max, value);
        }
        else if (name.compare(b.Optical_center_offset_X_max.name.c_str()) == 0)
        {
            setValue(b.Optical_center_offset_X_max, value);
        }
        else if (name.compare(b.Optical_center_offset_Y_max.name.c_str()) == 0)
        {
            setValue(b.Optical_center_offset_Y_max, value);
        }
        else if (name.compare(b.Relative_illumination_corner_diff_pct_max.name.c_str()) == 0)
        {
            setValue(b.Relative_illumination_corner_diff_pct_max, value);
        }
        else if (name.compare(b.Relative_illumination_worst_corner_pct_min.name.c_str()) == 0)
        {
            setValue(b.Relative_illumination_worst_corner_pct_min, value);
        }
        else if (name.compare(b.Uniformity_BoverG_corners_pct_max.name.c_str()) == 0)
        {
            setValue(b.Uniformity_BoverG_corners_pct_max, value);
        }
        else if (name.compare(b.Uniformity_RoverG_corners_pct_max.name.c_str()) == 0)
        {
            setValue(b.Uniformity_RoverG_corners_pct_max, value);
        }
    }
}

void PassFail::ProcessSfrplus(QString *section, SFRplus &s)
{
    QTextStream stream(section);

    for (QString line = stream.readLine(); !stream.atEnd(); line = stream.readLine())
    {
        QString name  = line.section('=', 0, 0).trimmed();
        QString value = line.section('=', 1, 1).trimmed();

        // if the name matches an sfrplus setting, process the entry
        if (name.compare(s.All_Edge_IDs_detected.name.c_str()) == 0)
        {
            setValue(s.All_Edge_IDs_detected, value);
        }
        else if (name.compare(s.Bayer_decode.name.c_str()) == 0)
        {
            setValue(s.Bayer_decode, value);
        }
        else if (name.compare(s.Chart_mean_pixel_level_bounds.group_name.c_str()) == 0)
        {
            setValue(s.Chart_mean_pixel_level_bounds, value);
        }
        else if (name.compare(s.Chart_radial_pixel_shift_max.name.c_str()) == 0)
        {
            setValue(s.Chart_radial_pixel_shift_max, value);
        }
        else if (name.compare(s.Color_expected_detected.name.c_str()) == 0)
        {
            setValue(s.Color_expected_detected, value);
        }
        else if (name.compare(s.DeltaE_00_mean_max.name.c_str()) == 0)
        {
            setValue(s.DeltaE_00_mean_max, value);
        }
        else if (name.compare(s.Convergence_angle_max.name.c_str()) == 0)
        {
            setValue(s.Convergence_angle_max, value);
        }
        else if (name.compare(s.FOV_degrees_diagonal_min.name.c_str()) == 0)
        {
            setValue(s.FOV_degrees_diagonal_min, value);
        }
        else if (name.compare(s.High_pixel_saturation_fraction_max.name.c_str()) == 0)
        {
            setValue(s.High_pixel_saturation_fraction_max, value);
        }
        else if (name.compare(s.Horizontal_bars_OK_min.name.c_str()) == 0)
        {
            setValue(s.Horizontal_bars_OK_min, value);
        }
        else if (name.compare(s.Low_pixel_saturation_fraction_max.name.c_str()) == 0)
        {
            setValue(s.Low_pixel_saturation_fraction_max, value);
        }
        else if (name.compare(s.Mirrored_chart.name.c_str()) == 0)
        {
            setValue(s.Mirrored_chart, value);
        }
        else if (name.compare(s.MTF50P_ratio_min.name.c_str()) == 0)
        {
            setValue(s.MTF50P_ratio_min, value);
        }
        else if (name.compare(s.MTF50P_CP_weighted_mean_min.name.c_str()) == 0)
        {
            setValue(s.MTF50P_CP_weighted_mean_min, value);
        }
        else if (name.compare(s.passfail_ini_file_date.name.c_str()) == 0)
        {
            s.passfail_ini_file_date.value.empty();
            s.passfail_ini_file_date.b_isUsed = true;
        }
        else if (name.compare(s.Rotation_degrees_max.name.c_str()) == 0)
        {
            setValue(s.Rotation_degrees_max, value);
        }
        else if (name.compare(s.Secondary_readout_1_center_mean_min.name.c_str()) == 0)
        {
            setValue(s.Secondary_readout_1_center_mean_min, value);
        }
        else if (name.compare(s.Secondary_readout_1_outer_mean_min.name.c_str()) == 0)
        {
            setValue(s.Secondary_readout_1_outer_mean_min, value);
        }
        else if (name.compare(s.Secondary_readout_1_outer_min_min.name.c_str()) == 0)
        {
            setValue(s.Secondary_readout_1_outer_min_min, value);
        }
        else if (name.compare(s.Secondary_readout_1_outer_quadrant_delta_max.name.c_str()) == 0)
        {
            setValue(s.Secondary_readout_1_outer_quadrant_delta_max, value);
        }
        else if (name.compare(s.Secondary_readout_1_outer_quadrant_mean_min_min.name.c_str()) == 0)
        {
            setValue(s.Secondary_readout_1_outer_quadrant_mean_min_min, value);
        }
        else if (name.compare(s.Secondary_readout_2_center_mean_min.name.c_str()) == 0)
        {
            setValue(s.Secondary_readout_2_center_mean_min, value);
        }
        else if (name.compare(s.Secondary_readout_2_outer_mean_min.name.c_str()) == 0)
        {
            setValue(s.Secondary_readout_2_outer_mean_min, value);
        }
        else if (name.compare(s.Secondary_readout_2_outer_min_min.name.c_str()) == 0)
        {
            setValue(s.Secondary_readout_2_outer_min_min, value);
        }
        else if (name.compare(s.Secondary_readout_2_outer_quadrant_delta_max.name.c_str()) == 0)
        {
            setValue(s.Secondary_readout_2_outer_quadrant_delta_max, value);
        }
        else if (name.compare(s.Secondary_readout_2_outer_quadrant_mean_min_min.name.c_str()) == 0)
        {
            setValue(s.Secondary_readout_2_outer_quadrant_mean_min_min, value);
        }
        else if (name.compare(s.Stepchart_expected_detected.name.c_str()) == 0)
        {
            setValue(s.Stepchart_expected_detected, value);
        }
        else if (name.compare(s.upside_down.name.c_str()) == 0)
        {
            setValue(s.upside_down, value);
        }
    }
}

void PassFail::ProcessOIS(QString *section, OIS &o)
{
    QTextStream stream(section);

    for (QString line = stream.readLine(); !stream.atEnd(); line = stream.readLine())
    {
        QString name  = line.section('=', 0, 0).trimmed();
        QString value = line.section('=', 1, 1).trimmed();

        // if the name matches an sfrplus setting, process the entry
        if (name.compare(o.L_MTF50_delta2_gain_summary_all_dB_min.name.c_str()) == 0)
        {
            setValue(o.L_MTF50_delta2_gain_summary_all_dB_min, value);
        }
        else if (name.compare(o.R_improve_H_dB_min.name.c_str()) == 0)
        {
            setValue(o.R_improve_H_dB_min, value);
        }
        else if (name.compare(o.R_improve_V_dB_min.name.c_str()) == 0)
        {
            setValue(o.R_improve_V_dB_min, value);
        }
        else if (name.compare(o.R_improve_ALL_dB_min.name.c_str()) == 0)
        {
            setValue(o.R_improve_ALL_dB_min, value);
        }
    }
}

bool PassFail::Read(PassFailSettings &settings)
{
    bool result=false;

    // First we must find the name of the Pass/Fail file as listed in imatest.ini
    mwArray vararginParam = mwArray(1,3,mxCELL_CLASS);
    mwArray readKeys = mwArray(1,5,mxCELL_CLASS);
    mwArray inifilename(settings.m_ini_file.c_str());
    mwArray mode("read");
    mwArray section("api");
    mwArray subsection_blank("");
    mwArray key_passFail("passFail");
    mwArray value_int("i"), value_string(""), value_double("d");
    int badval = -123456;
    std::string badstring = "-123456%^&%%$**#";
    mwArray default_0(badval), default_emptystring(badstring.c_str()),default_dbl(static_cast<double>(badval));
    mwSize getIndex = 1;

    getIndex = 1;
    readKeys.Get(1,getIndex++).Set(section);
#ifdef INI_INCLUDE_SUBSECTION
    readKeys.Get(1,getIndex++).Set(subsection_blank);
#endif
    readKeys.Get(1,getIndex++).Set(key_passFail);
    readKeys.Get(1,getIndex++).Set(value_string);
    readKeys.Get(1,getIndex++).Set(default_emptystring);

    vararginParam.Get(1,1).Set(inifilename);
    vararginParam.Get(1,2).Set(mode);
    vararginParam.Get(1,3).Set(readKeys);

    mwArray readSett = mwArray(1,1,mxCELL_CLASS);
    try
    {
        Inifile::inifile(1,readSett,vararginParam);
    }
    catch (mwException& e)
    {
        cout << "Run Error! Unable to read Pass/Fail file" << endl;
        cerr << e.what() << endl;
        e.print_stack_trace();
    }

    mwArray readSettContainer = readSett.Get(1, 1);

    if (readSettContainer.NumberOfElements() > 0)
    {
        remove_crlf(readSett.Get(1, 1).Get(1, 1).ToString(), settings.m_pass_fail_file);
    }
    else
    {
        /// inifile failed
        cout << "Run Error! Unable to read Pass/Fail file" << endl;
    }
    //Now we find the Pass/Fail file and check if it is read-only


    QFileInfo info(settings.m_pass_fail_file.c_str());
    if (!info.isWritable())
    {
        settings.b_isReadOnly = TRUE;
    }

    // Next we need to figure out which pass/fail modules are in the ini. We will just 'readAll', which returns the full list of sections.
    //[keys,sections,subsections] = INIFILE(fName,'readall')

    mwArray keys;
    mwArray sections;
    mwArray subsections;
    mwArray passFailFile(settings.m_pass_fail_file.c_str());
    mwArray mode_readall("readall"),mode_read("read");
#ifdef INI_INCLUDE_SUBSECTION
    int numArgOut = 3;
#else
    int numArgOut = 2;
#endif
    mwArray varargout = mwArray(1,static_cast<mwSize>(numArgOut),mxCELL_CLASS);

    varargout.Get(1,1).Set(keys);
    varargout.Get(1,2).Set(sections);
#ifdef INI_INCLUDE_SUBSECTION
    varargout.Get(1,3).Set(subsections);
#endif

    mwArray varargin = mwArray(1,2,mxCELL_CLASS);
    varargin.Get(1,1).Set(passFailFile);
    varargin.Get(1,2).Set(mode_readall);
    try
    {
        Inifile::inifile(numArgOut, varargout, varargin);
    }
    catch (mwException& e)
    {
#if 1
        cout << "Run Error! Unable to read Pass/Fail file" << endl;
        cerr << e.what() << endl;
        e.print_stack_trace();
#else
        const char *what = e.what();
        qDebug() << "Run Error! Unable to read Pass/Fail file" << endl << what << endl;
#endif

    }

    std::size_t numSections = varargout.Get(1,2).NumberOfElements();
    for (std::size_t idx = 1; idx <= numSections; ++idx)
    {
        QString name = (const char *)varargout.Get(1,2).Get(1,idx).ToString();

        name.remove('\n');
        name.remove('\r');

        if (name.compare("SFRplus", Qt::CaseInsensitive) ==0)
        {
            settings.sfrplus.b_enable = true;
        }
        else if(name.compare("Blemish", Qt::CaseInsensitive)==0)
        {
            settings.blemish.b_enable = true;
        }
        else if(name.compare("OIS", Qt::CaseInsensitive)==0)
        {
            settings.ois.b_enable = true;
        }
        else

        {
            settings.other.b_enable = true;
            settings.other.name = name.toStdString();
        }
    }

    // Next we can read in the keys for the 'other' section from what we already have for which all values are strings

    // Section index is different for 4.2
#ifdef INI_INCLUDE_SUBSECTION
    mwSize sectionIndex = 1;
#else
    mwSize sectionIndex = 2;
#endif

    /*
    if (settings.other.b_enable)
    {
        mwArray dims = varargout.Get(1,1).GetDimensions();
        std::vector<int> vdims;
        std::size_t arr_dims = varargout.Get(1,1).NumberOfDimensions();

        vdims.resize(arr_dims);

        dims.GetData(&vdims[0], arr_dims);
        std::size_t numRows = (std::size_t)vdims[0];
        for (std::size_t idx=1; idx <=numRows; ++idx)
        {
            CString section = _T(varargout.Get(1,sectionIndex).Get(2,idx,1).ToString());
            section.Remove('\n');
            section.Remove('\r');
            CString cstrkey;
            CString cstrval;
            entry<CString> ent;
            if (section.CompareNoCase(settings.other.name) ==0)
            {
                cstrkey = _T(varargout.Get(1,1).Get(2,idx,3).ToString());
                cstrkey.Remove('\n');
                cstrkey.Remove('\r');
                cstrval = _T(varargout.Get(1,1).Get(2,idx,4).ToString());
                cstrval.Remove('\n');
                cstrval.Remove('\r');
                ent.name = cstrkey;
                ent.value = cstrval;
                ent.data_type = "";
                ent.description = _T("");
                settings.other.ent_vec.push_back(ent);
            }
        }


    }
    */
    // Now we get to read in the other sections in a manner that respects their data type since a few of the inputs are vectors.
    // Now is a good time to grab a cup of tea...

    if (settings.blemish.b_enable)
    {

#ifdef INI_INCLUDE_SUBSECTION
        readKeys = mwArray(settings.blemish.numEntries, 5, mxCELL_CLASS);
#else
        readKeys = mwArray(settings.blemish.numEntries, 4, mxCELL_CLASS);
#endif

        mwArray section_Blemish(settings.blemish.name.c_str());

        varargin = mwArray(1,3,mxCELL_CLASS);

        std::vector<std::string> data_types;
        std::vector<std::string> blem_keys;
#if 0
        // the entry<T> corresponding to a given index in blem_keys will correspond to the same index + 1 in readKeys
        addDataTypeAndKey(settings.blemish.Blemish_maximum_count.data_type, settings.blemish.Blemish_maximum_count.group_name,data_types,blem_keys);										// 1
        addDataTypeAndKey(settings.blemish.Blemish_size_pixels.data_type, settings.blemish.Blemish_size_pixels.group_name,data_types,blem_keys);											// 2
        addDataTypeAndKey(settings.blemish.Dead_pixels_max.data_type, settings.blemish.Dead_pixels_max.name,data_types,blem_keys);															// 3
        addDataTypeAndKey(settings.blemish.Dead_pixel_clusters_max.data_type, settings.blemish.Dead_pixel_clusters_max.name,data_types,blem_keys);                                          // 4
        addDataTypeAndKey(settings.blemish.Defective_pixels_max_count.data_type, settings.blemish.Defective_pixels_max_count.name,data_types,blem_keys);									// 5
        addDataTypeAndKey(settings.blemish.Hot_pixel_clusters_max.data_type, settings.blemish.Hot_pixel_clusters_max.name,data_types,blem_keys);											// 6
        addDataTypeAndKey(settings.blemish.Hot_pixels_max.data_type, settings.blemish.Hot_pixels_max.name,data_types,blem_keys);															// 7
        addDataTypeAndKey(settings.blemish.Optical_center_offset_max.data_type, settings.blemish.Optical_center_offset_max.name,data_types,blem_keys);										// 8
        addDataTypeAndKey(settings.blemish.Optical_center_offset_X_max.data_type, settings.blemish.Optical_center_offset_X_max.name,data_types,blem_keys);									// 9
        addDataTypeAndKey(settings.blemish.Optical_center_offset_Y_max.data_type, settings.blemish.Optical_center_offset_Y_max.name,data_types,blem_keys);									//10
        addDataTypeAndKey(settings.blemish.Relative_illumination_corner_diff_pct_max.data_type, settings.blemish.Relative_illumination_corner_diff_pct_max.name,data_types,blem_keys);		//11
        addDataTypeAndKey(settings.blemish.Relative_illumination_worst_corner_pct_min.data_type, settings.blemish.Relative_illumination_worst_corner_pct_min.name,data_types,blem_keys);	//12
        addDataTypeAndKey(settings.blemish.Uniformity_BoverG_corners_pct_max.data_type, settings.blemish.Uniformity_BoverG_corners_pct_max.name,data_types,blem_keys);						//13
        addDataTypeAndKey(settings.blemish.Uniformity_RoverG_corners_pct_max.data_type, settings.blemish.Uniformity_RoverG_corners_pct_max.name,data_types,blem_keys);						//14
#else
        // the entry<T> corresponding to a given index in blem_keys will correspond to the same index + 1 in readKeys
        addDataTypeAndKey(settings.blemish.Blemish_maximum_count.data_type, settings.blemish.Blemish_maximum_count.group_name,data_types,blem_keys);										// 1
        addDataTypeAndKey(settings.blemish.Blemish_size_pixels.data_type, settings.blemish.Blemish_size_pixels.group_name,data_types,blem_keys);											// 2
        addDataTypeAndKey(settings.blemish.Dead_pixels_max.data_type, settings.blemish.Dead_pixels_max.name,data_types,blem_keys);															// 3
//        addDataTypeAndKey(settings.blemish.Dead_pixel_clusters_max.data_type, settings.blemish.Dead_pixel_clusters_max.name,data_types,blem_keys);										//
//        addDataTypeAndKey(settings.blemish.Defective_pixels_max_count.data_type, settings.blemish.Defective_pixels_max_count.name,data_types,blem_keys);									//
//        addDataTypeAndKey(settings.blemish.Hot_pixel_clusters_max.data_type, settings.blemish.Hot_pixel_clusters_max.name,data_types,blem_keys);											//
        addDataTypeAndKey(settings.blemish.Hot_pixels_max.data_type, settings.blemish.Hot_pixels_max.name,data_types,blem_keys);															// 4
        addDataTypeAndKey(settings.blemish.Optical_center_offset_max.data_type, settings.blemish.Optical_center_offset_max.name,data_types,blem_keys);										// 5
        addDataTypeAndKey(settings.blemish.Optical_center_offset_X_max.data_type, settings.blemish.Optical_center_offset_X_max.name,data_types,blem_keys);									// 6
        addDataTypeAndKey(settings.blemish.Optical_center_offset_Y_max.data_type, settings.blemish.Optical_center_offset_Y_max.name,data_types,blem_keys);									// 7
        addDataTypeAndKey(settings.blemish.Relative_illumination_corner_diff_pct_max.data_type, settings.blemish.Relative_illumination_corner_diff_pct_max.name,data_types,blem_keys);		// 8
        addDataTypeAndKey(settings.blemish.Relative_illumination_worst_corner_pct_min.data_type, settings.blemish.Relative_illumination_worst_corner_pct_min.name,data_types,blem_keys);	// 9
        addDataTypeAndKey(settings.blemish.Uniformity_BoverG_corners_pct_max.data_type, settings.blemish.Uniformity_BoverG_corners_pct_max.name,data_types,blem_keys);						//10
        addDataTypeAndKey(settings.blemish.Uniformity_RoverG_corners_pct_max.data_type, settings.blemish.Uniformity_RoverG_corners_pct_max.name,data_types,blem_keys);						//11
#endif
        // add the contents of blem_keys and data_types to readKeys
        for (std::size_t idx = 0; idx < settings.blemish.numEntries; ++idx)
        {
            try
            {
                mwArray key(blem_keys[idx].c_str());
                mwArray value(data_types[idx].c_str());
                mwArray default_ret;

                if (data_types[idx].compare("i") == 0)
                {
                    default_ret = default_0;
                }
                else if (data_types[idx].compare("d") == 0)
                {
                    default_ret = default_dbl;
                }
                else
                {
                    default_ret = default_emptystring;
                }

                getIndex = 1;
                readKeys.Get(2, idx + 1, getIndex++).Set(section_Blemish);
#ifdef INI_INCLUDE_SUBSECTION
                readKeys.Get(2, idx + 1, getIndex++).Set(subsection_blank);
#endif
                readKeys.Get(2, idx + 1, getIndex++).Set(key);
                readKeys.Get(2, idx + 1, getIndex++).Set(value);
                readKeys.Get(2, idx + 1, getIndex++).Set(default_ret);
            }
            catch (exception e)
            {
                string x = "";
            }
        }

        varargin.Get(1,1).Set(passFailFile);
        varargin.Get(1,2).Set(mode_read);
        varargin.Get(1,3).Set(readKeys);

        readSett = mwArray(1, settings.blemish.numEntries, mxCELL_CLASS);

        try
        {
            Inifile::inifile(1,readSett,varargin);
        }
        catch (mwException& e)
        {
            cout << "Run Error! Unable to read Blemish keys from Pass/Fail file" << endl;
            cerr << e.what() << endl;
            e.print_stack_trace();
        }

        // now to copy the values into settings.blemish

        std::vector<int> intVecBuf;
        std::size_t vecSize;
        vecSize = readSett.Get(1,1).Get(1,1).NumberOfElements();
        intVecBuf.resize(vecSize);
        readSett.Get(1,1).Get(1,1).GetData(&intVecBuf[0], vecSize);
        settings.blemish.Blemish_maximum_count.value.resize(vecSize);
        if (intVecBuf[0] != badval)
        {
            for ( std::size_t idx=0; idx < vecSize; ++idx)
            {
                settings.blemish.Blemish_maximum_count.value[idx] = intVecBuf[idx];
            }
            settings.blemish.Blemish_maximum_count.b_isUsed = true;
        }
//#error this fails-- it should read 20 and 6, but it reads 6 and 0
        vecSize = readSett.Get(1,1).Get(1,2).NumberOfElements();
        intVecBuf.clear();
        intVecBuf.resize(vecSize);
        settings.blemish.Blemish_size_pixels.value.resize(vecSize);
        readSett.Get(1,1).Get(1,2).GetData(&intVecBuf[0], vecSize);
        if (intVecBuf[0] != badval)
        {
            for ( std::size_t idx=0; idx < vecSize; ++idx)
            {
                settings.blemish.Blemish_size_pixels.value[idx] = intVecBuf[idx];
            }
            settings.blemish.Blemish_size_pixels.b_isUsed = true;
        }

        int intBuf = 0;
        double dblBuf = 0.0;

  #if 0
        readSett.Get(1,1).Get(1,3).GetData(&intBuf, 1);
        settings.blemish.Dead_pixels_max.assign_value(intBuf,badval);

        readSett.Get(1,1).Get(1,4).GetData(&intBuf, 1);
        settings.blemish.Dead_pixel_clusters_max.assign_value(intBuf,badval);

        readSett.Get(1,1).Get(1,5).GetData(&intBuf, 1);
        settings.blemish.Defective_pixels_max_count.assign_value(intBuf,badval);

        readSett.Get(1,1).Get(1,6).GetData(&intBuf, 1);
        settings.blemish.Hot_pixel_clusters_max.assign_value(intBuf,badval);

        readSett.Get(1,1).Get(1,7).GetData(&intBuf, 1);
        settings.blemish.Hot_pixels_max.assign_value(intBuf,badval);

        readSett.Get(1,1).Get(1,8).GetData(&dblBuf, 1);
        settings.blemish.Optical_center_offset_max.assign_value(dblBuf,badval);

        readSett.Get(1,1).Get(1,9).GetData(&dblBuf, 1);
        settings.blemish.Optical_center_offset_X_max.assign_value(dblBuf,badval);

        readSett.Get(1,1).Get(1,10).GetData(&dblBuf, 1);
        settings.blemish.Optical_center_offset_Y_max.assign_value(dblBuf,badval);

        readSett.Get(1,1).Get(1,11).GetData(&dblBuf, 1);
        settings.blemish.Relative_illumination_corner_diff_pct_max.assign_value(dblBuf,badval);

        readSett.Get(1,1).Get(1,12).GetData(&dblBuf, 1);
        settings.blemish.Relative_illumination_worst_corner_pct_min.assign_value(dblBuf,badval);

        readSett.Get(1,1).Get(1,13).GetData(&dblBuf, 1);
        settings.blemish.Uniformity_BoverG_corners_pct_max.assign_value(dblBuf,static_cast<double>(badval));

        readSett.Get(1,1).Get(1,14).GetData(&dblBuf, 1);
        settings.blemish.Uniformity_RoverG_corners_pct_max.assign_value(dblBuf,static_cast<double>(badval));
#else
        readSett.Get(1,1).Get(1,3).GetData(&intBuf, 1);
        settings.blemish.Dead_pixels_max.assign_value(intBuf,badval);

        readSett.Get(1,1).Get(1,4).GetData(&intBuf, 1);
        settings.blemish.Dead_pixel_clusters_max.assign_value(intBuf,badval);

        readSett.Get(1,1).Get(1,4).GetData(&intBuf, 1);
        settings.blemish.Hot_pixels_max.assign_value(intBuf,badval);

        readSett.Get(1,1).Get(1,5).GetData(&dblBuf, 1);
        settings.blemish.Optical_center_offset_max.assign_value(dblBuf,badval);

        readSett.Get(1,1).Get(1,6).GetData(&dblBuf, 1);
        settings.blemish.Optical_center_offset_X_max.assign_value(dblBuf,badval);

        readSett.Get(1,1).Get(1,7).GetData(&dblBuf, 1);
        settings.blemish.Optical_center_offset_Y_max.assign_value(dblBuf,badval);

        readSett.Get(1,1).Get(1,8).GetData(&dblBuf, 1);
        settings.blemish.Relative_illumination_corner_diff_pct_max.assign_value(dblBuf,badval);

        readSett.Get(1,1).Get(1,9).GetData(&dblBuf, 1);
        settings.blemish.Relative_illumination_worst_corner_pct_min.assign_value(dblBuf,badval);

        readSett.Get(1,1).Get(1,10).GetData(&dblBuf, 1);
        settings.blemish.Uniformity_BoverG_corners_pct_max.assign_value(dblBuf,static_cast<double>(badval));

        readSett.Get(1,1).Get(1,11).GetData(&dblBuf, 1);
        settings.blemish.Uniformity_RoverG_corners_pct_max.assign_value(dblBuf,static_cast<double>(badval));
#endif
    }

    if ( settings.sfrplus.b_enable)
    {

#ifdef INI_INCLUDE_SUBSECTION
        readKeys = mwArray(settings.sfrplus.numEntries, 5, mxCELL_CLASS);
#else
        readKeys = mwArray(settings.sfrplus.numEntries, 4, mxCELL_CLASS);
#endif


        mwArray section_sfr(settings.sfrplus.name.c_str());

        varargin = mwArray(1,3,mxCELL_CLASS);

        std::vector<std::string> data_types;
        std::vector<std::string> sfr_keys;
#if 1
        //
        // The order of the entries doesn't match the order in the pass/fail file (Spec-v2.ini)
        // If we need to support multiple versions of the file, we may need to read the ini_version
        // entry from the file and branch accordingly.  ini_version = Imatest IT 4.3 Sample Pass/Fail
        //
        // After looking at the file, there was only one entry out of order, so maybe it was just a coding
        // error in the original version of the application.  Either way, for now we'l just support the order
        // in the existing file.
        //

        // the entry<T> corresponding to a given index in sfr_keys will correspond to the same index + 1 in readKeys
        addDataTypeAndKey(settings.sfrplus.All_Edge_IDs_detected.data_type,settings.sfrplus.All_Edge_IDs_detected.name,data_types,sfr_keys);														// 1
        addDataTypeAndKey(settings.sfrplus.Bayer_decode.data_type,settings.sfrplus.Bayer_decode.name,data_types,sfr_keys);																			// 2
        addDataTypeAndKey(settings.sfrplus.Chart_mean_pixel_level_bounds.data_type,settings.sfrplus.Chart_mean_pixel_level_bounds.group_name,data_types,sfr_keys);									// 3
        addDataTypeAndKey(settings.sfrplus.Chart_radial_pixel_shift_max.data_type,settings.sfrplus.Chart_radial_pixel_shift_max.name,data_types,sfr_keys);											// 4
//      addDataTypeAndKey(settings.sfrplus.Color_expected_detected.data_type,settings.sfrplus.Color_expected_detected.name,data_types,sfr_keys);													//
//      addDataTypeAndKey(settings.sfrplus.Convergence_angle_max.data_type,settings.sfrplus.Convergence_angle_max.name,data_types,sfr_keys);														//
//      addDataTypeAndKey(settings.sfrplus.DeltaE_00_mean_max.data_type,settings.sfrplus.DeltaE_00_mean_max.name,data_types,sfr_keys);                                                              //
        addDataTypeAndKey(settings.sfrplus.FOV_degrees_diagonal_min.data_type,settings.sfrplus.FOV_degrees_diagonal_min.name,data_types,sfr_keys);													// 5
        addDataTypeAndKey(settings.sfrplus.High_pixel_saturation_fraction_max.data_type,settings.sfrplus.High_pixel_saturation_fraction_max.name,data_types,sfr_keys);								// 6
        addDataTypeAndKey(settings.sfrplus.Horizontal_bars_OK_min.data_type,settings.sfrplus.Horizontal_bars_OK_min.name,data_types,sfr_keys);														// 7
        addDataTypeAndKey(settings.sfrplus.Low_pixel_saturation_fraction_max.data_type,settings.sfrplus.Low_pixel_saturation_fraction_max.name,data_types,sfr_keys);								// 8
        addDataTypeAndKey(settings.sfrplus.Mirrored_chart.data_type,settings.sfrplus.Mirrored_chart.name,data_types,sfr_keys);																		// 9
        addDataTypeAndKey(settings.sfrplus.MTF50P_CP_weighted_mean_min.data_type,settings.sfrplus.MTF50P_CP_weighted_mean_min.name,data_types,sfr_keys);											//10
        addDataTypeAndKey(settings.sfrplus.passfail_ini_file_date.data_type,settings.sfrplus.passfail_ini_file_date.name,data_types,sfr_keys);														//11
        addDataTypeAndKey(settings.sfrplus.Rotation_degrees_max.data_type,settings.sfrplus.Rotation_degrees_max.name,data_types,sfr_keys);															//12
        addDataTypeAndKey(settings.sfrplus.Secondary_readout_1_center_mean_min.data_type,settings.sfrplus.Secondary_readout_1_center_mean_min.name,data_types,sfr_keys);							//13
        addDataTypeAndKey(settings.sfrplus.Secondary_readout_1_outer_mean_min.data_type,settings.sfrplus.Secondary_readout_1_outer_mean_min.name,data_types,sfr_keys);								//14
        addDataTypeAndKey(settings.sfrplus.Secondary_readout_1_outer_min_min.data_type,settings.sfrplus.Secondary_readout_1_outer_min_min.name,data_types,sfr_keys);								//15
        addDataTypeAndKey(settings.sfrplus.Secondary_readout_1_outer_quadrant_mean_min_min.data_type,settings.sfrplus.Secondary_readout_1_outer_quadrant_mean_min_min.name,data_types,sfr_keys);	//16
        addDataTypeAndKey(settings.sfrplus.Secondary_readout_2_center_mean_min.data_type,settings.sfrplus.Secondary_readout_2_center_mean_min.name,data_types,sfr_keys);							//17
        addDataTypeAndKey(settings.sfrplus.Secondary_readout_2_outer_mean_min.data_type,settings.sfrplus.Secondary_readout_2_outer_mean_min.name,data_types,sfr_keys);								//18
        addDataTypeAndKey(settings.sfrplus.Secondary_readout_2_outer_min_min.data_type,settings.sfrplus.Secondary_readout_2_outer_min_min.name,data_types,sfr_keys);								//19
        addDataTypeAndKey(settings.sfrplus.Secondary_readout_2_outer_quadrant_mean_min_min.data_type,settings.sfrplus.Secondary_readout_2_outer_quadrant_mean_min_min.name,data_types,sfr_keys);	//20
        addDataTypeAndKey(settings.sfrplus.Stepchart_expected_detected.data_type,settings.sfrplus.Stepchart_expected_detected.name,data_types,sfr_keys);											//21
        addDataTypeAndKey(settings.sfrplus.MTF50P_ratio_min.data_type,settings.sfrplus.MTF50P_ratio_min.name,data_types,sfr_keys);																	//22
        addDataTypeAndKey(settings.sfrplus.Secondary_readout_1_outer_quadrant_delta_max.data_type,settings.sfrplus.Secondary_readout_1_outer_quadrant_delta_max.name,data_types,sfr_keys);			//23
        addDataTypeAndKey(settings.sfrplus.Secondary_readout_2_outer_quadrant_delta_max.data_type,settings.sfrplus.Secondary_readout_2_outer_quadrant_delta_max.name,data_types,sfr_keys);			//24
        addDataTypeAndKey(settings.sfrplus.upside_down.data_type,settings.sfrplus.upside_down.name,data_types,sfr_keys);																			//25
#else
        // the entry<T> corresponding to a given index in sfr_keys will correspond to the same index + 1 in readKeys
        addDataTypeAndKey(settings.sfrplus.All_Edge_IDs_detected.data_type,settings.sfrplus.All_Edge_IDs_detected.name,data_types,sfr_keys);														// 1
        addDataTypeAndKey(settings.sfrplus.Bayer_decode.data_type,settings.sfrplus.Bayer_decode.name,data_types,sfr_keys);																			// 2
        addDataTypeAndKey(settings.sfrplus.Chart_mean_pixel_level_bounds.data_type,settings.sfrplus.Chart_mean_pixel_level_bounds.group_name,data_types,sfr_keys);									// 3
        addDataTypeAndKey(settings.sfrplus.Chart_radial_pixel_shift_max.data_type,settings.sfrplus.Chart_radial_pixel_shift_max.name,data_types,sfr_keys);											// 4
        addDataTypeAndKey(settings.sfrplus.Color_expected_detected.data_type,settings.sfrplus.Color_expected_detected.name,data_types,sfr_keys);													// 5
        addDataTypeAndKey(settings.sfrplus.DeltaE_00_mean_max.data_type,settings.sfrplus.DeltaE_00_mean_max.name,data_types,sfr_keys);
        addDataTypeAndKey(settings.sfrplus.Convergence_angle_max.data_type,settings.sfrplus.Convergence_angle_max.name,data_types,sfr_keys);														// 6
        addDataTypeAndKey(settings.sfrplus.FOV_degrees_diagonal_min.data_type,settings.sfrplus.FOV_degrees_diagonal_min.name,data_types,sfr_keys);													// 7
        addDataTypeAndKey(settings.sfrplus.High_pixel_saturation_fraction_max.data_type,settings.sfrplus.High_pixel_saturation_fraction_max.name,data_types,sfr_keys);								// 8
        addDataTypeAndKey(settings.sfrplus.Horizontal_bars_OK_min.data_type,settings.sfrplus.Horizontal_bars_OK_min.name,data_types,sfr_keys);														// 9
        addDataTypeAndKey(settings.sfrplus.Low_pixel_saturation_fraction_max.data_type,settings.sfrplus.Low_pixel_saturation_fraction_max.name,data_types,sfr_keys);								//10
        addDataTypeAndKey(settings.sfrplus.Mirrored_chart.data_type,settings.sfrplus.Mirrored_chart.name,data_types,sfr_keys);																		//11
        addDataTypeAndKey(settings.sfrplus.MTF50P_CP_weighted_mean_min.data_type,settings.sfrplus.MTF50P_CP_weighted_mean_min.name,data_types,sfr_keys);											//12
        addDataTypeAndKey(settings.sfrplus.MTF50P_ratio_min.data_type,settings.sfrplus.MTF50P_ratio_min.name,data_types,sfr_keys);																	//13
        addDataTypeAndKey(settings.sfrplus.passfail_ini_file_date.data_type,settings.sfrplus.passfail_ini_file_date.name,data_types,sfr_keys);														//14
        addDataTypeAndKey(settings.sfrplus.Rotation_degrees_max.data_type,settings.sfrplus.Rotation_degrees_max.name,data_types,sfr_keys);															//15
        addDataTypeAndKey(settings.sfrplus.Secondary_readout_1_center_mean_min.data_type,settings.sfrplus.Secondary_readout_1_center_mean_min.name,data_types,sfr_keys);							//16
        addDataTypeAndKey(settings.sfrplus.Secondary_readout_1_outer_mean_min.data_type,settings.sfrplus.Secondary_readout_1_outer_mean_min.name,data_types,sfr_keys);								//17
        addDataTypeAndKey(settings.sfrplus.Secondary_readout_1_outer_min_min.data_type,settings.sfrplus.Secondary_readout_1_outer_min_min.name,data_types,sfr_keys);								//18
        addDataTypeAndKey(settings.sfrplus.Secondary_readout_1_outer_quadrant_delta_max.data_type,settings.sfrplus.Secondary_readout_1_outer_quadrant_delta_max.name,data_types,sfr_keys);			//19
        addDataTypeAndKey(settings.sfrplus.Secondary_readout_1_outer_quadrant_mean_min_min.data_type,settings.sfrplus.Secondary_readout_1_outer_quadrant_mean_min_min.name,data_types,sfr_keys);	//20
        addDataTypeAndKey(settings.sfrplus.Secondary_readout_2_center_mean_min.data_type,settings.sfrplus.Secondary_readout_2_center_mean_min.name,data_types,sfr_keys);							//21
        addDataTypeAndKey(settings.sfrplus.Secondary_readout_2_outer_mean_min.data_type,settings.sfrplus.Secondary_readout_2_outer_mean_min.name,data_types,sfr_keys);								//22
        addDataTypeAndKey(settings.sfrplus.Secondary_readout_2_outer_min_min.data_type,settings.sfrplus.Secondary_readout_2_outer_min_min.name,data_types,sfr_keys);								//23
        addDataTypeAndKey(settings.sfrplus.Secondary_readout_2_outer_quadrant_delta_max.data_type,settings.sfrplus.Secondary_readout_2_outer_quadrant_delta_max.name,data_types,sfr_keys);			//24
        addDataTypeAndKey(settings.sfrplus.Secondary_readout_2_outer_quadrant_mean_min_min.data_type,settings.sfrplus.Secondary_readout_2_outer_quadrant_mean_min_min.name,data_types,sfr_keys);	//25
        addDataTypeAndKey(settings.sfrplus.Stepchart_expected_detected.data_type,settings.sfrplus.Stepchart_expected_detected.name,data_types,sfr_keys);											//26
        addDataTypeAndKey(settings.sfrplus.upside_down.data_type,settings.sfrplus.upside_down.name,data_types,sfr_keys);																			//27
#endif
        // add the contents of sfr_keys and data_types to readKeys
        for (std::size_t idx = 0; idx < settings.sfrplus.numEntries; ++idx)
        {
            mwArray key(sfr_keys[idx].c_str());
            mwArray value(data_types[idx].c_str());
            mwArray default_ret;

            if (data_types[idx].compare("i")==0)
            {
                default_ret = default_0;
            }
            else if(data_types[idx].compare("d")==0)
            {
                default_ret = default_dbl;
            }
            else
            {
                default_ret = default_emptystring;
            }

            getIndex = 1;
            readKeys.Get(2,idx+1,getIndex++).Set(section_sfr);
#ifdef INI_INCLUDE_SUBSECTION
            readKeys.Get(2,idx+1,getIndex++).Set(subsection_blank);
#endif
            readKeys.Get(2,idx+1,getIndex++).Set(key);
            readKeys.Get(2,idx+1,getIndex++).Set(value);
            readKeys.Get(2,idx+1,getIndex++).Set(default_ret);
        }

        int intBuf = 0;
        double dblBuf = 0.0;
        std::vector<int> intVecBuf;
        std::vector<double> dblVecBuf;
        varargin.Get(1,1).Set(passFailFile);
        varargin.Get(1,2).Set(mode_read);
        varargin.Get(1,3).Set(readKeys);
        readSett = mwArray(1,settings.sfrplus.numEntries,mxCELL_CLASS);

        try
        {
            Inifile::inifile(1,readSett,varargin);
        }
        catch (mwException& e)
        {
            cout << "Run Error! Unable to read SFRplus keys from Pass/Fail file" << endl;
            cerr << e.what() << endl;
            e.print_stack_trace();
        }
#if 1
        try
        {
            // copy the values read from file to the appropriate entries in settings.sfrplus
            readSett.Get(1,1).Get(1,1).GetData(&intBuf, 1);
            settings.sfrplus.All_Edge_IDs_detected.assign_value( intBuf,badval);

            readSett.Get(1,1).Get(1,2).GetData(&intBuf, 1);
            settings.sfrplus.Bayer_decode.assign_value( intBuf,badval);

            std::size_t vecSize = readSett.Get(1,1).Get(1,3).NumberOfElements();
            dblVecBuf.resize(vecSize);
            readSett.Get(1,1).Get(1,3).GetData(&dblVecBuf[0], vecSize);
            settings.sfrplus.Chart_mean_pixel_level_bounds.assign_value(dblVecBuf,static_cast<double>(badval));

            readSett.Get(1,1).Get(1,4).GetData(&dblBuf, 1);
            settings.sfrplus.Chart_radial_pixel_shift_max.assign_value( dblBuf,badval);

    //        readSett.Get(1,1).Get(1,5).GetData(&intBuf, 1);
    //        settings.sfrplus.Color_expected_detected.assign_value(intBuf,badval);

    //        readSett.Get(1,1).Get(1,6).GetData(&dblBuf, 1);
    //        settings.sfrplus.Convergence_angle_max.assign_value(dblBuf,static_cast<double>(badval));

    //        readSett.Get(1,1).Get(1,6).GetData(&dblBuf, 1);
    //        settings.sfrplus.DeltaE_00_mean_max.assign_value(dblBuf,static_cast<double>(badval));

            readSett.Get(1,1).Get(1,5).GetData(&dblBuf, 1);
            settings.sfrplus.FOV_degrees_diagonal_min.assign_value(dblBuf,badval);

            readSett.Get(1,1).Get(1,6).GetData(&dblBuf, 1);
            settings.sfrplus.High_pixel_saturation_fraction_max.assign_value(dblBuf,static_cast<double>(badval));

            readSett.Get(1,1).Get(1,7).GetData(&intBuf, 1);
            settings.sfrplus.Horizontal_bars_OK_min.assign_value(intBuf,badval);

            readSett.Get(1,1).Get(1,8).GetData(&dblBuf, 1);
            settings.sfrplus.Low_pixel_saturation_fraction_max.assign_value(dblBuf,static_cast<double>(badval));

            readSett.Get(1,1).Get(1,9).GetData(&intBuf, 1);
            settings.sfrplus.Mirrored_chart.assign_value(intBuf,badval);

            readSett.Get(1,1).Get(1,10).GetData(&dblBuf, 1);
            settings.sfrplus.MTF50P_CP_weighted_mean_min.assign_value(dblBuf,static_cast<double>(badval));

            readSett.Get(1,1).Get(1,22).GetData(&dblBuf, 1);
            settings.sfrplus.MTF50P_ratio_min.assign_value(dblBuf,static_cast<double>(badval));

#if 1
            remove_crlf(readSett.Get(1,1).Get(1,11).ToString(), settings.sfrplus.passfail_ini_file_date.value);

            if (settings.sfrplus.passfail_ini_file_date.value.compare(badstring.c_str()) != 0)
            {
                settings.sfrplus.passfail_ini_file_date.b_isUsed = true;
                if (settings.sfrplus.passfail_ini_file_date.value.compare("[]") == 0)
                {
                    settings.sfrplus.passfail_ini_file_date.value = "";
                }
            }
            else
            {
                settings.sfrplus.passfail_ini_file_date.b_isUsed = false;
            }
#endif
            readSett.Get(1,1).Get(1,12).GetData(&dblBuf, 1);
            settings.sfrplus.Rotation_degrees_max.assign_value(dblBuf,static_cast<double>(badval));

            readSett.Get(1,1).Get(1,13).GetData(&dblBuf, 1);
            settings.sfrplus.Secondary_readout_1_center_mean_min.assign_value(dblBuf,static_cast<double>(badval));

            readSett.Get(1,1).Get(1,14).GetData(&dblBuf, 1);
            settings.sfrplus.Secondary_readout_1_outer_mean_min.assign_value(dblBuf,static_cast<double>(badval));

            readSett.Get(1,1).Get(1,15).GetData(&dblBuf, 1);
            settings.sfrplus.Secondary_readout_1_outer_min_min.assign_value(dblBuf,static_cast<double>(badval));

            readSett.Get(1,1).Get(1,23).GetData(&dblBuf, 1);
            settings.sfrplus.Secondary_readout_1_outer_quadrant_delta_max.assign_value(dblBuf,static_cast<double>(badval));

            readSett.Get(1,1).Get(1,16).GetData(&dblBuf, 1);
            settings.sfrplus.Secondary_readout_1_outer_quadrant_mean_min_min.assign_value(dblBuf,static_cast<double>(badval));

            readSett.Get(1,1).Get(1,17).GetData(&dblBuf, 1);
            settings.sfrplus.Secondary_readout_2_center_mean_min.assign_value(dblBuf,static_cast<double>(badval));

            readSett.Get(1,1).Get(1,18).GetData(&dblBuf, 1);
            settings.sfrplus.Secondary_readout_2_outer_mean_min.assign_value(dblBuf,static_cast<double>(badval));

            readSett.Get(1,1).Get(1,19).GetData(&dblBuf, 1);
            settings.sfrplus.Secondary_readout_2_outer_min_min.assign_value(dblBuf,static_cast<double>(badval));

            readSett.Get(1,1).Get(1,24).GetData(&dblBuf, 1);
            settings.sfrplus.Secondary_readout_2_outer_quadrant_delta_max.assign_value(dblBuf,static_cast<double>(badval));

            readSett.Get(1,1).Get(1,20).GetData(&dblBuf, 1);
            settings.sfrplus.Secondary_readout_2_outer_quadrant_mean_min_min.assign_value(dblBuf,static_cast<double>(badval));

            readSett.Get(1,1).Get(1,21).GetData(&intBuf, 1);
            settings.sfrplus.Stepchart_expected_detected.assign_value(intBuf,badval);

            readSett.Get(1,1).Get(1,25).GetData(&intBuf, 1);
            settings.sfrplus.upside_down.assign_value(intBuf,badval);
        }

        catch (mwException& e)
        {
            qDebug() << "Run Error! Unable to read SFRplus keys from Pass/Fail file" << endl;
            qDebug() << e.what() << endl;
//            e.print_stack_trace();
        }
#else
        // copy the values read from file to the appropriate entries in settings.sfrplus
        readSett.Get(1,1).Get(1,1).GetData(&intBuf, 1);
        settings.sfrplus.All_Edge_IDs_detected.assign_value( intBuf,badval);

        readSett.Get(1,1).Get(1,2).GetData(&intBuf, 1);
        settings.sfrplus.Bayer_decode.assign_value( intBuf,badval);

        std::size_t vecSize = readSett.Get(1,1).Get(1,3).NumberOfElements();
        dblVecBuf.resize(vecSize);
        readSett.Get(1,1).Get(1,3).GetData(&dblVecBuf[0], vecSize);
        settings.sfrplus.Chart_mean_pixel_level_bounds.assign_value(dblVecBuf,static_cast<double>(badval));

        readSett.Get(1,1).Get(1,4).GetData(&dblBuf, 1);
        settings.sfrplus.Chart_radial_pixel_shift_max.assign_value( dblBuf,badval);

        readSett.Get(1,1).Get(1,5).GetData(&intBuf, 1);
        settings.sfrplus.Color_expected_detected.assign_value(intBuf,badval);

        readSett.Get(1,1).Get(1,6).GetData(&dblBuf, 1);
        settings.sfrplus.Convergence_angle_max.assign_value(dblBuf,static_cast<double>(badval));

        readSett.Get(1,1).Get(1,6).GetData(&dblBuf, 1);
        settings.sfrplus.DeltaE_00_mean_max.assign_value(dblBuf,static_cast<double>(badval));

        readSett.Get(1,1).Get(1,7).GetData(&dblBuf, 1);
        settings.sfrplus.FOV_degrees_diagonal_min.assign_value(dblBuf,badval);

        readSett.Get(1,1).Get(1,8).GetData(&dblBuf, 1);
        settings.sfrplus.High_pixel_saturation_fraction_max.assign_value(dblBuf,static_cast<double>(badval));

        readSett.Get(1,1).Get(1,9).GetData(&intBuf, 1);
        settings.sfrplus.Horizontal_bars_OK_min.assign_value(intBuf,badval);

        readSett.Get(1,1).Get(1,10).GetData(&dblBuf, 1);
        settings.sfrplus.Low_pixel_saturation_fraction_max.assign_value(dblBuf,static_cast<double>(badval));

        readSett.Get(1,1).Get(1,11).GetData(&intBuf, 1);
        settings.sfrplus.Mirrored_chart.assign_value(intBuf,badval);

        readSett.Get(1,1).Get(1,12).GetData(&dblBuf, 1);
        settings.sfrplus.MTF50P_CP_weighted_mean_min.assign_value(dblBuf,static_cast<double>(badval));

        readSett.Get(1,1).Get(1,13).GetData(&dblBuf, 1);
        settings.sfrplus.MTF50P_ratio_min.assign_value(dblBuf,static_cast<double>(badval));

        remove_crlf(readSett.Get(1,1).Get(1,14).ToString(), settings.sfrplus.passfail_ini_file_date.value);

        if (settings.sfrplus.passfail_ini_file_date.value.compare(badstring.c_str()) != 0)
        {
            settings.sfrplus.passfail_ini_file_date.b_isUsed = true;
            if (settings.sfrplus.passfail_ini_file_date.value.compare("[]") == 0)
            {
                settings.sfrplus.passfail_ini_file_date.value = "";
            }
        }
        else
        {
            settings.sfrplus.passfail_ini_file_date.b_isUsed = false;
        }

        readSett.Get(1,1).Get(1,15).GetData(&dblBuf, 1);
        settings.sfrplus.Rotation_degrees_max.assign_value(dblBuf,static_cast<double>(badval));

        readSett.Get(1,1).Get(1,16).GetData(&dblBuf, 1);
        settings.sfrplus.Secondary_readout_1_center_mean_min.assign_value(dblBuf,static_cast<double>(badval));

        readSett.Get(1,1).Get(1,17).GetData(&dblBuf, 1);
        settings.sfrplus.Secondary_readout_1_outer_mean_min.assign_value(dblBuf,static_cast<double>(badval));

        readSett.Get(1,1).Get(1,18).GetData(&dblBuf, 1);
        settings.sfrplus.Secondary_readout_1_outer_min_min.assign_value(dblBuf,static_cast<double>(badval));

        readSett.Get(1,1).Get(1,19).GetData(&dblBuf, 1);
        settings.sfrplus.Secondary_readout_1_outer_quadrant_delta_max.assign_value(dblBuf,static_cast<double>(badval));

        readSett.Get(1,1).Get(1,20).GetData(&dblBuf, 1);
        settings.sfrplus.Secondary_readout_1_outer_quadrant_mean_min_min.assign_value(dblBuf,static_cast<double>(badval));

        readSett.Get(1,1).Get(1,21).GetData(&dblBuf, 1);
        settings.sfrplus.Secondary_readout_2_center_mean_min.assign_value(dblBuf,static_cast<double>(badval));

        readSett.Get(1,1).Get(1,22).GetData(&dblBuf, 1);
        settings.sfrplus.Secondary_readout_2_outer_mean_min.assign_value(dblBuf,static_cast<double>(badval));

        readSett.Get(1,1).Get(1,23).GetData(&dblBuf, 1);
        settings.sfrplus.Secondary_readout_2_outer_min_min.assign_value(dblBuf,static_cast<double>(badval));

        readSett.Get(1,1).Get(1,24).GetData(&dblBuf, 1);
        settings.sfrplus.Secondary_readout_2_outer_quadrant_delta_max.assign_value(dblBuf,static_cast<double>(badval));

        readSett.Get(1,1).Get(1,25).GetData(&dblBuf, 1);
        settings.sfrplus.Secondary_readout_2_outer_quadrant_mean_min_min.assign_value(dblBuf,static_cast<double>(badval));

        readSett.Get(1,1).Get(1,26).GetData(&intBuf, 1);
        settings.sfrplus.Stepchart_expected_detected.assign_value(intBuf,badval);

        readSett.Get(1,1).Get(1,27).GetData(&intBuf, 1);
        settings.sfrplus.upside_down.assign_value(intBuf,badval);
#endif
    }

    if (settings.ois.b_enable)
    {

        readKeys = mwArray(settings.ois.numEntries,5,mxCELL_CLASS);
        varargin = mwArray(1,3,mxCELL_CLASS);
        mwArray section_ois(settings.ois.name.c_str());

        std::vector<std::string> data_types;
        std::vector<std::string> ois_keys;

        addDataTypeAndKey(settings.ois.L_MTF50_delta2_gain_summary_all_dB_min.data_type,settings.ois.L_MTF50_delta2_gain_summary_all_dB_min.name,data_types,ois_keys);	// 1
        addDataTypeAndKey(settings.ois.R_improve_ALL_dB_min.data_type,settings.ois.R_improve_ALL_dB_min.name,data_types,ois_keys);										// 2
        addDataTypeAndKey(settings.ois.R_improve_H_dB_min.data_type,settings.ois.R_improve_H_dB_min.name,data_types,ois_keys);											// 3
        addDataTypeAndKey(settings.ois.R_improve_V_dB_min.data_type,settings.ois.R_improve_V_dB_min.name,data_types,ois_keys);											// 4
        // add the contents of ois_keys and data_types to readKeys
        for (std::size_t idx = 0; idx < settings.ois.numEntries; ++idx)
        {
            mwArray key(ois_keys[idx].c_str());
            mwArray value(data_types[idx].c_str());
            mwArray default_ret;

            if (data_types[idx].compare("i")==0)
            {
                default_ret = default_0;
            }
            else if(data_types[idx].compare("d")==0)
            {
                default_ret = default_dbl;
            }
            else
            {
                default_ret = default_emptystring;
            }
            getIndex = 1;
            readKeys.Get(2,idx+1,getIndex++).Set(section_ois);
#ifdef INI_INCLUDE_SUBSECTION
            readKeys.Get(2,idx+1,getIndex++).Set(subsection_blank);
#endif
            readKeys.Get(2,idx+1,getIndex++).Set(key);
            readKeys.Get(2,idx+1,getIndex++).Set(value);
            readKeys.Get(2,idx+1,getIndex++).Set(default_ret);
        }

        int intBuf = 0;
        std::vector<int> intVecBuf;

        varargin.Get(1,1).Set(passFailFile);
        varargin.Get(1,2).Set(mode_read);
        varargin.Get(1,3).Set(readKeys);
        readSett = mwArray(1,settings.ois.numEntries,mxCELL_CLASS);

        try
        {
            Inifile::inifile(1,readSett,varargin);
        }
        catch (mwException& e)
        {
            cout << "Run Error!" << endl;
            cerr << e.what() << endl;
            e.print_stack_trace();
        }
        // copy the values read from file to the appropriate entries in settings.ois
        readSett.Get(1,1).Get(1,1).GetData(&intBuf, 1);
        settings.ois.L_MTF50_delta2_gain_summary_all_dB_min.assign_value(intBuf,badval);

        readSett.Get(1,1).Get(1,2).GetData(&intBuf, 1);
        settings.ois.R_improve_ALL_dB_min.assign_value(intBuf,badval);

        readSett.Get(1,1).Get(1,3).GetData(&intBuf, 1);
        settings.ois.R_improve_H_dB_min.assign_value(intBuf,badval);

        readSett.Get(1,1).Get(1,4).GetData(&intBuf, 1);
        settings.ois.R_improve_V_dB_min.assign_value(intBuf,badval);
    }

    return result;
}


bool PassFail::Write(PassFailSettings &settings)
{
    bool result = false;

    mwArray varargin = mwArray(1,4,mxCELL_CLASS);

    mwArray inifilename(settings.m_ini_file.c_str());
    mwArray passfailfilename(settings.m_pass_fail_file.c_str());
    mwArray mode("write");
    mwArray style("plain");
    mwArray subsection_blank("");
    mwArray value_int("i"), value_string(""), value_double("d");
    mwSize getIndex = 1;


    if (settings.blemish.b_enable)
    {
        mwArray section(settings.blemish.name.c_str());
        std::vector<std::string> keys;
        std::vector<std::string> vals;

        addKeysAndValues(settings.blemish.Blemish_maximum_count.group_name.c_str(), writeVecEntValueString(settings.blemish.Blemish_maximum_count), keys, vals, settings.blemish.Blemish_maximum_count.b_isUsed);				// 1
        addKeysAndValues(settings.blemish.Blemish_size_pixels.group_name.c_str(),writeVecEntValueString(settings.blemish.Blemish_size_pixels),keys,vals,settings.blemish.Blemish_size_pixels.b_isUsed);						// 2
        addKeysAndValues(settings.blemish.Dead_pixels_max.name.c_str(),writeEntValueString(settings.blemish.Dead_pixels_max), keys, vals, settings.blemish.Dead_pixels_max.b_isUsed);											// 3
        addKeysAndValues(settings.blemish.Dead_pixel_clusters_max.name.c_str(), writeEntValueString(settings.blemish.Dead_pixel_clusters_max), keys, vals, settings.blemish.Dead_pixel_clusters_max.b_isUsed);				// 4
        addKeysAndValues(settings.blemish.Defective_pixels_max_count.name.c_str(), writeEntValueString(settings.blemish.Defective_pixels_max_count), keys, vals, settings.blemish.Defective_pixels_max_count.b_isUsed);		// 5
        addKeysAndValues(settings.blemish.Hot_pixel_clusters_max.name.c_str(), writeEntValueString(settings.blemish.Hot_pixel_clusters_max), keys, vals, settings.blemish.Hot_pixel_clusters_max.b_isUsed);					// 6
        addKeysAndValues(settings.blemish.Hot_pixels_max.name.c_str(), writeEntValueString(settings.blemish.Hot_pixels_max), keys, vals, settings.blemish.Hot_pixels_max.b_isUsed);											// 7
        addKeysAndValues(settings.blemish.Optical_center_offset_max.name.c_str(), writeEntValueString(settings.blemish.Optical_center_offset_max), keys, vals, settings.blemish.Optical_center_offset_max.b_isUsed);			// 8
        addKeysAndValues(settings.blemish.Optical_center_offset_X_max.name.c_str(), writeEntValueString(settings.blemish.Optical_center_offset_X_max), keys, vals, settings.blemish.Optical_center_offset_X_max.b_isUsed);	// 9
        addKeysAndValues(settings.blemish.Optical_center_offset_Y_max.name.c_str(), writeEntValueString(settings.blemish.Optical_center_offset_Y_max), keys, vals, settings.blemish.Optical_center_offset_Y_max.b_isUsed);	//10
        addKeysAndValues(settings.blemish.Relative_illumination_corner_diff_pct_max.name.c_str(), writeEntValueString(settings.blemish.Relative_illumination_corner_diff_pct_max), keys, vals, settings.blemish.Relative_illumination_corner_diff_pct_max.b_isUsed);		//11
        addKeysAndValues(settings.blemish.Relative_illumination_worst_corner_pct_min.name.c_str(), writeEntValueString(settings.blemish.Relative_illumination_worst_corner_pct_min), keys, vals, settings.blemish.Relative_illumination_worst_corner_pct_min.b_isUsed);	//12
        addKeysAndValues(settings.blemish.Uniformity_BoverG_corners_pct_max.name.c_str(), writeEntValueString(settings.blemish.Uniformity_BoverG_corners_pct_max), keys, vals, settings.blemish.Uniformity_BoverG_corners_pct_max.b_isUsed);								//13
        addKeysAndValues(settings.blemish.Uniformity_RoverG_corners_pct_max.name.c_str(), writeEntValueString(settings.blemish.Uniformity_RoverG_corners_pct_max), keys, vals, settings.blemish.Uniformity_RoverG_corners_pct_max.b_isUsed);								//14
        // add the contents of keys and vals to writeKeys
        mwArray writeKeys = mwArray(keys.size(),4,mxCELL_CLASS);
        for (std::size_t idx=0; idx < keys.size(); ++idx)
        {
            mwArray key(keys[idx].c_str());
            mwArray val(vals[idx].c_str());
            getIndex = 1;
            writeKeys.Get(2,idx+1,getIndex++).Set(section);
#ifdef INI_INCLUDE_SUBSECTION
            writeKeys.Get(2,idx+1,getIndex++).Set(subsection_blank);
#endif
            writeKeys.Get(2,idx+1,getIndex++).Set(key);
            writeKeys.Get(2,idx+1,getIndex++).Set(val);
        }

        varargin.Get(1,1).Set(passfailfilename);
        varargin.Get(1,2).Set(mode);
        varargin.Get(1,3).Set(writeKeys);
        varargin.Get(1,4).Set(style);

        try
        {
            Inifile::inifile(static_cast<const mwArray>(varargin));
            result = true;
        }
        catch (mwException& e)
        {
            cout << "Run Error! Unable to write to Pass/Fail file" << endl;
            cerr << e.what() << endl;
            e.print_stack_trace();
            result = false;
        }

    }

    if (settings.ois.b_enable)
    {
        mwArray section(settings.ois.name.c_str());
        std::vector<std::string> keys;
        std::vector<std::string> vals;

        addKeysAndValues(settings.ois.L_MTF50_delta2_gain_summary_all_dB_min.name.c_str(), writeEntValueString(settings.ois.L_MTF50_delta2_gain_summary_all_dB_min), keys, vals, settings.ois.L_MTF50_delta2_gain_summary_all_dB_min.b_isUsed);// 1
        addKeysAndValues(settings.ois.R_improve_ALL_dB_min.name.c_str(), writeEntValueString(settings.ois.R_improve_ALL_dB_min), keys, vals, settings.ois.R_improve_ALL_dB_min.b_isUsed);	// 2
        addKeysAndValues(settings.ois.R_improve_H_dB_min.name.c_str(), writeEntValueString(settings.ois.R_improve_H_dB_min), keys, vals, settings.ois.R_improve_H_dB_min.b_isUsed);		// 3
        addKeysAndValues(settings.ois.R_improve_V_dB_min.name.c_str(), writeEntValueString(settings.ois.R_improve_V_dB_min), keys, vals, settings.ois.R_improve_V_dB_min.b_isUsed);		// 4

        mwArray writeKeys = mwArray(keys.size(),4,mxCELL_CLASS);
        // add the contents of keys and vals to writeKeys
        for (std::size_t idx=0; idx < keys.size(); ++idx)
        {
            mwArray key(keys[idx].c_str());
            mwArray val(vals[idx].c_str());
            getIndex = 1;
            writeKeys.Get(2,idx+1,getIndex++).Set(section);
#ifdef INI_INCLUDE_SUBSECTION
            writeKeys.Get(2,idx+1,getIndex++).Set(subsection_blank);
#endif
            writeKeys.Get(2,idx+1,getIndex++).Set(key);
            writeKeys.Get(2,idx+1,getIndex++).Set(val);
        }

        varargin.Get(1,1).Set(passfailfilename);
        varargin.Get(1,2).Set(mode);
        varargin.Get(1,3).Set(writeKeys);
        varargin.Get(1,4).Set(style);

        try
        {
            Inifile::inifile(static_cast<mwArray>(varargin));
            result = true;
        }
        catch (mwException& e)
        {
            cout << "Run Error writing OIS keys to Pass/Fail file!" << endl;
            cerr << e.what() << endl;
            e.print_stack_trace();
            result = false;
        }

    }

    if (settings.other.b_enable)
    {
        mwArray writeKeys = mwArray(settings.other.ent_vec.size(),4,mxCELL_CLASS);
        mwArray section(settings.other.name.c_str());
        // add the contents of ent_vec to writeKeys
        for (std::size_t idx=0; idx < settings.other.ent_vec.size(); ++idx)
        {
            mwArray key(settings.other.ent_vec[idx].name.c_str());
            mwArray val(writeEntValueString(settings.other.ent_vec[idx]).c_str());
            getIndex = 1;
            writeKeys.Get(2,idx+1,getIndex++).Set(section);
#ifdef INI_INCLUDE_SUBSECTION
            writeKeys.Get(2,idx+1,getIndex++).Set(subsection_blank);
#endif
            writeKeys.Get(2,idx+1,getIndex++).Set(key);
            writeKeys.Get(2,idx+1,getIndex++).Set(val);
        }

        varargin.Get(1,1).Set(passfailfilename);
        varargin.Get(1,2).Set(mode);
        varargin.Get(1,3).Set(writeKeys);
        varargin.Get(1,4).Set(style);

        try
        {
            Inifile::inifile(static_cast<const mwArray>(varargin));
            result = true;
        }
        catch (mwException& e)
        {
            cout << "Run Error writing Blemish keys to Pass/Fail file!" << endl;
            cerr << e.what() << endl;
            e.print_stack_trace();
            result = false;
        }

    }

    if ( settings.sfrplus.b_enable)
    {
        mwArray section(settings.sfrplus.name.c_str());
        std::vector<std::string> keys;
        std::vector<std::string> vals;

        addKeysAndValues(settings.sfrplus.All_Edge_IDs_detected.name.c_str(), writeEntValueString(settings.sfrplus.All_Edge_IDs_detected), keys, vals, settings.sfrplus.All_Edge_IDs_detected.b_isUsed);												// 1
        addKeysAndValues(settings.sfrplus.Bayer_decode.name.c_str(), writeEntValueString(settings.sfrplus.Bayer_decode), keys, vals, settings.sfrplus.Bayer_decode.b_isUsed);																			// 2
        addKeysAndValues(settings.sfrplus.Chart_mean_pixel_level_bounds.group_name.c_str(), writeMinMaxEntValueString(settings.sfrplus.Chart_mean_pixel_level_bounds), keys, vals, settings.sfrplus.Chart_mean_pixel_level_bounds.b_isUsed);			// 3
        addKeysAndValues(settings.sfrplus.Chart_radial_pixel_shift_max.name.c_str(), writeEntValueString(settings.sfrplus.Chart_radial_pixel_shift_max), keys, vals, settings.sfrplus.Chart_radial_pixel_shift_max.b_isUsed);							// 4
        addKeysAndValues(settings.sfrplus.Color_expected_detected.name.c_str(), writeEntValueString(settings.sfrplus.Color_expected_detected), keys, vals, settings.sfrplus.Color_expected_detected.b_isUsed);										// 5
        addKeysAndValues(settings.sfrplus.DeltaE_00_mean_max.name.c_str(), writeEntValueString(settings.sfrplus.DeltaE_00_mean_max), keys, vals, settings.sfrplus.DeltaE_00_mean_max.b_isUsed);										// 6
        addKeysAndValues(settings.sfrplus.Convergence_angle_max.name.c_str(), writeEntValueString(settings.sfrplus.Convergence_angle_max), keys, vals, settings.sfrplus.Convergence_angle_max.b_isUsed);												// 7
        addKeysAndValues(settings.sfrplus.FOV_degrees_diagonal_min.name.c_str(), writeEntValueString(settings.sfrplus.FOV_degrees_diagonal_min), keys, vals, settings.sfrplus.FOV_degrees_diagonal_min.b_isUsed);										// 8
        addKeysAndValues(settings.sfrplus.High_pixel_saturation_fraction_max.name.c_str(), writeEntValueString(settings.sfrplus.High_pixel_saturation_fraction_max), keys, vals, settings.sfrplus.High_pixel_saturation_fraction_max.b_isUsed);		// 9
        addKeysAndValues(settings.sfrplus.Horizontal_bars_OK_min.name.c_str(), writeEntValueString(settings.sfrplus.Horizontal_bars_OK_min), keys, vals, settings.sfrplus.Horizontal_bars_OK_min.b_isUsed);											// 10
        addKeysAndValues(settings.sfrplus.Low_pixel_saturation_fraction_max.name.c_str(), writeEntValueString(settings.sfrplus.Low_pixel_saturation_fraction_max), keys, vals, settings.sfrplus.Low_pixel_saturation_fraction_max.b_isUsed);			//11
        addKeysAndValues(settings.sfrplus.Mirrored_chart.name.c_str(), writeEntValueString(settings.sfrplus.Mirrored_chart), keys, vals, settings.sfrplus.Mirrored_chart.b_isUsed);																	//12
        addKeysAndValues(settings.sfrplus.MTF50P_CP_weighted_mean_min.name.c_str(), writeEntValueString(settings.sfrplus.MTF50P_CP_weighted_mean_min), keys, vals, settings.sfrplus.MTF50P_CP_weighted_mean_min.b_isUsed);							//13
        addKeysAndValues(settings.sfrplus.MTF50P_ratio_min.name.c_str(), writeEntValueString(settings.sfrplus.MTF50P_ratio_min), keys, vals, settings.sfrplus.MTF50P_ratio_min.b_isUsed);																//14
        addKeysAndValues(settings.sfrplus.passfail_ini_file_date.name.c_str(), writeEntValueString(settings.sfrplus.passfail_ini_file_date), keys, vals, settings.sfrplus.passfail_ini_file_date.b_isUsed);											//15
        addKeysAndValues(settings.sfrplus.Rotation_degrees_max.name.c_str(), writeEntValueString(settings.sfrplus.Rotation_degrees_max), keys, vals, settings.sfrplus.Rotation_degrees_max.b_isUsed);													//16
        addKeysAndValues(settings.sfrplus.Secondary_readout_1_center_mean_min.name.c_str(), writeEntValueString(settings.sfrplus.Secondary_readout_1_center_mean_min), keys, vals, settings.sfrplus.Secondary_readout_1_center_mean_min.b_isUsed);	//17
        addKeysAndValues(settings.sfrplus.Secondary_readout_1_outer_mean_min.name.c_str(), writeEntValueString(settings.sfrplus.Secondary_readout_1_outer_mean_min), keys, vals, settings.sfrplus.Secondary_readout_1_outer_mean_min.b_isUsed);		//18
        addKeysAndValues(settings.sfrplus.Secondary_readout_1_outer_min_min.name.c_str(), writeEntValueString(settings.sfrplus.Secondary_readout_1_outer_min_min), keys, vals, settings.sfrplus.Secondary_readout_1_outer_min_min.b_isUsed);			//19
        addKeysAndValues(settings.sfrplus.Secondary_readout_1_outer_quadrant_delta_max.name.c_str(), writeEntValueString(settings.sfrplus.Secondary_readout_1_outer_quadrant_delta_max), keys, vals, settings.sfrplus.Secondary_readout_1_outer_quadrant_delta_max.b_isUsed);			//20
        addKeysAndValues(settings.sfrplus.Secondary_readout_1_outer_quadrant_mean_min_min.name.c_str(), writeEntValueString(settings.sfrplus.Secondary_readout_1_outer_quadrant_mean_min_min), keys, vals, settings.sfrplus.Secondary_readout_1_outer_quadrant_mean_min_min.b_isUsed);//21
        addKeysAndValues(settings.sfrplus.Secondary_readout_2_center_mean_min.name.c_str(), writeEntValueString(settings.sfrplus.Secondary_readout_2_center_mean_min), keys, vals, settings.sfrplus.Secondary_readout_2_center_mean_min.b_isUsed);	//22
        addKeysAndValues(settings.sfrplus.Secondary_readout_2_outer_mean_min.name.c_str(), writeEntValueString(settings.sfrplus.Secondary_readout_2_outer_mean_min), keys, vals, settings.sfrplus.Secondary_readout_2_outer_mean_min.b_isUsed);		//23
        addKeysAndValues(settings.sfrplus.Secondary_readout_2_outer_min_min.name.c_str(), writeEntValueString(settings.sfrplus.Secondary_readout_2_outer_min_min), keys, vals, settings.sfrplus.Secondary_readout_2_outer_min_min.b_isUsed);			//24
        addKeysAndValues(settings.sfrplus.Secondary_readout_2_outer_quadrant_delta_max.name.c_str(), writeEntValueString(settings.sfrplus.Secondary_readout_2_outer_quadrant_delta_max), keys, vals, settings.sfrplus.Secondary_readout_2_outer_quadrant_delta_max.b_isUsed);			//25
        addKeysAndValues(settings.sfrplus.Secondary_readout_2_outer_quadrant_mean_min_min.name.c_str(), writeEntValueString(settings.sfrplus.Secondary_readout_2_outer_quadrant_mean_min_min), keys, vals, settings.sfrplus.Secondary_readout_2_outer_quadrant_mean_min_min.b_isUsed);//26
        addKeysAndValues(settings.sfrplus.Stepchart_expected_detected.name.c_str(), writeEntValueString(settings.sfrplus.Stepchart_expected_detected), keys, vals, settings.sfrplus.Stepchart_expected_detected.b_isUsed);							//27
        addKeysAndValues(settings.sfrplus.upside_down.name.c_str(), writeEntValueString(settings.sfrplus.upside_down), keys, vals, settings.sfrplus.upside_down.b_isUsed);																			//28

        mwArray writeKeys = mwArray(keys.size(),4,mxCELL_CLASS);
        // add the contents of keys and vals to writeKeys
        for (std::size_t idx=0; idx < keys.size(); ++idx)
        {
            mwArray key(keys[idx].c_str());
            mwArray val(vals[idx].c_str());
            getIndex = 1;
            writeKeys.Get(2,idx+1,getIndex++).Set(section);
#ifdef INI_INCLUDE_SUBSECTION
            writeKeys.Get(2,idx+1,getIndex++).Set(subsection_blank);
#endif
            writeKeys.Get(2,idx+1,getIndex++).Set(key);
            writeKeys.Get(2,idx+1,getIndex++).Set(val);
        }

        varargin.Get(1,1).Set(passfailfilename);
        varargin.Get(1,2).Set(mode);
        varargin.Get(1,3).Set(writeKeys);
        varargin.Get(1,4).Set(style);

        try
        {
            Inifile::inifile(varargin);
            result = true;
        }
        catch (mwException& e)
        {
            cout << "Run Error writing SFRplus keys to Pass/Fail file!" << endl;
            cerr << e.what() << endl;
            e.print_stack_trace();
            result = false;
        }
    }

    return result;
}

///
/// Converts the entry<T>.value into a std::string to facilitate writing to the pass/fail file
///
template <typename T> const std::string PassFail::writeEntValueString( const entry<T>& ent)
{
    stringstream ss;

    ss << ent.value;

    return ss.str();
}

///
/// Converts the minMaxEntry<CString>.min_val and .max_val into a std::string to facilitate writing to the pass/fail file
///
template <typename T> const std::string PassFail::writeMinMaxEntValueString(const minMaxEntry<T>& ent)
{
    stringstream ss;

    ss<< ent.min_val << " " << ent.max_val;
    return ss.str();
}

///
/// Converts the vecEntry<CString>.value into a single std::string to facilitate writing to the pass/fail file
///
template <typename T> const std::string PassFail::writeVecEntValueString(const vecEntry<T>& ent)
{
    stringstream ss;

    for(std::size_t idx =0 ; idx < ent.value.size(); ++idx)
    {
        ss << ent.value[idx] << " ";
    }
    return ss.str();

}

///
/// Conditionally adds a particular key (variable name) and value to the std::vector<std::string> keys, vals if the particular key is in use by the pass/fail file
///
void PassFail::addKeysAndValues(const std::string& candidateKey, const std::string& candidateVal, std::vector<std::string>& keys, std::vector<std::string>& vals, bool b_addKey)
{
    if (b_addKey)
    {
        keys.push_back(candidateKey);
        vals.push_back(candidateVal);
    }
}

///
/// Adds a data_type string and key string to the std::vector<std::string> data_types and keys, respectively
///
void PassFail::addDataTypeAndKey(const std::string& candidateDT, const std::string& candidateKey, std::vector<std::string>& data_types, std::vector<std::string>& keys)
{
    data_types.push_back(candidateDT);
    keys.push_back(candidateKey);
}

void PassFail::remove_crlf(const mwString &src, std::string &str)
{
    QString qstr = (const char *)src;

    qstr.remove('\r');
    qstr.remove('\n');

    str = qstr.toStdString();
}

