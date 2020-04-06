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

// Widget for SmudgeTool settings.

#ifndef SMUDGESETTINGSWIDGET_H
#define SMUDGESETTINGSWIDGET_H

#include <QWidget>

namespace Ui {
class SmudgeSettingsWidget;
}

class SmudgeSettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SmudgeSettingsWidget(QWidget *parent = nullptr);
    ~SmudgeSettingsWidget();

    int radius() const;
    int pressure() const;

signals:
    void settingsChanged();

private:
    Ui::SmudgeSettingsWidget *ui;
};

#endif // SMUDGESETTINGSWIDGET_H
