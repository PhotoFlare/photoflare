#-------------------------------------------------
#
# Project created by QtCreator 2014-05-24T20:21:27
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PhotoFiltreLX
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    aboutdialog.cpp \
    newdialog.cpp \
    textdialog.cpp \
    prefsdialog.cpp

HEADERS  += mainwindow.h \
    aboutdialog.h \
    newdialog.h \
    textdialog.h \
    prefsdialog.h

FORMS    += mainwindow.ui \
    aboutdialog.ui \
    newdialog.ui \
    textdialog.ui \
    prefsdialog.ui

RESOURCES += \
    Icons.qrc
