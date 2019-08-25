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

// SmudgeTool - Smudge pixels while dragging the mouse.

#include "SmudgeTool.h"
#include "PaintWidget.h"

#include <QPainter>
#include <QBitmap>
#include <QImage>
#include <QtMath>

class SmudgeToolPrivate
{
public:
    SmudgeToolPrivate()
    {
        primaryPen = QPen(QBrush(), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        pixmap = QPixmap(32,32);
    }
    ~SmudgeToolPrivate()
    {

    }

    int radius;
    int pressure;

    QPoint lastPos;
    QPen primaryPen;

    Qt::MouseButton mouseButton;
    QPixmap pixmap;
    QImage completeImage;

};

SmudgeTool::SmudgeTool(QObject *parent)
    : Tool(parent)
    , d(new SmudgeToolPrivate)
{

}

SmudgeTool::~SmudgeTool()
{
    delete d;
}

QCursor SmudgeTool::getCursor()
{
    int width = d->radius * m_scale;
    if(width < 5)
        width = 5;
    QPixmap pixmap(QSize(width,width));
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    QPen pen = QPen(QBrush(), 1, Qt::DashLine);
    pen.setColor(Qt::gray);
    painter.setPen(pen);

    painter.drawEllipse(pixmap.rect());
    return QCursor(pixmap);
}

void SmudgeTool::setRadius(int radius)
{
    d->radius = radius;
    emit cursorChanged(getCursor());
}

void SmudgeTool::setPressure(int pressure)
{
    d->pressure = pressure;
}

void SmudgeTool::onMousePress(const QPoint &pos, Qt::MouseButton button)
{
    Q_UNUSED(button);

    d->lastPos = pos;
    d->mouseButton = button;
    const QImage *image = dynamic_cast<QImage*>(m_paintDevice);
    d->completeImage = *image;
}

void SmudgeTool::onMouseMove(const QPoint &pos)
{
    if (m_paintDevice)
    {
        float pressure = d->pressure / 20.0f;
        QImage lastImage = d->completeImage.copy(d->lastPos.x() - d->radius/2, d->lastPos.y() - d->radius/2, d->radius, d->radius);
        QImage currImage = lastImage.scaled(int(d->radius * pressure), int(d->radius * pressure));
        QImage pattern(int(d->radius * pressure), int(d->radius * pressure), QImage::Format_ARGB32);
        pattern.fill(Qt::transparent);
        int w = pattern.width();
        for(int i=0; i<w; i++)
        {
            for(int j=0; j<w; j++)
            {
                float l = qSqrt( qPow(i-w/2, 2) + qPow(j-w/2, 2) );
                if(l <= w/2)
                {
                    pattern.setPixel(i, j, currImage.pixel(i, j));
                }
            }
        }

        QPainter painter(m_paintDevice);

        painter.drawPixmap(pos.x()-int(d->radius * pressure / 2), pos.y()-int(d->radius * pressure / 2), d->pixmap.fromImage(pattern));

        d->lastPos = pos;
        emit painted(m_paintDevice);

        const QImage *image = dynamic_cast<QImage*>(m_paintDevice);
        d->completeImage = *image;
    }
}

void SmudgeTool::onMouseRelease(const QPoint &pos)
{
    Q_UNUSED(pos);
    d->mouseButton = Qt::NoButton;
    d->completeImage = QImage();
}

