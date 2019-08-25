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

#ifndef TOOL_H
#define TOOL_H

#include <QObject>
#include <QPoint>
#include <QImage>
#include <QPainter>
#include <QCursor>
#include <QKeyEvent>

class QPaintDevice;

/*!
 * This is the base class for all paint tools
 */
class Tool : public QObject
{
    Q_OBJECT
public:
    explicit Tool(QObject *parent = 0);
    ~Tool();

    void setPaintDevice(QPaintDevice *paintDevice);
    QPaintDevice* paintDevice() const;

    void setScale(float scale);
    /*!
     * Use methods below to paint using painter()
     */
    virtual void onMousePress(const QPoint &pos, Qt::MouseButton button);
    virtual void onMouseMove(const QPoint &pos);
    virtual void onMouseRelease(const QPoint &pos);
    virtual void onKeyPressed(QKeyEvent *keyEvent);
    virtual void onKeyReleased(QKeyEvent *keyEvent);

    virtual void disconnect();

    virtual QCursor getCursor();

signals:

    void cursorChanged(QCursor);
    void selectionChanged(QPolygon);
    /*!
     * This signal must be emitted after the tool completes any paint action
     */
    void painted(QPaintDevice *paintDevice);

    void overlaid(QPaintDevice *paintDevice, const QImage &overlayImage, QPainter::CompositionMode mode);

protected:
    QPaintDevice *m_paintDevice;
    float m_scale;
};



#endif // TOOL_H

