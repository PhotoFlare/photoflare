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

// PointerTool - Selection and cropping, filling Rectangle

#include "PointerTool.h"

#include <QApplication>
#include <QClipboard>
#include <QPainter>
#include <QMenu>
#include <QtMath>

#include "../Settings.h"

//#include <QDebug>

enum SelectionMode {SELECT, HAND, RESIZE, STROKE, FILL, MOVE};
enum Corner {TOP_LEFT, TOP_RIGHT, BOTTOM_RIGHT, BOTTOM_LEFT};
enum OverlayHandle { OV_NONE, OV_BODY, OV_CORNER_TL, OV_CORNER_TR, OV_CORNER_BR, OV_CORNER_BL, OV_ROTATE };

class PointerToolPrivate
{
public:
    PointerToolPrivate()
    {
        selectionMode = SELECT;
        fillColor = Qt::white;
    }
    QPoint firstPos;
    QPoint secondPos;
    QPoint dragAnchor;
    QPoint moveStartFirst;
    QPoint moveStartSecond;
    SelectionMode selectionMode;
    bool fillEnabled = false;
    bool strokeEnabled = false;
    int strokeWidth = 1;
    QImage image;
    QPoint imagePos;
    QRect topLeftCorner;
    QRect topRightCorner;
    QRect bottomRightCorner;
    QRect bottomLeftCorner;
    Corner corner;
    QColor fillColor;
    QColor strokeColor;
    PointerTool::SelectionShape selectionShape = PointerTool::RECT;
    // Paste overlay transform state
    int    overlayRotation = 0;      // degrees
    double overlayScale    = 1.0;
    OverlayHandle activeHandle = OV_NONE;
    QPointF dragStartImagePos;
    double  dragStartScale    = 1.0;
    int     dragStartRotation = 0;
    QPointF dragStartCanvasPos;
    bool overlayJustCommitted = false; // suppress selection clear on the paired MouseRelease
};

static QPolygon makeEllipsePolygon(const QPoint &topLeft, const QPoint &bottomRight, int steps = 64)
{
    QRect bounds = QRect(topLeft, bottomRight).normalized();
    QPolygon poly;
    poly.reserve(steps);
    double cx = bounds.center().x();
    double cy = bounds.center().y();
    double rx = bounds.width()  / 2.0;
    double ry = bounds.height() / 2.0;
    for (int i = 0; i < steps; ++i) {
        double angle = 2.0 * M_PI * i / steps;
        poly << QPoint(qRound(cx + rx * qCos(angle)),
                       qRound(cy + ry * qSin(angle)));
    }
    return poly;
}

// Returns the canvas-space centre of the pasted overlay (after scale, before rotation it's
// still the same — rotation is around this point).
static QPointF overlayCenter(const QPoint &imagePos, const QImage &image)
{
    return QPointF(imagePos.x() + image.width()  / 2.0,
                   imagePos.y() + image.height() / 2.0);
}

// Build the full transform that maps local image coords -> canvas coords.
static QTransform overlayTransform(const QPoint &imagePos, const QImage &image,
                                    int rotation, double scale)
{
    const QPointF c = overlayCenter(imagePos, image);
    QTransform t;
    t.translate(c.x(), c.y());
    t.rotate(rotation);
    t.scale(scale, scale);
    return t;
}

// Canvas-space positions of the four corners and the rotate handle.
struct OverlayHandles {
    QPointF tl, tr, br, bl, rot;
};

// handleRadius: half-size of handles in canvas pixels. Scale-dependent — caller computes:
//   (m_scale > 1.0f) ? 10 : (m_scale < 0.5f ? 40 : 20)
static OverlayHandles computeHandles(const QPoint &imagePos, const QImage &image,
                                      int rotation, double scale, int handleRadius)
{
    const double hw = image.width()  / 2.0;
    const double hh = image.height() / 2.0;
    // Rotate handle sits above the top edge; keep a comfortable gap regardless of overlayScale
    const double rotGap = qMax(24.0, handleRadius / qMax(scale, 0.01));
    const double rotOffset = hh + rotGap + handleRadius / qMax(scale, 0.01);

    QTransform t = overlayTransform(imagePos, image, rotation, scale);
    OverlayHandles h;
    h.tl  = t.map(QPointF(-hw, -hh));
    h.tr  = t.map(QPointF( hw, -hh));
    h.br  = t.map(QPointF( hw,  hh));
    h.bl  = t.map(QPointF(-hw,  hh));
    h.rot = t.map(QPointF(  0, -rotOffset));
    return h;
}

static bool hitHandle(const QPointF &pos, const QPointF &handle, int handleRadius)
{
    return (pos - handle).manhattanLength() < handleRadius * 1.5;
}

static OverlayHandle hitTestOverlay(const QPoint &pos, const QPoint &imagePos,
                                     const QImage &image, int rotation, double scale,
                                     int handleRadius)
{
    const OverlayHandles h = computeHandles(imagePos, image, rotation, scale, handleRadius);
    const QPointF p(pos);
    if (hitHandle(p, h.rot, handleRadius)) return OV_ROTATE;
    if (hitHandle(p, h.tl,  handleRadius)) return OV_CORNER_TL;
    if (hitHandle(p, h.tr,  handleRadius)) return OV_CORNER_TR;
    if (hitHandle(p, h.br,  handleRadius)) return OV_CORNER_BR;
    if (hitHandle(p, h.bl,  handleRadius)) return OV_CORNER_BL;
    // Check if inside image body using inverse transform
    QTransform t = overlayTransform(imagePos, image, rotation, scale);
    bool ok = false;
    QTransform inv = t.inverted(&ok);
    if (ok) {
        QPointF local = inv.map(p);
        const double hw = image.width()  / 2.0;
        const double hh = image.height() / 2.0;
        if (local.x() >= -hw && local.x() <= hw &&
            local.y() >= -hh && local.y() <= hh)
            return OV_BODY;
    }
    return OV_NONE;
}

static QImage buildOverlaySurface(const QImage &src, const QPoint &imagePos,
                                   int rotation, double scale,
                                   const QSize &canvasSize, float dashWidth,
                                   int handleRadius)
{
    QImage surface(canvasSize, QImage::Format_ARGB32_Premultiplied);
    QPainter p(&surface);
    p.setCompositionMode(QPainter::CompositionMode_Source);
    p.fillRect(surface.rect(), Qt::transparent);

    // Draw image with transform
    p.save();
    p.setRenderHint(QPainter::SmoothPixmapTransform);
    QTransform t = overlayTransform(imagePos, src, rotation, scale);
    p.setTransform(t);
    p.drawImage(-src.width() / 2, -src.height() / 2, src);
    p.restore();

    // Draw dashed border
    p.save();
    p.setRenderHint(QPainter::Antialiasing);
    p.setTransform(t);
    QPen borderPen(QBrush(), dashWidth, Qt::DashLine);
    borderPen.setColor(QColor(0x00, 0xad, 0xee));
    p.setPen(borderPen);
    p.setBrush(Qt::NoBrush);
    p.drawRect(QRectF(-src.width() / 2.0, -src.height() / 2.0, src.width(), src.height()));
    p.restore();

    // Draw handles in canvas space (after computing their canvas positions)
    const OverlayHandles h = computeHandles(imagePos, src, rotation, scale, handleRadius);
    p.setRenderHint(QPainter::Antialiasing);
    const int r = handleRadius;

    // Draw line from top-centre to rotate handle
    const QPointF topCentre = overlayTransform(imagePos, src, rotation, scale).map(QPointF(0, -src.height() / 2.0));
    p.setPen(QPen(QColor(0x00, 0xad, 0xee), 1));
    p.drawLine(topCentre, h.rot);

    // Corner scale handles (white fill, blue border)
    p.setPen(QPen(QColor(0x00, 0xad, 0xee), 1.5));
    p.setBrush(Qt::white);
    const QPointF corners[4] = { h.tl, h.tr, h.br, h.bl };
    for (const QPointF &c : corners)
        p.drawRect(QRectF(c.x() - r, c.y() - r, r * 2, r * 2));

    // Rotate handle (circle)
    p.setBrush(QColor(0x00, 0xad, 0xee));
    p.drawEllipse(h.rot, r, r);

    p.end();
    return surface;
}

PointerTool::PointerTool(QObject *parent)
    : Tool(parent)
    , d(new PointerToolPrivate)
{
    this->setObjectName("PointerTool");
    setupRightClickMenu(false);
}

PointerTool::~PointerTool()
{
    delete d;
}

void PointerTool::onCrop()
{
    const QRect rect = QRect(d->firstPos, d->secondPos).normalized();
    const QPolygon maskPoly = (d->selectionShape == PointerTool::ELLIPSE)
        ? makeEllipsePolygon(d->firstPos, d->secondPos)
        : QPolygon();
    d->secondPos = d->firstPos;
    emit selectionChanged(QPolygon());
    emit crop(rect, maskPoly);
}

void PointerTool::onStrokeRect()
{
    const QRect rect = QRect(d->firstPos, d->secondPos).normalized();
    const QColor strokeColor = d->fillColor;
    const int strokeWidth = d->strokeWidth;
    const QPolygon maskPoly = (d->selectionShape == PointerTool::ELLIPSE)
        ? makeEllipsePolygon(d->firstPos, d->secondPos)
        : QPolygon();
    d->secondPos = d->firstPos;
    emit selectionChanged(QPolygon());
    emit strokeRect(rect, strokeColor, strokeWidth, maskPoly);
}

void PointerTool::onFillRect()
{
    const QRect rect = QRect(d->firstPos, d->secondPos).normalized();
    const QColor fillColor = d->fillColor;
    const QPolygon maskPoly = (d->selectionShape == PointerTool::ELLIPSE)
        ? makeEllipsePolygon(d->firstPos, d->secondPos)
        : QPolygon();
    d->secondPos = d->firstPos;
    emit selectionChanged(QPolygon());
    emit fillRect(rect, fillColor, maskPoly);
}

void PointerTool::setFillColor(const QColor &color)
{
    d->fillColor=color;
}

void PointerTool::restoreSelection(const QPolygon &poly)
{
    const int cornerSize = (m_scale > 1.0f) ? 20 : (m_scale < 0.5f ? 100 : 50);
    if (poly.size() == 4) {
        d->firstPos = poly.at(0);
        d->secondPos = poly.at(2);
        d->topLeftCorner     = QRect(poly.at(0).x(),              poly.at(0).y(),              cornerSize, cornerSize);
        d->topRightCorner    = QRect(poly.at(1).x() - cornerSize, poly.at(1).y(),              cornerSize, cornerSize);
        d->bottomRightCorner = QRect(poly.at(2).x() - cornerSize, poly.at(2).y() - cornerSize, cornerSize, cornerSize);
        d->bottomLeftCorner  = QRect(poly.at(3).x(),              poly.at(3).y() - cornerSize, cornerSize, cornerSize);
    } else {
        d->firstPos          = QPoint();
        d->secondPos         = QPoint();
        d->topLeftCorner     = QRect();
        d->topRightCorner    = QRect();
        d->bottomRightCorner = QRect();
        d->bottomLeftCorner  = QRect();
    }
    d->selectionMode = SELECT;
}

void PointerTool::onSave()
{
    emit save();
}

void PointerTool::onSaveAs()
{
    emit saveAs();
}

void PointerTool::onClose()
{
    emit close();
}

void PointerTool::onImageSize()
{
    emit imageSize();
}

void PointerTool::onCanvasSize()
{
    emit canvasSize();
}

void PointerTool::onCopy()
{
    emit copy();
}

void PointerTool::onPaste()
{
    emit paste();
}

void PointerTool::onPasteAsNewImage()
{
    emit pasteAsNewImage();
}

void PointerTool::onUndo()
{
    emit undo();
}

void PointerTool::onRedo()
{
    emit redo();
}

void PointerTool::setOverlayImage(const QImage& image)
{
    d->selectionMode = HAND;
    d->image = image;
    d->imagePos = QPoint(0, 0);
    d->overlayRotation = 0;
    d->overlayScale    = 1.0;
    d->activeHandle    = OV_NONE;
    emit cursorChanged(Qt::DragCopyCursor);
    const QImage *paintImage = dynamic_cast<QImage*>(m_paintDevice);
    if (paintImage) {
        float dashW    = (m_scale < 0.5f) ? 5.0f : (m_scale > 1.0f) ? 1.0f : 2.0f;
        int   handleR  = (m_scale > 1.0f) ? 10 : (m_scale < 0.5f ? 40 : 20);
        QImage surface = buildOverlaySurface(d->image, d->imagePos,
                                              d->overlayRotation, d->overlayScale,
                                              paintImage->size(), dashW, handleR);
        emit overlaid(m_paintDevice, surface, QPainter::CompositionMode_SourceOver);
    } else {
        emit overlaid(m_paintDevice, d->image, QPainter::CompositionMode_SourceOver);
    }
}

void PointerTool::onDeactivated()
{
    if (d->selectionMode == HAND && m_paintDevice && !d->image.isNull())
    {
        const QPointF c = overlayCenter(d->imagePos, d->image);
        QPainter painter(m_paintDevice);
        painter.setRenderHint(QPainter::SmoothPixmapTransform);
        painter.translate(c.x(), c.y());
        if (d->overlayRotation != 0) painter.rotate(d->overlayRotation);
        if (d->overlayScale != 1.0)  painter.scale(d->overlayScale, d->overlayScale);
        painter.drawImage(-d->image.width() / 2, -d->image.height() / 2, d->image);
        painter.end();
        d->image = QImage();
        d->selectionMode = SELECT;
        emit painted(m_paintDevice);
    }
}

void PointerTool::onMousePress(const QPoint &pos, Qt::MouseButton button)
{
    switch(button) {
        case Qt::LeftButton:
        {
            // Rebuild corner hit-test rects from the current selection bounds
            // before firstPos/secondPos are overwritten. This keeps them in
            // sync regardless of how the selection was last set.
            {
                const int cornerSize = (m_scale > 1.0f) ? 20 : (m_scale < 0.5f ? 100 : 50);
                QPolygon sel(QRect(d->firstPos, d->secondPos).normalized());
                if (sel.size() == 4) {
                    d->topLeftCorner     = QRect(sel.at(0).x(),              sel.at(0).y(),              cornerSize, cornerSize);
                    d->topRightCorner    = QRect(sel.at(1).x() - cornerSize, sel.at(1).y(),              cornerSize, cornerSize);
                    d->bottomRightCorner = QRect(sel.at(2).x() - cornerSize, sel.at(2).y() - cornerSize, cornerSize, cornerSize);
                    d->bottomLeftCorner  = QRect(sel.at(3).x(),              sel.at(3).y() - cornerSize, cornerSize, cornerSize);
                }
            }
            const QRect selectionRect = QRect(d->firstPos, d->secondPos).normalized();
            d->firstPos = pos;
            d->secondPos = pos;

            if(d->selectionShape == PointerTool::RECT)
            {
                if(d->topLeftCorner.contains(pos))
                {
                    d->corner = TOP_LEFT;
                    d->selectionMode = RESIZE;
                    emit cursorChanged(Qt::SizeFDiagCursor);
                }
                else if(d->topRightCorner.contains(pos))
                {
                    d->corner = TOP_RIGHT;
                    d->selectionMode = RESIZE;
                    emit cursorChanged(Qt::SizeBDiagCursor);
                }
                else if(d->bottomLeftCorner.contains(pos))
                {
                    d->corner = BOTTOM_LEFT;
                    d->selectionMode = RESIZE;
                    emit cursorChanged(Qt::SizeBDiagCursor);
                }
                else if(d->bottomRightCorner.contains(pos))
                {
                    d->corner = BOTTOM_RIGHT;
                    d->selectionMode = RESIZE;
                    emit cursorChanged(Qt::SizeFDiagCursor);
                }
            }

            if(d->selectionMode == SELECT && !selectionRect.isEmpty() && selectionRect.contains(pos))
            {
                d->selectionMode = MOVE;
                d->moveStartFirst = selectionRect.topLeft();
                d->moveStartSecond = selectionRect.bottomRight();
                d->dragAnchor = pos;
                d->firstPos = d->moveStartFirst;
                d->secondPos = d->moveStartSecond;
                emit cursorChanged(Qt::SizeAllCursor);
            }

            if(d->selectionMode == SELECT)
            {
                // Nothing is painted here; the canvas will repaint via
                // selectionChanged → setSelectionVisible → updateImageCanvas.
                // Emitting painted() would set imageChanged=true and create
                // a spurious undo step even though no pixels were modified.
            }
            else if(d->selectionMode == HAND)
            {
                const int handleR = (m_scale > 1.0f) ? 10 : (m_scale < 0.5f ? 40 : 20);
                const OverlayHandle hit = hitTestOverlay(pos, d->imagePos, d->image,
                                                          d->overlayRotation, d->overlayScale,
                                                          handleR);
                if (hit == OV_NONE)
                {
                    // Click outside overlay — commit it
                    d->selectionMode = SELECT;
                    d->overlayJustCommitted = true;
                    // Clear selection so no stale corner rects remain.
                    d->firstPos  = pos;
                    d->secondPos = pos;
                    d->topLeftCorner = d->topRightCorner = d->bottomLeftCorner = d->bottomRightCorner = QRect();
                    emit selectionChanged(QPolygon());
                    if (m_paintDevice)
                    {
                        // Clear the selection stroke snapshot BEFORE drawing so that
                        // PaintWidget's selection mask does not revert the paste.
                        emit overlayCommitted();
                        const QPointF c = overlayCenter(d->imagePos, d->image);
                        QPainter painter(m_paintDevice);
                        painter.setRenderHint(QPainter::SmoothPixmapTransform);
                        painter.translate(c.x(), c.y());
                        if (d->overlayRotation != 0) painter.rotate(d->overlayRotation);
                        if (d->overlayScale != 1.0)  painter.scale(d->overlayScale, d->overlayScale);
                        painter.drawImage(-d->image.width() / 2, -d->image.height() / 2, d->image);
                        painter.end();
                        emit painted(m_paintDevice);
                    }
                }
                else
                {
                    d->activeHandle        = hit;
                    d->dragStartImagePos   = QPointF(d->imagePos);
                    d->dragStartScale      = d->overlayScale;
                    d->dragStartRotation   = d->overlayRotation;
                    d->dragStartCanvasPos  = QPointF(pos);
                    if (hit == OV_BODY)
                        emit cursorChanged(Qt::OpenHandCursor);
                    else if (hit == OV_ROTATE)
                        emit cursorChanged(Qt::CrossCursor);
                    else
                        emit cursorChanged(Qt::SizeFDiagCursor);
                }
            }
        }
        break;
        case Qt::RightButton:
        {
            setupRightClickMenu(true);
        }
        break;
        default:
        break;
    }
}

void PointerTool::onMouseMove(const QPoint &pos)
{
    d->secondPos = pos;

    if (m_paintDevice)
    {
        if(d->selectionMode == HAND)
        {
            if (d->activeHandle == OV_NONE)
            {
                // No button held — just hovering, nothing to update
            }
            else if (d->activeHandle == OV_BODY)
            {
                // Move
                const QPointF delta = QPointF(pos) - d->dragStartCanvasPos;
                d->imagePos = QPoint(qRound(d->dragStartImagePos.x() + delta.x()),
                                     qRound(d->dragStartImagePos.y() + delta.y()));
                emit cursorChanged(Qt::ClosedHandCursor);
            }
            else if (d->activeHandle == OV_ROTATE)
            {
                // Rotate: angle from image centre to current mouse position
                const QPointF c = overlayCenter(QPoint(qRound(d->dragStartImagePos.x()),
                                                        qRound(d->dragStartImagePos.y())),
                                                 d->image);
                const double angle = qAtan2(pos.y() - c.y(), pos.x() - c.x()) * 180.0 / M_PI + 90.0;
                d->overlayRotation = qRound(angle);
                emit cursorChanged(Qt::CrossCursor);
            }
            else
            {
                // Scale corner: distance from image centre determines scale
                const QPointF c = overlayCenter(QPoint(qRound(d->dragStartImagePos.x()),
                                                        qRound(d->dragStartImagePos.y())),
                                                 d->image);
                const double origDist = (d->dragStartCanvasPos - c).manhattanLength();
                const double currDist = (QPointF(pos) - c).manhattanLength();
                if (origDist > 1.0)
                    d->overlayScale = qMax(0.05, d->dragStartScale * (currDist / origDist));
                emit cursorChanged(Qt::SizeFDiagCursor);
            }

            const QImage *paintImage = dynamic_cast<QImage*>(m_paintDevice);
            if (paintImage) {
                float dashW   = (m_scale < 0.5f) ? 5.0f : (m_scale > 1.0f) ? 1.0f : 2.0f;
                int   handleR = (m_scale > 1.0f) ? 10 : (m_scale < 0.5f ? 40 : 20);
                QImage surface = buildOverlaySurface(d->image, d->imagePos,
                                                      d->overlayRotation, d->overlayScale,
                                                      paintImage->size(), dashW, handleR);
                emit overlaid(m_paintDevice, surface, QPainter::CompositionMode_SourceOver);
            }
        }
        else if(d->selectionMode == SELECT)
        {
            QPoint topLeft(d->firstPos);
            QPoint bottomRight(d->secondPos);

            if (d->selectionShape == PointerTool::ELLIPSE)
                emit selectionChanged(makeEllipsePolygon(topLeft, bottomRight));
            else
                emit selectionChanged(QRect(topLeft, bottomRight));
            emit painted(m_paintDevice);
        }
        else if(d->selectionMode == RESIZE)
        {
            if(d->corner == TOP_LEFT)
            {
                d->topLeftCorner.moveTo(d->secondPos);
                d->topRightCorner.moveTo(QPoint(d->topRightCorner.x(), d->topLeftCorner.y()));
                d->bottomLeftCorner.moveTo(QPoint(d->topLeftCorner.x(), d->bottomLeftCorner.y()));
            }
            else if(d->corner == TOP_RIGHT)
            {
                d->topRightCorner.moveTo(d->secondPos);
                d->topLeftCorner.moveTo(QPoint(d->topLeftCorner.x(), d->topRightCorner.y()));
                d->bottomRightCorner.moveTo(QPoint(d->topRightCorner.x(), d->bottomRightCorner.y()));
            }
            else if(d->corner == BOTTOM_RIGHT)
            {
                d->bottomRightCorner.moveTo(d->secondPos);
                d->topRightCorner.moveTo(QPoint(d->bottomRightCorner.x(), d->topRightCorner.y()));
                d->bottomLeftCorner.moveTo(QPoint(d->bottomLeftCorner.x(), d->bottomRightCorner.y()));
            }
            else if(d->corner == BOTTOM_LEFT)
            {
                d->bottomLeftCorner.moveTo(d->secondPos);
                d->topLeftCorner.moveTo(QPoint(d->bottomLeftCorner.x(), d->topLeftCorner.y()));
                d->bottomRightCorner.moveTo(QPoint(d->bottomRightCorner.x(), d->bottomLeftCorner.y()));
            }

            emit selectionChanged(QRect(d->topLeftCorner.topLeft(),d->bottomRightCorner.bottomRight()));
            emit painted(m_paintDevice);
        }
        else if(d->selectionMode == STROKE)
        {
           // paint using PaintWidget
            QPoint topLeft(d->firstPos);
            QPoint bottomRight(d->secondPos);
            emit selectionChanged(QRect(topLeft,bottomRight));
        }
        else if(d->selectionMode == MOVE)
        {
            const QPoint delta = pos - d->dragAnchor;
            if (d->selectionShape == PointerTool::ELLIPSE)
                emit selectionChanged(makeEllipsePolygon(d->moveStartFirst + delta, d->moveStartSecond + delta));
            else
                emit selectionChanged(QRect(d->moveStartFirst + delta, d->moveStartSecond + delta));
            emit painted(m_paintDevice);
        }
    }
}

void PointerTool::onMouseRelease(const QPoint &pos)
{
    emit cursorChanged(Qt::ArrowCursor);
    emit showhotspots();

    if(d->selectionMode == HAND)
    {
        if (d->activeHandle == OV_BODY) {
            // imagePos was already updated live in onMouseMove, nothing more to do
        }
        d->activeHandle = OV_NONE;

        // Refresh cursor based on where mouse ended up
        const int handleR = (m_scale > 1.0f) ? 10 : (m_scale < 0.5f ? 40 : 20);
        const OverlayHandle hit = hitTestOverlay(pos, d->imagePos, d->image,
                                                  d->overlayRotation, d->overlayScale, handleR);
        if (hit == OV_BODY)        emit cursorChanged(Qt::OpenHandCursor);
        else if (hit == OV_ROTATE) emit cursorChanged(Qt::CrossCursor);
        else if (hit != OV_NONE)   emit cursorChanged(Qt::SizeFDiagCursor);
        else                       emit cursorChanged(Qt::DragCopyCursor);
    }
    else if(d->selectionMode == SELECT)
    {
        if (d->overlayJustCommitted) {
            // The overlay was just committed on the paired MousePress — don't
            // treat this release as a zero-size selection click that would
            // wipe the pre-existing selection marquee.
            d->overlayJustCommitted = false;
            return;
        }
        if(d->firstPos == d->secondPos)
        {
            emit selectionChanged(QPolygon());
        }
        else
        {
            QPoint topLeft(d->firstPos);
            QPoint bottomRight(d->secondPos);
            int temp;
            if(topLeft.x() > bottomRight.x()) {temp = topLeft.x();topLeft.setX(bottomRight.x());bottomRight.setX(temp);}
            if(topLeft.y() > bottomRight.y()) {temp = topLeft.y();topLeft.setY(bottomRight.y());bottomRight.setY(temp);}

            const int cornerSize = (m_scale > 1.0f) ? 20 : (m_scale < 0.5f ? 100 : 50);
            QPolygon selection;
            if (d->selectionShape == PointerTool::ELLIPSE) {
                selection = makeEllipsePolygon(topLeft, bottomRight);
                d->topLeftCorner     = QRect();
                d->topRightCorner    = QRect();
                d->bottomRightCorner = QRect();
                d->bottomLeftCorner  = QRect();
            } else {
                selection = QPolygon(QRect(topLeft, bottomRight));
                d->topLeftCorner     = QRect(selection.at(0).x(),              selection.at(0).y(),              cornerSize, cornerSize);
                d->topRightCorner    = QRect(selection.at(1).x() - cornerSize, selection.at(1).y(),              cornerSize, cornerSize);
                d->bottomRightCorner = QRect(selection.at(2).x() - cornerSize, selection.at(2).y() - cornerSize, cornerSize, cornerSize);
                d->bottomLeftCorner  = QRect(selection.at(3).x(),              selection.at(3).y() - cornerSize, cornerSize, cornerSize);
            }
            emit selectionChanged(selection);

            if(d->fillEnabled) {
                onFillRect();
            }
            if(d->strokeEnabled) {
                onStrokeRect();
            }
        }
        emit painted(m_paintDevice);
    }
    else if(d->selectionMode == RESIZE)
    {
        d->firstPos = d->topLeftCorner.topLeft();
        d->secondPos = d->bottomRightCorner.bottomRight();
        d->selectionMode = SELECT;
        emit painted(m_paintDevice);
    }
    else if(d->selectionMode == MOVE)
    {
        const QPoint delta = d->secondPos - d->dragAnchor;
        d->firstPos = d->moveStartFirst + delta;
        d->secondPos = d->moveStartSecond + delta;
        const int cornerSize = (m_scale > 1.0f) ? 20 : (m_scale < 0.5f ? 100 : 50);
        QPolygon selection;
        if (d->selectionShape == PointerTool::ELLIPSE) {
            selection = makeEllipsePolygon(d->firstPos, d->secondPos);
            d->topLeftCorner     = QRect();
            d->topRightCorner    = QRect();
            d->bottomRightCorner = QRect();
            d->bottomLeftCorner  = QRect();
        } else {
            selection = QPolygon(QRect(d->firstPos, d->secondPos).normalized());
            d->topLeftCorner     = QRect(selection.at(0).x(),              selection.at(0).y(),              cornerSize, cornerSize);
            d->topRightCorner    = QRect(selection.at(1).x() - cornerSize, selection.at(1).y(),              cornerSize, cornerSize);
            d->bottomRightCorner = QRect(selection.at(2).x() - cornerSize, selection.at(2).y() - cornerSize, cornerSize, cornerSize);
            d->bottomLeftCorner  = QRect(selection.at(3).x(),              selection.at(3).y() - cornerSize, cornerSize, cornerSize);
        }
        emit selectionChanged(selection);
        d->selectionMode = SELECT;
        emit painted(m_paintDevice);
    }
}

void PointerTool::onHover(const QPoint &pos)
{
    if (d->selectionMode == HAND)
    {
        const int handleR = (m_scale > 1.0f) ? 10 : (m_scale < 0.5f ? 40 : 20);
        const OverlayHandle hit = hitTestOverlay(pos, d->imagePos, d->image,
                                                  d->overlayRotation, d->overlayScale, handleR);
        if      (hit == OV_BODY)   emit cursorChanged(Qt::OpenHandCursor);
        else if (hit == OV_ROTATE) emit cursorChanged(Qt::CrossCursor);
        else if (hit != OV_NONE)   emit cursorChanged(Qt::SizeFDiagCursor);
        else                       emit cursorChanged(Qt::DragCopyCursor);
        return;
    }
    if (d->selectionMode != SELECT)
    {
        emit cursorChanged(Qt::ArrowCursor);
        return;
    }
    const QRect selRect = QRect(d->firstPos, d->secondPos).normalized();
    if (!d->topLeftCorner.isNull())
    {
        if (d->topLeftCorner.contains(pos))
            { emit cursorChanged(Qt::SizeFDiagCursor); return; }
        if (d->topRightCorner.contains(pos))
            { emit cursorChanged(Qt::SizeBDiagCursor); return; }
        if (d->bottomRightCorner.contains(pos))
            { emit cursorChanged(Qt::SizeFDiagCursor); return; }
        if (d->bottomLeftCorner.contains(pos))
            { emit cursorChanged(Qt::SizeBDiagCursor); return; }
    }
    if (!selRect.isEmpty() && selRect.contains(pos))
        emit cursorChanged(Qt::SizeAllCursor);
    else
        emit cursorChanged(Qt::ArrowCursor);
}

void PointerTool::onKeyPressed(QKeyEvent *keyEvent)
{
    if (d->firstPos == d->secondPos)
        return;

    QPoint delta;
    switch (keyEvent->key()) {
        case Qt::Key_Left:  delta = QPoint(-1, 0); break;
        case Qt::Key_Right: delta = QPoint( 1, 0); break;
        case Qt::Key_Up:    delta = QPoint( 0,-1); break;
        case Qt::Key_Down:  delta = QPoint( 0, 1); break;
        default: return;
    }

    d->firstPos  += delta;
    d->secondPos += delta;

    const int cornerSize = (m_scale > 1.0f) ? 20 : (m_scale < 0.5f ? 100 : 50);
    QPolygon selection;
    if (d->selectionShape == PointerTool::ELLIPSE) {
        selection = makeEllipsePolygon(d->firstPos, d->secondPos);
        d->topLeftCorner     = QRect();
        d->topRightCorner    = QRect();
        d->bottomRightCorner = QRect();
        d->bottomLeftCorner  = QRect();
    } else {
        selection = QPolygon(QRect(d->firstPos, d->secondPos).normalized());
        d->topLeftCorner     = QRect(selection.at(0).x(),              selection.at(0).y(),              cornerSize, cornerSize);
        d->topRightCorner    = QRect(selection.at(1).x() - cornerSize, selection.at(1).y(),              cornerSize, cornerSize);
        d->bottomRightCorner = QRect(selection.at(2).x() - cornerSize, selection.at(2).y() - cornerSize, cornerSize, cornerSize);
        d->bottomLeftCorner  = QRect(selection.at(3).x(),              selection.at(3).y() - cornerSize, cornerSize, cornerSize);
    }
    emit selectionChanged(selection);
    emit painted(m_paintDevice);
}

void PointerTool::setStroke(bool enabled)
{
    d->strokeEnabled = enabled;
}

void PointerTool::setStrokeWidth(int width)
{
    d->strokeWidth = width;
}

void PointerTool::setFill(bool enabled)
{
    d->fillEnabled = enabled;
}

void PointerTool::setSelectionShape(PointerTool::SelectionShape shape)
{
    d->selectionShape = shape;
}

void PointerTool::showContextMenu()
{
    setupRightClickMenu(true);
}

void PointerTool::setupRightClickMenu(bool execute)
{
    QClipboard *clipboard = QApplication::clipboard();

    QMenu contextMenu("default");
    QAction crop(tr("Crop"), this);
    contextMenu.addAction(&crop);
    crop.setDisabled(true);
    if(d->firstPos != d->secondPos)
    {
        crop.setDisabled(false);
    }
    QAction sep0(this);
    sep0.setSeparator(true);
    QAction save(tr("Save"), this);
    QAction saveAs(tr("Save as"), this);
    QAction close(tr("Close"), this);
    QAction sep1(this);
    sep1.setSeparator(true);
    QAction copy(tr("Copy"), this);
    QAction paste(tr("Paste"), this);
    paste.setDisabled(true);
    QAction pasteAsNew(tr("Paste as new image"), this);
    pasteAsNew.setDisabled(true);
    if(!clipboard->image().isNull())
    {
        paste.setDisabled(false);
        pasteAsNew.setDisabled(false);
    }
    QAction sep2(this);
    sep2.setSeparator(true);
    QAction imageSize(tr("Image size"), this);
    QAction canvasSize(tr("Canvas size"), this);
    QAction sep3(this);
    sep3.setSeparator(true);
    QAction undo(tr("Undo"), this);
    QAction redo(tr("Redo"), this);

    contextMenu.addAction(&save);
    contextMenu.addAction(&saveAs);
    contextMenu.addAction(&close);
    contextMenu.addAction(&sep1);
    contextMenu.addAction(&imageSize);
    contextMenu.addAction(&canvasSize);
    contextMenu.addAction(&sep2);
    contextMenu.addAction(&copy);
    contextMenu.addAction(&paste);
    contextMenu.addAction(&pasteAsNew);
    contextMenu.addAction(&sep3);
    contextMenu.addAction(&undo);
    contextMenu.addAction(&redo);

    connect(&crop, SIGNAL(triggered()), this, SLOT(onCrop()));
    connect(&save, SIGNAL(triggered()), this, SLOT(onSave()));
    connect(&saveAs, SIGNAL(triggered()), this, SLOT(onSaveAs()));
    connect(&close, SIGNAL(triggered()), this, SLOT(onClose()));
    connect(&imageSize, SIGNAL(triggered()), this, SLOT(onImageSize()));
    connect(&canvasSize, SIGNAL(triggered()), this, SLOT(onCanvasSize()));
    connect(&copy, SIGNAL(triggered()), this, SLOT(onCopy()));
    connect(&paste, SIGNAL(triggered()), this, SLOT(onPaste()));
    connect(&pasteAsNew, SIGNAL(triggered()), this, SLOT(onPasteAsNewImage()));
    connect(&undo, SIGNAL(triggered()), this, SLOT(onUndo()));
    connect(&redo, SIGNAL(triggered()), this, SLOT(onRedo()));

    if(execute)
    {
        contextMenu.exec(QCursor::pos());
    }
}
