QT += testlib
QT += gui widgets
CONFIG += qt warn_on depend_includepath testcase

TEMPLATE = app

SOURCES +=  tst_operatorconsoletests.cpp \
    ../app/inifile.cpp \
    ../app/acquisitiondeviceinfo.cpp \
    ../app/JSON/internalJSONNode.cpp \
    ../app/JSON/JSON_Base64.cpp \
    ../app/JSON/JSONAllocator.cpp \
    ../app/JSON/JSONChildren.cpp \
    ../app/JSON/JSONDebug.cpp \
    ../app/JSON/JSONIterators.cpp \
    ../app/JSON/JSONMemory.cpp \
    ../app/JSON/JSONNode.cpp \
    ../app/JSON/JSONNode_Mutex.cpp \
    ../app/JSON/JSONPreparse.cpp \
    ../app/JSON/JSONStream.cpp \
    ../app/JSON/JSONValidator.cpp \
    ../app/JSON/JSONWorker.cpp \
    ../app/JSON/JSONWriter.cpp \
    ../app/JSON/libjson.cpp \
    ../app/jsonhelpers.cpp \
    ../app/imagetest.cpp \
    ../app/sfrplustest.cpp \
    ../app/arbitrarycharttest.cpp \
    ../app/blemishtest.cpp \
    ../app/config.cpp \
    ../app/criticalbuf.cpp \
    ../app/criticalstring.cpp \
    ../app/fileacq.cpp \
    ../app/fileacquisition.cpp \
    ../app/imageacquisition.cpp \
    ../app/imatestlibacq.cpp \
    ../app/inifile.cpp \
    ../app/inisetting.cpp \
    ../app/passfail.cpp \
    ../app/passfailsettings.cpp \
    ../app/sfrregtest.cpp \
    ../app/threadcontrol.cpp \
    ../app/timer.cpp \
    ../app/timestamp.cpp

HEADERS += \
    ../app/inifile.h \
    ../app/acquisitiondeviceinfo.h \
    ../app/JSON/JSONDefs/GNU_C.h \
    ../app/JSON/JSONDefs/Strings_Defs.h \
    ../app/JSON/JSONDefs/Unknown_C.h \
    ../app/JSON/JSONDefs/Visual_C.h \
    ../app/JSON/internalJSONNode.h \
    ../app/JSON/JSON_Base64.h \
    ../app/JSON/JSONAllocator.h \
    ../app/JSON/JSONChildren.h \
    ../app/JSON/JSONDebug.h \
    ../app/JSON/JSONDefs.h \
    ../app/JSON/JSONGlobals.h \
    ../app/JSON/JSONMemory.h \
    ../app/JSON/JSONMemoryPool.h \
    ../app/JSON/JSONNode.h \
    ../app/JSON/JSONPreparse.h \
    ../app/JSON/JSONSharedString.h \
    ../app/JSON/JSONSingleton.h \
    ../app/JSON/JSONStream.h \
    ../app/JSON/JSONValidator.h \
    ../app/JSON/JSONWorker.h \
    ../app/JSON/NumberToString.h \
    ../app/jsonhelpers.h \
    ../app/JSONOptions.h \
    ../app/libjson.h \
    ../app/imagetest.h \
    ../app/sfrplustest.h \
    ../app/arbitrarycharttest.h \
    ../app/blemishtest.h \
    ../app/config.h \
    ../app/criticalbuf.h \
    ../app/criticalstring.h \
    ../app/fileacq.h \
    ../app/fileacquisition.h \
    ../app/imageacquisition.h \
    ../app/imatestacquisitionlibrary.h \
    ../app/imatestdefines.h \
    ../app/imatestlibacq.h \
    ../app/imatestlinuxpaths.h \
    ../app/imatestmainlibrary.h \
    ../app/imatestpaths.h \
    ../app/imatestsourceids.h \
    ../app/imatestwindowspaths.h \
    ../app/inifile.h \
    ../app/inisetting.h \
    ../app/passfail.h \
    ../app/passfailsettings.h \
    ../app/qso.h \
    ../app/setup_settings.h \
    ../app/sfrregtest.h \
    ../app/threadcontrol.h \
    ../app/timer.h \
    ../app/timestamp.h


# The following are some variable specific to the Operator Console's build

IT_VERSION = 5.1
MCR_VERSION = 93

win32 {
IT_INSTALL_ROOT = C:/Program Files/Imatest/v$$IT_VERSION/IT
MCR_INSTALL_ROOT = C:/Program Files/MATLAB/MATLAB Runtime/v$$MCR_VERSION
ARCH = win64
ARCH_PATH = $$ARCH/microsoft
}
unix {
IMATEST_INSTALL_ROOT = /usr/local/Imatest
# This retrieves the most recent version
FOLDERS = $$system(ls $$IMATEST_INSTALL_ROOT | grep v$$IT_VERSION | sort --version-sort)
IT_INSTALL_ROOT = $$IMATEST_INSTALL_ROOT/$$last(FOLDERS)/IT
MCR_INSTALL_ROOT = /usr/local/MATLAB/MATLAB_Runtime/v$$MCR_VERSION
ARCH = glnxa64
ARCH_PATH = runtime/$$ARCH
}

INCLUDEPATH += $$PWD
INCLUDEPATH += '$$IT_INSTALL_ROOT/libs/library/cpp'
INCLUDEPATH += '$$MCR_INSTALL_ROOT/extern/include'


INCLUDEPATH += '$$IT_INSTALL_ROOT/libs/acquisition/cpp'
DEPENDPATH += '$$IT_INSTALL_ROOT/libs/acquisition/cpp'

DEFINES += IMATEST_5_1

INCLUDEPATH += '$$MCR_INSTALL_ROOT/extern/lib/$$ARCH_PATH'
DEPENDPATH  += '$$MCR_INSTALL_ROOT/extern/lib/$$ARCH_PATH'

win32:LIBS += -L'$$IT_INSTALL_ROOT/libs/library/cpp/' -limatest_library
unix:LIBS += -L'$$IT_INSTALL_ROOT/libs/library/cpp/' -lImatest
unix:QMAKE_RPATHDIR += $$IT_INSTALL_ROOT/libs/library/cpp

INCLUDEPATH += '$$IT_INSTALL_ROOT/libs/library/cpp'
DEPENDPATH  += '$$IT_INSTALL_ROOT/libs/library/cpp'

win32:LIBS += -L'$$IT_INSTALL_ROOT/libs/acquisition/cpp/' -limatest_acquisition
unix:LIBS += -L'$$IT_INSTALL_ROOT/libs/acquisition/cpp/' -lImatest_acquisition
unix:QMAKE_RPATHDIR += $$IT_INSTALL_ROOT/libs/acquisition/cpp

INCLUDEPATH += '$$IT_INSTALL_ROOT/libs/acquisition/cpp'
DEPENDPATH  += '$$IT_INSTALL_ROOT/libs/acquisition/cpp'

win32:LIBS += -L'$$MCR_INSTALL_ROOT/extern/lib/$$ARCH_PATH/' -lmclmcrrt
unix:LIBS += -L'$$MCR_INSTALL_ROOT/$$ARCH_PATH/' -lmwmclmcrrt
unix:QMAKE_RPATHDIR += $$MCR_INSTALL_ROOT/$$ARCH_PATH/
unix:QMAKE_RPATHDIR += $$MCR_INSTALL_ROOT/bin/$$ARCH


INCLUDEPATH += '$$MCR_INSTALL_ROOT/extern/lib/$$ARCH_PATH'
win32:DEPENDPATH  += '$$MCR_INSTALL_ROOT/lib/glnxa64/lib/$$ARCH_PATH'
unix:DEPENDPATH  += '$$MCR_INSTALL_ROOT/sys/opengl/lib/$$ARCH'

# Copy the dependent libraries into the output directory
LIB_EXT =
win32: LIB_EXT = '.DLL'
unix: LIB_EXT = '.so'
macx: LIB_EXT = '.dylib'

CONFIG(debug, debug|release){
OUT_FOLDER = 'debug'
} else {
OUT_FOLDER = 'release'
}

win32{
DEPENDENT_LIBS = '$$IT_INSTALL_ROOT/bin/ShaferFilechck$$LIB_EXT' '$$IT_INSTALL_ROOT/libs/library/cpp/imatest_library$$LIB_EXT' '$$IT_INSTALL_ROOT/libs/acquisition/cpp/imatest_acquisition$$LIB_EXT'
BUILD_PATH = $$system_quote($$system_path($$OUT_PWD/$$OUT_FOLDER))
DATA_FOLDER_DEST = $$system_quote($$system_path($$BUILD_PATH/Data))
}
unix{
DEPENDENT_LIBS = '$$IT_INSTALL_ROOT/bin/ShaferFilechck$$LIB_EXT' '$$IT_INSTALL_ROOT/libs/library/cpp/libImatest$$LIB_EXT' '$$IT_INSTALL_ROOT/libs/acquisition/cpp/libImatest_acquisition$$LIB_EXT'
BUILD_PATH = $$system_quote($$system_path($$OUT_PWD))
DATA_FOLDER_DEST = $$system_quote($$system_path($$BUILD_PATH))
}


for (file, DEPENDENT_LIBS) {
QMAKE_POST_LINK += $${QMAKE_COPY} $$system_quote($$system_path($${file})) $$BUILD_PATH $$escape_expand(\\n\\t)
}

# Copy the /Data folder into the output directory
QMAKE_POST_LINK += $${QMAKE_COPY_DIR} $$system_quote($$system_path($$PWD/Data)) $$DATA_FOLDER_DEST $$escape_expand(\\n\\t)

DISTFILES += \
    ../app/DejaVuSans.ttf \
    ../app/fonts.qrc.autosave

