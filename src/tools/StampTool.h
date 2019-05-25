#ifndef STAMPTOOL_H
#define STAMPTOOL_H

#include "Tool.h"

class StampToolPrivate;

class StampTool : public Tool
{
public:
    StampTool(QObject *parent = 0);
    ~StampTool();

    // This color is used when painting the left mouse button
    QColor primaryColor() const;

    // This color is used when painting the right mouse button
    QColor secondaryColor() const;

    void setRadius(int radius);
    void setPressure(int pressure);
    void setStep(int step);
    void setFixed(bool fixed);
    void setPrecise(bool precise);
    void setDiffuse(bool diffuse);

    QCursor getCursor() override;

    void setCapStyle(Qt::PenCapStyle capStyle);

    void onMousePress(const QPoint &pos, Qt::MouseButton button) override;
    void onMouseMove(const QPoint &pos) override;
    void onMouseRelease(const QPoint &pos) override;
    void onKeyPressed(QKeyEvent * keyEvent) override;
    void onKeyReleased(QKeyEvent * keyEvent) override;
public slots:
    void setPrimaryColor(const QColor &color);
    void setSecondaryColor(const QColor &color);

private:
    StampToolPrivate *d;
};



#endif // STAMPTOOL_H

