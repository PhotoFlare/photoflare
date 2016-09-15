/*

  Tools manager class. Handles the different drawing tools.

*/


#include "ToolManager.h"
#include "./tools/ColourPickerTool.h"
#include "./tools/PaintBrushTool.h"
#include "./tools/PaintBucketTool.h"
#include "./tools/PointerTool.h"
#include "./tools/TextTool.h"

ToolManager * ToolManager::m_instance = 0;

class ToolManagerPrivate
{
public:
    ToolManagerPrivate()
    {
        colourPicker = new ColourPickerTool;
        paintBrush = new PaintBrushTool;
        paintBucket = new PaintBucketTool;
        mousePointer = new PointerTool;
        textTool = new TextTool;
    }

    ~ToolManagerPrivate()
    {
        delete colourPicker;
        delete paintBrush;
        delete paintBucket;
        delete mousePointer;
        delete textTool;
    }

    ColourPickerTool *colourPicker;
    PaintBrushTool *paintBrush;
    PaintBucketTool *paintBucket;
    PointerTool *mousePointer;
    TextTool *textTool;
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

PaintBucketTool *ToolManager::paintBucket() const
{
    return d->paintBucket;
}

PointerTool *ToolManager::mousePointer() const
{
    return d->mousePointer;
}

TextTool *ToolManager::textTool() const
{
    return d->textTool;
}

ToolManager *ToolManager::instance()
{
    if (!m_instance)
        m_instance = new ToolManager;

    return m_instance;
}
