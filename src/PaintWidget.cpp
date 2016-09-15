/*

  Paint widget class. Handles the drawing area.

*/

#include "PaintWidget.h"

#include <QLabel>
#include <QPainter>
#include <QScrollBar>
#include <QMouseEvent>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsSceneMouseEvent>

#include "./tools/Tool.h"

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
        canvas->setPixmap(QPixmap::fromImage(image));
    }

    void updateImageLabelWithOverlay(const QImage &overlayImage)
    {
        QImage surface = QImage(image.size(), QImage::Format_ARGB32_Premultiplied);
        QPainter painter(&surface);
        painter.setCompositionMode(QPainter::CompositionMode_Source);
        painter.fillRect(surface.rect(), Qt::transparent);
        painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
        painter.drawImage(0, 0, image);
        painter.setCompositionMode(QPainter::CompositionMode_Difference);
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
    }

    void mousePressEvent(QGraphicsSceneMouseEvent *event)
    {
        if (currentTool) {
            // Set current image to the tool when we start painting.
            currentTool->setPaintDevice(&image);
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
            currentTool->onMouseRelease(QPoint(event->scenePos().x(), event->scenePos().y()));
    }

    QString imagePath;
    QLabel *imageLabel;
    QImage image;
    Tool *currentTool;
    QMetaObject::Connection lastConnection;
    QMetaObject::Connection lastOverlayConnection;
    QGraphicsPixmapItem *canvas;
    float scale;

    PaintWidget *q;
};

PaintWidget::PaintWidget(const QString &imagePath, QWidget *parent)
    : QGraphicsView(parent)
    , d(new PaintWidgetPrivate(this))
{
    d->initialize(QImage(imagePath));
    d->imagePath = imagePath;
}

PaintWidget::PaintWidget(const QSize &imageSize, QWidget *parent)
    : QGraphicsView(parent)
    , d(new PaintWidgetPrivate(this))
{
    QImage image(imageSize, QImage::Format_ARGB32_Premultiplied);
    image.fill(Qt::white);
    d->initialize(image);
}

PaintWidget::~PaintWidget()
{
    d->disconnectLastTool();

    delete d;
}

void PaintWidget::setPaintTool(Tool *tool)
{
    if (d->currentTool) {
        d->currentTool->disconnect();
        d->disconnectLastTool();
    }

    d->currentTool = tool;

    if (d->currentTool) {
        d->currentTool->setPaintDevice(&d->image);
        d->updateImageLabel();

        d->lastConnection = connect(d->currentTool, &Tool::painted, [this] (QPaintDevice *paintDevice) {
                if (&d->image == paintDevice) {
                d->updateImageLabel();
                this->contentChanged();
                }
            });
        d->lastOverlayConnection = connect(d->currentTool, &Tool::overlaid, [this] (const QImage &overlayImage) {
                d->updateImageLabelWithOverlay(overlayImage);
            });
    }
}

void PaintWidget::setImage(const QImage &image)
{
    d->setImage(image);
    this->contentChanged();
}

QImage PaintWidget::image() const
{
    return d->image;
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
}

void PaintWidget::setScale(const QString &rate)
{
    resetMatrix();
    d->scale = (rate.mid(0,rate.lastIndexOf("%"))).toFloat() / 100.0f;
    scale(d->scale, d->scale);
}

void PaintWidget::wheelEvent(QWheelEvent *event)
{
    float scaleFactor = 1.1f;
    if(event->delta() > 0) {
       d->scale = d->scale / scaleFactor;
       d->scale = (d->scale < 0.1f) ? 0.1f : d->scale;
    } else {
       d->scale = d->scale * scaleFactor;
       d->scale = (d->scale > 8) ? 8 : d->scale;
    }

    resetMatrix();
    scale(d->scale, d->scale);
    emit zoomChanged(d->scale);
}
