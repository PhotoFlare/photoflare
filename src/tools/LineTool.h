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

    void onMousePress(const QPoint &pos, Qt::MouseButton button) override;
    void onMouseMove(const QPoint &pos) override;
    void onMouseRelease(const QPoint &pos) override;

public slots:
    void setPrimaryColor(const QColor &color);
    void setSecondaryColor(const QColor &color);
    void setWidth(int width);
    void setAntialias(bool antialias);
    void setOpacity(int opacity);
    void setStyle(int style);

private:
    LineToolPrivate *d;
};



#endif // LINETOOL_H

