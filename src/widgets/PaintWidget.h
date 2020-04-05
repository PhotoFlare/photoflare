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

#ifndef PAINTWIDGET_H
#define PAINTWIDGET_H

#include <QGraphicsView>

class PaintWidgetPrivate;
class QProgressIndicator;
class Tool;

class PaintWidget : public QGraphicsView
{
    friend class PaintWidgetPrivate;
    Q_OBJECT

public:
    /*!
     * Constructs the widget that contains an image loaded from imagePath
     */
    PaintWidget(const QString &imagePath, QWidget *parent = nullptr);
    /*!
     * Constructs the widget that contains an empty image with passed size
     */
    PaintWidget(const QSize &imageSize, const QColor &bgcolor, QWidget *parent = nullptr);
    ~PaintWidget();

    void setPaintTool(Tool *tool);
    void setImageOriginal(const QImage &image);
    void setImage(const QImage &image);
    QImage image() const;
    void onCursorChanged(QCursor cursor);
    void onSelectionChanged(QPolygon poly);
    void setImagePath(QString path);
    QString imagePath() const;
    void autoScale();
    void setScale(const QString &scale);
    float getScale();
    void undo();
    void redo();
    void revert();
    int undoCount();
    bool isUndoEnabled();
    bool isRedoEnabled();
    void clearUndoHistory();
    void setSelectionVisible(bool visible);
    QPolygon selection();
    bool isSelectionVisible();
    void setHotspotVisble(bool visible);
    void selectAll();
    void setImageModeIndexed(bool mode);
    void showProgressIndicator(bool visible);
    void showGrid(int width);
    bool isGridEnabled();
signals:
    // Temporary signal to inform UI that user painted something.
    void contentChanged();

    void zoomChanged(float scale);
    void selectionChanged(bool visible);

protected:
    void wheelEvent(QWheelEvent *event);

private:
    PaintWidgetPrivate *d;
    QList<QImage> historyList;
    int historyIndex;
    QProgressIndicator *progressIndicator;

private slots:
    void onContentChanged();
    void init();
};

#endif // PAINTWIDGET_H
