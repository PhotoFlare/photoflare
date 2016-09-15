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
    QVector<QPoint> points;
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
}

void PaintBrushTool::onMousePress(const QPoint &pos, Qt::MouseButton button)
{
    Q_UNUSED(button);

    d->lastPos = pos;
    d->mouseButton = button;

    d->points.clear();
    d->points.append(pos);
}

void PaintBrushTool::onMouseMove(const QPoint &pos)
{
    if (m_paintDevice) {
        QPainter painter(m_paintDevice);

        if (d->antialiasing)
            painter.setRenderHint(QPainter::Antialiasing);

        QPen pen = d->mouseButton == Qt::LeftButton ? d->primaryPen : d->secondaryPen;
        painter.setPen(pen);

        QPainterPath path;
        path.moveTo(d->points.at(0));

        if(pen.capStyle() == Qt::SquareCap) {
            if(QLineF(d->points.last().x(), d->points.last().y(), pos.x(), pos.y()).length() > 2)
                d->points.append(pos);
        } else {
            d->points.append(pos);
        }

        for(int i=1; i+1 < d->points.size(); i+=2) {
            path.quadTo(d->points.at(i),d->points.at(i+1));
        }

        QPainterPathStroker stroker;
        stroker.setWidth(pen.width());
        stroker.setJoinStyle(pen.joinStyle());
        stroker.setCapStyle(Qt::RoundCap);

        QPainterPath stroke = stroker.createStroke(path);
        painter.fillPath(stroke, pen.color());

        d->lastPos = pos;
        emit painted(m_paintDevice);
    }
}

void PaintBrushTool::onMouseRelease(const QPoint &pos)
{
    Q_UNUSED(pos);

    if (m_paintDevice) {
        if(d->points.size() > 1)
        {
            QPainter painter(m_paintDevice);

            if (d->antialiasing)
                painter.setRenderHint(QPainter::Antialiasing);

            QPen pen = d->mouseButton == Qt::LeftButton ? d->primaryPen : d->secondaryPen;

            if(pen.capStyle() == Qt::SquareCap)
            {
                painter.setPen(pen);
                painter.drawLine(d->points.at(0), d->points.at(1));
                QPoint preLast = d->points.at(d->points.size()-2);
                QPoint last = d->points.last();
                painter.drawLine((preLast + last)/2, last);
                emit painted(m_paintDevice);
            }
        }
    }


    d->mouseButton = Qt::NoButton;
}



