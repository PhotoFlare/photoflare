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
    void setDefaultSettings();
    void setCutoutApiKey(const QString val);
    QString getCutoutApiKey() const;
    void setCutoutEnabled(bool enabled);
    bool isCutoutEnabled() const;
    void setSDKEnabled(bool enabled);
    bool isSDKEnabled() const;
    void setMaximizeWindow(bool maximize);
    bool isMaximizeWindow() const;
    void setMultiWindowMode(bool multiWindowMode);
    bool isMultiWindowMode() const;
    void setCustomWindowGeometry(const QRect &rect);
    QRect customWindowGeometry() const;
    void addRecentFile(const QString &file);
    QList<QVariant> getRecentFiles() const;
    void setUserLanguage(const QString language);
    QString getUserLanguage() const;
    void setOpenFolder(const QString folderpath);
    QString getOpenFolder() const;
    void setPreviouslyOpened(bool enabled);
    bool getPreviouslyOpened() const;
    void setPreviouslyOpenedSave(bool enabled);
    bool getPreviouslyOpenedSave() const;
    void setSaveFolder(const QString folderpath);
    QString getSaveFolder() const;
    void setSaveFormat(const QString format);
    QString getSaveFormat() const;
    void setSaveFormatEnabled(bool enabled);
    bool getSaveFormatEnabled() const;
    void setCompressionDialogEnabled(bool enabled);
    bool getCompressionDialogEnabled() const;
    void setCompressionDefaultEnabled(bool enabled);
    bool getCompressionDefaultEnabled() const;
    void setCompressionDefaultValue(const QString enabled);
    QString getCompressionDefaultValue() const;
    void setHistoryLimit(const QString val);
    QString getHistoryLimit() const;
    void setDockLayout(const QString val);
    QString getDockLayout() const;
    void setZoomDirection(const QString val);
    QString getZoomDirection() const;
    void setMemParamsEnabled(bool enabled);
    bool getMemParamsEnabled() const;
    void setUnit(int);
    int getUnit() const;

private:
    explicit Settings(QObject *parent = 0);
    Settings(const Settings &);
    Settings& operator=(const Settings &);
    static Settings* m_instance;
    SettingsPrivate *d;
    static const int MAX_RECENTS_COUNT;
signals:
    void multiWindowModeChanged(bool multiWindowMode);
};

#endif // SETTINGS_H
