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

#ifndef TEXTTOOL_H
#define TEXTTOOL_H

#include "Tool.h"

class TextToolPrivate;

class TextTool : public Tool
{
    Q_OBJECT
public:
    TextTool(QObject *parent = 0);
    ~TextTool();

    void setText(const QString &text, const QFont &font, const QColor&, const bool &antialiasEnabled, const QString &position);

    void onMousePress(const QPoint &pos, Qt::MouseButton button) override;
    void onMouseMove(const QPoint &pos) override;
    void onMouseRelease(const QPoint &pos) override;

    void disconnect() override;
public slots:

signals:
    void editText(const QString&, const QFont&, const QColor&);
    void editTextFinished();

private:
    void previewText();
    void drawText();

    TextToolPrivate *d;
};



#endif // TEXTTOOL_H

