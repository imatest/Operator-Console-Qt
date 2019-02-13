#ifndef IMATESTDEFINES_H
#define IMATESTDEFINES_H

///
/// Define different keys for differing Imatest interfaces based on version
///
/// Set the Imatest version by adding the appropriate IMATEST version
/// to the DEFINES variable in OperatorConsole.pro.  For example:
/// DEFINES += IMATEST_5_1
///

#if defined IMATEST_4_2

#define INI_INCLUDE_SUBSECTION
#define INI_SEPARATE_PARAMS
#define INI_V2_FORMAT
#define LOWER_CASE_INI_SECTIONS

#elif defined IMATEST_4_3

#define INI_SEPARATE_PARAMS
#define INI_V2_FORMAT
#define LOWER_CASE_INI_SECTIONS

#elif defined IMATEST_4_4

#define INI_SEPARATE_PARAMS
#define INI_V2_FORMAT
#define LOWER_CASE_INI_SECTIONS

#elif defined IMATEST_4_5

#define INI_SEPARATE_PARAMS
#define INI_V2_FORMAT
#define LOWER_CASE_INI_SECTIONS

#elif defined IMATEST_5_0

#define INI_SEPARATE_PARAMS
#define INI_V2_FORMAT
#define LOWER_CASE_INI_SECTIONS

#elif defined IMATEST_5_1

#define INI_SEPARATE_PARAMS
#define INI_V2_FORMAT
#define LOWER_CASE_INI_SECTIONS

#else

// All versions below 4.2
#define INI_INCLUDE_SUBSECTION

#endif

#ifdef DEMO_VERSION
#define NO_AUTHORIZATION
#endif

//
// These values are put into m_config, and get passed to the sfrplus and blemish functions
//
#ifdef INI_V2_FORMAT
#define INI_FILENAME		"Data\\imatest-v2.ini"
#else
#define INI_FILENAME		"Data\\imatest.ini"
#endif

#ifndef IMA_CALL_CONV
#ifdef _WIN32
#define IMA_CALL_CONV __cdecl
#else
#define IMA_CALL_CONV
#endif
#endif

#if defined IMATEST_4_2
    #ifdef _M_X64
    #define PROGRAMPATH			"C:\\Program Files\\Imatest\\v4.2\\IT\\bin"
    #else
    #define PROGRAMPATH			"C:\\Program Files (x86)\\Imatest\\v4.2\\IT\\bin"
    #endif


#elif defined IMATEST_4_3
    #ifdef _M_X64
    #define PROGRAMPATH			"C:\\Program Files\\Imatest\\v4.3\\IT\\bin"
    #else
    #define PROGRAMPATH			"C:\\Program Files (x86)\\Imatest\\v4.3\\IT\\bin"
    #endif


#elif defined IMATEST_4_4
    #ifdef _M_X64
    #define PROGRAMPATH			"C:\\Program Files\\Imatest\\v4.4\\IT\\bin"
    #else
    #define PROGRAMPATH			"C:\\Program Files (x86)\\Imatest\\v4.4\\IT\\bin"
    #endif

#elif defined IMATEST_4_5
    #ifdef _M_X64
    #define PROGRAMPATH			"C:\\Program Files\\Imatest\\v4.5\\IT\\bin"
    #else
    #define PROGRAMPATH			"C:\\Program Files (x86)\\Imatest\\v4.5\\IT\\bin"
    #endif

#elif defined IMATEST_5_0
    #ifdef _M_X64
    #define PROGRAMPATH			"C:\\Program Files\\Imatest\\v5.0\\IT\\bin"
    #else
    #define PROGRAMPATH			"C:\\Program Files (x86)\\Imatest\\v5.0\\IT\\bin"
    #endif

#elif defined IMATEST_5_1
    #ifdef _M_X64
    #define PROGRAMPATH			"C:\\Program Files\\Imatest\\v5.1\\IT\\bin"
    #else
    #define PROGRAMPATH			"C:\\Program Files (x86)\\Imatest\\v5.1\\IT\\bin"
    #endif

#else
#define PROGRAMPATH			"C:\\Program Files (x86)\\Imatest\\v4.1\\IT\\bin"
#endif


#endif // IMATESTDEFINES_H
