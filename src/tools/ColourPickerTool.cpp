/*
    This file is part of Photoflare.

    Photoflare is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Photoflare is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Photoflare.  If not, see <https://www.gnu.org/licenses/>.
*/

// ColourPickerTool - Select colours from the image for the ColourBoxWidget.

#include "ColourPickerTool.h"
#include "../mainwindow.h"
#include "../widgets/PaintWidget.h"

class ColourPickerToolPrivate
{
public:
    QPoint lastPos;
    Qt::MouseButton mouseButton;
};

ColourPickerTool::ColourPickerTool(QObject *parent)
    : Tool(parent)
    , d(new ColourPickerToolPrivate){}

ColourPickerTool::~ColourPickerTool()
{
    delete d;
}

void ColourPickerTool::onMousePress(const QPoint &pos, Qt::MouseButton button)
{
    Q_UNUSED(button);
    d->lastPos = pos;

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

QCursor ColourPickerTool::getCursor()
{
    return QCursor(QPixmap (QString(":/cursors/assets/cursors/color-picker.png")));
}
