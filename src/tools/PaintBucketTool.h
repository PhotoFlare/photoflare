#ifndef PAINTBUCKETTOOL_H
#define PAINTBUCKETTOOL_H

#include "Tool.h"

class MainWindow;

class PaintBucketTool : public Tool
{
    Q_OBJECT
public:
    explicit PaintBucketTool(QObject *parent = 0);
    virtual ~PaintBucketTool();

    void onMousePress(const QPoint &pos, Qt::MouseButton button) override;

signals:
    void floodFillPrimaryColor(const QPoint&);
    void floodFillSecondaryColor(const QPoint&);
};

#endif // PAINTBUCKETTOOL_H
