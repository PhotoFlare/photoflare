/*

  PhotoFiltre LX main program.

*/


#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // TODO: Add support for filenames passed on the command line.

    app.setApplicationName(QObject::tr("PhotoFiltre LX Studio Demo"));
    app.setApplicationVersion(QObject::tr("0.3 demo version"));
    app.setOrganizationDomain(QObject::tr("photofiltre-lx.org"));
    app.setOrganizationName(QObject::tr("photofiltre-lx"));

    MainWindow w;
    w.show();

    return app.exec();
}
