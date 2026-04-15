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

// Worker class to run flood fill off the main thread.

#include "FloodFillWorker.h"
#include "../managers/FilterManager.h"

FloodFillWorker::FloodFillWorker(QObject *parent)
    : QObject(parent)
{
}

void FloodFillWorker::setImage(const QImage &image)
{
    m_image = image;
}

void FloodFillWorker::setPos(const QPoint &pos)
{
    m_pos = pos;
}

void FloodFillWorker::setColor(const QColor &color)
{
    m_color = color;
}

void FloodFillWorker::process()
{
    QImage result = FilterManager::instance()->floodFill(m_image, m_pos, m_color);
    emit finished(result);
}
