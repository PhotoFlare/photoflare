#ifndef PAINTBRUSHTOOL_H
#define PAINTBRUSHTOOL_H

#include "Tool.h"

class PaintBrushToolPrivate;

class PaintBrushTool : public Tool
{
public:
    PaintBrushTool(QObject *parent = 0);
    ~PaintBrushTool();

    void setWidth(int width);
    int width() const;

    // This color is used when painting the left mouse button
    QColor primaryColor() const;

    // This color is used when painting the right mouse button
    QColor secondaryColor() const;

    void setAntialiasing(bool value);
    bool antialiasing() const;

    void setPressure(int value) const;
    int pressure() const;

    QCursor getCursor();

    void setCapStyle(Qt::PenCapStyle capStyle);

    void onMousePress(const QPoint &pos, Qt::MouseButton button) override;
    void onMouseMove(const QPoint &pos) override;
    void onMouseRelease(const QPoint &pos) override;

public slots:
    void setPrimaryColor(const QColor &color);
    void setSecondaryColor(const QColor &color);

private:
    PaintBrushToolPrivate *d;
};



#endif // PAINTBRUSHTOOL_H

