/*

  PhotoFiltre LX main program.

*/


#include "mainwindow.h"
#include <QApplication>
#include <QFileInfo>
#include <QTranslator>
#include <QtSingleApplication>
#include <QStandardPaths>
#include "Settings.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    QtSingleApplication app("PhotoFiltre LX Studio", argc, argv);

        if(!app.isRunning())
        {
            app.setApplicationName(QObject::tr("PhotoFiltre LX Studio"));
            app.setApplicationVersion(QObject::tr("1.0 Beta (Build 93)"));
            app.setOrganizationDomain(QObject::tr("photofiltre-lx.org"));
            app.setOrganizationName(QObject::tr("photofiltre-lx"));

            QTranslator translator;
            QStringList paths = QStandardPaths::standardLocations(QStandardPaths::DataLocation);

            //if(translator.load("languages/fr.qm", app.applicationDirPath()))
            if(translator.load(SETTINGS->getUserLanguage()+".qm", paths[4]+"/languages/"))
                qDebug() << "Loaded translation";
            else
                qDebug() << "Translation not loaded";
            app.installTranslator(&translator);


            MainWindow w;
            w.show();

            if(argc > 1) {
                for (int i = 1; i < argc; ++i) {
                        w.handleMessage(QString(argv[i]));
                    }
                }

            app.setActivationWindow(&w, true);

            QObject::connect(&app, SIGNAL(messageReceived(QString)), &w, SLOT(handleMessage(QString)));

            return app.exec();

        } else
        {
            if(argc > 1) {
                for (int i = 1; i < argc; ++i) {
                        app.sendMessage(QString(argv[i]));
                    }
                }
        }

        return 0;
}
