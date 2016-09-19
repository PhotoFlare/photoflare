#ifndef MAGICWANDTTOOL_H
#define MAGICWANDTTOOL_H

#include "Tool.h"

class MainWindow;
class MagicWandToolPrivate;

class MagicWandTool : public Tool
{
    Q_OBJECT
public:
    explicit MagicWandTool(QObject *parent = 0);
    virtual ~MagicWandTool();

    void onMousePress(const QPoint &pos, Qt::MouseButton button) override;

    void setSelection(const QPolygon& poly);
signals:
    void selectPrimaryColor(const QPoint&);

private:
    MagicWandToolPrivate *d;
};

#endif // MAGICWANDTTOOL_H
