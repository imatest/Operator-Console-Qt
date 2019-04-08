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



#ifndef IMA_CALL_CONV
#ifdef _WIN32
#define IMA_CALL_CONV __cdecl
#else
#define IMA_CALL_CONV
#endif
#endif

// Define macros for suppressing warnings
#ifndef IMA_WARNING_PUSH
#ifdef _WIN32
#define IMA_WARNING_PUSH __pragma(warning( push ))
#else
#define IMA_WARNING_PUSH _Pragma("GCC diagnostic push")
#endif
#endif

#ifndef IMA_WARNING_POP
#ifdef _WIN32
#define IMA_WARNING_POP __pragma(warning( pop ))
#else
#define IMA_WARNING_POP _Pragma("GCC diagnostic pop")
#endif
#endif

#ifndef IMA_DISABLE_UNUSED_PARAM_WARNING
#ifdef _WIN32
#define IMA_DISABLE_UNUSED_PARAM_WARNING __pragma(warning( disable:4100 ))
#else
#define IMA_DISABLE_UNUSED_PARAM_WARNING _Pragma("GCC diagnostic ignored \"-Wunused-parameter\"")
#endif
#endif
#endif // IMATESTDEFINES_H
