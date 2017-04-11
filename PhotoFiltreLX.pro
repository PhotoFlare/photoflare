lessThan(QT_VERSION, 5.6.0): error(This project requires Qt 5.6.0 or later)

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
    src/dialogs/aboutdialog.cpp \
    src/dialogs/NewDialog.cpp \
    src/dialogs/textdialog.cpp \
    src/dialogs/prefsdialog.cpp \
    src/dialogs/batchdialog.cpp \
    src/dialogs/gradientdialog.cpp \
    src/PaintWidget.cpp \
    src/Settings.cpp \
    src/FilterManager.cpp \
    src/ToolManager.cpp \
    src/tools/Tool.cpp \
    src/tools/PaintBrushTool.cpp \
    src/tools/ColourPickerTool.cpp \
    src/tools/PaintBucketTool.cpp \
    src/tools/PointerTool.cpp \
    src/tools/TextTool.cpp \
    src/tools/PaintBrushAdvTool.cpp \
    src/tools/SprayCanTool.cpp \
    src/tools/LineTool.cpp \
    src/tools/MagicWandTool.cpp \
    src/tools/StampTool.cpp \
    src/tools/BlurTool.cpp \
    src/toolSettings/pointersettingswidget.cpp \
    src/toolSettings/PaintBrushSettingsWidget.cpp \
    src/toolSettings/PaintBrushAdvSettingsWidget.cpp \
    src/toolSettings/SprayCanSettingsWidget.cpp \
    src/toolSettings/LineSettingsWidget.cpp \
    src/toolSettings/MagicWandSettingsWidget.cpp \
    src/toolSettings/StampSettingsWidget.cpp \
    src/toolSettings/BlurSettingsWidget.cpp \
    src/ScanManager.cpp \
    src/ScanDevicesDialog.cpp \
    src/QProgressIndicator.cpp \
    src/imagepropertiesdialog.cpp \
    src/imagepositionwidget.cpp \
    src/outerframedialog.cpp \
    src/BatchProcessWorker.cpp \
    src/huedialog.cpp \
    src/brushtypecombobox.cpp \
    src/colorboxwidget.cpp \
    src/compressiondialog.cpp \
    src/batchprogress.cpp \
    src/transparentdialog.cpp \
    src/filterworker.cpp

HEADERS += src/mainwindow.h \
    src/dialogs/aboutdialog.h \
    src/dialogs/NewDialog.h \
    src/dialogs/textdialog.h \
    src/dialogs/prefsdialog.h \
    src/dialogs/batchdialog.h \
    src/dialogs/gradientdialog.h \
    src/PaintWidget.h \
    src/Settings.h \
    src/FilterManager.h \
    src/ToolManager.h \
    src/tools/Tool.h \
    src/tools/PaintBrushTool.h \
    src/tools/ColourPickerTool.h \
    src/tools/PaintBucketTool.h \
    src/tools/PointerTool.h \
    src/tools/TextTool.h \
    src/tools/MagicWandTool.h \
    src/tools/PaintBrushAdvTool.h \
    src/tools/SprayCanTool.h \
    src/tools/LineTool.h \
    src/tools/StampTool.h \
    src/tools/BlurTool.h \
    src/ScanManager.h \
    src/ScanDevicesDialog.h \
    src/QProgressIndicator.h \
    src/toolSettings/pointersettingswidget.h \
    src/toolSettings/PaintBrushSettingsWidget.h \
    src/toolSettings/PaintBrushAdvSettingsWidget.h \
    src/toolSettings/SprayCanSettingsWidget.h \
    src/toolSettings/LineSettingsWidget.h \
    src/toolSettings/MagicWandSettingsWidget.h \
    src/toolSettings/StampSettingsWidget.h \
    src/toolSettings/BlurSettingsWidget.h \
    src/imagepropertiesdialog.h \
    src/imagepositionwidget.h \
    src/outerframedialog.h \
    src/BatchProcessWorker.h \
    src/huedialog.h \
    src/brushtypecombobox.h \
    src/colorboxwidget.h \
    src/compressiondialog.h \
    src/batchprogress.h \
    src/transparentdialog.h \
    src/filterworker.h

FORMS += src/mainwindow.ui \
    src/dialogs/aboutdialog.ui \
    src/dialogs/NewDialog.ui \
    src/dialogs/textdialog.ui \
    src/dialogs/prefsdialog.ui \
    src/dialogs/batchdialog.ui \
    src/dialogs/gradientdialog.ui \
    src/ColorBoxWidget.ui \
    src/BrushTypeComboBox.ui \
    src/scandevicesdialog.ui \
    src/imagepropertiesdialog.ui \
    src/imagepositionwidget.ui \
    src/outerframedialog.ui \
    src/huedialog.ui \
    src/compressiondialog.ui \
    src/batchprogress.ui \
    src/transparentdialog.ui \
    src/toolSettings/pointersettingswidget.ui \
    src/toolSettings/PaintBrushSettingsWidget.ui \
    src/toolSettings/PaintBrushAdvSettingsWidget.ui \
    src/toolSettings/SprayCanSettingsWidget.ui \
    src/toolSettings/LineSettingsWidget.ui \
    src/toolSettings/MagicWandSettingsWidget.ui \
    src/toolSettings/StampSettingsWidget.ui \
    src/toolSettings/BlurSettingsWidget.ui 

RESOURCES += \
    Icons.qrc

include (external/qt-solutions/qtsingleapplication/qtsingleapplication.pri)

INCLUDEPATH += external/qt-solutions/qtsingleapplication

TRANSLATIONS = languages/en.ts\
                languages/fr.ts\
                languages/nl.ts
