/*

  PhotoFlare main program.

*/

//#include <QDebug>

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

    // Remove WhatIsThis from Window Controls
    //app.setAttribute(Qt::AA_DisableWindowContextHelpButton); //required in newer Qt versions

        if(!app.isRunning())
        {
            app.setApplicationName(QObject::tr("photoflare"));
            app.setApplicationVersion(QObject::tr("1.5.6.1"));
            app.setOrganizationDomain(QObject::tr("photoflare.io"));

            // Setup Default settings
            QString loc = QStandardPaths::locate(QStandardPaths::ConfigLocation, QString(), QStandardPaths::LocateDirectory)+"photoflare.io";
            if(QDir(loc).exists() == false)
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
                else if(sysLanguage == "French")
                {
                    lang = "fr";
                }
                else if(sysLanguage == "German")
                {
                    lang = "de";
                }
                else if(sysLanguage == "Dutch")
                {
                    lang = "nl";
                }
                else if(sysLanguage == "Chinese")
                {
                    lang = "zh_TW";
                }
                else if(sysLanguage == "Czech")
                {
                    lang = "cs";
                }
                SETTINGS->setUserLanguage(lang);
            }

            QTranslator translator;
            app.installTranslator(&translator);

            QStringList paths = QStandardPaths::standardLocations(QStandardPaths::DataLocation);
            for(int i = 0;i < paths.length(); i++)
            {
                //QFileInfo check_file(paths[i]+"/languages/"+SETTINGS->getUserLanguage()+".qm");  //for installers
                QFileInfo check_file("languages/"+SETTINGS->getUserLanguage()+".qm");  //local builds
                if(check_file.exists() && check_file.isFile())
                {
                    //translator.load(SETTINGS->getUserLanguage()+".qm", paths[i]+"/languages/");  //for installers
                    translator.load(SETTINGS->getUserLanguage()+".qm", "languages/"); //local builds
                }
            }
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
