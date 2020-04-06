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

#ifndef COLOURPICKERTOOL_H
#define COLOURPICKERTOOL_H

#include "Tool.h"

class ColourPickerToolPrivate;
class MainWindow;

class ColourPickerTool : public Tool
{
    Q_OBJECT
public:
    explicit ColourPickerTool(QObject *parent = nullptr);
    virtual ~ColourPickerTool() override;

    void onMousePress(const QPoint &pos, Qt::MouseButton button) override;
    QCursor getCursor() override;

signals:
    void pickPrimaryColor(const QPoint&);
    void pickSecondaryColor(const QPoint&);

private:
    ColourPickerToolPrivate *d;
};

#endif // COLOURPICKERTOOL_H
