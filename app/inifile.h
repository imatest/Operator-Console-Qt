#ifndef INIFILE_H
#define INIFILE_H
#include "imatestdefines.h"
#ifdef _WIN32
#include "imatest_library.h"
#else
#include "libImatest.h"
#endif
#include "setup_settings.h"
using namespace std;

class Inifile
{
public:
    Inifile(){}

static bool    ReadSettings(setup_settings& setup);                           //!< This function reads in items for the setup dialog from imatest.ini
static void    WriteSettings(setup_settings& setup, string& iniFilePathName); //!< This function writes items for the setup dialog to imatest.ini

#ifdef INI_SEPARATE_PARAMS  // this is defined in imatestdefines.h
static void MW_CALL_CONV   inifile(int nargout, mwArray& varargout, const mwArray& varargin);
static void MW_CALL_CONV   inifile(const mwArray& varargin);
#endif

};

#endif // INIFILE_H
