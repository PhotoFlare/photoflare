#include "PaintBrushAdvTool.h"

#include <QPainter>
#include <QBitmap>
#include <QtMath>

class PaintBrushAdvToolPrivate
{
public:
    PaintBrushAdvToolPrivate()
    {
        primaryPen = QPen(QBrush(), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        secondaryPen = QPen(QBrush(), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    }
    ~PaintBrushAdvToolPrivate()
    {

    }

    QPoint lastPos;
    QPen primaryPen;
    QPen secondaryPen;
    QPixmap pixmap;
    int preassure;
    float opacity;
    bool fade;
    int step;
    QPoint lastStep;
    Qt::MouseButton mouseButton;
    QCursor cursor;
};

PaintBrushAdvTool::PaintBrushAdvTool(QObject *parent)
    : Tool(parent)
    , d(new PaintBrushAdvToolPrivate)
{

}

PaintBrushAdvTool::~PaintBrushAdvTool()
{
    delete d;
}

void PaintBrushAdvTool::setWidth(int width)
{
    d->primaryPen.setWidth(width);
    d->secondaryPen.setWidth(width);
}

int PaintBrushAdvTool::width() const
{
    return d->primaryPen.width();
}

void PaintBrushAdvTool::setPrimaryColor(const QColor &color)
{
    d->primaryPen.setColor(color);
}

QColor PaintBrushAdvTool::primaryColor() const
{
    return d->primaryPen.color();
}

void PaintBrushAdvTool::setSecondaryColor(const QColor &color)
{
    d->secondaryPen.setColor(color);
}

QColor PaintBrushAdvTool::secondaryColor() const
{
    return d->secondaryPen.color();
}

void PaintBrushAdvTool::setBrushPixmap(QPixmap pixmap)
{
    d->pixmap = pixmap;
}

void PaintBrushAdvTool::setPreassure(int preassure)
{
    d->preassure = preassure;
}

void PaintBrushAdvTool::setFade(bool fade)
{
    d->fade = fade;
}

void PaintBrushAdvTool::setStep(int step)
{
    d->step = step;
}

QCursor PaintBrushAdvTool::getCursor()
{
    QPixmap pixmap(QSize(32*m_scale,32*m_scale));
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    QPen pen = QPen(QBrush(), 1, Qt::DashLine);
    pen.setColor(Qt::gray);
    painter.setPen(pen);

    painter.drawEllipse(pixmap.rect());
    return QCursor(pixmap);
}

void PaintBrushAdvTool::onMousePress(const QPoint &pos, Qt::MouseButton button)
{
    Q_UNUSED(button);

    d->lastPos = pos;
    d->mouseButton = button;

    QPen pen = d->mouseButton == Qt::LeftButton ? d->primaryPen : d->secondaryPen;

    QPixmap pxr(d->pixmap.size());
    pxr.fill(pen.color());
    pxr.setMask(d->pixmap.createMaskFromColor(Qt::transparent));

    d->pixmap = pxr;
    d->opacity = qPow(((float)d->preassure / 10.0f), 2);
}

void PaintBrushAdvTool::onMouseMove(const QPoint &pos)
{
    if (m_paintDevice)
    {
        int x1 = d->lastPos.x();
        int x2 = pos.x();
        int y1 = d->lastPos.y();
        int y2 = pos.y();

        {
            QPainter painter(m_paintDevice);
            if(x1 > x2) {
                int temp = x1; x1 = x2; x2 = temp;
                temp = y1; y1 = y2; y2 = temp;
            }

            int w = d->primaryPen.width();
            for(int y, x = x1; x < x2; x += 1) {
                y = (x - x1)*(y2 - y1)/(x2 - x1) + y1;
                if(qSqrt(qPow(x - d->lastStep.x(), 2) + qPow(y - d->lastStep.y(), 2)) >= d->step )
                {
                    painter.setOpacity(d->opacity);
                    painter.drawPixmap(x - w/2, y - w/2, d->pixmap);
                    d->lastStep = pos;
                }
            }

            if(y1 > y2) {
                int temp = x1; x1 = x2; x2 = temp;
                temp = y1; y1 = y2; y2 = temp;
            }

            for(int x, y = y1; y < y2; y += 1) {
                x = (y - y1)*(x2 - x1)/(y2 - y1) + x1;
                if(qSqrt(qPow(x - d->lastStep.x(), 2) + qPow(y - d->lastStep.y(), 2)) >= d->step )
                {
                    painter.setOpacity(d->opacity);
                    painter.drawPixmap(x - w/2, y - w/2, d->pixmap);
                    d->lastStep = pos;
                }
            }
        }

        if(d->fade) {
            d->opacity = d->opacity - 0.01f;
        }

        d->lastPos = pos;
        emit painted(m_paintDevice);
    }
}

void PaintBrushAdvTool::onMouseRelease(const QPoint &pos)
{
    Q_UNUSED(pos);

    d->mouseButton = Qt::NoButton;
}



