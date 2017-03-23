#ifndef SPRAYCANTOOL_H
#define SPRAYCANTOOL_H

#include "Tool.h"

class SprayCanToolPrivate;

class SprayCanTool : public Tool
{
public:
    SprayCanTool(QObject *parent = 0);
    ~SprayCanTool();

    // This color is used when painting the left mouse button
    QColor primaryColor() const;

    // This color is used when painting the right mouse button
    QColor secondaryColor() const;

    void setRadius(int radius);
    void setPressure(int pressure);
    void setDispersion(int dispersion);
    void setRainbow(bool rainbow);

    void onMousePress(const QPoint &pos, Qt::MouseButton button) override;
    void onMouseMove(const QPoint &pos) override;
    void onMouseRelease(const QPoint &pos) override;
    QCursor getCursor();

public slots:
    void setPrimaryColor(const QColor &color);
    void setSecondaryColor(const QColor &color);

private:
    void fillPattern();
    void timerEvent(QTimerEvent *event);
private:
    SprayCanToolPrivate *d;
};



#endif // SPRAYCANTOOL_H

