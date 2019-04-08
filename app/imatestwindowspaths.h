#ifndef IMATESTWINDOWSPATHS_H
#define IMATESTWINDOWSPATHS_H

#ifdef _WIN32
#include "imatestdefines.h"

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

#endif
#endif // IMATESTWINDOWSPATHS_H
