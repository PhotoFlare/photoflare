QT += core gui widgets
CONFIG += plugin c++17
CONFIG -= app_bundle
TEMPLATE = lib

TARGET = webp_exporter_plugin

# Path to the Photoflare plugin SDK headers (relative to this .pro file)
INCLUDEPATH += ../../../src/plugins

SOURCES += WebpExporterPlugin.cpp

OTHER_FILES += webp.json

# ─── Output directory ────────────────────────────────────────────────────────
# Outputs the plugin DLL/SO directly into a "plugins/" subfolder of the
# shadow build directory. Copy that folder next to the Photoflare
# executable to use it.
DESTDIR = $$OUT_PWD/plugins
