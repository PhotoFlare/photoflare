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

#ifndef STAMPTOOL_H
#define STAMPTOOL_H

#include "Tool.h"

class StampToolPrivate;

class StampTool : public Tool
{
public:
    StampTool(QObject *parent = nullptr);
    ~StampTool() override;

    // This color is used when painting the left mouse button
    QColor primaryColor() const;

    // This color is used when painting the right mouse button
    QColor secondaryColor() const;

    void setRadius(int radius);
    void setPressure(int pressure);
    void setStep(int step);
    void setFixed(bool fixed);
    void setPrecise(bool precise);
    void setDiffuse(bool diffuse);
    QCursor getCursor() override;
    void setCapStyle(Qt::PenCapStyle capStyle);
    void onMousePress(const QPoint &pos, Qt::MouseButton button) override;
    void onMouseMove(const QPoint &pos) override;
    void onMouseRelease(const QPoint &pos) override;
    void onKeyPressed(QKeyEvent * keyEvent) override;
    void onKeyReleased(QKeyEvent * keyEvent) override;

public slots:
    void setPrimaryColor(const QColor &color);
    void setSecondaryColor(const QColor &color);

private:
    StampToolPrivate *d;
};

#endif // STAMPTOOL_H
