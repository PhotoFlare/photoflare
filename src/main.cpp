/*

  PhotoFiltre LX main program.

*/


#include "mainwindow.h"
#include <QApplication>
#include <QFileInfo>
#include <QTranslator>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.setApplicationName(QObject::tr("PhotoFiltre LX Studio Demo"));
    app.setApplicationVersion(QObject::tr("0.9 demo version (Build 68)"));
    app.setOrganizationDomain(QObject::tr("photofiltre-lx.org"));
    app.setOrganizationName(QObject::tr("photofiltre-lx"));

    QTranslator translator;
    if(translator.load("languages/fr.qm", app.applicationDirPath()))
        qDebug() << "Loaded translation";
    else
        qDebug() << "Translation not loaded";
    app.installTranslator(&translator);

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

    return app.exec();
}
