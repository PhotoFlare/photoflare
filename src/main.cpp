/*

  PhotoFiltre LX main program.

*/


#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // TODO: Add support for filenames passed on the command line.

    app.setApplicationName(QObject::tr("PhotoFiltre LX Studio"));
    app.setApplicationVersion(QObject::tr("0.1 alpha"));
    app.setOrganizationDomain(QObject::tr("photofiltre-lx.org"));
    app.setOrganizationName(QObject::tr("photofiltre-lx"));

    MainWindow w;
    w.show();

    return app.exec();
}
