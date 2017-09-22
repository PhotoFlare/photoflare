/*

  Paint widget class. Handles the drawing area.

*/

#include <QLabel>
#include <QPainter>
#include <QScrollBar>
#include <QMouseEvent>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsProxyWidget>
#include <QImageReader>

#include "PaintWidget.h"
#include "./tools/Tool.h"

#include "QProgressIndicator.h"
#include "Settings.h"

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
        selection = QRect();
    }
    ~PaintWidgetPrivate()
    {
    }

    void initialize(const QImage &image)
    {
        this->image = image;
        q->setSceneRect(image.rect());
        canvas = addPixmap(QPixmap::fromImage(image));

        q->setStyleSheet("background-color: rgb(128, 128, 128);");
    }

    void updateImageLabel()
    {
        QImage surface = QImage(image.size(), QImage::Format_ARGB32_Premultiplied);
        QPainter painter(&surface);
        QBrush brush;
        brush.setTextureImage(QImage(":/pixmaps/pixmaps/checkers.png"));
        painter.setBrush(brush);
        painter.setCompositionMode(QPainter::CompositionMode_Source);
        painter.fillRect(surface.rect(), brush);
        painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
        painter.drawImage(0, 0, image);
        if(isSelectionVisible)
        {
            QPen pen = QPen(QBrush(), 1, Qt::DashLine);
            pen.setColor(Qt::gray);
            painter.setPen(pen);
            painter.setBrush(QBrush());
            painter.drawPolygon(selection, Qt::WindingFill);
            if(selection.size() == 4)
            {
                QRect rect(selection.at(0),selection.at(3));
                if(rect.topLeft() != rect.bottomLeft())
                {
                    painter.setPen(QPen(Qt::gray));
                    painter.setBrush(QBrush(Qt::gray));
                    painter.drawPolygon(QRect(selection.at(0).x(),selection.at(0).y(), 10, 10));
                    painter.drawPolygon(QRect(selection.at(1).x()-10,selection.at(1).y(), 10, 10));
                    painter.drawPolygon(QRect(selection.at(2).x()-10,selection.at(2).y()-10, 10, 10));
                    painter.drawPolygon(QRect(selection.at(3).x(),selection.at(3).y()-10, 10, 10));
                }
            }
        }
        painter.end();
        canvas->setPixmap(QPixmap::fromImage(surface));
    }

    void updateImageLabelWithOverlay(const QImage &overlayImage, QPainter::CompositionMode mode)
    {
        QImage surface = QImage(image.size(), QImage::Format_ARGB32_Premultiplied);
        QPainter painter(&surface);
        QBrush brush;
        brush.setTextureImage(QImage(":/pixmaps/pixmaps/checkers.png"));
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

        this->image = image;
        this->updateImageLabel();
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
        if (currentTool)
        {
            // Set current image to the tool when we start painting.
            currentTool->setPaintDevice(&image);
            imageChanged = false;
            currentTool->onMousePress(QPoint(event->scenePos().x(), event->scenePos().y()) , event->button());
        }
    }

    void mouseMoveEvent(QGraphicsSceneMouseEvent *event)
    {
        if(event->buttons() != Qt::LeftButton && event->buttons() != Qt::RightButton)
            return;

        if (currentTool)
            currentTool->onMouseMove(QPoint(event->scenePos().x(), event->scenePos().y()));
    }

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
    {
        if (currentTool)
        {
            currentTool->onMouseRelease(QPoint(event->scenePos().x(), event->scenePos().y()));
            if(imageChanged)
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
                    d->updateImageLabel();
                    this->contentChanged();
                    d->imageChanged = true;
                }
            });
        d->lastOverlayConnection = connect(d->currentTool, &Tool::overlaid, [this] (QPaintDevice *paintDevice, const QImage &overlayImage, QPainter::CompositionMode mode) {
                if (&d->image == paintDevice)
                {
                    d->updateImageLabelWithOverlay(overlayImage, mode);
                }
            });

        d->cursorConnection = connect(d->currentTool, &Tool::cursorChanged, this, &PaintWidget::onCursorChanged);

        d->selectionConnection = connect(d->currentTool, &Tool::selectionChanged, this, &PaintWidget::onSelectionChanged);

        d->currentTool->setScale(d->scale);
        d->currentTool->setPaintDevice(&d->image);
        d->updateImageLabel();
    }
}

void PaintWidget::setImage(const QImage &image)
{
    d->setImage(image);
    onContentChanged();
    this->contentChanged();
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
    setSelectionVisible(d->selection.first() != d->selection.last());
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
        resetMatrix();
        scale(d->scale, d->scale);
    }

    emit zoomChanged(d->scale);

    if(d->currentTool)
        d->currentTool->setScale(d->scale);
}

void PaintWidget::setScale(const QString &rate)
{
    resetMatrix();
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
    const QPointF p0scene = mapToScene(event->pos());
    float scaleFactor = 1.1f;

    if(event->delta() > 0)
    {
       d->scale = d->scale / scaleFactor;
       d->scale = (d->scale < 0.1f) ? 0.1f : d->scale;
    }
    else
    {
       d->scale = d->scale * scaleFactor;
       d->scale = (d->scale > 8) ? 8 : d->scale;
    }

    resetMatrix();
    scale(d->scale, d->scale);
    emit zoomChanged(d->scale);

    if(d->currentTool)
        d->currentTool->setScale(d->scale);

    const QPointF p1mouse = mapFromScene(p0scene);
    const QPointF move = p1mouse - event->pos();
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

void PaintWidget::setSelectionVisible(bool visible)
{
    d->isSelectionVisible = visible;
    d->updateImageLabel();
    emit selectionChanged(visible);
}

QPolygon PaintWidget::selection()
{
    return d->selection;
}

bool PaintWidget::isSelectionVisible()
{
    return d->isSelectionVisible && d->selection.first() != d->selection.last();
}
