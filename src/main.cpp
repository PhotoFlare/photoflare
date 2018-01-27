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
#include <QDir>

#include "mainwindow.h"
#include "Settings.h"


int main(int argc, char *argv[])
{
    QtSingleApplication app("PhotoFlare", argc, argv);

        if(!app.isRunning())
        {
            app.setApplicationName(QObject::tr("PhotoFlare"));
            app.setApplicationVersion(QObject::tr("1.5.0"));
            app.setOrganizationDomain(QObject::tr("photoflare.io"));
            app.setOrganizationName(QObject::tr("photoflare"));

            // Setup Default settings
            QString loc = QStandardPaths::locate(QStandardPaths::ConfigLocation, QString(), QStandardPaths::LocateDirectory)+"photoflare";
            if(QDir(loc).exists())
            {
                //qCritical() << "Exists";
            }
            else
            {
                SETTINGS->setDefaultSettings();
            }

            // Set language based on System locale
            if(SETTINGS->getUserLanguage() == "notset")
            {
                QString sysLanguage =  QLocale::languageToString(QLocale::system().language());
                QString lang;

                if(sysLanguage == "English")
                {
                    lang = "en";
                }
                SETTINGS->setUserLanguage(lang);
            }

            QTranslator translator;
            QStringList paths = QStandardPaths::standardLocations(QStandardPaths::DataLocation);

            for(int i = 0;i < paths.length(); i++)
            {
                //QFileInfo check_file(paths[i]+"/languages/"+SETTINGS->getUserLanguage()+".qm");  for .deb installer

                QFileInfo check_file("languages/"+SETTINGS->getUserLanguage()+".qm");  //local builds
                if(check_file.exists() && check_file.isFile())
                {
                    //if(translator.load(SETTINGS->getUserLanguage()+".qm", paths[i]+"/languages/"))  for .deb installer

                    if(translator.load(SETTINGS->getUserLanguage()+".qm", "languages/")) //local builds
                        qCritical() << "Loaded translation";
                    else
                        qCritical() << "Translation not loaded";
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
