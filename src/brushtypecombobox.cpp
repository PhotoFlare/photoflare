/*

  Special combo to hold Brush Types.

*/

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
