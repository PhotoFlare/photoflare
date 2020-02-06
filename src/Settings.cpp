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

// Settings class for the Photoflare application.

#include <QSettings>
#include <QRect>

#include "Settings.h"

class SettingsPrivate
{
public:
    SettingsPrivate()
    {
        maximizeWindow = settings.value("MaximizeOnStartup").toBool();
        geometry = settings.value("CustomWindowGeometry").toRect();
        recentFiles = settings.value("RecentFiles").toList();
        multiWindowMode = settings.value("MultiWindowMode").toBool();
        userLanguage = settings.value("UserLanguage").toString();
        openFolder = settings.value("OpenFolder").toString();
        saveFolder = settings.value("SaveFolder").toString();
        saveFormat = settings.value("SaveFormat").toString();
        saveFormatEnabled = settings.value("SaveFormatEnabled").toBool();
        compressionDialogEnabled = settings.value("compressionDialogEnabled").toBool();
        compressionDefaultValue = settings.value("compressionDefaultValue").toString();
        historyLimit = settings.value("historyLimit").toString();
        dockLayout = settings.value("dockLayout").toString();
        zoomDirection = settings.value("zoomDirection").toString();
        memDialogParams = settings.value("memorizeParamsEnabled").toBool();
        unit = settings.value("unit").toInt();
    }

    ~SettingsPrivate()
    {

    }

    void setValue(const QString &propertyName, const QVariant &value)
    {
        settings.setValue(propertyName, value);
        settings.sync();
    }

    bool maximizeWindow;
    bool multiWindowMode;
    QRect geometry;
    QList<QVariant> recentFiles;
    QString userLanguage;
    QString openFolder;
    bool prevOpened;
    bool prevOpenedSave;
    QString saveFolder;
    QString saveFormat;
    bool saveFormatEnabled;
    bool compressionDialogEnabled;
    bool compressionDefaultEnabled;
    QString compressionDefaultValue;
    QString historyLimit;
    QString dockLayout;
    QString zoomDirection;
    bool memDialogParams;
    int unit;
    QSettings settings;
};

Settings* Settings::m_instance = 0;
const int Settings::MAX_RECENTS_COUNT = 8;

Settings::Settings(QObject *parent)
    : QObject(parent)
    , d(new SettingsPrivate)
{

}

Settings::~Settings()
{
    delete d;
}

Settings *Settings::instance()
{
    if (!m_instance)
        m_instance = new Settings;

    return m_instance;
}

void Settings::setDefaultSettings()
{
    d->setValue("MaximizeOnStartup", false);
    d->setValue("OpenFolder", "");
    d->setValue("RecentFiles", "");
    d->setValue("SaveFolder", "");
    d->setValue("SaveFormat", "");
    if(getUserLanguage() == "")
    {
        d->setValue("UserLanguage", "notset");
    }
    d->setValue("compressionDefaultEnabled", false);
    d->setValue("compressionDefaultValue", 90);
    d->setValue("compressionDialogEnabled", true);
    d->setValue("dockLayout", "0");
    d->setValue("zoomDirection", "0");
    d->setValue("historyLimit", 24);
    d->setValue("memorizeParamsEnabled", true);
    d->setValue("prevOpened", false);
    d->setValue("prevOpenedSave", false);
    d->setValue("saveFormatEnabled", false);
    d->setValue("unit", 0);
}

void Settings::setUnit(int unitcode)
{
    d->unit = unitcode;
    d->setValue("unit", unitcode);
}

int Settings::getUnit() const
{
    return d->unit;
}

void Settings::setMaximizeWindow(bool maximize)
{
    d->maximizeWindow = maximize;
    d->setValue("MaximizeOnStartup", maximize);
}

bool Settings::isMaximizeWindow() const
{
    return d->maximizeWindow;
}

void Settings::setMultiWindowMode(bool multiWindowMode)
{
    d->multiWindowMode = multiWindowMode;
    d->setValue("MultiWindowMode", multiWindowMode);
    emit multiWindowModeChanged(multiWindowMode);
}

bool Settings::isMultiWindowMode() const
{
    return d->multiWindowMode;
}

void Settings::setCustomWindowGeometry(const QRect &rect)
{
    d->geometry = rect;
    d->setValue("CustomWindowGeometry", rect);
}

QRect Settings::customWindowGeometry() const
{
    return d->geometry;
}

void Settings::addRecentFile(const QString &file)
{
    if(!d->recentFiles.contains(file))
    {
        d->recentFiles.append(file);

        if(d->recentFiles.size() > MAX_RECENTS_COUNT)
            d->recentFiles.removeFirst();
    }

    d->setValue("RecentFiles", d->recentFiles);
}

QList<QVariant> Settings::getRecentFiles() const
{
    return d->recentFiles;
}

void Settings::setUserLanguage(const QString language)
{
    d->userLanguage = language;
    d->setValue("UserLanguage",language);
}

QString Settings::getUserLanguage() const
{
    return d->userLanguage;
}

void Settings::setOpenFolder(const QString folderpath)
{
    d->openFolder = folderpath;
    d->setValue("OpenFolder",folderpath);
}

QString Settings::getOpenFolder() const
{
    return d->openFolder;
}

void Settings::setPreviouslyOpened(bool enabled)
{
    d->prevOpened = enabled;
    d->setValue("prevOpened",enabled);
}

bool Settings::getPreviouslyOpened() const
{
    return d->prevOpened;
}

void Settings::setPreviouslyOpenedSave(bool enabled)
{
    d->prevOpened = enabled;
    d->setValue("prevOpenedSave",enabled);
}

bool Settings::getPreviouslyOpenedSave() const
{
    return d->prevOpenedSave;
}

void Settings::setSaveFolder(const QString folderpath)
{
    d->saveFolder = folderpath;
    d->setValue("SaveFolder",folderpath);
}

QString Settings::getSaveFolder() const
{
    return d->saveFolder;
}


void Settings::setSaveFormat(const QString format)
{
    d->saveFormat = format;
    d->setValue("SaveFormat",format);
}

QString Settings::getSaveFormat() const
{
    return d->saveFormat;
}

void Settings::setSaveFormatEnabled(bool enabled)
{
    d->saveFormatEnabled = enabled;
    d->setValue("saveFormatEnabled",enabled);
}

bool Settings::getSaveFormatEnabled() const
{
    return d->saveFormatEnabled;
}

void Settings::setCompressionDialogEnabled(bool enabled)
{
    d->compressionDialogEnabled = enabled;
    d->setValue("compressionDialogEnabled",enabled);
}

bool Settings::getCompressionDialogEnabled() const
{
    return d->compressionDialogEnabled;
}

void Settings::setCompressionDefaultEnabled(bool enabled)
{
    d->compressionDefaultEnabled = enabled;
    d->setValue("compressionDefaultEnabled",enabled);
}

bool Settings::getCompressionDefaultEnabled() const
{
    return d->compressionDefaultEnabled;
}

void Settings::setCompressionDefaultValue(const QString val)
{
    d->compressionDefaultValue = val;
    d->setValue("compressionDefaultValue",val);
}

QString Settings::getCompressionDefaultValue() const
{
    return d->compressionDefaultValue;
}

void Settings::setHistoryLimit(const QString val)
{
    d->historyLimit = val;
    d->setValue("historyLimit",val);
}

QString Settings::getHistoryLimit() const
{
    return d->historyLimit;
}

void Settings::setDockLayout(const QString val)
{
    d->dockLayout = val;
    d->setValue("dockLayout",val);
}

QString Settings::getDockLayout() const
{
    return d->dockLayout;
}

void Settings::setZoomDirection(const QString val)
{
    d->zoomDirection = val;
    d->setValue("zoomDirection",val);
}

QString Settings::getZoomDirection() const
{
    return d->zoomDirection;
}

void Settings::setMemParamsEnabled(bool enabled)
{
    d->memDialogParams = enabled;
    d->setValue("memorizeParamsEnabled",enabled);
}

bool Settings::getMemParamsEnabled() const
{
    return d->memDialogParams;
}
