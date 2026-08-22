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

#include <QtSingleApplication>
#include <QStandardPaths>
#include <QDir>
#include <QFile>
#include <QSettings>

#include "mainwindow.h"
#include "Settings.h"


int main(int argc, char *argv[])
{
    QApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
    QtSingleApplication app("Photoflare", argc, argv);
    // Fix wayland titlebar icons
    QApplication::setDesktopFileName("photoflare");

    if(!app.isRunning())
    {
        // App details
        app.setApplicationName("photoflare");
        app.setApplicationVersion("1.7.4");
        app.setOrganizationName("PhotoFlare");
        app.setOrganizationDomain("PhotoFlare");

        // Setup Default settings
        if (Settings::isPortableMode()) {
            // Portable mode: settings go to photoflare.ini next to the executable.
            // Treat a missing ini file as first run.
            const QString iniPath = QCoreApplication::applicationDirPath() + "/photoflare.ini";
            if (!QFile::exists(iniPath)) {
                SETTINGS->setDefaultSettings();
            }
        } else {
            // QStandardPaths::locate() with an empty filename is unreliable across
            // Qt versions/distros, so check QSettings' own backing store instead.
            QSettings settings;
            if (settings.allKeys().isEmpty()) {
                SETTINGS->setDefaultSettings();
            }
        }

        // Setup plugins folder if needed
        // Must match the directory MainWindow::loadPlugins() scans.
        QString pluginLoc;
        if (Settings::isPortableMode()) {
            pluginLoc = QCoreApplication::applicationDirPath() + "/plugins";
        } else {
            pluginLoc = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/plugins";
        }
        if(!QDir(pluginLoc).exists())
        {
            QDir().mkpath(pluginLoc);
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
            else if(sysLanguage == "Japan")
            {
                lang = "ja";
            }
            else if(sysLanguage == "Turkey")
            {
                lang = "tr";
            }
            else if(sysLanguage == "Korean")
            {
                lang = "ko";
            }
            else if(sysLanguage == "Poland")
            {
                lang = "pl";
            }
            else
            {
                lang = "en";
            }
            SETTINGS->setUserLanguage(lang);
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
