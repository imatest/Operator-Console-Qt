#ifndef IMATESTLINUXPATHS_H
#define IMATESTLINUXPATHS_H
#ifndef _WIN32
#include "imatestdefines.h"

//
// IMAGE_NAME is the name of the static file to be displayed in the application window (until live acquisition is implemented)
//
#define LOGO_NAME			"Data/imatest_logo.png"	// imatest logo to be displayed in dialog
#define IMAGE_NAME			"Data/sfrplus.raw.jpg"			// image file to be used for acquisition from a file (mainly used during development)

//
// These values are put into m_config, and get passed to the sfrplus and blemish functions
//
#ifdef INI_V2_FORMAT
#define INI_FILENAME		"Data/imatest-v2.ini"
#else
#define INI_FILENAME		"Data/imatest.ini"
#endif


#if defined IMATEST_4_3
#define PROGRAMPATH			"/usr/local/Imatest/v4.3.9/IT/bin"

#elif defined IMATEST_4_4
#define PROGRAMPATH			"/usr/local/Imatest/v4.4.21/IT/bin"

#elif defined IMATEST_4_5
#define PROGRAMPATH			"/usr/local/Imatest/v4.5.34/IT/bin"

#elif defined IMATEST_5_0
#define PROGRAMPATH			"/usr/local/Imatest/v5.0.25/IT/bin"

#elif defined IMATEST_5_1
#define PROGRAMPATH			"/usr/local/Imatest/v5.1.26/IT/bin"

#else
#define PROGRAMPATH			"/usr/local/Imatest/v5.1.26/IT/bin"
#endif

#endif
#endif // IMATESTLINUXPATHS_H
