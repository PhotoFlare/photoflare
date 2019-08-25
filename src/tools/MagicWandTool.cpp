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

// MagicWandTool - Select areas on the image.

#include "MagicWandTool.h"
#include "../mainwindow.h"
#include "../widgets/PaintWidget.h"

class MagicWandToolPrivate
{
public:
    MagicWandToolPrivate()
    {
        tolerance = 0;
        color = false;
    }

    ~MagicWandToolPrivate()
    {
    }

    int tolerance;
    bool color;
};


MagicWandTool::MagicWandTool(QObject *parent)
    : Tool(parent)
    , d(new MagicWandToolPrivate)
{
}

MagicWandTool::~MagicWandTool()
{
    delete d;
}

void MagicWandTool::onMousePress(const QPoint &pos, Qt::MouseButton button)
{
    switch(button)
    {
        case Qt::LeftButton:
            emit selectPrimaryColor(pos, d->tolerance, d->color);
//            if(m_paintDevice)
//            {
//                const QImage *image = dynamic_cast<QImage*>(m_paintDevice);
//                QImage surface = QImage(image->size(), QImage::Format_ARGB32_Premultiplied);
//                QPainter painter(&surface);
//                painter.setCompositionMode(QPainter::CompositionMode_Source);
//                painter.fillRect(surface.rect(), Qt::transparent);
//                QPen pen = QPen(QBrush(), 1, Qt::DashLine);
//                pen.setColor(Qt::gray);
//                painter.setPen(pen);

//                painter.drawPolygon(d->selection, Qt::WindingFill);
//                painter.drawRect(d->selection.boundingRect());
//                painter.end();

//                emit overlaid(m_paintDevice, surface, QPainter::CompositionMode_Difference);
//            }
            break;
        case Qt::RightButton:
            break;
        default:
            break;
    }
}

void MagicWandTool::setSelection(const QPolygon& poly)
{
    //d->selection = poly;
    emit selectionChanged(poly);
    emit painted(m_paintDevice);
}

void MagicWandTool::setTolerance(int tolerance)
{
    d->tolerance = tolerance;
}

void MagicWandTool::setColor(bool color)
{
    d->color = color;
}
