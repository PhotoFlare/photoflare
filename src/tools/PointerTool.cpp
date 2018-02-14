/*

  PointerTool - Selection and cropping.

*/

#include "PointerTool.h"

#include <QApplication>
#include <QClipboard>
#include <QPainter>
#include <QMenu>

//#include <QDebug>

enum SelectionMode {SELECT, HAND, RESIZE, STROKE, FILL};
enum Corner {TOP_LEFT, TOP_RIGHT, BOTTOM_RIGHT, BOTTOM_LEFT};

class PointerToolPrivate
{
public:
    PointerToolPrivate()
    {
        selectionMode = SELECT;
    }
    ~PointerToolPrivate()
    {
    }

    QPoint firstPos;
    QPoint secondPos;
    SelectionMode selectionMode;
    QImage image;
    QPoint imagePos;

    QRect topLeftCorner;
    QRect topRightCorner;
    QRect bottomRightCorner;
    QRect bottomLeftCorner;

    Corner corner;
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

            if(d->topLeftCorner.contains(pos))
            {
                d->corner = TOP_LEFT;
                d->selectionMode = RESIZE;
                emit cursorChanged(Qt::ClosedHandCursor);
            }
            else if(d->topRightCorner.contains(pos))
            {
                d->corner = TOP_RIGHT;
                d->selectionMode = RESIZE;
                emit cursorChanged(Qt::ClosedHandCursor);
            }
            else if(d->bottomLeftCorner.contains(pos))
            {
                d->corner = BOTTOM_LEFT;
                d->selectionMode = RESIZE;
                emit cursorChanged(Qt::ClosedHandCursor);
            }
            else if(d->bottomRightCorner.contains(pos))
            {
                d->corner = BOTTOM_RIGHT;
                d->selectionMode = RESIZE;
                emit cursorChanged(Qt::ClosedHandCursor);
            }

            if(d->selectionMode == SELECT)
            {
                emit painted(m_paintDevice);
            }
            else if(d->selectionMode == HAND)
            {
                QRect rect = QRect(d->imagePos.x(), d->imagePos.y(), d->image.width(), d->image.height());
                if(!rect.contains(pos))
                {
                    d->selectionMode = SELECT;
                    if (m_paintDevice)
                    {
                        QPainter painter(m_paintDevice);
                        painter.drawImage(d->imagePos.x(), d->imagePos.y(), d->image);
                        painter.end();

                        emit painted(m_paintDevice);
                    }
                }
                else
                {
                    emit cursorChanged(Qt::OpenHandCursor);
                }
            }
            break;
        case Qt::RightButton: {
            QMenu contextMenu("default");
            QAction crop(tr("Crop"), this);
            contextMenu.addAction(&crop);
            crop.setDisabled(true);
            if(d->firstPos != d->secondPos)
            {
                crop.setDisabled(false);
            }
            QAction sep0(this);
            sep0.setSeparator(true);
            QAction save(tr("Save"), this);
            QAction saveAs(tr("Save as"), this);
            QAction close(tr("Close"), this);
            QAction sep1(this);
            sep1.setSeparator(true);
            QAction copy(tr("Copy"), this);
            QAction paste(tr("Paste"), this);
            QAction sep2(this);
            sep2.setSeparator(true);
            QAction imageSize(tr("Image size"), this);
            QAction canvasSize(tr("Canvas size"), this);
            QAction sep3(this);
            sep3.setSeparator(true);
            QAction undo(tr("Undo"), this);
            QAction redo(tr("Redo"), this);

            contextMenu.addAction(&save);
            contextMenu.addAction(&saveAs);
            contextMenu.addAction(&close);
            contextMenu.addAction(&sep1);
            contextMenu.addAction(&imageSize);
            contextMenu.addAction(&canvasSize);
            contextMenu.addAction(&sep2);
            contextMenu.addAction(&copy);
            contextMenu.addAction(&paste);
            contextMenu.addAction(&sep3);
            contextMenu.addAction(&undo);
            contextMenu.addAction(&redo);

            connect(&crop, SIGNAL(triggered()), this, SLOT(onCrop()));
            connect(&save, SIGNAL(triggered()), this, SLOT(onSave()));
            connect(&saveAs, SIGNAL(triggered()), this, SLOT(onSaveAs()));
            connect(&close, SIGNAL(triggered()), this, SLOT(onClose()));
            connect(&imageSize, SIGNAL(triggered()), this, SLOT(onImageSize()));
            connect(&canvasSize, SIGNAL(triggered()), this, SLOT(onCanvasSize()));
            connect(&copy, SIGNAL(triggered()), this, SLOT(onCopy()));

            QClipboard *clipboard = QApplication::clipboard();
            if(!clipboard->image().isNull())
            {
                connect(&paste, SIGNAL(triggered()), this, SLOT(onPaste()));
                contextMenu.addAction(&paste);
            }
            connect(&undo, SIGNAL(triggered()), this, SLOT(onUndo()));
            connect(&redo, SIGNAL(triggered()), this, SLOT(onRedo()));

            contextMenu.exec(QCursor::pos());
            }
        break;
    }
}

void PointerTool::onCrop()
{
    const QRect &rect = QRect(d->firstPos, d->secondPos);
    d->secondPos = d->firstPos;
    emit selectionChanged(QRect());
    emit crop(rect);
}

void PointerTool::onSave()
{
    emit save();
}

void PointerTool::onSaveAs()
{
    emit saveAs();
}

void PointerTool::onClose()
{
    emit close();
}

void PointerTool::onImageSize()
{
    emit imageSize();
}

void PointerTool::onCanvasSize()
{
    emit canvasSize();
}

void PointerTool::onCopy()
{
    emit copy();
}

void PointerTool::onPaste()
{
    emit paste();
}


void PointerTool::onUndo()
{
    emit undo();
}

void PointerTool::onRedo()
{
    emit redo();
}

void PointerTool::setOverlayImage(const QImage& image)
{
    d->selectionMode = HAND;
    d->image = image;
    d->imagePos = QPoint(0,0);
    emit overlaid(m_paintDevice, d->image, QPainter::CompositionMode_SourceOver);
}

void PointerTool::onMouseMove(const QPoint &pos)
{
    d->secondPos = pos;

    if (m_paintDevice)
    {
        if(d->selectionMode == HAND)
        {
            const QImage *image = dynamic_cast<QImage*>(m_paintDevice);
            QImage surface = QImage(image->size(), QImage::Format_ARGB32_Premultiplied);
            QPainter painter(&surface);
            painter.setCompositionMode(QPainter::CompositionMode_Source);
            painter.fillRect(surface.rect(), Qt::transparent);
            QPen pen = QPen(QBrush(), (5*m_scale), Qt::DashLine);
            pen.setColor(Qt::red);
            painter.setPen(pen);

            QRect rect = QRect(d->imagePos.x() + d->secondPos.x() - d->firstPos.x(), d->imagePos.y() + d->secondPos.y() - d->firstPos.y(),
                               d->image.width(), d->image.height());
            painter.drawImage(rect, d->image);
            painter.drawRect(rect);
            painter.end();

            emit overlaid(m_paintDevice, surface, QPainter::CompositionMode_SourceOver);
        }
        else if(d->selectionMode == SELECT)
        {
            QPoint topLeft(d->firstPos);
            QPoint bottomRight(d->secondPos);

            emit selectionChanged(QRect(topLeft,bottomRight));
            emit painted(m_paintDevice);
        }
        else if(d->selectionMode == RESIZE)
        {
            if(d->corner == TOP_LEFT)
            {
                d->topLeftCorner.moveTo(d->secondPos);
                d->topRightCorner.moveTo(QPoint(d->topRightCorner.x(), d->topLeftCorner.y()));
                d->bottomLeftCorner.moveTo(QPoint(d->topLeftCorner.x(), d->bottomLeftCorner.y()));
            }
            else if(d->corner == TOP_RIGHT)
            {
                d->topRightCorner.moveTo(d->secondPos);
                d->topLeftCorner.moveTo(QPoint(d->topLeftCorner.x(), d->topRightCorner.y()));
                d->bottomRightCorner.moveTo(QPoint(d->topRightCorner.x(), d->bottomRightCorner.y()));
            }
            else if(d->corner == BOTTOM_RIGHT)
            {
                d->bottomRightCorner.moveTo(d->secondPos);
                d->topRightCorner.moveTo(QPoint(d->bottomRightCorner.x(), d->topRightCorner.y()));
                d->bottomLeftCorner.moveTo(QPoint(d->bottomLeftCorner.x(), d->bottomRightCorner.y()));
            }
            else if(d->corner == BOTTOM_LEFT)
            {
                d->bottomLeftCorner.moveTo(d->secondPos);
                d->topLeftCorner.moveTo(QPoint(d->bottomLeftCorner.x(), d->topLeftCorner.y()));
                d->bottomRightCorner.moveTo(QPoint(d->bottomRightCorner.x(), d->bottomLeftCorner.y()));
            }

            emit selectionChanged(QRect(d->topLeftCorner.topLeft(),d->bottomRightCorner.bottomRight()));
            emit painted(m_paintDevice);
        }
        else if(d->selectionMode == STROKE)
        {
           // paint using PaintWidget
            QPoint topLeft(d->firstPos);
            QPoint bottomRight(d->secondPos);
            emit selectionChanged(QRect(topLeft,bottomRight));
        }
    }
}

void PointerTool::onMouseRelease(const QPoint &pos)
{
    Q_UNUSED(pos);
    emit cursorChanged(Qt::ArrowCursor);

    if(d->selectionMode == HAND)
    {
        d->imagePos = QPoint(d->imagePos.x() + d->secondPos.x() - d->firstPos.x(), d->imagePos.y() + d->secondPos.y() - d->firstPos.y());
        d->firstPos = d->secondPos;

        emit cursorChanged(Qt::ArrowCursor);
    }
    else if(d->selectionMode == SELECT)
    {
        if(d->firstPos == d->secondPos)
        {
            emit selectionChanged(QRect());
        }
        else
        {
            QPoint topLeft(d->firstPos);
            QPoint bottomRight(d->secondPos);
            int temp;
            if(topLeft.x() > bottomRight.x()) {temp = topLeft.x();topLeft.setX(bottomRight.x());bottomRight.setX(temp);}
            if(topLeft.y() > bottomRight.y()) {temp = topLeft.y();topLeft.setY(bottomRight.y());bottomRight.setY(temp);}
            QPolygon selection = QPolygon(QRect(topLeft, bottomRight));
            emit selectionChanged(selection);

            int cornerSize = 50;

            d->topLeftCorner = QRect(selection.at(0).x(),selection.at(0).y(), cornerSize, cornerSize);
            d->topRightCorner = QRect(selection.at(1).x()-cornerSize,selection.at(1).y(), cornerSize, cornerSize);
            d->bottomRightCorner = QRect(selection.at(2).x()-cornerSize,selection.at(2).y()-cornerSize, cornerSize, cornerSize);
            d->bottomLeftCorner = QRect(selection.at(3).x(),selection.at(3).y()-cornerSize, cornerSize, cornerSize);
        }
        emit painted(m_paintDevice);
    }
    else if(d->selectionMode == RESIZE)
    {
        d->firstPos = d->topLeftCorner.topLeft();
        d->secondPos = d->bottomRightCorner.bottomRight();
        d->selectionMode = SELECT;
    }
}

void PointerTool::setStroke(bool enabled)
{
    if(enabled)
    {
        d->selectionMode = STROKE;
    }
}

void PointerTool::setFill(bool enabled)
{
    if(enabled)
    {
        d->selectionMode = FILL;
    }
}


