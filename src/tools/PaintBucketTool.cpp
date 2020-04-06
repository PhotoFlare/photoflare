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

// PaintBucketTool - Flood fill areas.

#include "PaintBucketTool.h"
#include "../mainwindow.h"
#include "../widgets/PaintWidget.h"

PaintBucketTool::PaintBucketTool(QObject *parent)
    : Tool(parent){}

void PaintBucketTool::onMousePress(const QPoint &pos, Qt::MouseButton button)
{
    switch(button)
    {
        case Qt::LeftButton:
            emit floodFillPrimaryColor(pos);
            break;
        case Qt::RightButton:
            emit floodFillSecondaryColor(pos);
            break;
        default:
            break;
    }
}

QCursor PaintBucketTool::getCursor()
{
    return QCursor(QPixmap (QString(":/cursors/assets/cursors/bucket.png")));
}
