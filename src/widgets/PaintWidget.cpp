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

// Paint widget class. Handles the drawing area.

//#include <QDebug>

#include <QLabel>
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>
#include <QScrollBar>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsProxyWidget>
#include <QGraphicsEffect>
#include <QImageReader>
#include <QTimer>

#include <Magick++.h>

#include <functional>

#include <QGraphicsItem>
#include <QStyleOptionGraphicsItem>

#include "../Settings.h"
#include "PaintWidget.h"
#include "./Tool.h"
#include "RulerWidget.h"

// ---------------------------------------------------------------------------
// ImageCanvasItem — custom QGraphicsItem that paints directly from a QImage*,
// compositing checkers only for the exposed (dirty) region.  Zero extra copies.
// ---------------------------------------------------------------------------
class ImageCanvasItem : public QGraphicsItem
{
public:
    // Pointers into PaintWidgetPrivate — never own these.
    const QImage *m_image        = nullptr;
    // Optional overlay (e.g. tool preview line).  Null size = no overlay.
    QImage        m_overlay;
    QPainter::CompositionMode m_overlayMode = QPainter::CompositionMode_SourceOver;
    bool          m_hasOverlay   = false;

    // Selection polygon to paint on top (may be empty).
    const QPolygon *m_selection          = nullptr;
    bool            m_selectionVisible   = false;
    bool            m_hotspotVisible     = false;

    // drawSelection callback — we call it via a std::function so we don't need
    // to duplicate the selection drawing logic.
    std::function<void(QPainter &)> drawSelectionFn;

    QRectF boundingRect() const override
    {
        return m_image ? QRectF(m_image->rect()) : QRectF();
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *) override
    {
        if (!m_image) return;

        const QRect exposed = option->exposedRect.toAlignedRect().intersected(m_image->rect());
        if (exposed.isEmpty()) return;

        const bool opaque = (m_image->format() == QImage::Format_RGB32);

        if (opaque && !m_hasOverlay && (!m_selectionVisible || !m_selection || m_selection->isEmpty())) {
            // Fastest path: fully opaque, no overlay, no selection.
            painter->drawImage(exposed, *m_image, exposed);
            return;
        }

        // Composite into a small temporary surface covering only the exposed rect.
        QImage tile(exposed.size(), QImage::Format_ARGB32_Premultiplied);
        QPainter p(&tile);

        if (opaque) {
            // Base layer: image (no checkers needed).
            p.drawImage(QPoint(0, 0), *m_image, exposed);
        } else {
            // Base layer: checkers then image.
            QBrush brush;
            brush.setTextureImage(QImage(":/pixmaps/assets/pixmaps/checkers.png"));
            p.setCompositionMode(QPainter::CompositionMode_Source);
            p.fillRect(tile.rect(), brush);
            p.setCompositionMode(QPainter::CompositionMode_SourceOver);
            p.drawImage(QPoint(0, 0), *m_image, exposed);
        }

        if (m_hasOverlay && !m_overlay.isNull()) {
            p.setCompositionMode(m_overlayMode);
            p.drawImage(QPoint(0, 0), m_overlay, exposed);
        }

        p.end();

        // Draw the composited tile, then paint selection on top (in image coords).
        painter->drawImage(exposed.topLeft(), tile);

        if (m_selectionVisible && m_selection && !m_selection->isEmpty() && drawSelectionFn) {
            drawSelectionFn(*painter);
        }
    }
};

#include "QProgressIndicator.h"

class PaintWidgetPrivate : public QGraphicsScene
{
public:
    PaintWidgetPrivate(PaintWidget *widget) :
        QGraphicsScene(widget)
    {
        currentTool = 0;
        scale = 1.0f;
        q = widget;
        q->setScene(this);

        isSelectionVisible = true;
        hotspotVisible = false;

        updateTimer = new QTimer(this);
        updateTimer->setSingleShot(true);
        updateTimer->setInterval(0);
        connect(updateTimer, &QTimer::timeout, [this]() { flushPendingUpdate(); });
    }
    ~PaintWidgetPrivate()
    {
    }

    void initialize(const QImage &image)
    {
        QImage img = image;
        if (img.hasAlphaChannel()) {
            // Convert to RGB32 if no pixel is actually transparent so that
            // format-based checks (e.g. eraser transparency) behave correctly.
            const QImage argb = img.convertToFormat(QImage::Format_ARGB32);
            bool actuallyTransparent = false;
            for (int y = 0; y < argb.height() && !actuallyTransparent; ++y) {
                const QRgb *line = reinterpret_cast<const QRgb *>(argb.constScanLine(y));
                for (int x = 0; x < argb.width() && !actuallyTransparent; ++x) {
                    if (qAlpha(line[x]) < 255)
                        actuallyTransparent = true;
                }
            }
            if (!actuallyTransparent)
                img = img.convertToFormat(QImage::Format_RGB32);
        }
        this->image = img;
        q->setSceneRect(img.rect());

        canvas = new ImageCanvasItem();
        canvas->m_image = &this->image;
        canvas->m_selection = &this->selection;
        canvas->m_selectionVisible = this->isSelectionVisible;
        canvas->drawSelectionFn = [this](QPainter &painter){ drawSelection(painter); };
        addItem(canvas);

        q->setStyleSheet("background-color: rgb(160, 160, 160);");
    }

    void drawSelection(QPainter &painter)
    {
        // Keep screen-visible line width at ~2px regardless of zoom and
        // device pixel ratio: image-space width = desired_screen_px / (scale * dpr).
        float dpr = q->devicePixelRatioF();
        float effectiveScale = scale * dpr;
        float scaledVal = qBound(1.0f, 2.0f / effectiveScale, 20.0f);
        int cornerSize = 50;

        if(effectiveScale < 0.5)
        {
            cornerSize = 100;
        }
        else if(effectiveScale > 1) {
            cornerSize = 20;
        }

        //Draw the selection hotspots
        if(selection.size() == 4)
        {
            QRect rect(selection.at(0),selection.at(3));
            if(rect.topLeft() != rect.bottomLeft() && hotspotVisible)
            {
                QPen penbg = QPen(QBrush(), scaledVal, Qt::SolidLine);
                penbg.setColor(Qt::white);
                painter.setPen(penbg);
                painter.setBrush(QBrush());
                painter.drawPolygon(QRect(selection.at(0).x(),selection.at(0).y(), cornerSize, cornerSize));
                painter.drawPolygon(QRect(selection.at(1).x()-cornerSize,selection.at(1).y(), cornerSize, cornerSize));
                painter.drawPolygon(QRect(selection.at(2).x()-cornerSize,selection.at(2).y()-cornerSize, cornerSize, cornerSize));
                painter.drawPolygon(QRect(selection.at(3).x(),selection.at(3).y()-cornerSize, cornerSize, cornerSize));

                QPen cornerpen = QPen(QBrush(), scaledVal, Qt::DashLine);
                cornerpen.setColor(Qt::gray);
                painter.setPen(cornerpen);
                painter.setBrush(QBrush());
                painter.drawPolygon(QRect(selection.at(0).x(),selection.at(0).y(), cornerSize, cornerSize));
                painter.drawPolygon(QRect(selection.at(1).x()-cornerSize,selection.at(1).y(), cornerSize, cornerSize));
                painter.drawPolygon(QRect(selection.at(2).x()-cornerSize,selection.at(2).y()-cornerSize, cornerSize, cornerSize));
                painter.drawPolygon(QRect(selection.at(3).x(),selection.at(3).y()-cornerSize, cornerSize, cornerSize));
            }
        }
        //Grey and white dashed line for visibility no matter the background colours
        QPen penbg = QPen(QBrush(), scaledVal, Qt::SolidLine);
        penbg.setColor(Qt::white);
        painter.setPen(penbg);
        painter.setBrush(QBrush());
        painter.drawPolygon(selection, Qt::WindingFill);

        QPen pen = QPen(QBrush(), scaledVal, Qt::DashLine);
        pen.setColor(Qt::gray);
        painter.setPen(pen);
        painter.setBrush(QBrush());
        painter.drawPolygon(selection, Qt::WindingFill);
    }

    // Invalidate the canvas item for the given region (or fully if null).
    // The item's paint() will composite checkers/overlay/selection on demand,
    // touching only the pixels Qt actually needs to redraw.
    void updateImageCanvas(const QRect &region = QRect())
    {
        canvas->m_hasOverlay = false;
        canvas->m_selectionVisible = isSelectionVisible;
        if (region.isNull())
            canvas->update();
        else
            canvas->update(QRectF(region));
    }

    void updateImageCanvasWithOverlay(const QImage &overlayImage, QPainter::CompositionMode mode, const QRect &region = QRect())
    {
        canvas->m_overlay      = overlayImage;
        canvas->m_overlayMode  = mode;
        canvas->m_hasOverlay   = true;
        canvas->m_selectionVisible = isSelectionVisible;
        if (region.isNull())
            canvas->update();
        else
            canvas->update(QRectF(region));
    }

    void scheduleCanvasUpdate(const QRect &dirty = QRect())
    {
        pendingOverlayUpdate = false;
        pendingCanvasUpdate = true;
        // Accumulate dirty rects: null means full image
        if (dirty.isNull() || pendingDirtyRect.isNull())
            pendingDirtyRect = QRect();
        else
            pendingDirtyRect = pendingDirtyRect.united(dirty);
        if (!updateTimer->isActive())
            updateTimer->start();
    }

    void scheduleOverlayUpdate(const QImage &overlay, QPainter::CompositionMode mode, const QRect &dirty = QRect())
    {
        pendingOverlayImage = overlay;
        pendingOverlayMode = mode;
        pendingOverlayUpdate = true;
        if (dirty.isNull() || pendingDirtyRect.isNull())
            pendingDirtyRect = QRect();
        else
            pendingDirtyRect = pendingDirtyRect.united(dirty);
        if (!updateTimer->isActive())
            updateTimer->start();
    }

    void flushPendingUpdate()
    {
        // Apply selection mask now — all tool QPainter scopes have unwound by
        // the time this timer callback fires, so it is safe to replace d->image.
        if (pendingSelectionMask) {
            pendingSelectionMask = false;
            if (!selection.isEmpty() && !preStrokeSnapshot.isNull()) {
                QPainterPath inside;
                inside.addPolygon(selection);
                inside.closeSubpath();
                QImage result = preStrokeSnapshot;
                QPainter p(&result);
                p.setCompositionMode(QPainter::CompositionMode_Source);
                p.setClipPath(inside);
                p.drawImage(0, 0, image);
                image = result;
            }
        }
        QRect dirty = pendingDirtyRect;
        pendingDirtyRect = QRect();
        if (pendingOverlayUpdate) {
            pendingOverlayUpdate = false;
            updateImageCanvasWithOverlay(pendingOverlayImage, pendingOverlayMode, dirty);
        } else if (pendingCanvasUpdate) {
            pendingCanvasUpdate = false;
            updateImageCanvas(dirty);
        }
    }

    void resetPendingUpdates()
    {
        updateTimer->stop();
        pendingCanvasUpdate = false;
        pendingOverlayUpdate = false;
        pendingSelectionMask = false;
        pendingDirtyRect = QRect();
    }

    void setImage(const QImage &image)
    {
        resetPendingUpdates();
        bool sizeChanged = (this->image.size() != image.size());
        if(sizeChanged)
        {
            q->setSceneRect(image.rect());
        }
        this->image = image;
        this->updateImageCanvas();
        // Adjust the grid if the image size changed and the grid is currently enabled
        if(sizeChanged && showGridEnabled)
        {
            for (int i = 0; i < lines.length(); i++)
            {
                q->scene()->removeItem(lines[i]);
            }
            lines.clear();
            QPen pen(Qt::gray);
            pen.setCosmetic(true);
            for (int x = 0; x <= image.width(); x += gridWidth)
            {
                xline = q->scene()->addLine(x, 0, x, image.height(), pen);
                lines.append(xline);
            }
            for (int y = 0; y <= image.height(); y += gridWidth)
            {
                yline = q->scene()->addLine(0, y, image.width(), y, pen);
                lines.append(yline);
            }
        }
    }

    void disconnectLastTool()
    {
        Q_ASSERT( QObject::disconnect(lastConnection) );
        Q_ASSERT( QObject::disconnect(lastOverlayConnection) );
        Q_ASSERT( QObject::disconnect(cursorConnection) );
        Q_ASSERT( QObject::disconnect(selectionConnection) );
    }

    void mousePressEvent(QGraphicsSceneMouseEvent *event)
    {
        hotspotVisible = false;
        if (currentTool)
        {
            // Set current image to the tool when we start painting.
            currentTool->setPaintDevice(&image);
            imageChanged = false;
            // Snapshot the image before the stroke so we can mask tool painting
            // to the active selection on every subsequent paint event.
            if (!selection.isEmpty())
                preStrokeSnapshot = image;
            else
                preStrokeSnapshot = QImage();
            int s_x = static_cast<int>(event->scenePos().x());
            int s_y = static_cast<int>(event->scenePos().y());
            currentTool->onMousePress(QPoint(s_x, s_y) , event->button());
        }
    }

    void mouseMoveEvent(QGraphicsSceneMouseEvent *event)
    {
        if (currentTool)
        {
            int s_x = static_cast<int>(event->scenePos().x());
            int s_y = static_cast<int>(event->scenePos().y());
            if(event->buttons() != Qt::LeftButton && event->buttons() != Qt::RightButton)
            {
                currentTool->onHover(QPoint(s_x, s_y));
                return;
            }
            currentTool->onMouseMove(QPoint(s_x, s_y));
        }
    }

    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
    {
        if (currentTool)
        {
            int s_x = static_cast<int>(event->scenePos().x());
            int s_y = static_cast<int>(event->scenePos().y());
            currentTool->onMouseRelease(QPoint(s_x, s_y));
            // Guard against tools that emit painted() without actually modifying
            // pixels (e.g. PointerTool selection/resize/move operations).
            // QImage::cacheKey() is O(1): it changes only when the image data is
            // detached, which happens when a QPainter begins on it. Pure signal
            // emissions without an open QPainter leave the key unchanged.
            const bool pixelsActuallyChanged = q->historyList.isEmpty() ||
                (image.cacheKey() != q->historyList.at(q->historyIndex).cacheKey());
            if(q->selection().size()==0 && imageChanged && pixelsActuallyChanged)
            {
                q->onContentChanged();
                q->contentChanged();
            }
        }
    }

    void keyPressEvent(QKeyEvent * keyEvent)
    {
        if (currentTool)
            currentTool->onKeyPressed(keyEvent);
    }

    void keyReleaseEvent(QKeyEvent * keyEvent)
    {
        if (currentTool)
            currentTool->onKeyReleased(keyEvent);
    }

    void showGrid(int width)
    {
        if(showGridEnabled)
        {
            for (int i=0; i<lines.length(); i++)
            {
                q->scene()->removeItem(lines[i]);
            }
            lines.clear();
            showGridEnabled = false;
        }
        else if(width > 0)
        {
            gridWidth = width;
            // Costmetic pen ensures the line width stays consistent regardless of zoom level and device pixel ratio
            QPen pen(Qt::gray);
            pen.setCosmetic(true);
            for (int x=0; x<=image.width(); x+=width)
            {
                xline = q->scene()->addLine(x,0,x,image.height(), pen);
                lines.append(xline);
            }

            for (int y=0; y<=image.height(); y+=width)
            {
                yline = q->scene()->addLine(0,y,image.width(),y, pen);
                lines.append(yline);
            }
            showGridEnabled = true;
        }
    }

    QString imagePath;
    QByteArray exifBlob;    // raw EXIF profile bytes preserved for save
    QString exifString;     // parsed EXIF key=value pairs for display
    QLabel *imageLabel;
    QImage image;
    Tool *currentTool;
    QMetaObject::Connection lastConnection;
    QMetaObject::Connection lastOverlayConnection;
    QMetaObject::Connection cursorConnection;
    QMetaObject::Connection selectionConnection;
    ImageCanvasItem *canvas;
    float scale;
    bool imageChanged;
    QPolygon selection;
    bool isSelectionVisible;
    bool hotspotVisible;
    bool showGridEnabled = false;
    int gridWidth = 1;
    QGraphicsLineItem *xline;
    QGraphicsLineItem *yline;
    QVector<QGraphicsLineItem*> lines;
    QTimer *updateTimer;
    bool pendingCanvasUpdate = false;
    bool pendingOverlayUpdate = false;
    bool pendingSelectionMask = false;
    QImage pendingOverlayImage;
    QPainter::CompositionMode pendingOverlayMode = QPainter::CompositionMode_SourceOver;
    QRect pendingDirtyRect;
    // Snapshot taken at the start of each stroke when a selection is active.
    // Used to confine tool changes to the selected region.
    QImage preStrokeSnapshot;

    // Middle-mouse-button pan state
    bool isPanning = false;
    QPoint panLastPos;

    PaintWidget *q;
};

PaintWidget::PaintWidget(const QString &imagePath, QWidget *parent)
    : QGraphicsView(parent)
    , d(new PaintWidgetPrivate(this))
    , progressIndicator(nullptr)
{
    QStringList list = imagePath.split(".");
    QString fileNameNoExt = list[1];

     QString rawExtensions[] = {
         "ARW",
         "BAY",
         "CR2",
         "DCS",
         "MOS",
         "NEF",
         "RAW"
    };
    bool exists = std::find(std::begin(rawExtensions), std::end(rawExtensions), fileNameNoExt.toUpper()) != std::end(rawExtensions);

    if(exists)
    {
        QImageReader reader(imagePath);
        QSize size = reader.size();
        int w = size.width() -1;
        int h = size.height() -1;
        QSize newSize = QSize(w,h);
        reader.setScaledSize(newSize);
        QImage raw = reader.read();
        d->initialize(raw);
    }
    else
    {
        d->initialize(QImage(imagePath));
    }
    d->imagePath = imagePath;

    // Read and store EXIF data from the original file so it can be preserved on save.
    try {
        Magick::Image gmImage(imagePath.toStdString());
        Magick::Blob exifProfile = gmImage.profile("EXIF");
        if (exifProfile.length() > 0) {
            d->exifBlob = QByteArray(static_cast<const char *>(exifProfile.data()),
                                     static_cast<int>(exifProfile.length()));
        }
        std::string exifAttr = gmImage.attribute("EXIF:*");
        if (!exifAttr.empty()) {
            d->exifString = QString::fromStdString(exifAttr);
        }
    } catch (Magick::Exception &) {
        // File format may not support EXIF — silently ignore
    }

    this->init();
}

PaintWidget::PaintWidget(const QSize &imageSize, const QColor &bgcolor, QWidget *parent)
    : QGraphicsView(parent)
    , d(new PaintWidgetPrivate(this))
    , progressIndicator(nullptr)
{
    QImage image(imageSize, bgcolor.alpha() < 255 ? QImage::Format_ARGB32_Premultiplied : QImage::Format_RGB32);
    image.fill(bgcolor);

    d->initialize(image);
    this->init();
}

void PaintWidget::init()
{
    historyIndex = 0;
    historyList.clear();
    historyList.append(d->image);
    viewport()->setMouseTracking(true);

    // Create ruler widgets and corner square
    m_rulerCorner = new QWidget(this);
    m_rulerCorner->setStyleSheet("background-color: rgb(210,210,210);");

    m_hRuler = new RulerWidget(RulerWidget::Orientation::Horizontal, this);
    m_vRuler = new RulerWidget(RulerWidget::Orientation::Vertical, this);

    // Sync unit changes between the two rulers
    connect(m_hRuler, &RulerWidget::unitChanged, m_vRuler, &RulerWidget::setUnit);
    connect(m_vRuler, &RulerWidget::unitChanged, m_hRuler, &RulerWidget::setUnit);

    // Update DPI from image metadata (dotsPerMeterX/Y → DPI)
    float dpiX = d->image.dotsPerMeterX() * 0.0254f;
    float dpiY = d->image.dotsPerMeterY() * 0.0254f;
    if (dpiX > 0.0f) m_hRuler->setDpi(dpiX);
    if (dpiY > 0.0f) m_vRuler->setDpi(dpiY);

    setViewportMargins(RulerWidget::THICKNESS, RulerWidget::THICKNESS, 0, 0);
    updateRulers();
}

PaintWidget::~PaintWidget()
{
    d->disconnectLastTool();
    delete d;
}

void PaintWidget::drawForeground(QPainter *painter, const QRectF &rect)
{
    // Lightweight method to draw a border around the image to visually separate it from the workspace
    Q_UNUSED(rect)
    QGraphicsView::drawForeground(painter, rect);
    painter->save();
    painter->resetTransform();
    QRect borderRect = mapFromScene(sceneRect()).boundingRect().adjusted(-1, -1, 0, 0);
    painter->setPen(QPen(QColor(80, 80, 80, 160), 1));
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(borderRect);
    painter->restore();
}

void PaintWidget::setPaintTool(Tool *tool)
{
    if (d->currentTool)
    {
        // If we have a pending change we apply it before changing tools
        d->currentTool->onDeactivated();
        d->disconnectLastTool();
    }

    d->currentTool = tool;

    if (d->currentTool)
    {
        // Tools are singletons shared across all tabs. Disconnect these widget-facing signals
        // from whichever widget they were previously connected to before wiring up this one.
        disconnect(d->currentTool, &Tool::painted,          nullptr, nullptr);
        disconnect(d->currentTool, &Tool::overlaid,         nullptr, nullptr);
        disconnect(d->currentTool, &Tool::cursorChanged,    nullptr, nullptr);
        disconnect(d->currentTool, &Tool::selectionChanged, nullptr, nullptr);

        d->lastConnection = connect(d->currentTool, &Tool::painted, [this] (QPaintDevice *paintDevice, QRect dirtyRect) {
            if (&d->image == paintDevice)
            {
                // Mark that selection masking is needed.  The actual image
                // modification happens in flushPendingUpdate() (timer callback)
                // so that no tool QPainter is still open on d->image when we
                // do the assignment — modifying an image under an active
                // QPainter is undefined behaviour and causes crashes.
                if (!d->selection.isEmpty() && !d->preStrokeSnapshot.isNull())
                    d->pendingSelectionMask = true;
                d->scheduleCanvasUpdate(dirtyRect);
                this->contentChanged();
                d->imageChanged = true;
            }
        });
        d->lastOverlayConnection = connect(d->currentTool, &Tool::overlaid, [this] (QPaintDevice *paintDevice, const QImage &overlayImage, QPainter::CompositionMode mode, QRect dirtyRect) {
            if (&d->image == paintDevice)
            {
                d->scheduleOverlayUpdate(overlayImage, mode, dirtyRect);
            }
        });

        d->cursorConnection = connect(d->currentTool, &Tool::cursorChanged, this, &PaintWidget::onCursorChanged);
        d->selectionConnection = connect(d->currentTool, &Tool::selectionChanged, this, &PaintWidget::onSelectionChanged);

        d->currentTool->setScale(d->scale);
        d->currentTool->setPaintDevice(&d->image);
        d->updateImageCanvas();
    }
}

void PaintWidget::setImage(const QImage &image)
{
    d->setImage(image);
    updateRulers();
    onContentChanged();
    this->contentChanged();
}

void PaintWidget::setImageOriginal(const QImage &image)
{
    // Replace the image without creating any undo steps
    d->setImage(image);
    updateRulers();
}

QImage PaintWidget::image() const
{
    return d->image;
}

void PaintWidget::onCursorChanged(QCursor cursor)
{
    viewport()->setCursor(cursor);
}

void PaintWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MiddleButton) {
        d->isPanning = true;
        d->panLastPos = event->pos();
        viewport()->setCursor(Qt::ClosedHandCursor);
        event->accept();
        return;
    }
    QGraphicsView::mousePressEvent(event);
}

void PaintWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MiddleButton && d->isPanning) {
        d->isPanning = false;
        if (d->currentTool)
            viewport()->setCursor(d->currentTool->getCursor());
        else
            viewport()->unsetCursor();
        event->accept();
        return;
    }
    QGraphicsView::mouseReleaseEvent(event);
}

void PaintWidget::mouseMoveEvent(QMouseEvent *event)
{
    if (d->isPanning) {
        const QPoint delta = event->pos() - d->panLastPos;
        d->panLastPos = event->pos();
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - delta.x());
        verticalScrollBar()->setValue(verticalScrollBar()->value() - delta.y());
        event->accept();
        return;
    }
    QGraphicsView::mouseMoveEvent(event);
    if (d->currentTool) {
        if (sceneRect().contains(mapToScene(event->pos())))
            viewport()->setCursor(d->currentTool->getCursor());
        else
            viewport()->unsetCursor();
    }
    if (m_hRuler && m_hRuler->isVisible())
        m_hRuler->setMousePos(event->pos().x());
    if (m_vRuler && m_vRuler->isVisible())
        m_vRuler->setMousePos(event->pos().y());
}

void PaintWidget::leaveEvent(QEvent *event)
{
    QGraphicsView::leaveEvent(event);
    viewport()->unsetCursor();
    if (m_hRuler) m_hRuler->clearMousePos();
    if (m_vRuler) m_vRuler->clearMousePos();
}

void PaintWidget::enterEvent(QEnterEvent *event)
{
    QGraphicsView::enterEvent(event);
    if (d->currentTool)
        viewport()->setCursor(d->currentTool->getCursor());
}

void PaintWidget::resizeEvent(QResizeEvent *event)
{
    QGraphicsView::resizeEvent(event);
    updateRulers();
}

void PaintWidget::scrollContentsBy(int dx, int dy)
{
    QGraphicsView::scrollContentsBy(dx, dy);
    updateRulers();
}

void PaintWidget::updateRulers()
{
    if (!m_hRuler || !m_vRuler || !m_rulerCorner)
        return;

    const int t = RulerWidget::THICKNESS;
    // Position corner square, horizontal ruler, and vertical ruler inside
    // the QAbstractScrollArea frame (outside the viewport).
    m_rulerCorner->setGeometry(0, 0, t, t);
    m_hRuler->setGeometry(t, 0, qMax(0, width() - t), t);
    m_vRuler->setGeometry(0, t, t, qMax(0, height() - t));

    // Compute the scene coordinate visible at screen position 0
    QPointF topLeft = mapToScene(QPoint(0, 0));
    m_hRuler->setOrigin(static_cast<float>(topLeft.x()));
    m_vRuler->setOrigin(static_cast<float>(topLeft.y()));
    m_hRuler->setScale(d->scale);
    m_vRuler->setScale(d->scale);
}

void PaintWidget::showRulers(bool visible)
{
    if (m_hRuler)     m_hRuler->setVisible(visible);
    if (m_vRuler)     m_vRuler->setVisible(visible);
    if (m_rulerCorner) m_rulerCorner->setVisible(visible);
    setViewportMargins(visible ? RulerWidget::THICKNESS : 0,
                       visible ? RulerWidget::THICKNESS : 0, 0, 0);
    updateRulers();
}

bool PaintWidget::isRulersVisible() const
{
    return m_hRuler && m_hRuler->isVisible();
}

void PaintWidget::setRulerUnit(RulerWidget::Unit unit)
{
    if (m_hRuler) m_hRuler->setUnit(unit);
    if (m_vRuler) m_vRuler->setUnit(unit);
}

void PaintWidget::onSelectionChanged(QPolygon poly)
{
    d->selection = poly;
    if(!d->selection.isEmpty())
    {
        setSelectionVisible(true);
    }
}

void PaintWidget::setImagePath(QString path)
{
    d->imagePath = path;
}

QString PaintWidget::imagePath() const
{
    return d->imagePath;
}

QByteArray PaintWidget::exifBlob() const
{
    return d->exifBlob;
}

QString PaintWidget::exifString() const
{
    return d->exifString;
}

void PaintWidget::autoScale()
{
    //Scale paint area to fit window
    float scaleX = (float)this->geometry().width() / (float)image().width();
    float scaleY = (float)this->geometry().height() / (float)image().height();
    float scaleFactor = scaleX < scaleY ? scaleX : scaleY;

    if(scaleFactor < 1)
    {
        d->scale = scaleFactor;
        resetTransform();
        scale(d->scale, d->scale);
    }

    emit zoomChanged(d->scale);

    if(d->currentTool)
        d->currentTool->setScale(d->scale);

    updateRulers();
}

void PaintWidget::setScale(const QString &rate)
{
    resetTransform();
    d->scale = (rate.mid(0,rate.lastIndexOf("%"))).toFloat() / 100.0f;
    scale(d->scale, d->scale);

    if(d->currentTool)
        d->currentTool->setScale(d->scale);

    updateRulers();
}

float PaintWidget::getScale()
{
    return d->scale;
}

void PaintWidget::showProgressIndicator(bool visible)
{
    if(visible)
    {
        progressIndicator = new QProgressIndicator;
        progressIndicator->setSize(QSize(d->image.width()/4,d->image.height()/4));
        progressIndicator->setVisible(true);
        progressIndicator->startAnimation();
        QGraphicsProxyWidget *progressIndicatorProxy = scene()->addWidget(progressIndicator);
        progressIndicatorProxy->setPos(3*d->image.width()/8, 3*d->image.height()/8);
    }
    else
    {
        if(progressIndicator)
        {
            progressIndicator->stopAnimation();
            progressIndicator->setVisible(false);
        }
    }
}

void PaintWidget::wheelEvent(QWheelEvent *event)
{
    const QPointF p0scene = mapToScene(QPoint(event->position().x(), event->position().y()));
    float scaleFactor = 1.1f;
    bool zoomDirection;

    // Zoom in or out depending on user preference
    if(SETTINGS->getZoomDirection()=="0")
    {
        zoomDirection = event->angleDelta().y() > 0;
    }
    else
    {
        zoomDirection = event->angleDelta().y() < 0;
    }

    if(zoomDirection)
    {
       d->scale = d->scale / scaleFactor;
       d->scale = (d->scale < 0.1f) ? 0.1f : d->scale;
    }
    else
    {
       d->scale = d->scale * scaleFactor;
       d->scale = (d->scale > 8) ? 8 : d->scale;
    }

    resetTransform();
    scale(d->scale, d->scale);
    emit zoomChanged(d->scale);

    if(d->currentTool)
        d->currentTool->setScale(d->scale);

    const QPointF p1mouse = mapFromScene(p0scene);
    const QPointF move = p1mouse - event->position();
    horizontalScrollBar()->setValue(move.x() + horizontalScrollBar()->value());
    verticalScrollBar()->setValue(move.y() + verticalScrollBar()->value());
    updateRulers();
}

void PaintWidget::onContentChanged()
{

    for(int i=historyList.size() - 1; i > historyIndex; i--)
    {
        historyList.removeAt(i);
    }

    historyList.append(d->image);
    historyIndex++;

    if(historyIndex > SETTINGS->getHistoryLimit().toInt())
    {
        historyList.removeFirst();
        historyIndex--;
    }
}

void PaintWidget::revert()
{
    historyIndex = 0;
    d->setImage(historyList.at(historyIndex));
    updateRulers();
    this->contentChanged();
}

void PaintWidget::undo()
{
    if(isUndoEnabled())
    {
        historyIndex--;
        d->setImage(historyList.at(historyIndex));
        updateRulers();
        this->contentChanged();
    }
}

void PaintWidget::redo()
{
    if(isRedoEnabled())
    {
        historyIndex++;
        d->setImage(historyList.at(historyIndex));
        updateRulers();
        this->contentChanged();
    }
}

bool PaintWidget::isUndoEnabled()
{
    return (historyIndex > 0);
}

bool PaintWidget::isRedoEnabled()
{
    return (historyIndex < historyList.size() - 1);
}

int PaintWidget::undoCount()
{
    return historyIndex;
}

void PaintWidget::clearUndoHistory()
{
    historyIndex = 0;
    historyList.clear();
    historyList.append(d->image);
}

void PaintWidget::setSelectionVisible(bool visible)
{
    d->isSelectionVisible = visible;
    d->updateImageCanvas();
    emit selectionChanged(visible);
}

QPolygon PaintWidget::selection()
{
    return d->selection;
}

bool PaintWidget::isSelectionVisible()
{
    return d->isSelectionVisible;
}

void PaintWidget::setHotspotVisble(bool visible)
{
    d->hotspotVisible = visible;
}

void PaintWidget::clearStrokeSnapshot()
{
    d->preStrokeSnapshot = QImage();
}

void PaintWidget::selectAll()
{
    d->selection = d->image.rect();
    d->updateImageCanvas();
    emit selectionChanged(true);
}



void PaintWidget::showGrid(int width)
{
    d->showGrid(width);
}

bool PaintWidget::isGridEnabled()
{
    return d->showGridEnabled;
}

int PaintWidget::gridWidth() const
{
    return d->gridWidth;
}
