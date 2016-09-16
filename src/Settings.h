/*

  Settings class for the PhotoFiltre LX application.

*/

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>

#define SETTINGS Settings::instance()

class SettingsPrivate;

class Settings : public QObject
{
    Q_OBJECT
public:
    ~Settings();
    static Settings* instance();
    void setMaximizeWindow(bool maximize);
    bool isMaximizeWindow() const;
    void setCustomWindowGeometry(const QRect &rect);
    QRect customWindowGeometry() const;
    void addRecentFile(const QString &file);
    QList<QVariant> getRecentFiles() const;

private:
    explicit Settings(QObject *parent = 0);
    Settings(const Settings &);
    Settings& operator=(const Settings &);
    static Settings* m_instance;
    SettingsPrivate *d;
    static const int MAX_RECENTS_COUNT;
};

#endif // SETTINGS_H
