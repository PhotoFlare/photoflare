/*

  PhotoFiltre LX main program.

*/


#include "mainwindow.h"
#include <QApplication>
#include <QFileInfo>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.setApplicationName(QObject::tr("PhotoFiltre LX Studio Demo"));
    app.setApplicationVersion(QObject::tr("0.9 demo version (Build 66)"));
    app.setOrganizationDomain(QObject::tr("photofiltre-lx.org"));
    app.setOrganizationName(QObject::tr("photofiltre-lx"));

    MainWindow w;
    w.show();

    if(argc > 1) {
        for (int i = 0; i < argc; ++i) {
            QFileInfo fileInfo(argv[i]);
            if(fileInfo.exists()) {
                w.openFile(QString(fileInfo.absoluteFilePath()));
            }
        }
    }

    QTranslator translator;
    translator.load("fr");
    app.installTranslator(&translator);

    return app.exec();
}
