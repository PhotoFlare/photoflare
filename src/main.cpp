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

#include <QApplication>
#include <QStandardPaths>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QSettings>
#include <QLocale>
#include <QSet>

#include "mainwindow.h"
#include "Settings.h"
#include "SingleInstance.h"
#include "Theme.h"

namespace {
    // Maps the system locale to one of the bundled translations (languages/*.ts).
    QString systemLanguageCode()
    {
        static const QSet<QString> supported = {
            "en", "fr", "de", "nl", "cs", "sv", "id", "es", "ja", "tr", "ko", "pl"
        };
        const QLocale locale = QLocale::system();
        const QString code = QLocale::languageToCode(locale.language());

        if (code == "zh") {
            const bool traditional = locale.script() == QLocale::TraditionalChineseScript
                || locale.territory() == QLocale::Taiwan
                || locale.territory() == QLocale::HongKong
                || locale.territory() == QLocale::Macao;
            return traditional ? "zh_TW" : "zh_CN";
        }
        if (code == "pt") {
            return locale.territory() == QLocale::Brazil ? "pt_BR" : "pt_PT";
        }
        if (code == "ru") {
            return "ru_RU";
        }
        return supported.contains(code) ? code : "en";
    }
}

int main(int argc, char *argv[])
{
    QApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
    QApplication app(argc, argv);
    // Fix wayland titlebar icons
    QApplication::setDesktopFileName("photoflare");

    SingleInstance instance("Photoflare");
    bool runHere = instance.isPrimary();
    if(!runHere)
    {
        // Forward absolute paths: the primary instance has a different working directory.
        QStringList files;
        const QStringList args = app.arguments();
        for (int i = 1; i < args.size(); ++i)
        {
            files << QFileInfo(args.at(i)).absoluteFilePath();
        }
        // If the primary can't be reached, open a normal window instead of silently doing nothing.
        runHere = !instance.sendMessages(files);
    }

    if(runHere)
    {
        // App details
        app.setApplicationName("photoflare");
        app.setApplicationVersion("1.7.6");
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
            SETTINGS->setUserLanguage(systemLanguageCode());
        }

        Theme::apply(Theme::modeFromSetting(SETTINGS->getTheme()));

        MainWindow w;
        w.show();

        // Pass filenames to be opened
        const QStringList args = app.arguments();
        for (int i = 1; i < args.size(); ++i)
        {
            w.handleMessage(args.at(i));
        }

        QObject::connect(&instance, &SingleInstance::messageReceived, &w, &MainWindow::handleMessage);
        QObject::connect(&instance, &SingleInstance::activationRequested, &w, [&w]() {
            w.setWindowState(w.windowState() & ~Qt::WindowMinimized);
            w.raise();
            w.activateWindow();
        });

        return app.exec();
    }
    return 0;
}
