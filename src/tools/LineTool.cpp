#include "LineTool.h"

#include <QApplication>
#include <QClipboard>
#include <QPainter>
#include <QMenu>

class LineToolPrivate
{
public:
    LineToolPrivate()
    {
    }
    ~LineToolPrivate()
    {
    }

    QPoint firstPos;
    QPoint secondPos;
    bool handMode;
    QImage image;
    QPoint imagePos;
};

LineTool::LineTool(QObject *parent)
    : Tool(parent)
    , d(new LineToolPrivate)
{

}

LineTool::~LineTool()
{
    delete d;
}

void LineTool::onMousePress(const QPoint &pos, Qt::MouseButton button)
{
    switch(button)
    {
        case Qt::LeftButton:
            d->firstPos = pos;
            d->secondPos = pos;
            if(!d->handMode) {
                emit painted(m_paintDevice);
            } else {
                QRect rect = QRect(d->imagePos.x(), d->imagePos.y(), d->image.width(), d->image.height());
                if(!rect.contains(pos)) {
                    d->handMode = false;
                    if (m_paintDevice) {
                        QPainter painter(m_paintDevice);
                        painter.drawImage(d->imagePos.x(), d->imagePos.y(), d->image);
                        painter.end();

                        emit painted(m_paintDevice);
                    }
                }
            }
            break;
        case Qt::RightButton: {
            if(d->firstPos != d->secondPos)
            {
                QMenu contextMenu("crop");
                QAction crop("Crop", this);
                connect(&crop, SIGNAL(triggered()), this, SLOT(onCrop()));
                contextMenu.addAction(&crop);
                contextMenu.exec(QCursor::pos());
            } else
            {
                QMenu contextMenu("copy");
                QAction copy("Copy", this);
                QAction paste("Paste", this);

                connect(&copy, SIGNAL(triggered()), this, SLOT(onCopy()));
                contextMenu.addAction(&copy);

                QClipboard *clipboard = QApplication::clipboard();
                if(!clipboard->image().isNull())
                {
                    connect(&paste, SIGNAL(triggered()), this, SLOT(onPaste()));
                    contextMenu.addAction(&paste);
                }

                contextMenu.exec(QCursor::pos());
            }}
            break;
        default:
            break;
    }
}

void LineTool::onCrop()
{
    const QRect &rect = QRect(d->firstPos, d->secondPos);
    d->secondPos = d->firstPos;
    emit crop(rect);
}

void LineTool::onCopy()
{
    emit copy();
}

void LineTool::onPaste()
{
    emit paste();
}

void LineTool::setOverlayImage(const QImage& image)
{
    d->handMode = true;
    d->image = image;
    d->imagePos = QPoint(0,0);
    emit overlaid(d->image, QPainter::CompositionMode_SourceOver);
}

void LineTool::onMouseMove(const QPoint &pos)
{
    d->secondPos = pos;

    if (m_paintDevice) {

        if(d->handMode)
        {
            const QImage *image = dynamic_cast<QImage*>(m_paintDevice);
            QImage surface = QImage(image->size(), QImage::Format_ARGB32_Premultiplied);
            QPainter painter(&surface);
            painter.setCompositionMode(QPainter::CompositionMode_Source);
            painter.fillRect(surface.rect(), Qt::transparent);
            QPen pen = QPen(QBrush(), 1, Qt::DashLine);
            pen.setColor(Qt::red);
            painter.setPen(pen);

            QRect rect = QRect(d->imagePos.x() + d->secondPos.x() - d->firstPos.x(), d->imagePos.y() + d->secondPos.y() - d->firstPos.y(),
                               d->image.width(), d->image.height());
            painter.drawImage(rect, d->image);
            painter.drawRect(rect);
            painter.end();

            emit overlaid(surface, QPainter::CompositionMode_SourceOver);
        } else
        {
            const QImage *image = dynamic_cast<QImage*>(m_paintDevice);
            QImage surface = QImage(image->size(), QImage::Format_ARGB32_Premultiplied);
            QPainter painter(&surface);
            painter.setCompositionMode(QPainter::CompositionMode_Source);
            painter.fillRect(surface.rect(), Qt::transparent);
            QPen pen = QPen(QBrush(), 1, Qt::DashLine);
            pen.setColor(Qt::gray);
            painter.setPen(pen);
            painter.drawRect(QRect(d->firstPos, d->secondPos));
            painter.end();

            emit overlaid(surface, QPainter::CompositionMode_Difference);
        }
    }
}

void LineTool::onMouseRelease(const QPoint &pos)
{
    Q_UNUSED(pos);
    if(d->handMode)
    {
        d->imagePos = QPoint(d->imagePos.x() + d->secondPos.x() - d->firstPos.x(), d->imagePos.y() + d->secondPos.y() - d->firstPos.y());
        d->firstPos = d->secondPos;
    }
}



