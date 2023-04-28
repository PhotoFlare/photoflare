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

// Paint widget class. Handles the drawing area.

//#include <QDebug>

#include <QLabel>
#include <QPainter>
#include <QScrollBar>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsProxyWidget>
#include <QGraphicsEffect>
#include <QImageReader>

#include "../Settings.h"
#include "PaintWidget.h"
#include "./Tool.h"

#include "QProgressIndicator.h"

class PaintWidgetPrivate : public QGraphicsScene
{
public:
    PaintWidgetPrivate(PaintWidget *widget) :
        QGraphicsScene(widget)
    {
        currentTool = 0;
        scale = 1.0f;
        q = widget;
        q->setScene(this);

        isSelectionVisible = true;
        hotspotVisible = false;
    }
    ~PaintWidgetPrivate()
    {
    }

    void initialize(const QImage &image)
    {
        this->image = image;
        q->setSceneRect(image.rect());
        canvas = addPixmap(QPixmap::fromImage(image));
        q->setStyleSheet("background-color: rgb(160, 160, 160);");
    }

    void updateImageCanvas()
    {
        QImage surface = QImage(image.size(), QImage::Format_ARGB32_Premultiplied);
        QPainter painter(&surface);
        QBrush brush;
        brush.setTextureImage(QImage(":/pixmaps/assets/pixmaps/checkers.png"));
        painter.setBrush(brush);
        painter.setCompositionMode(QPainter::CompositionMode_Source);
        painter.fillRect(surface.rect(), brush);
        painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
        painter.drawImage(0, 0, image);
        if(isSelectionVisible)
        {
            float scaledVal = 2.00;
            int cornerSize = 50;

            if(scale < 0.5)
            {
                cornerSize = 100;
                scaledVal = 5.00;
            }
            else if(scale > 1) {
                cornerSize = 20;
                scaledVal = 1.00;
            }

            //Draw the selection hotspots
            if(selection.size() == 4)
            {
                QRect rect(selection.at(0),selection.at(3));
                if(rect.topLeft() != rect.bottomLeft() && hotspotVisible)
                {
                    QPen penbg = QPen(QBrush(), scaledVal, Qt::SolidLine);
                    penbg.setColor(Qt::white);
                    painter.setPen(penbg);
                    painter.setBrush(QBrush());
                    painter.drawPolygon(QRect(selection.at(0).x(),selection.at(0).y(), cornerSize, cornerSize));
                    painter.drawPolygon(QRect(selection.at(1).x()-cornerSize,selection.at(1).y(), cornerSize, cornerSize));
                    painter.drawPolygon(QRect(selection.at(2).x()-cornerSize,selection.at(2).y()-cornerSize, cornerSize, cornerSize));
                    painter.drawPolygon(QRect(selection.at(3).x(),selection.at(3).y()-cornerSize, cornerSize, cornerSize));

                    QPen cornerpen = QPen(QBrush(), scaledVal, Qt::DashLine);
                    cornerpen.setColor(Qt::gray);
                    painter.setPen(cornerpen);
                    painter.setBrush(QBrush());
                    painter.drawPolygon(QRect(selection.at(0).x(),selection.at(0).y(), cornerSize, cornerSize));
                    painter.drawPolygon(QRect(selection.at(1).x()-cornerSize,selection.at(1).y(), cornerSize, cornerSize));
                    painter.drawPolygon(QRect(selection.at(2).x()-cornerSize,selection.at(2).y()-cornerSize, cornerSize, cornerSize));
                    painter.drawPolygon(QRect(selection.at(3).x(),selection.at(3).y()-cornerSize, cornerSize, cornerSize));
                }
            }
            //Grey and white dashed line for visibility no matter the background colours
            QPen penbg = QPen(QBrush(), scaledVal, Qt::SolidLine);
            penbg.setColor(Qt::white);
            painter.setPen(penbg);
            painter.setBrush(QBrush());
            painter.drawPolygon(selection, Qt::WindingFill);

            QPen pen = QPen(QBrush(), scaledVal, Qt::DashLine);
            pen.setColor(Qt::gray);
            painter.setPen(pen);
            painter.setBrush(QBrush());
            painter.drawPolygon(selection, Qt::WindingFill);
        }
        painter.end();
        canvas->setPixmap(QPixmap::fromImage(surface));
    }

    void updateImageCanvasWithOverlay(const QImage &overlayImage, QPainter::CompositionMode mode)
    {
        QImage surface = QImage(image.size(), QImage::Format_ARGB32_Premultiplied);
        QPainter painter(&surface);
        QBrush brush;
        brush.setTextureImage(QImage(":/pixmaps/assets/pixmaps/checkers.png"));
        painter.setBrush(brush);
        painter.setCompositionMode(QPainter::CompositionMode_Source);
        painter.fillRect(surface.rect(), brush);
        painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
        painter.drawImage(0, 0, image);
        painter.setCompositionMode(mode);
        painter.drawImage(0, 0, overlayImage);
        painter.end();
        canvas->setPixmap(QPixmap::fromImage(surface));
    }

    void setImage(const QImage &image)
    {
        if(this->image.size() != image.size())
        {
            q->setSceneRect(image.rect());
        }

        if(indexedMode)
        {
            this->image = image.convertToFormat(QImage::Format_Indexed8);
        }
        else
        {
            this->image = image;
        }
        this->updateImageCanvas();
    }

    void disconnectLastTool()
    {
        Q_ASSERT( QObject::disconnect(lastConnection) );
        Q_ASSERT( QObject::disconnect(lastOverlayConnection) );
        Q_ASSERT( QObject::disconnect(cursorConnection) );
        Q_ASSERT( QObject::disconnect(selectionConnection) );
    }

    void mousePressEvent(QGraphicsSceneMouseEvent *event)
    {
        hotspotVisible = false;
        if (currentTool)
        {
            // Set current image to the tool when we start painting.
            currentTool->setPaintDevice(&image);
            imageChanged = false;
            int s_x = static_cast<int>(event->scenePos().x());
            int s_y = static_cast<int>(event->scenePos().y());
            currentTool->onMousePress(QPoint(s_x, s_y) , event->button());
        }
    }

    void mouseMoveEvent(QGraphicsSceneMouseEvent *event)
    {
        if(event->buttons() != Qt::LeftButton && event->buttons() != Qt::RightButton)
            return;

        if (currentTool)
        {
            int s_x = static_cast<int>(event->scenePos().x());
            int s_y = static_cast<int>(event->scenePos().y());
            currentTool->onMouseMove(QPoint(s_x, s_y));
        }
    }

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
    {
        if (currentTool)
        {
            int s_x = static_cast<int>(event->scenePos().x());
            int s_y = static_cast<int>(event->scenePos().y());
            currentTool->onMouseRelease(QPoint(s_x, s_y));
            if(q->selection().size()==0 && imageChanged)
            {
                q->onContentChanged();
                q->contentChanged();
            }
        }
    }

    void keyPressEvent(QKeyEvent * keyEvent)
    {
        if (currentTool)
            currentTool->onKeyPressed(keyEvent);
    }

    void keyReleaseEvent(QKeyEvent * keyEvent)
    {
        if (currentTool)
            currentTool->onKeyReleased(keyEvent);
    }

    void showGrid(int width)
    {
        if(showGridEnabled)
        {
            for (int i=0; i<lines.length(); i++)
            {
                q->scene()->removeItem(lines[i]);
            }
            showGridEnabled = false;
        }
        else if(width > 0)
        {
            QPen pen = QPen(QBrush(), 0.1, Qt::SolidLine);
            pen.setColor(Qt::gray);
            for (int x=0; x<=image.width(); x+=width)
            {
                xline = q->scene()->addLine(x,0,x,image.height(), pen);
                lines.append(xline);
            }

            for (int y=0; y<=image.height(); y+=width)
            {
                yline = q->scene()->addLine(0,y,image.width(),y, pen);
                lines.append(yline);
            }
            showGridEnabled = true;
        }
    }

    QString imagePath;
    QLabel *imageLabel;
    QImage image;
    Tool *currentTool;
    QMetaObject::Connection lastConnection;
    QMetaObject::Connection lastOverlayConnection;
    QMetaObject::Connection cursorConnection;
    QMetaObject::Connection selectionConnection;
    QGraphicsPixmapItem *canvas;
    float scale;
    bool imageChanged;
    QPolygon selection;
    bool isSelectionVisible;
    bool hotspotVisible;
    bool indexedMode = false;
    bool showGridEnabled = false;
    QGraphicsLineItem *xline;
    QGraphicsLineItem *yline;
    QVector<QGraphicsLineItem*> lines;

    PaintWidget *q;
};

PaintWidget::PaintWidget(const QString &imagePath, QWidget *parent)
    : QGraphicsView(parent)
    , d(new PaintWidgetPrivate(this))
    , progressIndicator(nullptr)
{
    QStringList list = imagePath.split(".");
    QString fileNameNoExt = list[1];

     QString rawExtensions[] = {
         "ARW",
         "BAY",
         "CR2",
         "DCS",
         "MOS",
         "NEF",
         "RAW"
    };
    bool exists = std::find(std::begin(rawExtensions), std::end(rawExtensions), fileNameNoExt.toUpper()) != std::end(rawExtensions);

    if(exists)
    {
        QImageReader reader(imagePath);
        QSize size = reader.size();
        int w = size.width() -1;
        int h = size.height() -1;
        QSize newSize = QSize(w,h);
        reader.setScaledSize(newSize);
        QImage raw = reader.read();
        d->initialize(raw);
    }
    else
    {
        d->initialize(QImage(imagePath));
    }
    d->imagePath = imagePath;
    this->init();
}

PaintWidget::PaintWidget(const QSize &imageSize, const QColor &bgcolor, QWidget *parent)
    : QGraphicsView(parent)
    , d(new PaintWidgetPrivate(this))
    , progressIndicator(nullptr)
{
    QImage image(imageSize, QImage::Format_ARGB32_Premultiplied);
    image.fill(bgcolor);

    d->initialize(image);
    this->init();
}

void PaintWidget::init()
{
    historyIndex = 0;
    historyList.clear();
    historyList.append(d->image);
}

PaintWidget::~PaintWidget()
{
    d->disconnectLastTool();
    delete d;
}

void PaintWidget::setPaintTool(Tool *tool)
{
    if (d->currentTool)
    {
        d->currentTool->disconnect();
        d->disconnectLastTool();
    }

    d->currentTool = tool;

    if (d->currentTool)
    {
        d->lastConnection = connect(d->currentTool, &Tool::painted, [this] (QPaintDevice *paintDevice) {
            if (&d->image == paintDevice)
            {
                d->updateImageCanvas();
                this->contentChanged();
                d->imageChanged = true;
            }
        });
        d->lastOverlayConnection = connect(d->currentTool, &Tool::overlaid, [this] (QPaintDevice *paintDevice, const QImage &overlayImage, QPainter::CompositionMode mode) {
            if (&d->image == paintDevice)
            {
                d->updateImageCanvasWithOverlay(overlayImage, mode);
            }
        });

        d->cursorConnection = connect(d->currentTool, &Tool::cursorChanged, this, &PaintWidget::onCursorChanged);
        d->selectionConnection = connect(d->currentTool, &Tool::selectionChanged, this, &PaintWidget::onSelectionChanged);

        d->currentTool->setScale(d->scale);
        d->currentTool->setPaintDevice(&d->image);
        d->updateImageCanvas();
    }
}

void PaintWidget::setImage(const QImage &image)
{
    d->setImage(image);
    onContentChanged();
    this->contentChanged();
}

void PaintWidget::setImageOriginal(const QImage &image)
{
    // Replace the image without creating any undo steps
    d->setImage(image);
}

QImage PaintWidget::image() const
{
    return d->image;
}

void PaintWidget::onCursorChanged(QCursor cursor)
{
    setCursor(cursor);
}

void PaintWidget::onSelectionChanged(QPolygon poly)
{
    d->selection = poly;
    if(!d->selection.isEmpty())
    {
        setSelectionVisible(true);
    }
}

void PaintWidget::setImagePath(QString path)
{
    d->imagePath = path;
}

QString PaintWidget::imagePath() const
{
    return d->imagePath;
}

void PaintWidget::autoScale()
{
    //Scale paint area to fit window
    float scaleX = (float)this->geometry().width() / (float)image().width();
    float scaleY = (float)this->geometry().height() / (float)image().height();
    float scaleFactor = scaleX < scaleY ? scaleX : scaleY;

    if(scaleFactor < 1)
    {
        d->scale = scaleFactor;
        resetTransform();
        scale(d->scale, d->scale);
    }

    emit zoomChanged(d->scale);

    if(d->currentTool)
        d->currentTool->setScale(d->scale);
}

void PaintWidget::setScale(const QString &rate)
{
    resetTransform();
    d->scale = (rate.mid(0,rate.lastIndexOf("%"))).toFloat() / 100.0f;
    scale(d->scale, d->scale);

    if(d->currentTool)
        d->currentTool->setScale(d->scale);
}

float PaintWidget::getScale()
{
    return d->scale;
}

void PaintWidget::showProgressIndicator(bool visible)
{
    if(visible)
    {
        progressIndicator = new QProgressIndicator;
        progressIndicator->setSize(QSize(d->image.width()/4,d->image.height()/4));
        progressIndicator->setVisible(true);
        progressIndicator->startAnimation();
        QGraphicsProxyWidget *progressIndicatorProxy = scene()->addWidget(progressIndicator);
        progressIndicatorProxy->setPos(3*d->image.width()/8, 3*d->image.height()/8);
    }
    else
    {
        if(progressIndicator)
        {
            progressIndicator->stopAnimation();
            progressIndicator->setVisible(false);
        }
    }
}

void PaintWidget::wheelEvent(QWheelEvent *event)
{
    QPointF pos = event->position();
    const QPointF p0scene = mapToScene(pos.toPoint());
    float scaleFactor = 1.1f;
    bool zoomDirection = false;
    //QPointF delta = event->pixelDelta();

    // Zoom in or out depending on user preference
    if(SETTINGS->getZoomDirection()=="0")
    {
        zoomDirection = true; //delta > 0;
    }
    else
    {
        zoomDirection = false; //delta < 0;
    }

    if(zoomDirection)
    {
       d->scale = d->scale / scaleFactor;
       d->scale = (d->scale < 0.1f) ? 0.1f : d->scale;
    }
    else
    {
       d->scale = d->scale * scaleFactor;
       d->scale = (d->scale > 8) ? 8 : d->scale;
    }

    resetTransform();
    scale(d->scale, d->scale);
    emit zoomChanged(d->scale);

    if(d->currentTool)
        d->currentTool->setScale(d->scale);

    const QPointF p1mouse = mapFromScene(p0scene);
    const QPointF move = p1mouse - event->position();
    horizontalScrollBar()->setValue(move.x() + horizontalScrollBar()->value());
    verticalScrollBar()->setValue(move.y() + verticalScrollBar()->value());
}

void PaintWidget::onContentChanged()
{

    for(int i=historyList.size() - 1; i > historyIndex; i--)
    {
        historyList.removeAt(i);
    }

    historyList.append(d->image);
    historyIndex++;
/*
    if(historyIndex > SETTINGS->getHistoryLimit().toInt())
    {
        historyList.removeFirst();
        historyIndex--;
    }
*/
}

void PaintWidget::revert()
{
    historyIndex = 0;
    d->setImage(historyList.at(historyIndex));
    this->contentChanged();
}

void PaintWidget::undo()
{
    if(isUndoEnabled())
    {
        historyIndex--;
        d->setImage(historyList.at(historyIndex));
        this->contentChanged();
    }
}

void PaintWidget::redo()
{
    if(isRedoEnabled())
    {
        historyIndex++;
        d->setImage(historyList.at(historyIndex));
        this->contentChanged();
    }
}

bool PaintWidget::isUndoEnabled()
{
    return (historyIndex > 0);
}

bool PaintWidget::isRedoEnabled()
{
    return (historyIndex < historyList.size() - 1);
}

int PaintWidget::undoCount()
{
    return historyIndex;
}

void PaintWidget::clearUndoHistory()
{
    historyIndex = 0;
    historyList.clear();
}

void PaintWidget::setSelectionVisible(bool visible)
{
    d->isSelectionVisible = visible;
    d->updateImageCanvas();
    emit selectionChanged(visible);
}

QPolygon PaintWidget::selection()
{
    return d->selection;
}

bool PaintWidget::isSelectionVisible()
{
    return d->isSelectionVisible;
}

void PaintWidget::setHotspotVisble(bool visible)
{
    d->hotspotVisible = visible;
}

void PaintWidget::selectAll()
{
    d->selection = d->image.rect();
    d->updateImageCanvas();
    emit selectionChanged(true);
}

void PaintWidget::setImageModeIndexed(bool mode)
{
    d->indexedMode = mode;
}

void PaintWidget::showGrid(int width)
{
    d->showGrid(width);
}

bool PaintWidget::isGridEnabled()
{
    return d->showGridEnabled;
}
