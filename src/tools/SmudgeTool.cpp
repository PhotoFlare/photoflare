/*

  SmudgeTool - Smudge pixels while dragging the mouse.

*/

#include "SmudgeTool.h"
#include "../PaintWidget.h"

class SmudgeToolPrivate
{
public:
    SmudgeToolPrivate()
    {

    }
    ~SmudgeToolPrivate()
    {

    }

    int radius;
};

SmudgeTool::SmudgeTool(QObject *parent)
    : Tool(parent)
    , d(new SmudgeToolPrivate)
{

}

SmudgeTool::~SmudgeTool()
{
    delete d;
}

QCursor SmudgeTool::getCursor()
{
    int width = d->radius * m_scale;
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

void SmudgeTool::setRadius(int radius)
{
    d->radius = radius;
    emit cursorChanged(getCursor());
}
