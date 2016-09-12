/*

  Tools manager class. Handles the different drawing tools.

*/


#include "ToolManager.h"
#include "./tools/PaintBrushTool.h"

ToolManager * ToolManager::m_instance = 0;

class ToolManagerPrivate
{
public:
    ToolManagerPrivate()
    {
        paintBrush = new PaintBrushTool;
    }

    ~ToolManagerPrivate()
    {
        delete paintBrush;
    }

    PaintBrushTool *paintBrush;
};

ToolManager::ToolManager(QObject *parent)
    : QObject(parent)
    , d(new ToolManagerPrivate)
{

}

ToolManager::~ToolManager()
{
    delete d;
}

PaintBrushTool *ToolManager::paintBrush() const
{
    return d->paintBrush;
}

ToolManager *ToolManager::instance()
{
    if (!m_instance)
        m_instance = new ToolManager;

    return m_instance;
}
