#include "EraserTool.h"
#include "../PaintWidget.h"

class EraserToolPrivate
{
public:
    EraserToolPrivate()
    {

    }
    ~EraserToolPrivate()
    {

    }

    QPoint lastPos;
    QPen primaryPen;
    Qt::MouseButton mouseButton;
};

EraserTool::EraserTool(QObject *parent)
    : Tool(parent)
    , d(new EraserToolPrivate)
{

}

EraserTool::~EraserTool()
{
    delete d;
}

QCursor EraserTool::getCursor()
{
    int width = d->primaryPen.width()*m_scale;
    if(width < 5)
        width = 5;
    QPixmap pixmap(QSize(width,width));
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    if(d->primaryPen.capStyle() == Qt::SquareCap)
    {
        QPen pen = QPen(QBrush(), 1.5, Qt::DashLine);
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

int EraserTool::width() const
{
    return d->primaryPen.width();
}

void EraserTool::setWidth(int width)
{
    d->primaryPen.setWidth(width);

    emit cursorChanged(getCursor());
}

void EraserTool::onMousePress(const QPoint &pos, Qt::MouseButton button)
{
    Q_UNUSED(button);

    d->lastPos = pos;
    d->mouseButton = button;

    QPen pen;
    pen.setColor(Qt::white);

    QPainter painter(m_paintDevice);
    painter.setPen(pen);
    painter.drawPoint(pos.x(), pos.y());
    painter.end();
    emit painted(m_paintDevice);
}

void EraserTool::onMouseMove(const QPoint &pos)
{
    if (m_paintDevice) {
        QPainter painter(m_paintDevice);

        QPen pen;
        pen.setColor(Qt::white);
        pen.setWidth(d->primaryPen.width());

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
