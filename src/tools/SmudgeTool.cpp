/*

  SmudgeTool - Smudge pixels while dragging the mouse.

*/

#include "SmudgeTool.h"
#include "../PaintWidget.h"

class SmudgeToolPrivate
{
public:
    SmudgeToolPrivate()
    {

    }
    ~SmudgeToolPrivate()
    {

    }
};

SmudgeTool::SmudgeTool(QObject *parent)
    : Tool(parent)
    , d(new SmudgeToolPrivate)
{

}

SmudgeTool::~SmudgeTool()
{
    delete d;
}
