#ifndef SMUDGETOOL_H
#define SMUDGETOOL_H

#include "Tool.h"

class SmudgeToolPrivate;

class SmudgeTool : public Tool
{

public:
    SmudgeTool(QObject *parent = 0);
    ~SmudgeTool();

    QCursor getCursor() override;

    void setRadius(int radius);
    void setPressure(int pressure);

    void onMousePress(const QPoint &pos, Qt::MouseButton button) override;
    void onMouseMove(const QPoint &pos) override;
    void onMouseRelease(const QPoint &pos) override;

private:
    void fillPattern();

private:
    SmudgeToolPrivate *d;
};

#endif // SMUDGETOOL_H
