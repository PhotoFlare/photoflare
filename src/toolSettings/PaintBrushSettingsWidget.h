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

// Widget for PaintBrushTool settings.

#ifndef PAINTBRUSHSETTINGSWIDGET_H
#define PAINTBRUSHSETTINGSWIDGET_H

#include <QWidget>

namespace Ui {
class PaintBrushSettingsWidget;
}

class PaintBrushSettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PaintBrushSettingsWidget(QWidget *parent = nullptr);
    ~PaintBrushSettingsWidget();

    void setAntialiasing(bool value);
    bool antialiasing() const;
    void setBrushWidth(int width);
    int brushWidth() const;
    int brushPressure() const;
    Qt::PenCapStyle brushCapStyle();

signals:
    void settingsChanged();

private slots:
    void on_buttonRoundCap_clicked();
    void on_buttonSquareCap_clicked();
    void on_pressureSlider_valueChanged(int value);

private:
    void setLabelRadiusValue(int value);
    Ui::PaintBrushSettingsWidget *ui;
};

#endif // PAINTBRUSHSETTINGSWIDGET_H
