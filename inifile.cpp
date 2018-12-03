#include <iostream>
#include "inifile.h"
#include "imatestsourceids.h"
#include "setup_settings.h"

using namespace std;


///
/// This function reads in items for the setup dialog from imatest.ini
///

bool Inifile::ReadSettings(setup_settings& setup)
{
    bool result = false;
    mwArray vararginParam = mwArray(1,3,mxCELL_CLASS);
    mwArray readKeys = mwArray(1,5,mxCELL_CLASS);
    mwArray inifilename(INI_FILENAME);
    mwArray mode("read");
    mwArray section_ovt("ovt"),section_imatest("imatest"),section_op("op_console"),section;
    mwArray subsection_blank(""), subsection_current("current"),subsection;
    mwArray key_acquire("acquire"),key_width("width"),key_height("height"),key_bitdepth("bitdepth"),key_bayer("bayer_pattern"),key_omniregister("register_files"),key_epiphan_deviceid("deviceID");
    mwArray value_int("i"), value_string(""), value_double("d");
    mwArray default_0(0), default_emptystring("");
    mwSize getIndex = 1;
    // NOTE: the mwArray::Get function has input syntax Get(number of indexes, i1, i2,...in)

    // first read the 'acquire' key from [imatest]
    getIndex = 1;
    readKeys.Get(2,1,getIndex++).Set(section_imatest);
#ifdef INI_INCLUDE_SUBSECTION
    readKeys.Get(2,1,getIndex++).Set(subsection_blank);
#endif
    readKeys.Get(2, 1, getIndex++).Set(key_acquire);
    readKeys.Get(2, 1, getIndex++).Set(value_int);
    readKeys.Get(2, 1, getIndex++).Set(default_0);

    vararginParam.Get(2,1,1).Set(inifilename);
    vararginParam.Get(2,1,2).Set(mode);
    vararginParam.Get(2,1,3).Set(readKeys);
    mwArray readSett = mwArray(1,1,mxCELL_CLASS);
    int temp_source_id = setup.sourceID;
    try
    {
        inifile(1, readSett,vararginParam);
        temp_source_id = static_cast<int>(readSett.Get(1,1).Get(1,1));
        setup.sourceID = temp_source_id;

    }
    catch (mwException& e)
    {
        cout << "Run Error!" << endl;
        cerr << e.what() << endl;
        e.print_stack_trace();
    }

    if (setup.sourceID ==2)
    {
        section = section_ovt;
        subsection = subsection_current;
    }
    else
    {
        section = section_op;
        subsection = subsection_blank;
    }

    readKeys = mwArray(6,5,mxCELL_CLASS);
    // to read the Epiphan 'device_ID' key
    getIndex = 1;
    readKeys.Get(2,1,getIndex++).Set(section);
#ifdef INI_INCLUDE_SUBSECTION
    readKeys.Get(2,1,getIndex++).Set(subsection);
#endif
    readKeys.Get(2,1,getIndex++).Set(key_epiphan_deviceid);
    readKeys.Get(2,1,getIndex++).Set(value_int);
    readKeys.Get(2,1,getIndex++).Set(default_0);

    // to read the 'width' key
    getIndex = 1;
    readKeys.Get(2,2,getIndex++).Set(section);
#ifdef INI_INCLUDE_SUBSECTION
    readKeys.Get(2,2,getIndex++).Set(subsection);
#endif
    readKeys.Get(2,2,getIndex++).Set(key_width);
    readKeys.Get(2,2,getIndex++).Set(value_int);
    readKeys.Get(2,2,getIndex++).Set(default_0);

    // to read the 'height' key
    getIndex = 1;
    readKeys.Get(2,3,getIndex++).Set(section);
#ifdef INI_INCLUDE_SUBSECTION
    readKeys.Get(2,3,getIndex++).Set(subsection);
#endif
    readKeys.Get(2,3,getIndex++).Set(key_height);
    readKeys.Get(2,3,getIndex++).Set(value_int);
    readKeys.Get(2,3,getIndex++).Set(default_0);

    // to read the 'bitdepth' key
    getIndex = 1;
    readKeys.Get(2,4,getIndex++).Set(section);
#ifdef INI_INCLUDE_SUBSECTION
    readKeys.Get(2,4,getIndex++).Set(subsection);
#endif
    readKeys.Get(2,4,getIndex++).Set(key_bitdepth);
    readKeys.Get(2,4,getIndex++).Set(value_int);
    readKeys.Get(2,4,getIndex++).Set(default_0);

    // to read the 'bayer_pattern' key
    getIndex = 1;
    readKeys.Get(2,5,getIndex++).Set(section);
#ifdef INI_INCLUDE_SUBSECTION
    readKeys.Get(2,5,getIndex++).Set(subsection);
#endif
    readKeys.Get(2,5,getIndex++).Set(key_bayer);
    readKeys.Get(2,5,getIndex++).Set(value_int);
    readKeys.Get(2,5,getIndex++).Set(default_0);

    // to read the 'register_files' key
    getIndex = 1;
    readKeys.Get(2,6,getIndex++).Set(section);
#ifdef INI_INCLUDE_SUBSECTION
    readKeys.Get(2,6,getIndex++).Set(subsection);
#endif
    readKeys.Get(2,6,getIndex++).Set(key_omniregister);
    readKeys.Get(2,6,getIndex++).Set(value_string);
    readKeys.Get(2,6,getIndex++).Set(default_emptystring);

    vararginParam.Get(1,1).Set(inifilename);
    vararginParam.Get(1,2).Set(mode);
    vararginParam.Get(1,3).Set(readKeys);

    readSett = mwArray(1,6,mxCELL_CLASS);
    int temp_epiphan_deviceid = setup.epiphan_deviceID;
    int temp_width = setup.width;
    int temp_height = setup.height;
    int temp_bits_per_pixel = setup.bits_per_pixel;
    int temp_bayer = setup.bayer;
    std::string temp_reg_file = setup.omnivision_reg_file.toStdString();
    try
    {
        inifile(1, readSett,vararginParam);
        temp_epiphan_deviceid =	static_cast<int>(readSett.Get(1,1).Get(1,1));
        temp_width =			static_cast<int>(readSett.Get(1,1).Get(1,2));
        temp_height =			static_cast<int>(readSett.Get(1,1).Get(1,3));
        temp_bits_per_pixel =	static_cast<int>(readSett.Get(1,1).Get(1,4));
        temp_bayer =			static_cast<int>(readSett.Get(1,1).Get(1,5));
        temp_reg_file =			readSett.Get(1,1).Get(1,6).ToString();

        // copy the values into the corresponding fields in setup
        setup.epiphan_deviceID =	temp_epiphan_deviceid;
        setup.width =				temp_width;
        setup.height =				temp_height;
        setup.bits_per_pixel =		temp_bits_per_pixel;
        setup.bayer =				temp_bayer;
        setup.omnivision_reg_file =	temp_reg_file.c_str();
    }
    catch (mwException& e)
    {
        cout << "Run Error!" << endl;
        cerr << e.what() << endl;
        e.print_stack_trace();

    }

//    setup.omnivision_reg_file.remove('\n'); // inifile() reads newline and carriage return characters into the string
//    setup.omnivision_reg_file.remove('\r'); // which causes the setup window to be unable to find the file. These characters must be removed.

    if (setup.omnivision_reg_file.compare("[]")==0) // Matlab returns empty strings slightly differently when cells are involved...
    {
        setup.omnivision_reg_file="";
    }

    if ( setup.width > 0 && setup.height > 0) // we must have the width and height > 0 since we will be allocating buffers soon
    {
        result = true;
    }

    return result;
}


//
// This function writes various items used in the setup dialog to imatest.ini
//
void Inifile::WriteSettings(setup_settings& setup, string& iniFilePathName)
{
    mwArray vararginParam = mwArray(1,4,mxCELL_CLASS);
    mwArray writeKeys = mwArray(7,4,mxCELL_CLASS);
    mwArray inifilename(iniFilePathName.c_str());
    mwArray mode("write"),style("plain");
    mwArray section_ovt("ovt"),section_imatest("imatest"),section_op("op_console"),section("");
    mwArray subsection_blank(""), subsection_current("current"),subsection("");
    mwArray key_acquire("acquire"),key_width("width"),key_height("height"),key_bitdepth("bitdepth");
    mwArray key_bayer("bayer_pattern"),key_omniregister("register_files"),key_epiphan_deviceid("deviceID");
    mwArray val_acquire(setup.sourceID), val_width(setup.width), val_height(setup.height), val_bitdepth(setup.bits_per_pixel);
    mwArray val_bayer(setup.bayer), val_omniregister(setup.omnivision_reg_file.toStdString().c_str()), val_epiphan_deviceid(setup.epiphan_deviceID);
    mwSize getIndex = 1;
    // NOTE: the mwArray::Get function has input syntax Get(number of indexes, i1, i2,...in)
    // first read the 'acquire' key from [imatest]
    getIndex = 1;
    writeKeys.Get(2,1,getIndex++).Set(section_imatest);
#ifdef INI_INCLUDE_SUBSECTION
    writeKeys.Get(2,1,getIndex++).Set(subsection_blank);
#endif
    writeKeys.Get(2,1,getIndex++).Set(key_acquire);
    writeKeys.Get(2,1,getIndex++).Set(val_acquire);

    if (setup.sourceID == SOURCE_Omnivision) // Omnivision
    {
        section = section_ovt;
        subsection = subsection_current;
    }
    else
    {
        section = section_op;
        subsection = subsection_blank;
    }

    // to write the Epiphan 'device_ID' key
    getIndex = 1;
    writeKeys.Get(2,2,getIndex++).Set(section);
#ifdef INI_INCLUDE_SUBSECTION
    writeKeys.Get(2,2,getIndex++).Set(subsection);
#endif
    writeKeys.Get(2,2,getIndex++).Set(key_epiphan_deviceid);
    writeKeys.Get(2,2,getIndex++).Set(val_epiphan_deviceid);

    // to write the 'width' key
    getIndex = 1;
    writeKeys.Get(2,3,getIndex++).Set(section);
#ifdef INI_INCLUDE_SUBSECTION
    writeKeys.Get(2,3,getIndex++).Set(subsection);
#endif
    writeKeys.Get(2,3,getIndex++).Set(key_width);
    writeKeys.Get(2,3,getIndex++).Set(val_width);


    // to write the 'height' key
    getIndex = 1;
    writeKeys.Get(2,4,getIndex++).Set(section);
#ifdef INI_INCLUDE_SUBSECTION
    writeKeys.Get(2,4,getIndex++).Set(subsection);
#endif
    writeKeys.Get(2,4,getIndex++).Set(key_height);
    writeKeys.Get(2,4,getIndex++).Set(val_height);


    // to write the 'bitdepth' key
    getIndex = 1;
    writeKeys.Get(2,5,getIndex++).Set(section);
#ifdef INI_INCLUDE_SUBSECTION
    writeKeys.Get(2,5,getIndex++).Set(subsection);
#endif
    writeKeys.Get(2,5,getIndex++).Set(key_bitdepth);
    writeKeys.Get(2,5,getIndex++).Set(val_bitdepth);


    // to write the 'bayer_pattern'
    getIndex = 1;
    writeKeys.Get(2,6,getIndex++).Set(section);
#ifdef INI_INCLUDE_SUBSECTION
    writeKeys.Get(2,6,getIndex++).Set(subsection);
#endif
    writeKeys.Get(2,6,getIndex++).Set(key_bayer);
    writeKeys.Get(2,6,getIndex++).Set(val_bayer);


    // to write the 'register_files'
    getIndex = 1;
    writeKeys.Get(2,7,getIndex++).Set(section);
#ifdef INI_INCLUDE_SUBSECTION
    writeKeys.Get(2,7,getIndex++).Set(subsection);
#endif
    writeKeys.Get(2,7,getIndex++).Set(key_omniregister);
    writeKeys.Get(2,7,getIndex++).Set(val_omniregister);

    vararginParam.Get(2,1,1).Set(inifilename);
    vararginParam.Get(2,1,2).Set(mode);
    vararginParam.Get(2,1,3).Set(writeKeys);
    vararginParam.Get(2,1,4).Set(style);
    try
    {
         inifile(vararginParam);
    }

    catch (mwException& e)
    {
        cout << "Error writing settings to ini file!" << endl;
        cerr << e.what() << endl;
        e.print_stack_trace();
    }
}


#ifdef INI_SEPARATE_PARAMS
void MW_CALL_CONV Inifile::inifile(int nargout, mwArray& varargout, const mwArray& varargin)
{
    mwArray filePath    = varargin.Get(1, 1);
    mwArray mode        = varargin.Get(1, 2);
    mwArray vararginNew = mwArray(1, varargin.NumberOfElements() - 2, mxCELL_CLASS);

    for (mwSize i = 3; i <= varargin.NumberOfElements(); i++)
    {
        vararginNew.Get(1, i - 2).Set(varargin.Get(1, i));
    }

    ::inifile(nargout, varargout, filePath, mode, vararginNew);
}

//void MW_CALL_CONV Inifile::inifile(int nargout, mwArray& varargout)
//{

//}

void MW_CALL_CONV Inifile::inifile(const mwArray& varargin)
{
    mwArray vargoutNone = mwArray();
    inifile(0, vargoutNone, varargin);
}
#endif
