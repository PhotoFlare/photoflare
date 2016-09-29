lessThan(QT_VERSION, 5.0.0): error(This project requires Qt 5.0.0 or later)

QT += core gui widgets printsupport

TARGET = PhotoFiltreLX
TEMPLATE = app
CONFIG += c++11

# This path is used inside ui_mainwindow.h in order to find ColorBoxWidget.h
INCLUDEPATH += ./src

# Project settings for Windows. Adjust the paths as needed on your system.
win32 {
    INCLUDEPATH +=  $$PWD/external/GraphicsMagick-1.3.21/Magick++/lib
    LIBS += -L$$PWD/external/GraphicsMagick-1.3.21/VisualMagick/lib -lCORE_RL_Magick++_
    RC_ICONS += pixmaps/logo.ico
}

# Project settings for Linux. Adjust the paths as needed on your system.
linux {
    INCLUDEPATH += /usr/include/GraphicsMagick
    LIBS += -L/usr/lib -lGraphicsMagick++
}

# Project settings for Mac OS. Adjust the paths as needed on your system.
macx {
  INCLUDEPATH += /usr/local/Cellar/graphicsmagick/1.3.24/include/GraphicsMagick
  LIBS += -L$$PWD/../../../../usr/local/Cellar/graphicsmagick/1.3.24/lib/ -lGraphicsMagick++
  LIBS += -L$$PWD/../../../../usr/local/Cellar/graphicsmagick/1.3.24/lib/ -lGraphicsMagick
  LIBS += -L$$PWD/../../../../usr/local/Cellar/graphicsmagick/1.3.24/lib/ -lGraphicsMagickWand
  INCLUDEPATH += $$PWD/../../../../usr/local/Cellar/graphicsmagick/1.3.24/include/GraphicsMagick
  DEPENDPATH += $$PWD/../../../../usr/local/Cellar/graphicsmagick/1.3.24/include/GraphicsMagick
  PRE_TARGETDEPS += $$PWD/../../../../usr/local/Cellar/graphicsmagick/1.3.24/lib/libGraphicsMagick++.la
  PRE_TARGETDEPS += $$PWD/../../../../usr/local/Cellar/graphicsmagick/1.3.24/lib/libGraphicsMagick.la
  PRE_TARGETDEPS += $$PWD/../../../../usr/local/Cellar/graphicsmagick/1.3.24/lib/libGraphicsMagickWand.la
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
    src/tools/ColourPickerTool.cpp \
    src/tools/PaintBucketTool.cpp \
    src/tools/PointerTool.cpp \
    src/tools/TextTool.cpp \
    src/registerdialog.cpp \
    src/tools/PaintBrushAdvTool.cpp \
    src/PaintBrushAdvSettingsWidget.cpp \
    src/tools/SprayCanTool.cpp \
    src/tools/LineTool.cpp \
    src/tools/MagicWandTool.cpp \
    src/SprayCanSettingsWidget.cpp \
    src/LineSettingsWidget.cpp \
    src/ScanManager.cpp \
    src/ScanDevicesDialog.cpp \
    src/QProgressIndicator.cpp \
    src/MagicWandSettingsWidget.cpp \
    src/tools/StampTool.cpp \
    src/StampSettingsWidget.cpp \
    src/tools/BlurTool.cpp \
    src/BlurSettingsWidget.cpp \
    src/imagepropertiesdialog.cpp \
    src/batchdialog.cpp \
    src/imagepositionwidget.cpp \
    src/outerframedialog.cpp \
    src/BatchProcessWorker.cpp \
    src/huedialog.cpp \
    src/gradientdialog.cpp \
    src/brushtypecombobox.cpp \
    src/colorboxwidget.cpp \
    src/compressiondialog.cpp \
    src/batchprogress.cpp \
    src/buydialog.cpp \
    src/transparentdialog.cpp

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
    src/tools/ColourPickerTool.h \
    src/tools/PaintBucketTool.h \
    src/tools/PointerTool.h \
    src/tools/TextTool.h \
    src/tools/MagicWandTool.h \
    src/registerdialog.h \
    src/tools/PaintBrushAdvTool.h \
    src/PaintBrushAdvSettingsWidget.h \
    src/tools/SprayCanTool.h \
    src/tools/LineTool.h \
    src/SprayCanSettingsWidget.h \
    src/LineSettingsWidget.h \
    src/ScanManager.h \
    src/ScanDevicesDialog.h \
    src/QProgressIndicator.h \
    src/MagicWandSettingsWidget.h \
    src/tools/StampTool.h \
    src/StampSettingsWidget.h \
    src/tools/BlurTool.h \
    src/BlurSettingsWidget.h \
    src/imagepropertiesdialog.h \
    src/batchdialog.h \
    src/imagepositionwidget.h \
    src/outerframedialog.h \
    src/BatchProcessWorker.h \
    src/huedialog.h \
    src/gradientdialog.h \
    src/brushtypecombobox.h \
    src/colorboxwidget.h \
    src/compressiondialog.h \
    src/batchprogress.h \
    src/buydialog.h \
    src/transparentdialog.h

FORMS += src/mainwindow.ui \
    src/aboutdialog.ui \
    src/textdialog.ui \
    src/prefsdialog.ui \
    src/NewDialog.ui \
    src/PaintBrushSettingsWidget.ui \
    src/PaintBrushAdvSettingsWidget.ui \
    src/ColorBoxWidget.ui \
    src/registerdialog.ui \
    src/BrushTypeComboBox.ui \
    src/SprayCanSettingsWidget.ui \
    src/LineSettingsWidget.ui \
    src/scandevicesdialog.ui \
    src/MagicWandSettingsWidget.ui \
    src/StampSettingsWidget.ui \
    src/BlurSettingsWidget.ui \
    src/imagepropertiesdialog.ui \
    src/batchdialog.ui \
    src/imagepositionwidget.ui \
    src/outerframedialog.ui \
    src/huedialog.ui \
    src/gradientdialog.ui \
    src/compressiondialog.ui \
    src/batchprogress.ui \
    src/buydialog.ui \
    src/transparentdialog.ui

RESOURCES += \
    Icons.qrc

include (external/qt-solutions/qtsingleapplication/qtsingleapplication.pri)

INCLUDEPATH += external/qt-solutions/qtsingleapplication

TRANSLATIONS = languages/en.ts\
                languages/fr.ts
