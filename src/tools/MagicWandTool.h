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

#ifndef MAGICWANDTTOOL_H
#define MAGICWANDTTOOL_H

#include "Tool.h"

class MainWindow;
class MagicWandToolPrivate;

class MagicWandTool : public Tool
{
    Q_OBJECT
public:
    explicit MagicWandTool(QObject *parent = nullptr);
    virtual ~MagicWandTool() override;

    void onMousePress(const QPoint &pos, Qt::MouseButton button) override;
    void setSelection(const QPolygon& poly);
    void setTolerance(int tolerance);
    void setColor(bool color);

signals:
    void selectPrimaryColor(const QPoint&,int,bool);

private:
    MagicWandToolPrivate *d;
};

#endif // MAGICWANDTTOOL_H
