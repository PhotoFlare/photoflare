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

// TextTool - Put text on the image. Tool not present in the Toolpalette but launched from the TextDialog.

#include "TextTool.h"

#include <QPainter>
#include <QMenu>

class TextToolPrivate
{
public:
    TextToolPrivate()
    {
        textRect = QRect(0,0,0,0);
    }
    ~TextToolPrivate()
    {
    }

    QPoint firstPos;
    QPoint secondPos;
    QRect textRect;
    QString text;
    QFont font;
    QColor color;
    bool antialiasEnabled;
    QString position;
    bool previewMode;
};

TextTool::TextTool(QObject *parent)
    : Tool(parent)
    , d(new TextToolPrivate)
{

}

TextTool::~TextTool()
{
    delete d;
}

void TextTool::setText(const QString &text, const QFont &font, const QColor &color, const bool &antialiasEnabled, const QString &position)
{
    const QImage *image = dynamic_cast<QImage*>(m_paintDevice);
    d->text = text;
    d->font = font;
    d->color = color;
    d->antialiasEnabled = antialiasEnabled;
    d->position = position;

    QFontMetrics fontMetrics(font);
    d->textRect = fontMetrics.boundingRect(d->textRect ,Qt::TextLongestVariant, text);

    if(d->position == "left")
    {
        d->textRect.moveTo(QPoint(0,image->height()/2));
    }
    else if(d->position == "center")
    {
        d->textRect.moveCenter(QPoint(image->width()/2,image->height()/2));
    }
    else if(d->position == "right")
    {
        d->textRect.moveCenter(QPoint(image->width()-(d->textRect.width()/2),image->height()/2));
    }

    d->previewMode = true;
    previewText();
}

void TextTool::previewText()
{
    if (m_paintDevice)
    {
        const QImage *image = dynamic_cast<QImage*>(m_paintDevice);
        QImage surface = QImage(image->size(), QImage::Format_ARGB32_Premultiplied);

        QPainter painter(&surface);
        painter.setCompositionMode(QPainter::CompositionMode_Source);
        painter.fillRect(surface.rect(), Qt::transparent);
        QPen pen = QPen(QBrush(), 1, Qt::DashLine);
        pen.setColor(Qt::gray);
        painter.setPen(pen);
        painter.drawRect(d->textRect);

        painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
        painter.fillRect(d->textRect, Qt::gray);
        painter.setPen(d->color);
        painter.setFont(d->font);
        painter.setRenderHint(QPainter::Antialiasing, d->antialiasEnabled);
        painter.drawText(d->textRect, Qt::TextWordWrap, d->text);

        painter.end();

        emit overlaid(m_paintDevice, surface, QPainter::CompositionMode_Difference);
    }
}

void TextTool::drawText()
{
    if (m_paintDevice)
    {
        QPainter painter(m_paintDevice);
        painter.setPen(d->color);
        painter.setFont(d->font);
        painter.setRenderHint(QPainter::Antialiasing, d->antialiasEnabled);
        painter.drawText(d->textRect, d->text);
        painter.end();

        emit painted(m_paintDevice);
    }
}

void TextTool::disconnect()
{
    if(d->previewMode)
    {
        drawText();
        d->previewMode = false;
        d->textRect = QRect(0,0,0,0);
        emit editTextFinished();
    }
}

void TextTool::onMousePress(const QPoint &pos, Qt::MouseButton button)
{
    switch(button)
    {
        case Qt::LeftButton:
            d->firstPos = pos;
            d->secondPos = pos;
            if(!d->textRect.contains(pos))
                disconnect();
            break;
        case Qt::RightButton:
            break;
        default:
            break;
    }
}

void TextTool::onMouseMove(const QPoint &pos)
{
    d->secondPos = pos;
    if(d->previewMode)
    {
        d->textRect.moveTo(pos);
        previewText();
    }
}

void TextTool::onMouseRelease(const QPoint &pos)
{
    if(d->previewMode && d->firstPos == d->secondPos && d->textRect.contains(pos))
    {
        emit editText(d->text, d->font, d->color);
    }
}



