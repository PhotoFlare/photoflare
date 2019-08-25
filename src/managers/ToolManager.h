/*
    This file is part of Photoflare.

    Photoflare is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Photoflare is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Photoflare.  If not, see <https://www.gnu.org/licenses/>.
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
class BlurTool;
class EraserTool;
class SmudgeTool;

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
    BlurTool* blurTool() const;
    EraserTool* eraserTool() const;
    SmudgeTool* smudgeTool() const;

private:
    explicit ToolManager(QObject *parent = 0);
    Q_DISABLE_COPY(ToolManager)

    ToolManagerPrivate *d;

    static ToolManager *m_instance;
};

#endif // TOOLMANAGER_H
