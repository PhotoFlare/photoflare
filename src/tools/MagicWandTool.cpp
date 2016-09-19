#include "MagicWandTool.h"
#include "../mainwindow.h"
#include "../PaintWidget.h"

class MagicWandToolPrivate
{
public:
    QPolygon selection;
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
            emit selectPrimaryColor(pos);
            if(m_paintDevice)
            {
                const QImage *image = dynamic_cast<QImage*>(m_paintDevice);
                QImage surface = QImage(image->size(), QImage::Format_ARGB32_Premultiplied);
                QPainter painter(&surface);
                painter.setCompositionMode(QPainter::CompositionMode_Source);
                painter.fillRect(surface.rect(), Qt::transparent);
                QPen pen = QPen(QBrush(), 1, Qt::DashLine);
                pen.setColor(Qt::gray);
                painter.setPen(pen);

                painter.drawPolygon(d->selection, Qt::WindingFill);
                painter.end();

                emit overlaid(m_paintDevice, surface, QPainter::CompositionMode_Difference);
            }
            break;
        case Qt::RightButton:
            break;
        default:
            break;
    }
}

void MagicWandTool::setSelection(const QPolygon& poly)
{
    d->selection = poly;
}
