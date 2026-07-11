lessThan(QT_MAJOR_VERSION, 6) {
    error("This project requires Qt 6.4.0 or later")
}
isEqual(QT_MAJOR_VERSION, 6) : lessThan(QT_MINOR_VERSION, 4) {
    error("This project requires Qt 6.4.0 or later")
}

QT += core gui widgets printsupport

TARGET = photoflare
TEMPLATE = app
CONFIG += c++11

# This path is used in order to find all of the header files
INCLUDEPATH += ./src/widgets \
               ./src/dialogs \
               ./src/tools \
               ./src/progress \
               ./src/managers
               ./src/workers

# Project settings for Windows. Adjust the paths as needed on your system.
win32 {
    # GraphicsMagick is provided via vcpkg (classic mode). Adjust VCPKG_ROOT if
    # your vcpkg installation lives somewhere other than C:/vcpkg.
    # Install with: vcpkg install graphicsmagick:x64-windows
    VCPKG_ROOT = C:/vcpkg/installed/x64-windows
    INCLUDEPATH += $$VCPKG_ROOT/include/GraphicsMagick
    LIBS += -L$$VCPKG_ROOT/lib -lGraphicsMagick++ -lGraphicsMagick
    RC_ICONS += assets/light/pixmaps/logo.ico

    # vcpkg builds shared DLLs (GraphicsMagick itself plus its delegate libs:
    # freetype, libpng, tiff, libjpeg-turbo, libwebp, bzip2, zlib, lzma, brotli).
    # Copy them next to the built exe so the app can find them at launch.
    # The MSVC spec places the target in an OUT_PWD/debug or OUT_PWD/release
    # subdirectory (DESTDIR), so the DLLs need to go there too, not OUT_PWD.
    CONFIG(debug, debug|release) {
        DLL_DEST_DIR = $$OUT_PWD/debug
    } else {
        DLL_DEST_DIR = $$OUT_PWD/release
    }
    QMAKE_POST_LINK += $$quote(cmd /c copy /y \"$$shell_path($$VCPKG_ROOT/bin)\\*.dll\" \"$$shell_path($$DLL_DEST_DIR)\" >nul$$escape_expand(\n\t))

    # GraphicsMagick also needs its *.mgk config files (colors.mgk, modules.mgk,
    # delegates.mgk, type.mgk, etc.) at runtime. It looks for them relative to
    # the running executable, so copy vcpkg's copies of them alongside the exe.
    # vcpkg splits these across two separate directories:
    #   share/graphicsmagick/<ver>/config  -> colors.mgk, log.mgk, modules.mgk
    #   lib/GraphicsMagick-<ver>/config    -> delegates.mgk, type*.mgk
    GM_CONFIG_MARKER = $$files($$VCPKG_ROOT/share/graphicsmagick/colors.mgk, true)
    !isEmpty(GM_CONFIG_MARKER) {
        GM_CONFIG_DIR = $$dirname(GM_CONFIG_MARKER)
        QMAKE_POST_LINK += $$quote(cmd /c copy /y \"$$shell_path($$GM_CONFIG_DIR)\\*.mgk\" \"$$shell_path($$DLL_DEST_DIR)\" >nul$$escape_expand(\n\t))
    }
    GM_TYPE_CONFIG_MARKER = $$files($$VCPKG_ROOT/lib/delegates.mgk, true)
    !isEmpty(GM_TYPE_CONFIG_MARKER) {
        GM_TYPE_CONFIG_DIR = $$dirname(GM_TYPE_CONFIG_MARKER)
        QMAKE_POST_LINK += $$quote(cmd /c copy /y \"$$shell_path($$GM_TYPE_CONFIG_DIR)\\*.mgk\" \"$$shell_path($$DLL_DEST_DIR)\" >nul$$escape_expand(\n\t))
    }
}

# Suppress warnings from vendored third-party headers we don't control.
win32-msvc {
    # C4828: illegal character in GraphicsMagick's cdl.h (non-UTF-8 byte in a vendored header)
    QMAKE_CXXFLAGS += /wd4828
}

# Project settings for Linux and Hurd. Adjust the paths as needed on your system.
linux|hurd {
    INCLUDEPATH += /usr/include/GraphicsMagick
    LIBS += -lGraphicsMagick++
    QMAKE_CXXFLAGS += -fopenmp
    LIBS += -fopenmp
}

# Project settings for Mac OS. Adjust the paths as needed on your system.
macx {
  INCLUDEPATH += /usr/local/Cellar/graphicsmagick/1.3.47/include/GraphicsMagick
  LIBS += -L/usr/local/Cellar/graphicsmagick/1.3.47/lib/ -lGraphicsMagick++
  LIBS += -L/usr/local/Cellar/graphicsmagick/1.3.47/lib/ -lGraphicsMagick
  LIBS += -L/usr/local/Cellar/graphicsmagick/1.3.47/lib/ -lGraphicsMagickWand
  INCLUDEPATH += /usr/local/Cellar/graphicsmagick/1.3.47/include/GraphicsMagick
  DEPENDPATH += /usr/local/Cellar/graphicsmagick/1.3.47/include/GraphicsMagick
  PRE_TARGETDEPS += /usr/local/Cellar/graphicsmagick/1.3.47/lib/libGraphicsMagick++.la
  PRE_TARGETDEPS += /usr/local/Cellar/graphicsmagick/1.3.47/lib/libGraphicsMagick.la
  PRE_TARGETDEPS += /usr/local/Cellar/graphicsmagick/1.3.47/lib/libGraphicsMagickWand.la
  LIBS += -lbz2 -lxml2 -lz -lm -L /usr/local/lib /usr/local/lib/libomp.dylib
  QMAKE_CXXFLAGS += -Xpreprocessor -fopenmp -lomp -I/usr/local/include
  QMAKE_LFLAGS += -lomp
}

SOURCES += src/main.cpp \
    src/mainwindow.cpp \
    src/dialogs/aboutdialog.cpp \
    src/dialogs/NewDialog.cpp \
    src/dialogs/textdialog.cpp \
    src/dialogs/prefsdialog.cpp \
    src/dialogs/batchdialog.cpp \
    src/dialogs/gradientdialog.cpp \
    src/dialogs/transparentdialog.cpp \
    src/dialogs/imagepropertiesdialog.cpp \
    src/dialogs/huedialog.cpp \
    src/dialogs/outerframedialog.cpp \
    src/dialogs/compressiondialog.cpp \
    src/dialogs/plugindialog.cpp \
    src/widgets/PaintWidget.cpp \
    src/widgets/RulerWidget.cpp \
    src/Settings.cpp \
    src/managers/FilterManager.cpp \
    src/managers/ToolManager.cpp \
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
    src/progress/QProgressIndicator.cpp \
    src/widgets/imagepositionwidget.cpp \
    src/workers/BatchProcessWorker.cpp \
    src/widgets/brushtypecombobox.cpp \
    src/widgets/colorboxwidget.cpp \
    src/progress/batchprogress.cpp \
    src/workers/filterworker.cpp \
    src/workers/filterworkermp.cpp \
    src/workers/pluginfilterworker.cpp \
    src/workers/FloodFillWorker.cpp \
    src/tools/EraserTool.cpp \
    src/toolSettings/erasersettingswidget.cpp \
    src/tools/SmudgeTool.cpp \
    src/toolSettings/SmudgeSettingsWidget.cpp \
    src/dialogs/colourmanagerdialog.cpp \
    src/dialogs/checkupdatedialog.cpp \
    src/dialogs/dropshadowdialog.cpp

HEADERS += src/mainwindow.h \
    src/plugins/AppContext.h \
    src/plugins/IPhotoflarePlugin.h \
    src/plugins/PluginManager.h \
    src/dialogs/aboutdialog.h \
    src/dialogs/NewDialog.h \
    src/dialogs/textdialog.h \
    src/dialogs/prefsdialog.h \
    src/dialogs/batchdialog.h \
    src/dialogs/gradientdialog.h \
    src/dialogs/transparentdialog.h \
    src/dialogs/imagepropertiesdialog.h \
    src/dialogs/huedialog.h \
    src/dialogs/outerframedialog.h \
    src/dialogs/compressiondialog.h \
    src/dialogs/plugindialog.h \
    src/widgets/PaintWidget.h \
    src/widgets/RulerWidget.h \
    src/Settings.h \
    src/managers/FilterManager.h \
    src/managers/ToolManager.h \
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
    src/progress/QProgressIndicator.h \
    src/toolSettings/pointersettingswidget.h \
    src/toolSettings/PaintBrushSettingsWidget.h \
    src/toolSettings/PaintBrushAdvSettingsWidget.h \
    src/toolSettings/SprayCanSettingsWidget.h \
    src/toolSettings/LineSettingsWidget.h \
    src/toolSettings/MagicWandSettingsWidget.h \
    src/toolSettings/StampSettingsWidget.h \
    src/toolSettings/BlurSettingsWidget.h \
    src/widgets/imagepositionwidget.h \
    src/workers/BatchProcessWorker.h \
    src/widgets/brushtypecombobox.h \
    src/widgets/colorboxwidget.h \
    src/progress/batchprogress.h \
    src/workers/filterworker.h \
    src/workers/filterworkermp.h \
    src/workers/pluginfilterworker.h \
    src/workers/FloodFillWorker.h \
    src/tools/EraserTool.h \
    src/toolSettings/erasersettingswidget.h \
    src/tools/SmudgeTool.h \
    src/toolSettings/SmudgeSettingsWidget.h \
    src/dialogs/colourmanagerdialog.h \
    src/dialogs/checkupdatedialog.h \
    src/dialogs/dropshadowdialog.h

FORMS += src/mainwindow.ui \
    src/dialogs/aboutdialog.ui \
    src/dialogs/NewDialog.ui \
    src/dialogs/textdialog.ui \
    src/dialogs/prefsdialog.ui \
    src/dialogs/batchdialog.ui \
    src/dialogs/gradientdialog.ui \
    src/dialogs/transparentdialog.ui \
    src/dialogs/imagepropertiesdialog.ui \
    src/dialogs/huedialog.ui \
    src/dialogs/outerframedialog.ui \
    src/dialogs/compressiondialog.ui \
    src/dialogs/plugindialog.ui \
    src/widgets/ColorBoxWidget.ui \
    src/widgets/BrushTypeComboBox.ui \
    src/widgets/imagepositionwidget.ui \
    src/progress/batchprogress.ui \
    src/toolSettings/pointersettingswidget.ui \
    src/toolSettings/PaintBrushSettingsWidget.ui \
    src/toolSettings/PaintBrushAdvSettingsWidget.ui \
    src/toolSettings/SprayCanSettingsWidget.ui \
    src/toolSettings/LineSettingsWidget.ui \
    src/toolSettings/MagicWandSettingsWidget.ui \
    src/toolSettings/StampSettingsWidget.ui \
    src/toolSettings/BlurSettingsWidget.ui \
    src/toolSettings/erasersettingswidget.ui \
    src/toolSettings/smudgesettingswidget.ui \
    src/dialogs/colourmanagerdialog.ui \
    src/dialogs/checkupdatedialog.ui \
    src/dialogs/dropshadowdialog.ui

RESOURCES += \
    Icons.qrc \
    IconsDark.qrc

include (external/qt-solutions/qtsingleapplication/qtsingleapplication.pri)

INCLUDEPATH += external/qt-solutions/qtsingleapplication

TRANSLATIONS = languages/en.ts \
               languages/fr.ts \
               languages/nl.ts \
               languages/de.ts \
               languages/zh_TW.ts \
               languages/cs.ts \
               languages/zh_CN.ts \
               languages/pt_PT.ts \
               languages/pt_BR.ts \
               languages/sv.ts \
               languages/ru_RU.ts \
               languages/id.ts \
               languages/es.ts \
               languages/ja.ts \
               languages/tr.ts \
               languages/ko.ts

# Generate translations in build
TRANSLATIONS_FILES =

qtPrepareTool(LRELEASE, lrelease)
for(tsfile, TRANSLATIONS) {
    qmfile = $$shadowed($$tsfile)
    qmfile ~= s,.ts$,.qm,
    qmdir = $$dirname(qmfile)
    !exists($$qmdir) {
        mkpath($$qmdir)|error("Aborting.")
    }
    command = $$LRELEASE -removeidentical $$tsfile -qm $$qmfile
    system($$command)|error("Failed to run: $$command")
    TRANSLATIONS_FILES += $$qmfile
}

# installs
unix:!macx {
    isEmpty(PREFIX) {
      packaging {
        PREFIX = /usr
      } else {
        PREFIX = /usr/local
      }
    }
    DEFINES += APP_PREFIX=\\\"$$PREFIX\\\"

    target.path = $${PREFIX}/bin/
    qmfile.path = $${PREFIX}/share/$${TARGET}/languages/
    qmfile.files = $${TRANSLATIONS_FILES}
    icon.path = $${PREFIX}/share/icons/
    icon.files = installers/deb/DEBIAN/usr/share/icons/*
    pixmaps.path = $${PREFIX}/share/pixmaps/
    pixmaps.files = installers/deb/DEBIAN/usr/share/icons/hicolor/48x48/apps/photoflare.png
    desktopentry.path = $${PREFIX}/share/applications
    desktopentry.files = installers/deb/DEBIAN/usr/share/applications/$${TARGET}.desktop
    manpage.path = $${PREFIX}/share/man/man1/
    manpage.files = photoflare.1
    appstream.path = $${PREFIX}/share/metainfo/
    appstream.files = io.photoflare.photoflare.appdata.xml

    # Do not strip data files (icons, pixmaps, translations, etc.) during install.
    # qmake applies $(STRIP) to all INSTALLS targets; clearing QMAKE_STRIP here
    # prevents it from being called on non-binary files. Packagers can handle binary
    # stripping independently.
    QMAKE_STRIP =

     INSTALLS += target \
        qmfile \
        icon \
        pixmaps \
        desktopentry \
        manpage \
        appstream
}
