/*

  PhotoFiltre LX main program.

*/


#include "mainwindow.h"
#include <QApplication>
#include <QFileInfo>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.setApplicationName(QObject::tr("PhotoFiltre LX Studio Demo"));
    app.setApplicationVersion(QObject::tr("0.7 demo version (Build 38)"));
    app.setOrganizationDomain(QObject::tr("photofiltre-lx.org"));
    app.setOrganizationName(QObject::tr("photofiltre-lx"));

    MainWindow w;
    w.show();

    if(argc > 1) {
        QFileInfo fileInfo(argv[1]);
        if(fileInfo.exists()) {
            w.openFile(QString(fileInfo.absoluteFilePath()));
        }
    }

    return app.exec();
}
