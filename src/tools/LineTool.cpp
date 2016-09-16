#include "LineTool.h"

#include <QApplication>
#include <QClipboard>
#include <QPainter>
#include <QMenu>
#include <QtMath>

class LineToolPrivate
{
public:
    LineToolPrivate()
    {
        primaryPen = QPen(QBrush(), 1, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin);
        secondaryPen = QPen(QBrush(), 1, Qt::DashDotLine, Qt::RoundCap, Qt::RoundJoin);
    }
    ~LineToolPrivate()
    {
    }

    QPoint firstPos;
    QPoint secondPos;
    QPen primaryPen;
    QPen secondaryPen;
    Qt::MouseButton mouseButton;

    bool antialias;
    int opacity;
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
    d->firstPos = pos;
    d->secondPos = pos;
    d->mouseButton = button;
}

void LineTool::setPrimaryColor(const QColor &color)
{
    d->primaryPen.setColor(color);
}

void LineTool::setSecondaryColor(const QColor &color)
{
    d->secondaryPen.setColor(color);
}

void LineTool::setWidth(int width)
{
    d->primaryPen.setWidth(width);
    d->secondaryPen.setWidth(width);
}

void LineTool::setAntialias(bool antialias)
{
    d->antialias = antialias;
}

void LineTool::setOpacity(int opacity)
{
    d->opacity = opacity;
}

void LineTool::setStyle(int style)
{
    Qt::PenStyle penStyle;
    Qt::PenCapStyle capStyle;
    switch(style)
    {
        case 0:
        default:
            penStyle = Qt::SolidLine;
            capStyle = Qt::SquareCap;
            break;
        case 1:
            penStyle = Qt::SolidLine;
            capStyle = Qt::RoundCap;
            break;
        case 2:
            penStyle = Qt::DashLine;
            capStyle = Qt::RoundCap;
           break;
        case 3:
            penStyle = Qt::DotLine;
            capStyle = Qt::RoundCap;
            break;
        case 4:
            penStyle = Qt::DashDotLine;
            capStyle = Qt::RoundCap;
            break;
        case 5:
            penStyle = Qt::DashDotDotLine;
            capStyle = Qt::RoundCap;
            break;
    }
    d->primaryPen.setStyle(penStyle);
    d->secondaryPen.setStyle(penStyle);

    d->primaryPen.setCapStyle(capStyle);
    d->secondaryPen.setCapStyle(capStyle);
}

void LineTool::onMouseMove(const QPoint &pos)
{
    d->secondPos = pos;

    if (m_paintDevice) {
        const QImage *image = dynamic_cast<QImage*>(m_paintDevice);
        QImage surface = QImage(image->size(), QImage::Format_ARGB32_Premultiplied);
        QPainter painter(&surface);
        painter.setCompositionMode(QPainter::CompositionMode_Source);
        painter.fillRect(surface.rect(), Qt::transparent);
        QPen pen = d->mouseButton == Qt::LeftButton ? d->primaryPen : d->secondaryPen;
        painter.setPen(pen);
        QBrush brush = QBrush(QColor(255,0,0), Qt::NoBrush);
        painter.setBrush(brush);
        painter.drawLine(d->firstPos, d->secondPos);
        painter.end();

        emit overlaid(surface, QPainter::CompositionMode_SourceOver);
    }
}

void LineTool::onMouseRelease(const QPoint &pos)
{
    Q_UNUSED(pos);

    if (m_paintDevice) {
        QPainter painter(m_paintDevice);
        QPen pen = d->mouseButton == Qt::LeftButton ? d->primaryPen : d->secondaryPen;
        if (d->antialias) {
            painter.setRenderHint(QPainter::Antialiasing);
            painter.setRenderHint(QPainter::HighQualityAntialiasing);
        }
        painter.setOpacity((float)d->opacity / 100.0f);
        painter.setPen(pen);
        painter.drawLine(d->firstPos, d->secondPos);
        painter.end();

        emit painted(m_paintDevice);
    }

    d->mouseButton = Qt::NoButton;
}



