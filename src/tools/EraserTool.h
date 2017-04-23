#ifndef ERASERTOOL_H
#define ERASERTOOL_H

#include "Tool.h"

class EraserToolPrivate;

class EraserTool : public Tool
{
public:
    EraserTool(QObject *parent = 0);
    ~EraserTool();

private:
    EraserToolPrivate *d;

};

#endif // ERASERTOOL_H
