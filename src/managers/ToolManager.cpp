/*

  Tools manager class. Handles the different drawing tools.

*/

#include "ToolManager.h"
#include "./ColourPickerTool.h"
#include "./PaintBrushTool.h"
#include "./PaintBrushAdvTool.h"
#include "./PaintBucketTool.h"
#include "./PointerTool.h"
#include "./TextTool.h"
#include "./SprayCanTool.h"
#include "./LineTool.h"
#include "./MagicWandTool.h"
#include "./StampTool.h"
#include "./BlurTool.h"
#include "./EraserTool.h"
#include "./SmudgeTool.h"

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
        magicWandTool = new MagicWandTool;
        stampTool = new StampTool;
        blurTool = new BlurTool;
        eraserTool = new EraserTool;
        smudgeTool = new SmudgeTool;
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
        delete magicWandTool;
        delete stampTool;
        delete eraserTool;
        delete smudgeTool;
    }

    ColourPickerTool *colourPicker;
    PaintBrushTool *paintBrush;
    PaintBrushAdvTool *paintBrushAdv;
    PaintBucketTool *paintBucket;
    PointerTool *mousePointer;
    TextTool *textTool;
    SprayCanTool *sprayCanTool;
    LineTool *lineTool;
    MagicWandTool *magicWandTool;
    StampTool *stampTool;
    BlurTool *blurTool;
    EraserTool *eraserTool;
    SmudgeTool *smudgeTool;
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

MagicWandTool *ToolManager::magicWandTool() const
{
    return d->magicWandTool;
}


StampTool *ToolManager::stampTool() const
{
    return d->stampTool;
}

BlurTool *ToolManager::blurTool() const
{
    return d->blurTool;
}

EraserTool *ToolManager::eraserTool() const
{
    return d->eraserTool;
}

SmudgeTool *ToolManager::smudgeTool() const
{
    return d->smudgeTool;
}

ToolManager *ToolManager::instance()
{
    if (!m_instance)
        m_instance = new ToolManager;

    return m_instance;
}
