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

#ifndef LINETOOL_H
#define LINETOOL_H

#include "Tool.h"

class LineToolPrivate;

class LineTool : public Tool
{
    Q_OBJECT
public:
    LineTool(QObject *parent = nullptr);
    ~LineTool() override;

    void onMousePress(const QPoint &pos, Qt::MouseButton button) override;
    void onMouseMove(const QPoint &pos) override;
    void onMouseRelease(const QPoint &pos) override;
    QCursor getCursor() override;

public slots:
    void setPrimaryColor(const QColor &color);
    void setSecondaryColor(const QColor &color);
    void setWidth(int width);
    void setAntialias(bool antialias);
    void setOpacity(int opacity);
    void setStyle(int style);
    void setArrowStyle(int arrowStyle);
    void drawArrow(QPainter& painter, QPoint from, QPoint to, QPoint& pOutMedian);

private:
    LineToolPrivate *d;
};

#endif // LINETOOL_H

