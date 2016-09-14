#include "ColourPickerTool.h"
#include "../mainwindow.h"
#include "../PaintWidget.h"

#include <QPainter>

class ColourPickerToolPrivate
{
public:
    ColourPickerToolPrivate()
    {
    }
    ~ColourPickerToolPrivate()
    {
    }

    QPoint lastPos;
    Qt::MouseButton mouseButton;
};

ColourPickerTool::ColourPickerTool(QObject *parent)
    : Tool(parent)
    , d(new ColourPickerToolPrivate)
{
}

ColourPickerTool::~ColourPickerTool()
{
    delete d;
}

void ColourPickerTool::onMousePress(const QPoint &pos, Qt::MouseButton button)
{
    Q_UNUSED(button);

    d->lastPos = pos;
    d->mouseButton = button;

    switch(button)
    {
        case Qt::LeftButton:
            emit pickPrimaryColor(d->lastPos);
            break;
        case Qt::RightButton:
            emit pickSecondaryColor(d->lastPos);
            break;
        default:
            break;
    }
}
