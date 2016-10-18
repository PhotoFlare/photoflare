/*

  LineTool - Draw various lines on the image.

*/

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
        primaryPen = QPen(QBrush(), 1, Qt::DotLine, Qt::RoundCap, Qt::MiterJoin);
        secondaryPen = QPen(QBrush(), 1, Qt::DashDotLine, Qt::RoundCap, Qt::MiterJoin);
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

    bool oneWayArrow;
    bool twoWaysArrow;
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

    emit cursorChanged(getCursor());
}

void LineTool::setAntialias(bool antialias)
{
    d->antialias = antialias;
}

void LineTool::setOpacity(int opacity)
{
    d->opacity = opacity;
}

void LineTool::setArrowStyle(int arrowStyle)
{
    d->oneWayArrow = false;
    d->twoWaysArrow = false;
    switch(arrowStyle)
    {
        case 1:
            d->oneWayArrow =true;
            break;
        case 2:
            d->twoWaysArrow =true;
            break;
    }
}

void LineTool::drawArrow(QPainter& painter, QPoint pFrom, QPoint pTo, QPoint& pOutMedian)
{
    QPoint pBase;
    QPoint aptPoly[3];
    float vecLine[2];
    float vecLeft[2];
    float fLength;
    float th;
    float ta;

    QPen pen = d->mouseButton == Qt::LeftButton ? d->primaryPen : d->secondaryPen;

    aptPoly[0] = pTo;

    vecLine[0] = aptPoly[0].x() - pFrom.x();
    vecLine[1] = aptPoly[0].y() - pFrom.y();

    vecLeft[0] = -vecLine[1];
    vecLeft[1] = vecLine[0];

    float width = 15 + 2*pen.width();
    float angle = (float)3.1415 / 4;

    fLength = qSqrt(vecLine[0]*vecLine[0] + vecLine[1]*vecLine[1]);
    th =  width / (2 * fLength);
    ta = width / (qTan(angle) * fLength);

    pBase.setX(aptPoly[0].x() + -ta * vecLine[0]);
    pBase.setY(aptPoly[0].y() + -ta * vecLine[1]);

    aptPoly[1].setX(pBase.x() + th*vecLeft[0]);
    aptPoly[1].setY(pBase.y() + th*vecLeft[1]);
    aptPoly[2].setX(pBase.x() + -th*vecLeft[0]);
    aptPoly[2].setY(pBase.y() + -th*vecLeft[1]);

    if(pen.capStyle() == Qt::RoundCap)
    {
        painter.drawLine(aptPoly[0], aptPoly[1]);
        painter.drawLine(aptPoly[0], aptPoly[2]);
    }
    else
    {
        painter.drawPolygon(aptPoly, 3);
    }

    pOutMedian.setX(aptPoly[1].x()/2 + aptPoly[2].x()/2);
    pOutMedian.setY(aptPoly[1].y()/2 + aptPoly[2].y()/2);
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

QCursor LineTool::getCursor()
{
    int width = d->primaryPen.width();
    if(width < 5)
        width = 5;
    QPixmap pixmap(QSize(width,width));
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    QPen pen = QPen(QBrush(), 1, Qt::DashLine);
    pen.setColor(Qt::gray);
    painter.setPen(pen);

    painter.drawEllipse(pixmap.rect());
    return QCursor(pixmap);
}


void LineTool::onMouseMove(const QPoint &pos)
{
    d->secondPos = pos;

    if (m_paintDevice) {
        const QImage *image = dynamic_cast<QImage*>(m_paintDevice);
        QImage surface = QImage(image->size(), QImage::Format_ARGB32_Premultiplied);
        QPainter painter(&surface);
        painter.setCompositionMode(QPainter::CompositionMode_Source);
        //painter.fillRect(surface.rect(), Qt::transparent);
        QPen pen = d->mouseButton == Qt::LeftButton ? d->primaryPen : d->secondaryPen;
        //painter.setPen(pen);

        QBrush brush = QBrush(pen.color(), Qt::SolidPattern);
        painter.setBrush(brush);

        Qt::PenStyle penStyle = pen.style();
        pen.setStyle(Qt::SolidLine);
        painter.setPen(pen);

       QPoint firstPos = d->firstPos;
       QPoint secondPos = d->secondPos;
        if(d->oneWayArrow)
            drawArrow(painter, d->firstPos, d->secondPos, secondPos);

        if(d->twoWaysArrow)
        {
            drawArrow(painter, d->firstPos, d->secondPos, secondPos);
            drawArrow(painter, d->secondPos, d->firstPos, firstPos);
        }

        pen.setStyle(penStyle);
        painter.setPen(pen);

        painter.drawLine(firstPos, secondPos);

        painter.end();

        emit overlaid(m_paintDevice, surface, QPainter::CompositionMode_SourceOver);
    }
}

void LineTool::onMouseRelease(const QPoint &pos)
{
    Q_UNUSED(pos);

    if (m_paintDevice)
    {
        QPainter painter(m_paintDevice);
        QPen pen = d->mouseButton == Qt::LeftButton ? d->primaryPen : d->secondaryPen;
        if (d->antialias)
        {
            painter.setRenderHint(QPainter::Antialiasing);
            painter.setRenderHint(QPainter::HighQualityAntialiasing);
        }
        painter.setOpacity((float)d->opacity / 100.0f);
        painter.setPen(pen);
        QBrush brush = QBrush(pen.color(), Qt::SolidPattern);
        painter.setBrush(brush);

        Qt::PenStyle penStyle = pen.style();
        pen.setStyle(Qt::SolidLine);
        painter.setPen(pen);

        QPoint firstPos = d->firstPos;
        QPoint secondPos = d->secondPos;
        if(d->oneWayArrow)
            drawArrow(painter, d->firstPos, d->secondPos, secondPos);

        if(d->twoWaysArrow)
        {
            drawArrow(painter, d->firstPos, d->secondPos, secondPos);
            drawArrow(painter, d->secondPos, d->firstPos, firstPos);
        }

        pen.setStyle(penStyle);
        painter.setPen(pen);

        painter.drawLine(firstPos, secondPos);

        painter.end();

        emit painted(m_paintDevice);
    }

    d->mouseButton = Qt::NoButton;
}



