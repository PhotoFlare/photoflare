/*

  Tools manager class. Handles the different drawing tools.

*/


#include "ToolManager.h"
#include "./tools/ColourPickerTool.h"
#include "./tools/PaintBrushTool.h"
#include "./tools/PaintBucketTool.h"
#include "./tools/PointerTool.h"

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
    }

    ~ToolManagerPrivate()
    {
        delete colourPicker;
        delete paintBrush;
        delete paintBucket;
        delete mousePointer;
    }

    ColourPickerTool *colourPicker;
    PaintBrushTool *paintBrush;
    PaintBucketTool *paintBucket;
    PointerTool *mousePointer;
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

ToolManager *ToolManager::instance()
{
    if (!m_instance)
        m_instance = new ToolManager;

    return m_instance;
}
