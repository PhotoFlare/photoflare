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

#ifndef PAINTBRUSHADVTOOL_H
#define PAINTBRUSHADVTOOL_H

#include "Tool.h"

class PaintBrushAdvToolPrivate;

class PaintBrushAdvTool : public Tool
{
public:
    PaintBrushAdvTool(QObject *parent = 0);
    ~PaintBrushAdvTool();

    void setWidth(int width);
    int width() const;

    // This color is used when painting the left mouse button
    QColor primaryColor() const;

    // This color is used when painting the right mouse button
    QColor secondaryColor() const;

    void setBrushPixmap(QPixmap pixmap);

    void setCapStyle(Qt::PenCapStyle capStyle);
    void setPressure(int pressure);
    void setFade(bool fade);
    void setStep(int step);

    QCursor getCursor() override;

    void onMousePress(const QPoint &pos, Qt::MouseButton button) override;
    void onMouseMove(const QPoint &pos) override;
    void onMouseRelease(const QPoint &pos) override;

public slots:
    void setPrimaryColor(const QColor &color);
    void setSecondaryColor(const QColor &color);

private:
    PaintBrushAdvToolPrivate *d;
};



#endif // PAINTBRUSHADVTOOL_H

