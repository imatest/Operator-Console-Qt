#ifndef IMATESTWINDOWSPATHS_H
#define IMATESTWINDOWSPATHS_H

#ifdef _WIN32
#include "imatestdefines.h"

//
// IMAGE_NAME is the name of the static file to be displayed in the application window (until live acquisition is implemented)
//
#define LOGO_NAME			"Data\\imatest_logo.png"	// imatest logo to be displayed in dialog
#define IMAGE_NAME			"Data\\sfrplus.raw.jpg"			// image file to be used for acquisition from a file (mainly used during development)

//
// These values are put into m_config, and get passed to the sfrplus and blemish functions
//
#ifdef INI_V2_FORMAT
#define INI_FILENAME		"Data\\imatest-v2.ini"
#else
#define INI_FILENAME		"Data\\imatest.ini"
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
    #define PROGRAMPATH			"C:\\Program Files\\Imatest\\v4.5\\IT\\bin"


#elif defined IMATEST_5_0
    #define PROGRAMPATH			"C:\\Program Files\\Imatest\\v5.0\\IT\\bin"


#elif defined IMATEST_5_1
    #define PROGRAMPATH			"C:\\Program Files\\Imatest\\v5.1\\IT\\bin"


#elif defined IMATEST_5_2
    #define PROGRAMPATH			"C:\\Program Files\\Imatest\\v5.2\\IT\\bin"

#elif defined IMATEST_2020_1
    #define PROGRAMPATH			"C:\\Program Files\\Imatest\\v2020.1\\IT\\bin"


#elif defined IMATEST_2020_2

    #define PROGRAMPATH			"C:\\Program Files\\Imatest\\v2020.2\\IT\\bin"


#elif defined IMATEST_2021_1
    #define PROGRAMPATH			"C:\\Program Files\\Imatest\\v2021.1\\IT\\bin"

#else
#define PROGRAMPATH			"C:\\Program Files (x86)\\Imatest\\v4.1\\IT\\bin"
#endif

#endif
#endif // IMATESTWINDOWSPATHS_H
