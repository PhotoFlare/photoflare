/*

  PaintBrushTool - Paint using basic styles.

*/

#include "PaintBrushTool.h"

#include <QPainter>

class PaintBrushToolPrivate
{
public:
    PaintBrushToolPrivate()
    {
        primaryPen = QPen(QBrush(), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        secondaryPen = QPen(QBrush(), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        antialiasing = false;
    }
    ~PaintBrushToolPrivate()
    {

    }

    QPoint lastPos;
    QPen primaryPen;
    QPen secondaryPen;
    bool antialiasing;
    Qt::MouseButton mouseButton;
};

PaintBrushTool::PaintBrushTool(QObject *parent)
    : Tool(parent)
    , d(new PaintBrushToolPrivate)
{

}

PaintBrushTool::~PaintBrushTool()
{
    delete d;
}

void PaintBrushTool::setWidth(int width)
{
    d->primaryPen.setWidth(width);
    d->secondaryPen.setWidth(width);

    emit cursorChanged(getCursor());
}

int PaintBrushTool::width() const
{
    return d->primaryPen.width();
}

void PaintBrushTool::setPrimaryColor(const QColor &color)
{
    d->primaryPen.setColor(color);
}

QColor PaintBrushTool::primaryColor() const
{
    return d->primaryPen.color();
}

void PaintBrushTool::setSecondaryColor(const QColor &color)
{
    d->secondaryPen.setColor(color);
}

QColor PaintBrushTool::secondaryColor() const
{
    return d->secondaryPen.color();
}

void PaintBrushTool::setAntialiasing(bool value)
{
    d->antialiasing = value;
}

bool PaintBrushTool::antialiasing() const
{
    return d->antialiasing;
}

void PaintBrushTool::setCapStyle(Qt::PenCapStyle capStyle)
{
    d->primaryPen.setCapStyle(capStyle);
    d->secondaryPen.setCapStyle(capStyle);

    emit cursorChanged(getCursor());
}

QCursor PaintBrushTool::getCursor()
{
    int width = d->primaryPen.width()*m_scale;
    if(width < 5)
        width = 5;
    QPixmap pixmap(QSize(width,width));
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    if(d->primaryPen.capStyle() == Qt::SquareCap)
    {
        QPen pen = QPen(QBrush(), 2, Qt::DashLine);
        pen.setColor(Qt::gray);
        painter.setPen(pen);
        painter.drawRect(pixmap.rect());
    }
    else
    {
        QPen pen = QPen(QBrush(), 1, Qt::DashLine);
        pen.setColor(Qt::gray);
        painter.setPen(pen);
        painter.drawEllipse(pixmap.rect());
    }
    return QCursor(pixmap);
}

void PaintBrushTool::onMousePress(const QPoint &pos, Qt::MouseButton button)
{
    Q_UNUSED(button);

    d->lastPos = pos;
    d->mouseButton = button;
}

void PaintBrushTool::onMouseMove(const QPoint &pos)
{
    if (m_paintDevice) {
        QPainter painter(m_paintDevice);

        if (d->antialiasing)
            painter.setRenderHint(QPainter::Antialiasing);

        QPen pen = d->mouseButton == Qt::LeftButton ? d->primaryPen : d->secondaryPen;

        int w = pen.width();
        if(pen.capStyle() == Qt::SquareCap && w > 1)
        {
            painter.setPen(QPen(pen.color()));
            QBrush brush(pen.color());
            painter.setBrush(brush);

            int x1 = d->lastPos.x();
            int x2 = pos.x();
            int y1 = d->lastPos.y();
            int y2 = pos.y();
            if(x1 > x2) {
                int temp = x1; x1 = x2; x2 = temp;
                temp = y1; y1 = y2; y2 = temp;
            }

            for(int y, x = x1; x < x2; x += w/2) {
                y = (x - x1)*(y2 - y1)/(x2 - x1) + y1;
                painter.drawRect(QRect(x - w/2, y - w/2, w, w));
            }

            if(y1 > y2) {
                int temp = x1; x1 = x2; x2 = temp;
                temp = y1; y1 = y2; y2 = temp;
            }

            for(int x, y = y1; y < y2; y += w/2) {
                x = (y - y1)*(x2 - x1)/(y2 - y1) + x1;
                painter.drawRect(QRect(x - w/2, y - w/2, w, w));
            }

        } else
        {
            painter.setPen(pen);
            painter.drawLine(d->lastPos, pos);
        }

        d->lastPos = pos;
        emit painted(m_paintDevice);
    }
}

void PaintBrushTool::onMouseRelease(const QPoint &pos)
{
    Q_UNUSED(pos);

    d->mouseButton = Qt::NoButton;
}



