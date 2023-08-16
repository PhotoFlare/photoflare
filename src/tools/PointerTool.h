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

#ifndef POINTERTOOL_H
#define POINTERTOOL_H

#include "Tool.h"

class PointerToolPrivate;

class PointerTool : public Tool
{
    Q_OBJECT
public:
    PointerTool(QObject *parent = nullptr);
    ~PointerTool() override;

    void setOverlayImage(const QImage& image);
    void onMousePress(const QPoint &pos, Qt::MouseButton button) override;
    void onMouseMove(const QPoint &pos) override;
    void onMouseRelease(const QPoint &pos) override;
    void onKeyPressed(QKeyEvent *keyEvent) override;
    void setStroke(bool enabled);
    void setFill(bool enabled);
    void setFillColor(const QColor &color);

public slots:
    void onCrop();
    void onFillRect();
    void onSave();
    void onSaveAs();
    void onClose();
    void onImageSize();
    void onCanvasSize();
    void onCopy();
    void onPaste();
    void onUndo();
    void onRedo();

signals:
    void crop(const QRect&);
    void fillRect(const QRect&, const QColor&);
    void save();
    void saveAs();
    void close();
    void imageSize();
    void canvasSize();
    void copy();
    void paste();
    void undo();
    void redo();
    void showhotspots();

private:
    PointerToolPrivate *d;
    int x_pos;
    int y_pos;
    void setupRightClickMenu(bool execute);
};

#endif // POINTERTOOL_H
