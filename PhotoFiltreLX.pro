lessThan(QT_VERSION, 5.0.0): error(This project requires Qt 5.0.0 or later)

QT += core gui widgets

TARGET = PhotoFiltreLX
TEMPLATE = app
CONFIG += c++11

# Adjust this for the path to the header files and library for GraphicsMagick on your system.
win32 {
    INCLUDEPATH += C:/Program Files (x86)/ImageSoftware/Magick++/lib/Magick.h
    LIBS += -L/usr/lib -lGraphicsMagick++
}
linux {
    INCLUDEPATH += /usr/include/GraphicsMagick
    LIBS += -L/usr/lib -lGraphicsMagick++
}
macx {
    INCLUDEPATH += /usr/include/GraphicsMagick
    LIBS += -L/usr/lib -lGraphicsMagick++
}

SOURCES += src/main.cpp\
    src/mainwindow.cpp \
    src/aboutdialog.cpp \
    src/textdialog.cpp \
    src/prefsdialog.cpp \
    src/PaintWidget.cpp \
    src/tools/Tool.cpp \
    src/tools/PaintBrushTool.cpp \
    src/NewDialog.cpp \
    src/Settings.cpp \
    src/FilterManager.cpp \
    src/ToolManager.cpp \
    src/PaintBrushSettingsWidget.cpp \
    src/ColorBoxWidget.cpp \
    src/tools/ColourPickerTool.cpp

HEADERS += src/mainwindow.h \
    src/aboutdialog.h \
    src/textdialog.h \
    src/prefsdialog.h \
    src/PaintWidget.h \
    src/tools/Tool.h \
    src/tools/PaintBrushTool.h \
    src/NewDialog.h \
    src/Settings.h \
    src/FilterManager.h \
    src/ToolManager.h \
    src/PaintBrushSettingsWidget.h \
    src/ColorBoxWidget.h \
    src/tools/ColourPickerTool.h

FORMS += src/mainwindow.ui \
    src/aboutdialog.ui \
    src/textdialog.ui \
    src/prefsdialog.ui \
    src/NewDialog.ui \
    src/PaintBrushSettingsWidget.ui \
    src/ColorBoxWidget.ui

RESOURCES += \
    Icons.qrc
