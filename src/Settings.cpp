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
        maximizeWindow = settings.value(QStringLiteral("MaximizeOnStartup")).toBool();
        geometry = settings.value(QStringLiteral("CustomWindowGeometry")).toRect();
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
    QSettings settings;
};

Settings* Settings::m_instance = 0;

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
    d->setValue(QStringLiteral("MaximizeOnStartup"), maximize);
}

bool Settings::isMaximizeWindow() const
{
    return d->maximizeWindow;
}

void Settings::setCustomWindowGeometry(const QRect &rect)
{
    d->geometry = rect;
    d->setValue(QStringLiteral("CustomWindowGeometry"), rect);
}

QRect Settings::customWindowGeometry() const
{
    return d->geometry;
}
