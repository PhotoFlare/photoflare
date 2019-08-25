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

//Widget for AdvancedPaintbrushTool settings.

#ifndef PAINTBRUSHADVSETTINGSWIDGET_H
#define PAINTBRUSHADVSETTINGSWIDGET_H

#include <QWidget>

namespace Ui {
class PaintBrushAdvSettingsWidget;
}

class PaintBrushAdvSettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PaintBrushAdvSettingsWidget(QWidget *parent = 0);
    ~PaintBrushAdvSettingsWidget();

    QPixmap brushPixmap();
    int brushWidth() const;
    int pressure() const;
    bool fade() const;
    int step() const;
signals:
    void settingsChanged();

private slots:

private:

    Ui::PaintBrushAdvSettingsWidget *ui;
};

#endif // PAINTBRUSHADVSETTINGSWIDGET_H
