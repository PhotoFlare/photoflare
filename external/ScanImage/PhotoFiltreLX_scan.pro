unix {
    TEMPLATE = app
    VERSION = 1.0
    DEFINES += UNIX \
        kTWAIN_DS_DIR=\"/usr/local/lib/twain\"
    LIBS += -ldl
}
TARGET = PhotoFlare_scan

win32 {
    LIBS += -luser32
    QMAKE_LFLAGS += /DYNAMICBASE:NO
}

DEFINES -= UNICODE

INCLUDEPATH = /usr/local/include \
        ../PhotoFlare_scan

SOURCES += CommonTWAIN.cpp \
    DSMInterface.cpp \
    main.cpp \
    TwainApp.cpp \
    TwainApp_ui.cpp \
    TwainString.cpp \
    TwainAppCMD.cpp

HEADERS += Common.h \
    CommonTWAIN.h \
    TwainAppCMD.h \
    DSMInterface.h \
    TwainApp.h \
    TwainApp_ui.h \
    FreeImage.h

CONFIG += warn_off

# this project doesn't need any QT libs.
QMAKE_LIBS_QT =
QMAKE_LIBS_QT_THREAD =
