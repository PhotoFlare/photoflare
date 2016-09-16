#include <QPainter>
#include <QStyledItemDelegate>
#include <QStyleOptionViewItem>

#include "BrushTypeComboBox.h"

class BrushTypeDelegate : public QStyledItemDelegate
{
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        QStyledItemDelegate::paint(painter, option, index);
        const QPixmap pixmap = index.data(Qt::UserRole).value<QPixmap>();
        QRect rect = QRect(option.rect.x(), option.rect.y(), 32, 32);
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
    QRect rect = QRect(1,1,32,this->height()-1);

    QPainter painter(this);
    painter.drawPixmap(rect, pixmap);
}
