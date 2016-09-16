#ifndef LINETOOL_H
#define LINETOOL_H

#include "Tool.h"

class LineToolPrivate;

class LineTool : public Tool
{
    Q_OBJECT
public:
    LineTool(QObject *parent = 0);
    ~LineTool();

    void setOverlayImage(const QImage& image);

    void onMousePress(const QPoint &pos, Qt::MouseButton button) override;
    void onMouseMove(const QPoint &pos) override;
    void onMouseRelease(const QPoint &pos) override;

public slots:
    void onCrop();
    void onCopy();
    void onPaste();
signals:
    void crop(const QRect&);
    void copy();
    void paste();

private:
    LineToolPrivate *d;
};



#endif // LINETOOL_H

