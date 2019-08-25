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

// Widget for StampTool settings.

#ifndef STAMPSETTINGSWIDGET_H
#define STAMPSETTINGSWIDGET_H

#include <QWidget>

namespace Ui {
class StampSettingsWidget;
}

class StampSettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StampSettingsWidget(QWidget *parent = 0);
    ~StampSettingsWidget();

    int radius() const;
    int pressure() const;
    int step() const;
    bool fixed() const;
    bool diffuse() const;
    bool precise() const;
signals:
    void settingsChanged();

private slots:
    void onFixedChanged();
private:
    Ui::StampSettingsWidget *ui;
};

#endif // STAMPSETTINGSWIDGET_H
