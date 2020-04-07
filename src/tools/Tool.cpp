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

// Generic Tool class. All Tools will extend this class.

#include "Tool.h"
#include <QPainter>

Tool::Tool(QObject *parent)
    : QObject(parent)
    , m_paintDevice(nullptr){}

Tool::~Tool()
{
}

void Tool::setPaintDevice(QPaintDevice *paintDevice)
{
    m_paintDevice = paintDevice;
    emit cursorChanged(getCursor());
}

QPaintDevice *Tool::paintDevice() const
{
    return m_paintDevice;
}

void Tool::setScale(float scale)
{
    m_scale = scale;
    emit cursorChanged(getCursor());
}

void Tool::disconnect()
{
}

void Tool::onMousePress(const QPoint &pos, Qt::MouseButton button)
{
    Q_UNUSED(pos);
    Q_UNUSED(button);
}

void Tool::onMouseMove(const QPoint &pos)
{
    Q_UNUSED(pos);
}

void Tool::onMouseRelease(const QPoint &pos)
{
    Q_UNUSED(pos);
}

void Tool::onKeyPressed(QKeyEvent * keyEvent)
{
    Q_UNUSED(keyEvent);
}

void Tool::onKeyReleased(QKeyEvent * keyEvent)
{
    Q_UNUSED(keyEvent);
}

QCursor Tool::getCursor()
{
    return Qt::ArrowCursor;
}
