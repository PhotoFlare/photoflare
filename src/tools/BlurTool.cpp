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

// BlurTool - Blur while dragging the mouse.

#include "BlurTool.h"
#include "PaintWidget.h"

#include <QPainter>
#include <QGraphicsBlurEffect>
#include <QGraphicsPixmapItem>

class BlurToolPrivate
{
public:
    BlurToolPrivate()
    {
        primaryPen = QPen(QBrush(), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        secondaryPen = QPen(QBrush(), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    }
    QPoint lastPos;
    QPen primaryPen;
    QPen secondaryPen;
    int radius;
    int pressure;
    QImage origin;
};

BlurTool::BlurTool(QObject *parent)
    : Tool(parent)
    , d(new BlurToolPrivate){}

BlurTool::~BlurTool()
{
    delete d;
}

void BlurTool::setPrimaryColor(const QColor &color)
{
    d->primaryPen.setColor(color);
}

QColor BlurTool::primaryColor() const
{
    return d->primaryPen.color();
}

void BlurTool::setSecondaryColor(const QColor &color)
{
    d->secondaryPen.setColor(color);
}

QColor BlurTool::secondaryColor() const
{
    return d->secondaryPen.color();
}

void BlurTool::setRadius(int radius)
{
    d->radius = radius;
    emit cursorChanged(getCursor());
}

void BlurTool::setPressure(int pressure)
{
    d->pressure = pressure;
}

void BlurTool::setCapStyle(Qt::PenCapStyle capStyle)
{
    d->primaryPen.setCapStyle(capStyle);
    d->secondaryPen.setCapStyle(capStyle);
}

QCursor BlurTool::getCursor()
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

void BlurTool::onMousePress(const QPoint &pos, Qt::MouseButton button)
{
    Q_UNUSED(button);
    d->lastPos = pos;
    const QImage *image = dynamic_cast<QImage*>(m_paintDevice);
    d->origin = *image;
    onMouseMove(pos);
}

void BlurTool::onMouseMove(const QPoint &pos)
{
    if (m_paintDevice)
    {
        const QImage *image = dynamic_cast<QImage*>(m_paintDevice);
        QPainter painter(m_paintDevice);

        QPixmap pixmap(QSize(d->radius,d->radius));
        pixmap.fill(Qt::transparent);
        QPainter painterPix(&pixmap);
        painterPix.setRenderHint(QPainter::Antialiasing);
        QPen pen = QPen(QBrush(), 0);

        for(int i=-d->radius/2; i < d->radius/2; i++)
        {
            for(int j=-d->radius/2; j < d->radius/2; j++)
            {
                if(pos.x() + i < 0 || pos.y() + j < 0)
                    continue;

                if(pos.x() + i >= image->width() || pos.y() + j >= image->height())
                    continue;

                if(i*i + j*j > d->radius*d->radius/4)
                    continue;
                pen.setColor(image->pixel(pos.x() + i, pos.y() + j));
                painterPix.setPen(pen);
                painterPix.setOpacity(((float)1 - ((float)(i*i + j*j) / (float)(d->radius*d->radius/4))));
                painterPix.drawPoint(i+d->radius/2, j+d->radius/2);
            }
        }

        QGraphicsBlurEffect *effect = new QGraphicsBlurEffect();
        effect->setBlurHints(QGraphicsBlurEffect::QualityHint);
        effect->setBlurRadius(2.f);

        QGraphicsScene scene;
        QGraphicsPixmapItem item;
        item.setPixmap(pixmap);
        item.setGraphicsEffect(effect);
        scene.addItem(&item);
        scene.render(&painterPix, QRectF(), QRectF( 0, 0, d->radius, d->radius));

        painter.setOpacity((float)d->pressure / 20.f);
        painter.drawPixmap(pos.x()-d->radius/2, pos.y()-d->radius/2, pixmap);

        emit painted(m_paintDevice);
    }
}
