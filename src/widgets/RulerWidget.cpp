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

#include "RulerWidget.h"

#include <QPainter>
#include <QContextMenuEvent>
#include <QMenu>
#include <QAction>
#include <cmath>

RulerWidget::RulerWidget(Orientation orientation, QWidget *parent)
    : QWidget(parent)
    , m_orientation(orientation)
{
    if (orientation == Orientation::Horizontal)
        setFixedHeight(THICKNESS);
    else
        setFixedWidth(THICKNESS);
}

void RulerWidget::setScale(float scale)
{
    if (m_scale != scale) {
        m_scale = scale;
        update();
    }
}

void RulerWidget::setOrigin(float origin)
{
    if (m_origin != origin) {
        m_origin = origin;
        update();
    }
}

void RulerWidget::setUnit(Unit unit)
{
    if (m_unit != unit) {
        m_unit = unit;
        update();
        emit unitChanged(unit);
    }
}

void RulerWidget::setDpi(float dpi)
{
    if (dpi > 0.0f && m_dpi != dpi) {
        m_dpi = dpi;
        update();
    }
}

void RulerWidget::setMousePos(int pos)
{
    if (m_mousePos != pos) {
        m_mousePos = pos;
        update();
    }
}

void RulerWidget::clearMousePos()
{
    if (m_mousePos != -1) {
        m_mousePos = -1;
        update();
    }
}

RulerWidget::Unit RulerWidget::unit() const
{
    return m_unit;
}

// ---------------------------------------------------------------------------
// computeIntervalPx — returns a "nice" tick interval in image-pixel space
// such that ticks are at least minImagePx image pixels apart.
// ---------------------------------------------------------------------------
float RulerWidget::computeIntervalPx(float minImagePx) const
{
    if (m_unit == Unit::Pixels) {
        static const float kPixelSteps[] = {
            1, 2, 5, 10, 20, 25, 50, 100, 200, 250,
            500, 1000, 2000, 2500, 5000, 10000
        };
        for (float step : kPixelSteps) {
            if (step >= minImagePx)
                return step;
        }
        return kPixelSteps[std::size(kPixelSteps) - 1];
    }

    if (m_unit == Unit::Inches) {
        static const float kInchSteps[] = {
            1.0f/64, 1.0f/32, 1.0f/16, 1.0f/8, 1.0f/4, 1.0f/2,
            1, 2, 5, 10, 20, 50, 100
        };
        float minInches = minImagePx / m_dpi;
        for (float step : kInchSteps) {
            if (step >= minInches)
                return step * m_dpi;
        }
        return kInchSteps[std::size(kInchSteps) - 1] * m_dpi;
    }

    // Centimeters
    static const float kCmSteps[] = {
        0.01f, 0.02f, 0.05f, 0.1f, 0.2f, 0.5f,
        1, 2, 5, 10, 20, 50, 100
    };
    float minCm = minImagePx * 2.54f / m_dpi;
    for (float step : kCmSteps) {
        if (step >= minCm)
            return step * m_dpi / 2.54f;
    }
    return kCmSteps[std::size(kCmSteps) - 1] * m_dpi / 2.54f;
}

// ---------------------------------------------------------------------------
// formatLabel — converts an image-pixel position to a display string.
// ---------------------------------------------------------------------------
QString RulerWidget::formatLabel(float imagePx) const
{
    if (m_unit == Unit::Pixels) {
        return QString::number(qRound(imagePx));
    }

    if (m_unit == Unit::Inches) {
        float inches = imagePx / m_dpi;
        if (std::fabs(inches - std::round(inches)) < 0.005f)
            return QString::number(static_cast<int>(std::round(inches)));
        return QString::number(inches, 'f', 2);
    }

    // Centimeters
    float cm = imagePx * 2.54f / m_dpi;
    if (std::fabs(cm - std::round(cm)) < 0.005f)
        return QString::number(static_cast<int>(std::round(cm)));
    return QString::number(cm, 'f', 1);
}

void RulerWidget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    const bool horizontal = (m_orientation == Orientation::Horizontal);
    const int length = horizontal ? width() : height();

    // Background
    p.fillRect(rect(), QColor(210, 210, 210));

    // Border line (bottom for H, right for V)
    p.setPen(QColor(130, 130, 130));
    if (horizontal)
        p.drawLine(0, height() - 1, width() - 1, height() - 1);
    else
        p.drawLine(width() - 1, 0, width() - 1, height() - 1);

    if (m_scale <= 0.0f)
        return;

    // Minimum 50 screen pixels between major ticks
    const float minScreenDist = 50.0f;
    float intervalPx = computeIntervalPx(minScreenDist / m_scale);
    if (intervalPx <= 0.0f)
        return;

    // Minor ticks at half the interval
    float halfIntervalPx = intervalPx * 0.5f;

    // Find first visible major tick in image space
    float firstMajorTick = std::ceil(m_origin / intervalPx) * intervalPx;

    // Tick and label metrics (in logical pixels)
    const int majorTickLen = static_cast<int>(THICKNESS * 0.55);
    const int minorTickLen = static_cast<int>(THICKNESS * 0.30);

    QFont font = p.font();
    font.setPointSize(7);
    p.setFont(font);
    p.setPen(QColor(60, 60, 60));

    // Draw minor ticks (half interval)
    float firstMinorTick = std::ceil(m_origin / halfIntervalPx) * halfIntervalPx;
    for (float tick = firstMinorTick; ; tick += halfIntervalPx) {
        float screenPos = (tick - m_origin) * m_scale;
        if (screenPos > length)
            break;
        if (screenPos >= 0.0f) {
            int sp = static_cast<int>(screenPos);
            if (horizontal)
                p.drawLine(sp, height() - minorTickLen, sp, height() - 1);
            else
                p.drawLine(width() - minorTickLen, sp, width() - 1, sp);
        }
    }

    // Draw major ticks and labels
    for (float tick = firstMajorTick; ; tick += intervalPx) {
        float screenPos = (tick - m_origin) * m_scale;
        if (screenPos > length)
            break;
        if (screenPos >= -1.0f) {
            int sp = static_cast<int>(screenPos);
            if (horizontal) {
                p.drawLine(sp, height() - majorTickLen, sp, height() - 1);
                // Label: draw just to the right of the tick mark, near top
                QString label = formatLabel(tick);
                p.drawText(sp + 2, 1, width() - sp - 2, height() - majorTickLen - 1,
                           Qt::AlignLeft | Qt::AlignVCenter, label);
            } else {
                p.drawLine(width() - majorTickLen, sp, width() - 1, sp);
                // Rotated label: translate to tick position and rotate
                QString label = formatLabel(tick);
                p.save();
                p.translate(width() - majorTickLen - 1, sp - 2);
                p.rotate(-90.0);
                p.drawText(0, 0, label);
                p.restore();
            }
        }
    }

    // Mouse cursor indicator
    if (m_mousePos >= 0) {
        p.setPen(QPen(QColor(0, 100, 200), 1));
        if (horizontal)
            p.drawLine(m_mousePos, 0, m_mousePos, height() - 1);
        else
            p.drawLine(0, m_mousePos, width() - 1, m_mousePos);
    }
}

void RulerWidget::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);
    QAction *actPixels = menu.addAction(tr("Pixels"));
    QAction *actInches = menu.addAction(tr("Inches"));
    QAction *actCm     = menu.addAction(tr("Centimeters"));

    actPixels->setCheckable(true);
    actInches->setCheckable(true);
    actCm->setCheckable(true);

    actPixels->setChecked(m_unit == Unit::Pixels);
    actInches->setChecked(m_unit == Unit::Inches);
    actCm->setChecked(m_unit == Unit::Centimeters);

    QAction *chosen = menu.exec(event->globalPos());
    if (chosen == actPixels)
        setUnit(Unit::Pixels);
    else if (chosen == actInches)
        setUnit(Unit::Inches);
    else if (chosen == actCm)
        setUnit(Unit::Centimeters);
}
