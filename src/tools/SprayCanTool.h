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

#ifndef SPRAYCANTOOL_H
#define SPRAYCANTOOL_H

#include "Tool.h"

class SprayCanToolPrivate;

class SprayCanTool : public Tool
{
public:
    SprayCanTool(QObject *parent = 0);
    ~SprayCanTool();

    // This color is used when painting the left mouse button
    QColor primaryColor() const;

    // This color is used when painting the right mouse button
    QColor secondaryColor() const;

    void setRadius(int radius);
    void setPressure(int pressure);
    void setDispersion(int dispersion);
    void setRainbow(bool rainbow);

    void onMousePress(const QPoint &pos, Qt::MouseButton button) override;
    void onMouseMove(const QPoint &pos) override;
    void onMouseRelease(const QPoint &pos) override;
    QCursor getCursor() override;

public slots:
    void setPrimaryColor(const QColor &color);
    void setSecondaryColor(const QColor &color);

private:
    void fillPattern();
    void timerEvent(QTimerEvent *event) override;
private:
    SprayCanToolPrivate *d;
};



#endif // SPRAYCANTOOL_H

