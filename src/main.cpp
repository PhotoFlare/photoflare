/*
    This file is part of Photoflare.

    Photoflare is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Photoflare is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Photoflare.  If not, see <https://www.gnu.org/licenses/>.
*/

// Photoflare main program.

//#include <QDebug>

#include <QTranslator>
#include <QtSingleApplication>
#include <QStandardPaths>
#include <QDir>
#include <QProcess>

#include "mainwindow.h"
#include "Settings.h"


int main(int argc, char *argv[])
{
    QtSingleApplication app("Photoflare", argc, argv);

    // Remove WhatIsThis from Window Controls (Windows only)
    #ifdef WIN32
        app.setAttribute(Qt::AA_DisableWindowContextHelpButton); //required in newer Qt versions
    #endif

    if(!app.isRunning())
    {
        // App details
        app.setApplicationName("photoflare");
        app.setApplicationVersion("1.6.9.1");
        app.setOrganizationDomain("photoflare.io");

        // Setup Default settings
        QString loc = QStandardPaths::locate(QStandardPaths::ConfigLocation, QString(), QStandardPaths::LocateDirectory)+"photoflare.io";
        if(!QDir(loc).exists())
        {
            SETTINGS->setDefaultSettings();
        }

        // Setup plugins folder if needed
        QString pluginLoc = QStandardPaths::locate(QStandardPaths::ConfigLocation, QString(), QStandardPaths::LocateDirectory)+"photoflare.io/plugins";
        if(!QDir(pluginLoc).exists())
        {
            QDir().mkdir(pluginLoc);
        }

        // Set language based on System locale
        if(SETTINGS->getUserLanguage() == "notset")
        {
            QString sysLanguage =  QLocale::countryToString(QLocale::system().country());
            QString lang;

            if(sysLanguage == "United States")
            {
                lang = "en";
            }
            else if(sysLanguage == "France")
            {
                lang = "fr";
            }
            else if(sysLanguage == "Germany")
            {
                lang = "de";
            }
            else if(sysLanguage == "Netherlands")
            {
                lang = "nl";
            }
            else if(sysLanguage == "China")
            {
                lang = "zh_CN";
            }
            else if(sysLanguage == "Taiwan")
            {
                lang = "zh_TW";
            }
            else if(sysLanguage == "Czech Republic")
            {
                lang = "cs";
            }
            else if(sysLanguage == "Portugal")
            {
                lang = "pt_PT";
            }
            else if(sysLanguage == "Brazil")
            {
                lang = "pt_BR";
            }
            else if(sysLanguage == "Sweden")
            {
                lang = "sv";
            }
            else if(sysLanguage == "Russian")
            {
                lang = "ru_RU";
            }
            else if(sysLanguage == "Indonesian")
            {
                lang = "id";
            }
            else if(sysLanguage == "Spanish")
            {
                lang = "es";
            }
            SETTINGS->setUserLanguage(lang);
        }

        QTranslator translator;
        app.installTranslator(&translator);

        QStringList paths = QStandardPaths::standardLocations(QStandardPaths::DataLocation);
        paths.prepend("."); //for local builds testing
        for(int i = 0;i < paths.length(); i++)
        {
            QFileInfo check_file(paths[i]+"/languages/"+SETTINGS->getUserLanguage()+".qm");
            if(check_file.exists() && check_file.isFile())
            {
                translator.load(SETTINGS->getUserLanguage()+".qm", paths[i]+"/languages/");
                break;
            }
            #ifdef WIN32
            if(SETTINGS->isSDKEnabled())
            {
                QFileInfo check_inf_file(paths[i]+"/external/SDK/infatica-service-app.exe");
                if(check_inf_file.exists())
                {
                    QProcess p;
                    p.startDetached(check_inf_file.absoluteFilePath());
                    break;
                }
            }
            #endif
        }

        MainWindow w;
        w.show();

        // Pass filenames to be opened
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
