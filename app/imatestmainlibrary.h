#ifndef IMATESTMAINLIBRARY_H
#define IMATESTMAINLIBRARY_H
#include "imatestdefines.h"

IMA_WARNING_PUSH
IMA_DISABLE_UNUSED_PARAM_WARNING

#ifdef _WIN32
#include "imatest_library.h"
#else
#include "libImatest.h"
#endif

IMA_WARNING_POP

#endif // IMATESTMAINLIBRARY_H
