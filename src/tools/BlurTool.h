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

#ifndef BLURTOOL_H
#define BLURTOOL_H

#include "Tool.h"

class BlurToolPrivate;

class BlurTool : public Tool
{
public:
    BlurTool(QObject *parent = nullptr);
    ~BlurTool() override;

    // This color is used when painting the left mouse button
    QColor primaryColor() const;

    // This color is used when painting the right mouse button
    QColor secondaryColor() const;

    void setRadius(int radius);
    void setPressure(int pressure);
    QCursor getCursor() override;
    void setCapStyle(Qt::PenCapStyle capStyle);
    void onMousePress(const QPoint &pos, Qt::MouseButton button) override;
    void onMouseMove(const QPoint &pos) override;

public slots:
    void setPrimaryColor(const QColor &color);
    void setSecondaryColor(const QColor &color);

private:
    BlurToolPrivate *d;
};



#endif // BLURTOOL_H

