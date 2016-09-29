/*

  Settings class for the PhotoFiltre LX application.

*/

#include "Settings.h"
#include <QSettings>
#include <QRect>

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
        saveFormatEnabled = settings.value("SaveFormatEnabled").toString();
        compressionDialogEnabled = settings.value("compressionDialogEnabled").toString();
        compressionDefaultValue = settings.value("compressionDefaultValue").toString();
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
    QString saveFolder;
    QString saveFormat;
    QString saveFormatEnabled;
    QString compressionDialogEnabled;
    QString compressionDefaultEnabled;
    QString compressionDefaultValue;
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
    if(!d->recentFiles.contains(file)) {
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

void Settings::setSaveFormatEnabled(const QString enabled)
{
    d->saveFormatEnabled = enabled;
    d->setValue("saveFormatEnabled",enabled);
}

bool Settings::getSaveFormatEnabled() const
{
    bool checkedState = false;
    if(d->saveFormatEnabled == "true")
    {
        checkedState = true;
    }

    return checkedState;
}

void Settings::setCompressionDialogEnabled(const QString enabled)
{
    d->compressionDialogEnabled = enabled;
    d->setValue("compressionDialogEnabled",enabled);
}

bool Settings::getCompressionDialogEnabled() const
{
    bool checkedState = false;
    if(d->compressionDialogEnabled == "true")
    {
        checkedState = true;
    }

    return checkedState;
}

void Settings::setCompressionDefaultEnabled(const QString enabled)
{
    d->compressionDefaultEnabled = enabled;
    d->setValue("compressionDefaultEnabled",enabled);
}

bool Settings::getCompressionDefaultEnabled() const
{
    bool checkedState = false;
    if(d->compressionDefaultEnabled == "true")
    {
        checkedState = true;
    }

    return checkedState;
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
