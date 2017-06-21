/*

  PhotoFlare main program.

*/

#include <QDebug>

#include <QApplication>
#include <QFileInfo>
#include <QTranslator>
#include <QtSingleApplication>
#include <QStandardPaths>
#include <QFileInfo>

#include "mainwindow.h"
#include "Settings.h"


int main(int argc, char *argv[])
{
    QtSingleApplication app("PhotoFlare", argc, argv);

        if(!app.isRunning())
        {
            app.setApplicationName(QObject::tr("PhotoFlare"));
            app.setApplicationVersion(QObject::tr("1.0.7"));
            app.setOrganizationDomain(QObject::tr("photoflare.io"));
            app.setOrganizationName(QObject::tr("photoflare"));

            QTranslator translator;
            QStringList paths = QStandardPaths::standardLocations(QStandardPaths::DataLocation);

            for(int i = 0;i < paths.length(); i++)
            {
                QFileInfo check_file(paths[i]+"/languages/"+SETTINGS->getUserLanguage()+".qm");
                if(check_file.exists() && check_file.isFile())
                {
                    if(translator.load(SETTINGS->getUserLanguage()+".qm", paths[i]+"/languages/"))
                        qDebug() << "Loaded translation";
                    else
                        qDebug() << "Translation not loaded";
                }
            }
            app.installTranslator(&translator);


            MainWindow w;
            w.show();

            if(argc > 1)
            {
                for (int i = 1; i < argc; ++i)
                {
                    w.handleMessage(QString(argv[i]));
                }
            }

            app.setActivationWindow(&w, true);

            QObject::connect(&app, SIGNAL(messageReceived(QString)), &w, SLOT(handleMessage(QString)));

            return app.exec();

        }
        else
        {
            if(argc > 1)
            {
                for (int i = 1; i < argc; ++i)
                {
                    app.sendMessage(QString(argv[i]));
                }
            }
        }
        return 0;
}
