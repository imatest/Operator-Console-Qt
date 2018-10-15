#-------------------------------------------------
#
# Project created by QtCreator 2018-09-08T10:46:22
#
#-------------------------------------------------

QT       += core gui widgets multimedia multimediawidgets

TARGET = OperatorConsole
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

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
    textdialog.cpp \
    setupdialog.cpp \
    waitdialog.cpp \
    operatorconsole.cpp \
    cameraacquisition.cpp \
    passworddialog.cpp \
    passfaildialog.cpp

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
    cameraacquisition.h \
    passworddialog.h \
    passfaildialog.h

FORMS += \
        operatorconsoledialog.ui \
    textdialog.ui \
    setupdialog.ui \
    waitdialog.ui \
    passworddialog.ui \
    passfaildialog.ui

INCLUDEPATH += 'C:/Program Files/Imatest/v5.1/IT/libs/library/cpp'
INCLUDEPATH += 'C:/Program Files/MATLAB/MATLAB Runtime/v93/extern/include'

DEFINES += __cplusplus

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += 'C:/Program Files/Imatest/v5.1/IT/libs/acquisition/cpp'
DEPENDPATH += 'C:/Program Files/Imatest/v5.1/IT/libs/acquisition/cpp'

DEFINES += IMATEST_5_1

INCLUDEPATH += 'C:/Program Files/MATLAB/MATLAB Runtime/v93/extern/lib/win64/microsoft'
DEPENDPATH  += 'C:/Program Files/MATLAB/MATLAB Runtime/v93/extern/lib/win64/microsoft'

DISTFILES += \
    Data/imatest_logo.png

win32: LIBS += -L'C:/Program Files/Imatest/v5.1/IT/libs/library/cpp/' -limatest_library

INCLUDEPATH += 'C:/Program Files/Imatest/v5.1/IT/libs/library/cpp'
DEPENDPATH  += 'C:/Program Files/Imatest/v5.1/IT/libs/library/cpp'

win32: LIBS += -L'C:/Program Files/Imatest/v5.1/IT/libs/acquisition/cpp/' -limatest_acquisition

INCLUDEPATH += 'C:/Program Files/Imatest/v5.1/IT/libs/acquisition/cpp'
DEPENDPATH  += 'C:/Program Files/Imatest/v5.1/IT/libs/acquisition/cpp'

win32: LIBS += -L'C:/Program Files/MATLAB/MATLAB Runtime/v93/extern/lib/win64/microsoft/' -lmclmcrrt

INCLUDEPATH += 'C:/Program Files/MATLAB/MATLAB Runtime/v93/extern/lib/win64/microsoft'
DEPENDPATH  += 'C:/Program Files/MATLAB/MATLAB Runtime/v93/extern/lib/win64/microsoft'
