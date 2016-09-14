/*

  Tools manager class. Handles the different drawing tools.

*/


#include "ToolManager.h"
#include "./tools/ColourPickerTool.h"
#include "./tools/PaintBrushTool.h"

ToolManager * ToolManager::m_instance = 0;

class ToolManagerPrivate
{
public:
    ToolManagerPrivate()
    {
        colourPicker = new ColourPickerTool;
        paintBrush = new PaintBrushTool;
    }

    ~ToolManagerPrivate()
    {
        delete colourPicker;
        delete paintBrush;
    }

    ColourPickerTool *colourPicker;
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

ColourPickerTool *ToolManager::colourPicker() const
{
    return d->colourPicker;
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
