#ifndef IMATESTACQUISITIONLIBRARY_H
#define IMATESTACQUISITIONLIBRARY_H

#include "imatestdefines.h"

IMA_WARNING_PUSH
IMA_DISABLE_UNUSED_PARAM_WARNING


#if defined(_WIN32) && defined(SEPARATE_ACQUISITION_LIBRARY)
#include "imatest_acquisition.h"
#elif defined(_WIN32) && !defined(SEPARATE_ACQUISITION_LIBRARY)
#include "imatest_library.h"
#elif !defined(_WIN32) && defined(SEPARATE_ACQUISITION_LIBRARY)
#include "libImatest_acquisition.h"
#elif !defined(_WIN32) && !defined(SEPARATE_ACQUISITION_LIBRARY)
#include "libImatest.h"
#endif


IMA_WARNING_POP

#endif // IMATESTACQUISITIONLIBRARY_H
