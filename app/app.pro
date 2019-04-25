#-------------------------------------------------
#
# Project created by QtCreator 2018-09-08T10:46:22
#
#-------------------------------------------------

QT       += core gui widgets #multimedia multimediawidgets

TARGET = OperatorConsole
TEMPLATE = app

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
# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11 console

INCLUDEPATH += $$PWD
INCLUDEPATH += $$PWD/JSON

SOURCES += \
        main.cpp \
        operatorconsoledialog.cpp \
    imagetest.cpp \
    config.cpp \
    timestamp.cpp \
    jsonhelpers.cpp \
    JSON/internalJSONNode.cpp \
    JSON/JSON_Base64.cpp \
    JSON/JSONAllocator.cpp \
    JSON/JSONChildren.cpp \
    JSON/JSONDebug.cpp \
    JSON/JSONIterators.cpp \
    JSON/JSONMemory.cpp \
    JSON/JSONNode.cpp \
    JSON/JSONNode_Mutex.cpp \
    JSON/JSONPreparse.cpp \
    JSON/JSONStream.cpp \
    JSON/JSONValidator.cpp \
    JSON/JSONWorker.cpp \
    JSON/JSONWriter.cpp \
    JSON/libjson.cpp \
    timer.cpp \
    criticalstring.cpp \
    arbitrarycharttest.cpp \
    blemishtest.cpp \
    sfrplustest.cpp \
    criticalbuf.cpp \
    threadcontrol.cpp \
    fileacq.cpp \
    imageacquisition.cpp \
    fileacquisition.cpp \
    passfailsettings.cpp \
    imatestlibacq.cpp \
    passfail.cpp \
    operatorconsole.cpp \
  #  cameraacquisition.cpp \
    inifile.cpp \
    acquisitiondeviceinfo.cpp \
    inisetting.cpp \
#    inistringsetting.cpp \
    sfrregtest.cpp \
    setupdialog.cpp \
    textdialog.cpp \
    passfaildialog.cpp \
    passworddialog.cpp \
    waitdialog.cpp
  #  criticalmwarray.cpp


HEADERS += \
    operatorconsoledialog.h \
    imagetest.h \
    config.h \
    timestamp.h \
    jsonhelpers.h \
    JSONOptions.h \
    libjson.h \
    JSON/JSONDefs.h \
    JSON/internalJSONNode.h \
    JSON/JSON_Base64.h \
    JSON/JSONAllocator.h \
    JSON/JSONChildren.h \
    JSON/JSONDebug.h \
    JSON/JSONDefs.h \
    JSON/JSONGlobals.h \
    JSON/JSONMemory.h \
    JSON/JSONMemoryPool.h \
    JSON/JSONNode.h \
    JSON/JSONPreparse.h \
    JSON/JSONSharedString.h \
    JSON/JSONSingleton.h \
    JSON/JSONStream.h \
    JSON/JSONValidator.h \
    JSON/JSONWorker.h \
    JSON/NumberToString.h \
    config.h \
    timer.h \
    criticalstring.h \
    arbitrarycharttest.h \
    blemishtest.h \
    sfrplustest.h \
    qso.h \
    criticalbuf.h \
    imatestsourceids.h \
    operatorconsole.h \
    setup_settings.h \
    threadcontrol.h \
    fileacq.h \
    imageacquisition.h \
    fileacquisition.h \
    passfailsettings.h \
    imatestdefines.h \
    imatestlibacq.h \
    passfail.h \
    textdialog.h \
    setupdialog.h \
    waitdialog.h \
  #  cameraacquisition.h \
    passworddialog.h \
    passfaildialog.h \
    inifile.h \
    acquisitiondeviceinfo.h \
    inisetting.h \
#    inistringsetting.h \
    sfrregtest.h \
    imatestmainlibrary.h \
    imatestacquisitionlibrary.h \
    imatestpaths.h \
    imatestwindowspaths.h \
    imatestlinuxpaths.h
  #  criticalmwarray.h

FORMS += \
        operatorconsoledialog.ui \
    textdialog.ui \
    setupdialog.ui \
    waitdialog.ui \
    passworddialog.ui \
    passfaildialog.ui

unix {
RESOURCES += \
    resources.qrc
}

INCLUDEPATH += $$PWD
INCLUDEPATH += '$$IT_INSTALL_ROOT/libs/library/cpp'
INCLUDEPATH += '$$MCR_INSTALL_ROOT/extern/include'

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# QT static lib includes
unix{
CONFIG += STATIC

LOCAL_BUILD_FOLDER = /home/ty/qt5/build
LIBS += -L$$LOCAL_BUILD_FOLDER/plugins/platforms -lqxcb
LIBS += -L$$LOCAL_BUILD_FOLDER/plugins/xcblintegrations -lqxcb-glx-integration
LIBS += -L$$LOCAL_BUILD_FOLDER/lib -lQt5Core -lQt5Widgets -lQt5Gui -lQt5OpenGL -lQt5OpenGLExtensions -lqtpcre -lqtharfbuzz -lQt5XcbQpa -lQt5X11Extras -lQt5DBus -lqtfreetype
}


INCLUDEPATH += '$$IT_INSTALL_ROOT/libs/acquisition/cpp'
DEPENDPATH += '$$IT_INSTALL_ROOT/libs/acquisition/cpp'

DEFINES += IMATEST_5_1

INCLUDEPATH += '$$MCR_INSTALL_ROOT/extern/lib/$$ARCH_PATH'
DEPENDPATH  += '$$MCR_INSTALL_ROOT/extern/lib/$$ARCH_PATH'

DISTFILES += \
    Data/imatest_logo.png

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


