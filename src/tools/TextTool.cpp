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

void TextTool::setText(const QString &text, const QFont &font, const QColor &color)
{
    d->text = text;
    d->font = font;
    d->color = color;

    QFontMetrics fontMetrics(font);
    d->textRect = fontMetrics.boundingRect(d->textRect ,Qt::TextWordWrap, text);

    d->previewMode = true;
    previewText();
}

void TextTool::previewText()
{
    if (m_paintDevice) {
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
        painter.drawText(d->textRect, Qt::TextWordWrap, d->text);

        painter.end();

        emit overlaid(m_paintDevice, surface, QPainter::CompositionMode_Difference);
    }
}

void TextTool::drawText()
{
    if (m_paintDevice) {
        QPainter painter(m_paintDevice);
        painter.setPen(d->color);
        painter.setFont(d->font);
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
    }
}

void TextTool::onMousePress(const QPoint &pos, Qt::MouseButton button)
{
    switch(button)
    {
        case Qt::LeftButton:
            d->firstPos = pos;
            d->secondPos = pos;
            if(!d->textRect.contains(pos)) {
                disconnect();
            }
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
    if(d->previewMode) {
        d->textRect.moveTo(pos);
        previewText();
    }
}

void TextTool::onMouseRelease(const QPoint &pos)
{
    if(d->previewMode &&
       d->firstPos == d->secondPos &&
       d->textRect.contains(pos))
    {
        emit editText(d->text, d->font, d->color);
    }
}



