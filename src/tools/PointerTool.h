#ifndef POINTERTOOL_H
#define POINTERTOOL_H

#include "Tool.h"

class PointerToolPrivate;

class PointerTool : public Tool
{
    Q_OBJECT
public:
    PointerTool(QObject *parent = 0);
    ~PointerTool();

    void setOverlayImage(const QImage& image);

    void onMousePress(const QPoint &pos, Qt::MouseButton button) override;
    void onMouseMove(const QPoint &pos) override;
    void onMouseRelease(const QPoint &pos) override;
    void onKeyPressed(QKeyEvent *keyEvent) override;
    void setStroke(bool enabled);
    void setFill(bool enabled);

public slots:
    void onCrop();
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
