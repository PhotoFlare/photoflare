lessThan(QT_VERSION, 5.0.0): error(This project requires Qt 5.0.0 or later)

QT += core gui widgets

TARGET = PhotoFiltreLX
TEMPLATE = app
CONFIG += c++11

# This path is used inside ui_mainwindow.h in order to find ColorBoxWidget.h
INCLUDEPATH += ./src

# Project settings for Windows. Adjust the paths as needed on your system.
win32 {
    INCLUDEPATH += C:/ImageMagick-6.9.0/Magick++/lib C:/ImageMagick-6.9.0
    LIBS += -LC:/ImageMagick-6.9.0/VisualMagick/lib -lCORE_DB_Magick++_
}

# Project settings for Linux. Adjust the paths as needed on your system.
linux {
    INCLUDEPATH += /usr/include/GraphicsMagick
    LIBS += -L/usr/lib -lGraphicsMagick++
}

# Project settings for Mac OS. Adjust the paths as needed on your system.
macx {
  INCLUDEPATH += /usr/local/graphicsmagick/include/GraphicsMagick
  LIBS += -L$$PWD/../../../../usr/local/graphicsmagick-1.3.20/lib/ -lGraphicsMagick++
  LIBS += -L$$PWD/../../../../usr/local/graphicsmagick-1.3.20/lib/ -lGraphicsMagick
  LIBS += -L$$PWD/../../../../usr/local/graphicsmagick-1.3.20/lib/ -lGraphicsMagickWand
  INCLUDEPATH += $$PWD/../../../../usr/local/graphicsmagick-1.3.20/include
  DEPENDPATH += $$PWD/../../../../usr/local/graphicsmagick-1.3.20/include
  PRE_TARGETDEPS += $$PWD/../../../../usr/local/graphicsmagick-1.3.20/lib/libGraphicsMagick++.a
  PRE_TARGETDEPS += $$PWD/../../../../usr/local/graphicsmagick-1.3.20/lib/libGraphicsMagick.a
  PRE_TARGETDEPS += $$PWD/../../../../usr/local/graphicsmagick-1.3.20/lib/libGraphicsMagickWand.a
  LIBS += -lbz2 -lxml2 -lz -lm
}

SOURCES += src/main.cpp \
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
    src/tools/ColourPickerTool.cpp \
    src/tools/PaintBucketTool.cpp \
    src/tools/PointerTool.cpp \
    src/tools/TextTool.cpp

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
    src/tools/ColourPickerTool.h \
    src/tools/PaintBucketTool.h \
    src/tools/PointerTool.h \
    src/tools/TextTool.h

FORMS += src/mainwindow.ui \
    src/aboutdialog.ui \
    src/textdialog.ui \
    src/prefsdialog.ui \
    src/NewDialog.ui \
    src/PaintBrushSettingsWidget.ui \
    src/ColorBoxWidget.ui

RESOURCES += \
    Icons.qrc
