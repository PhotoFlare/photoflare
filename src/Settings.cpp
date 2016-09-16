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
    QRect geometry;
    QList<QVariant> recentFiles;
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
