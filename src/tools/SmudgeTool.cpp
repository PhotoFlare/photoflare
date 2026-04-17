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

// SmudgeTool - Smudge pixels while dragging the mouse.

#include "SmudgeTool.h"
#include "PaintWidget.h"

#include <QPainter>
#include <QImage>
#include <QtMath>

class SmudgeToolPrivate
{
public:
    SmudgeToolPrivate()
    {
        primaryPen = QPen(QBrush(), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    }
    int radius;
    int pressure;
    QPoint lastPos;
    QPen primaryPen;
    Qt::MouseButton mouseButton;
    QImage smudgeBuffer; // colours "carried" by the brush tip
};

SmudgeTool::SmudgeTool(QObject *parent)
    : Tool(parent)
    , d(new SmudgeToolPrivate){}

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
    pixmap.setDevicePixelRatio(qApp->devicePixelRatio());
    return QCursor(pixmap);
}

void SmudgeTool::setRadius(int radius)
{
    d->radius = radius;
    emit cursorChanged(getCursor());
}

void SmudgeTool::setPressure(int pressure)
{
    d->pressure = pressure;
}

void SmudgeTool::onMousePress(const QPoint &pos, Qt::MouseButton button)
{
    Q_UNUSED(button);
    d->lastPos = pos;
    d->mouseButton = button;
    const QImage *image = dynamic_cast<QImage*>(m_paintDevice);
    if (!image) return;
    // Seed the smudge buffer with the pixels directly under the cursor.
    int half = d->radius / 2;
    d->smudgeBuffer = image->copy(pos.x() - half, pos.y() - half, d->radius, d->radius)
                           .convertToFormat(QImage::Format_ARGB32);
}

void SmudgeTool::onMouseMove(const QPoint &pos)
{
    if (!m_paintDevice || d->smudgeBuffer.isNull()) return;

    QImage *image = dynamic_cast<QImage*>(m_paintDevice);
    if (!image) return;

    const int r    = d->radius;
    const int half = r / 2;
    // strength: pressure 1-10 → 0.10 – 1.00
    const float strength = d->pressure / 10.0f;

    const int destX = pos.x() - half;
    const int destY = pos.y() - half;

    // Current pixels under the new cursor position (same size as smudge buffer)
    QImage currentPatch = image->copy(destX, destY, r, r)
                               .convertToFormat(QImage::Format_ARGB32);

    // Ensure smudge buffer matches expected dimensions
    if (d->smudgeBuffer.width() != r || d->smudgeBuffer.height() != r)
        d->smudgeBuffer = d->smudgeBuffer.scaled(r, r);

    // Build the blended patch: smudge buffer painted over current pixels,
    // modulated by a circular falloff mask so edges are smooth.
    QImage blended = currentPatch.copy();

    for (int y = 0; y < r; ++y) {
        for (int x = 0; x < r; ++x) {
            const float dx   = x - half;
            const float dy   = y - half;
            const float dist = qSqrt(dx * dx + dy * dy);
            if (dist > half) continue;

            // Smooth quadratic falloff (full strength at centre, zero at edge)
            const float t = 1.0f - (dist / half);
            const float alpha = strength * t * t;

            const QRgb src = d->smudgeBuffer.pixel(x, y);
            const QRgb dst = (x < currentPatch.width() && y < currentPatch.height())
                             ? currentPatch.pixel(x, y)
                             : src;

            const int nr = int(qRed(src)   * alpha + qRed(dst)   * (1.0f - alpha));
            const int ng = int(qGreen(src) * alpha + qGreen(dst) * (1.0f - alpha));
            const int nb = int(qBlue(src)  * alpha + qBlue(dst)  * (1.0f - alpha));
            const int na = int(qAlpha(src) * alpha + qAlpha(dst) * (1.0f - alpha));
            blended.setPixel(x, y, qRgba(nr, ng, nb, na));
        }
    }

    // Commit blended patch to canvas
    QPainter painter(image);
    painter.drawImage(destX, destY, blended);
    painter.end();

    // Update smudge buffer: gradually absorb the destination pixels so the
    // smear fades naturally the further the brush travels.
    const float pickup = strength * 0.4f;
    for (int y = 0; y < r; ++y) {
        for (int x = 0; x < r; ++x) {
            const QRgb buf = d->smudgeBuffer.pixel(x, y);
            const QRgb cur = (x < currentPatch.width() && y < currentPatch.height())
                             ? currentPatch.pixel(x, y)
                             : buf;

            const int nr = int(qRed(buf)   * (1.0f - pickup) + qRed(cur)   * pickup);
            const int ng = int(qGreen(buf) * (1.0f - pickup) + qGreen(cur) * pickup);
            const int nb = int(qBlue(buf)  * (1.0f - pickup) + qBlue(cur)  * pickup);
            const int na = int(qAlpha(buf) * (1.0f - pickup) + qAlpha(cur) * pickup);
            d->smudgeBuffer.setPixel(x, y, qRgba(nr, ng, nb, na));
        }
    }

    const int updateHalf = half + 1;
    const QPoint prevPos = d->lastPos;
    d->lastPos = pos;
    emit painted(m_paintDevice,
                 QRect(prevPos, pos).normalized().adjusted(-updateHalf, -updateHalf, updateHalf, updateHalf));
}

void SmudgeTool::onMouseRelease(const QPoint &pos)
{
    Q_UNUSED(pos);
    d->mouseButton = Qt::NoButton;
    d->smudgeBuffer = QImage();
}

