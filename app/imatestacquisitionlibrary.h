#ifndef IMATESTACQUISITIONLIBRARY_H
#define IMATESTACQUISITIONLIBRARY_H

#include "imatestdefines.h"

IMA_WARNING_PUSH
IMA_DISABLE_UNUSED_PARAM_WARNING

#ifdef _WIN32
#include "imatest_acquisition.h"
#else
#include "libImatest_acquisition.h"
#endif

IMA_WARNING_POP

#endif // IMATESTACQUISITIONLIBRARY_H
