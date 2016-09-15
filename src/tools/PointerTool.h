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

    void onMousePress(const QPoint &pos, Qt::MouseButton button) override;
    void onMouseMove(const QPoint &pos) override;
    void onMouseRelease(const QPoint &pos) override;

public slots:
    void onCrop();
signals:
    void crop(const QRect&);

private:
    PointerToolPrivate *d;
};



#endif // POINTERTOOL_H

