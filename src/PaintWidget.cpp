/*

  Paint widget class. Handles the drawing area.

*/

#include "PaintWidget.h"

#include <QLabel>
#include <QPainter>
#include <QScrollBar>
#include <QMouseEvent>

#include "./tools/Tool.h"

class PaintWidgetPrivate
{
public:
    PaintWidgetPrivate(PaintWidget *widget)
    {
        imageLabel = new QLabel;
        currentTool = 0;

        q = widget;
    }
    ~PaintWidgetPrivate()
    {
    }

    void initialize(const QImage &image)
    {
        this->image = image;
        updateImageLabel();

        q->setAlignment(Qt::AlignCenter);
        q->setStyleSheet("background-color: rgb(128, 128, 128);");
        q->setWidget(imageLabel);
    }

    void updateImageLabel()
    {
        imageLabel->setPixmap(QPixmap::fromImage(image));
    }

    QPoint calcScrollAreaPos(const QPoint &pos) const
    {
        QPoint offsetPoint = QPoint(imageLabel->x(), imageLabel->y());// Needed because the image is in the center of scroll area.

        int contentX = q->horizontalScrollBar()->value();
        int contentY = q->verticalScrollBar()->value();
        QPoint scrollPoint = QPoint(contentX, contentY);

        return pos - (scrollPoint.isNull() ? offsetPoint : QPoint()) + scrollPoint;
    }

    void disconnectLastTool()
    {
        bool result = QObject::disconnect(lastConnection);
        Q_ASSERT(result);
        Q_UNUSED(result);
    }

    QString imagePath;
    QLabel *imageLabel;
    QImage image;
    Tool *currentTool;
    QMetaObject::Connection lastConnection;

    PaintWidget *q;
};

PaintWidget::PaintWidget(const QString &imagePath, QWidget *parent)
    : QScrollArea(parent)
    , d(new PaintWidgetPrivate(this))
{
    d->initialize(QImage(imagePath));
    d->imagePath = imagePath;
}

PaintWidget::PaintWidget(const QSize &imageSize, QWidget *parent)
    : QScrollArea(parent)
    , d(new PaintWidgetPrivate(this))
{
    QImage image(imageSize, QImage::Format_RGB888);
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
    if (d->currentTool)
        d->disconnectLastTool();

    d->currentTool = tool;

    if (d->currentTool) {
        d->currentTool->setPaintDevice(&d->image);

        d->lastConnection = connect(d->currentTool, &Tool::painted, [this] (QPaintDevice *paintDevice) {
                if (&d->image == paintDevice) {
                d->updateImageLabel();
                this->contentChanged();
                }
            });
    }
}

void PaintWidget::setImage(const QImage &image)
{
    d->image = image;
    d->updateImageLabel();
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

void PaintWidget::mousePressEvent(QMouseEvent *event)
{
    if (d->currentTool) {
        // Set current image to the tool when we start painting.
        d->currentTool->setPaintDevice(&d->image);
        d->currentTool->onMousePress(d->calcScrollAreaPos(event->pos()), event->button());
    }
}

void PaintWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (d->currentTool)
        d->currentTool->onMouseMove(d->calcScrollAreaPos(event->pos()));
}

void PaintWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (d->currentTool)
        d->currentTool->onMouseRelease(d->calcScrollAreaPos(event->pos()));
}
