/*

  Tool manager class. Handles the different drawing tools.

*/


#ifndef TOOLMANAGER_H
#define TOOLMANAGER_H

#include <QObject>

class ColourPickerTool;
class PaintBrushTool;
class PaintBrushAdvTool;
class PaintBucketTool;
class ToolManagerPrivate;
class PointerTool;
class TextTool;
class SprayCanTool;
class MagicWandTool;
class LineTool;
class StampTool;

class ToolManager : public QObject
{
    Q_OBJECT
public:
    static ToolManager* instance();
    ~ToolManager();

    ColourPickerTool* colourPicker() const;
    PaintBrushTool* paintBrush() const;
    PaintBrushAdvTool* paintBrushAdv() const;
    PaintBucketTool* paintBucket() const;
    PointerTool* mousePointer() const;
    TextTool* textTool() const;
    SprayCanTool* sprayCanTool() const;
    LineTool* lineTool() const;
    MagicWandTool* magicWandTool() const;
    StampTool* stampTool() const;

private:
    explicit ToolManager(QObject *parent = 0);
    Q_DISABLE_COPY(ToolManager)

    ToolManagerPrivate *d;

    static ToolManager *m_instance;
};

#endif // TOOLMANAGER_H
