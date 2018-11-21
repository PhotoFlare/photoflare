#ifndef PAINTBRUSHADVTOOL_H
#define PAINTBRUSHADVTOOL_H

#include "Tool.h"

class PaintBrushAdvToolPrivate;

class PaintBrushAdvTool : public Tool
{
public:
    PaintBrushAdvTool(QObject *parent = 0);
    ~PaintBrushAdvTool();

    void setWidth(int width);
    int width() const;

    // This color is used when painting the left mouse button
    QColor primaryColor() const;

    // This color is used when painting the right mouse button
    QColor secondaryColor() const;

    void setBrushPixmap(QPixmap pixmap);

    void setCapStyle(Qt::PenCapStyle capStyle);
    void setPressure(int pressure);
    void setFade(bool fade);
    void setStep(int step);

    QCursor getCursor();

    void onMousePress(const QPoint &pos, Qt::MouseButton button) override;
    void onMouseMove(const QPoint &pos) override;
    void onMouseRelease(const QPoint &pos) override;

public slots:
    void setPrimaryColor(const QColor &color);
    void setSecondaryColor(const QColor &color);

private:
    PaintBrushAdvToolPrivate *d;
};



#endif // PAINTBRUSHADVTOOL_H

