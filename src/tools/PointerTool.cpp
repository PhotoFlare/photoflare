#include "PointerTool.h"

#include <QPainter>
#include <QMenu>

class PointerToolPrivate
{
public:
    PointerToolPrivate()
    {
    }
    ~PointerToolPrivate()
    {
    }

    QPoint firstPos;
    QPoint secondPos;
};

PointerTool::PointerTool(QObject *parent)
    : Tool(parent)
    , d(new PointerToolPrivate)
{

}

PointerTool::~PointerTool()
{
    delete d;
}

void PointerTool::onMousePress(const QPoint &pos, Qt::MouseButton button)
{
    switch(button)
    {
        case Qt::LeftButton:
            d->firstPos = pos;
            d->secondPos = pos;
            emit painted(m_paintDevice);
            break;
        case Qt::RightButton: {
            if(d->firstPos != d->secondPos)
            {
                QMenu contextMenu("crop");
                QAction crop("Crop", this);
                connect(&crop, SIGNAL(triggered()), this, SLOT(onCrop()));
                contextMenu.addAction(&crop);
                contextMenu.exec(QCursor::pos());
            }}
            break;
        default:
            break;
    }
}

void PointerTool::onCrop()
{
    const QRect &rect = QRect(d->firstPos, d->secondPos);
    d->secondPos = d->firstPos;
    emit crop(rect);
}

void PointerTool::onMouseMove(const QPoint &pos)
{
    d->secondPos = pos;

    if (m_paintDevice) {

        const QImage *image = dynamic_cast<QImage*>(m_paintDevice);
        QImage surface = QImage(image->size(), QImage::Format_ARGB32_Premultiplied);
        QPainter painter(&surface);

        QPen pen = QPen(QBrush(), 1, Qt::DashLine);
        pen.setColor(Qt::black);
        painter.setPen(pen);

        painter.fillRect(surface.rect(), Qt::white);
        painter.drawRect(QRect(d->firstPos, pos));
        painter.end();

        emit overlaid(surface);
    }
}

void PointerTool::onMouseRelease(const QPoint &pos)
{
    Q_UNUSED(pos);
}



