#include "EraserTool.h"
#include "../PaintWidget.h"

class EraserToolPrivate
{
public:
    EraserToolPrivate()
    {
       // primaryPen = QPen(QBrush(), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
       // secondaryPen = QPen(QBrush(), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    }
    ~EraserToolPrivate()
    {

    }


};

EraserTool::EraserTool(QObject *parent)
    : Tool(parent)
    , d(new EraserToolPrivate)
{

}

EraserTool::~EraserTool()
{
    delete d;
}
