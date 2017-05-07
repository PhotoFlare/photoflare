#ifndef ERASERTOOL_H
#define ERASERTOOL_H

#include "Tool.h"

class EraserToolPrivate;

class EraserTool : public Tool
{
public:
    EraserTool(QObject *parent = 0);
    ~EraserTool();
    void setWidth(int width);
    int width() const;
    void onMousePress(const QPoint &pos, Qt::MouseButton button) override;
    void onMouseMove(const QPoint &pos) override;

private:
    EraserToolPrivate *d;

};

#endif // ERASERTOOL_H
