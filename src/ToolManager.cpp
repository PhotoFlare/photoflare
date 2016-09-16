/*

  Tools manager class. Handles the different drawing tools.

*/


#include "ToolManager.h"
#include "./tools/ColourPickerTool.h"
#include "./tools/PaintBrushTool.h"
#include "./tools/PaintBrushAdvTool.h"
#include "./tools/PaintBucketTool.h"
#include "./tools/PointerTool.h"
#include "./tools/TextTool.h"
#include "./tools/SprayCanTool.h"
#include "./tools/LineTool.h"

ToolManager * ToolManager::m_instance = 0;

class ToolManagerPrivate
{
public:
    ToolManagerPrivate()
    {
        colourPicker = new ColourPickerTool;
        paintBrush = new PaintBrushTool;
        paintBrushAdv = new PaintBrushAdvTool;
        paintBucket = new PaintBucketTool;
        mousePointer = new PointerTool;
        textTool = new TextTool;
        sprayCanTool = new SprayCanTool;
        lineTool = new LineTool;
    }

    ~ToolManagerPrivate()
    {
        delete colourPicker;
        delete paintBrush;
        delete paintBrushAdv;
        delete paintBucket;
        delete mousePointer;
        delete textTool;
        delete sprayCanTool;
        delete lineTool;
    }

    ColourPickerTool *colourPicker;
    PaintBrushTool *paintBrush;
    PaintBrushAdvTool *paintBrushAdv;
    PaintBucketTool *paintBucket;
    PointerTool *mousePointer;
    TextTool *textTool;
    SprayCanTool *sprayCanTool;
    LineTool *lineTool;
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

PaintBrushAdvTool *ToolManager::paintBrushAdv() const
{
    return d->paintBrushAdv;
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

SprayCanTool *ToolManager::sprayCanTool() const
{
    return d->sprayCanTool;
}

LineTool *ToolManager::lineTool() const
{
    return d->lineTool;
}

ToolManager *ToolManager::instance()
{
    if (!m_instance)
        m_instance = new ToolManager;

    return m_instance;
}
