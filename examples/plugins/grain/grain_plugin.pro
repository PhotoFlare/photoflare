QT += core gui widgets
CONFIG += plugin c++17
CONFIG -= app_bundle
TEMPLATE = lib

TARGET = grain_plugin

# Path to the Photoflare plugin SDK headers (relative to this .pro file)
INCLUDEPATH += ../../../src/plugins

SOURCES += GrainFilterPlugin.cpp

OTHER_FILES += grain.json

# ─── Output directory ────────────────────────────────────────────────────────
# Outputs the plugin DLL/SO directly into a "plugins/" subfolder of the
# shadow build directory (e.g. build/.../debug/plugins/).
# Copy that folder next to the Photoflare executable to use it.
DESTDIR = $$OUT_PWD/plugins

# Copy the metadata JSON alongside the DLL after each build
win32 {
    PLUGINS_DIR = $$shell_path($$DESTDIR)
    QMAKE_POST_LINK = \
        cmd /c if not exist \"$$PLUGINS_DIR\" mkdir \"$$PLUGINS_DIR\" & \
        copy /Y \"$$shell_path($$PWD/grain.json)\" \"$$PLUGINS_DIR\\grain.json\"
}
unix {
    QMAKE_POST_LINK = mkdir -p $$DESTDIR && cp $$PWD/grain.json $$DESTDIR/grain.json
}
