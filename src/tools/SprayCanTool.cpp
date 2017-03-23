#include "SprayCanTool.h"

#include <QPainter>
#include <QBitmap>
#include <QtMath>

class SprayCanToolPrivate
{
public:
    SprayCanToolPrivate()
    {
        primaryPen = QPen(QBrush(), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        secondaryPen = QPen(QBrush(), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        pixmap = QPixmap(32,32);
        rainbowColor = QColor(255,0,0);
    }
    ~SprayCanToolPrivate()
    {

    }

    QPoint lastPos;
    QPen primaryPen;
    QPen secondaryPen;
    QPixmap pixmap;
    int radius;
    int pressure;
    int dispersion;
    bool rainbow;
    QColor rainbowColor;
    float opacity;
    QPoint lastStep;
    Qt::MouseButton mouseButton;
};

SprayCanTool::SprayCanTool(QObject *parent)
    : Tool(parent)
    , d(new SprayCanToolPrivate)
{
    startTimer(10);
}

SprayCanTool::~SprayCanTool()
{
    delete d;
}

void SprayCanTool::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);
    static int x = 0;
    if (m_paintDevice && d->mouseButton != Qt::NoButton)
    {
        if(d->rainbow) 
        {
            x = (x + 1)%100;
            d->rainbowColor = d->rainbowColor.fromHslF(x/100.0, 0.95, 0.5);
        }

        QPainter painter(m_paintDevice);
        painter.setOpacity(d->opacity);
        fillPattern();
        painter.drawPixmap(d->lastPos.x() - d->pixmap.width()/2, d->lastPos.y() - d->pixmap.height()/2, d->pixmap);
        emit painted(m_paintDevice);
    }
}


void SprayCanTool::setPrimaryColor(const QColor &color)
{
    d->primaryPen.setColor(color);
}

QColor SprayCanTool::primaryColor() const
{
    return d->primaryPen.color();
}

void SprayCanTool::setSecondaryColor(const QColor &color)
{
    d->secondaryPen.setColor(color);
}

QColor SprayCanTool::secondaryColor() const
{
    return d->secondaryPen.color();
}

void SprayCanTool::fillPattern()
{
    QPen pen = d->mouseButton == Qt::LeftButton ? d->primaryPen : d->secondaryPen;

    QImage pattern(d->radius, d->radius, QImage::Format_ARGB32);
    pattern.fill(Qt::transparent);
    int w = pattern.width();
    for(int i=0; i<w; i++) 
    {
        for(int j=0; j<w; j++) 
        {
            float rnd = (float)rand()/(float)RAND_MAX;
            float l = qSqrt( qPow(i-w/2, 2) + qPow(j-w/2, 2) );
            if(rnd > (float)d->dispersion/51 && l <= w/2) 
            {
                float a = 1 - l/(w/2);
                if(d->rainbow)
                    pattern.setPixel(i, j, qRgba(d->rainbowColor.red(), d->rainbowColor.green(), d->rainbowColor.blue(), 255*a));
                else
                    pattern.setPixel(i, j, qRgba(pen.color().red(), pen.color().green(), pen.color().blue(), 255*a));
            }
        }
    }

    d->pixmap = d->pixmap.fromImage(pattern);
}

void SprayCanTool::setRadius(int radius)
{
    d->radius = radius;
    emit cursorChanged(getCursor());
}

void SprayCanTool::setPressure(int pressure)
{
    d->pressure = pressure;
}

void SprayCanTool::setDispersion(int dispersion)
{
    d->dispersion = dispersion;
}

void SprayCanTool::setRainbow(bool rainbow)
{
    d->rainbow = rainbow;
}

void SprayCanTool::onMousePress(const QPoint &pos, Qt::MouseButton button)
{
    Q_UNUSED(button);

    d->lastPos = pos;
    d->mouseButton = button;
    d->opacity = d->pressure / 10.0f;

    fillPattern();
}

void SprayCanTool::onMouseMove(const QPoint &pos)
{
    if (m_paintDevice)
    {
        QPainter painter(m_paintDevice);
        int w = d->pixmap.width();
        fillPattern();
        painter.setOpacity(d->opacity);
        painter.drawPixmap(pos.x() - w/2, pos.y() - w/2, d->pixmap);
        d->lastStep = pos;

        d->lastPos = pos;
        emit painted(m_paintDevice);
    }
}

void SprayCanTool::onMouseRelease(const QPoint &pos)
{
    Q_UNUSED(pos);
    d->mouseButton = Qt::NoButton;
}

QCursor SprayCanTool::getCursor()
{
    QImage pattern(d->radius, d->radius, QImage::Format_ARGB32);
    pattern.fill(Qt::transparent);
    int w = pattern.width();
    QPixmap pixmap(QSize(w/1.5,w/1.5));
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    QPen pen = QPen(QBrush(), 1, Qt::DashLine);
    pen.setColor(Qt::gray);
    painter.setPen(pen);
    painter.drawEllipse(pixmap.rect());

    return QCursor(pixmap);
}



