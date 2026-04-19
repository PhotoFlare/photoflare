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

#ifndef RULERWIDGET_H
#define RULERWIDGET_H

#include <QWidget>

class RulerWidget : public QWidget
{
    Q_OBJECT

public:
    enum class Orientation { Horizontal, Vertical };
    enum class Unit { Pixels, Inches, Centimeters };

    static constexpr int THICKNESS = 20;

    explicit RulerWidget(Orientation orientation, QWidget *parent = nullptr);

    void setScale(float scale);
    void setOrigin(float origin);
    void setUnit(Unit unit);
    void setDpi(float dpi);
    void setMousePos(int pos);
    void clearMousePos();

    Unit unit() const;

signals:
    void unitChanged(Unit unit);

protected:
    void paintEvent(QPaintEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;

private:
    float computeIntervalPx(float minImagePx) const;
    QString formatLabel(float imagePx) const;

    Orientation m_orientation;
    Unit m_unit = Unit::Pixels;
    float m_scale = 1.0f;
    float m_origin = 0.0f;
    float m_dpi = 96.0f;
    int m_mousePos = -1;
};

#endif // RULERWIDGET_H
