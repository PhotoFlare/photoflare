#ifndef SMUDGETOOL_H
#define SMUDGETOOL_H

#include "Tool.h"

class SmudgeToolPrivate;

class SmudgeTool : public Tool
{
public:
    SmudgeTool(QObject *parent = 0);
    ~SmudgeTool();

    QCursor getCursor();
    void setRadius(int radius);
    void onMousePress(const QPoint &pos, Qt::MouseButton button) override;
    void onMouseMove(const QPoint &pos) override;

private:
    SmudgeToolPrivate *d;
};

#endif // SMUDGETOOL_H
