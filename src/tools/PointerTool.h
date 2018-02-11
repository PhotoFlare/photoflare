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
    void setStroke(bool enabled);
    void setFill(bool enabled);

public slots:
    void onCrop();
    void onSave();
    void onSaveAs();
    void onCopy();
    void onPaste();
signals:
    void crop(const QRect&);
    void save();
    void saveAs();
    void copy();
    void paste();

private:
    PointerToolPrivate *d;
};

#endif // POINTERTOOL_H
