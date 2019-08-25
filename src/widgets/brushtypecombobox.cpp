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

// Special combo to hold Brush Types.

#include <QPainter>
#include <QStyledItemDelegate>
#include <QStyleOptionViewItem>
#include <QMouseEvent>

#include "brushtypecombobox.h"

class BrushTypeDelegate : public QStyledItemDelegate
{
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        QStyledItemDelegate::paint(painter, option, index);
        const QPixmap pixmap = index.data(Qt::UserRole).value<QPixmap>();
        QRect rect = QRect(option.rect.x(), option.rect.y(), pixmap.width(),pixmap.height());
        painter->drawPixmap(rect, pixmap);
    }

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        Q_UNUSED(option)
        const QPixmap pixmap = index.data(Qt::UserRole).value<QPixmap>();
        return pixmap.size();
    }
};

BrushTypeComboBox::BrushTypeComboBox(QWidget *parent) : QComboBox(parent)
    ,onClickHandler(0)
{
    setItemDelegate(new BrushTypeDelegate());
}

BrushTypeComboBox::~BrushTypeComboBox()
{
}

void BrushTypeComboBox::paintEvent(QPaintEvent* pEvent)
{
    QComboBox::paintEvent(pEvent);
    const QPixmap pixmap = this->itemData(this->currentIndex(), Qt::UserRole).value<QPixmap>();
    QRect rect = QRect(1,1,pixmap.width()-20,pixmap.height()-2);

    QPainter painter(this);
    painter.drawPixmap(rect, pixmap);
}

void BrushTypeComboBox::mousePressEvent(QMouseEvent *e)
{
    if(onClickHandler)
    {
        onClickHandler->mousePressEvent(this,e);
        if(e->isAccepted())
            QComboBox::mousePressEvent(e);
    } else
    {
        QComboBox::mousePressEvent(e);
    }
}

void BrushTypeComboBox::setOnClickHandler(QComboBoxEventsInterface *handler)
{
    onClickHandler = handler;
}
