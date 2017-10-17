/*

  Dialog for managing colour palettes

*/

//#include <QDebug>

#include <QBoxLayout>
#include <QLabel>
#include <QWidget>

#include "colourmanagerdialog.h"
#include "ui_colourmanagerdialog.h"

colourManagerDialog::colourManagerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::colourManagerDialog)
{
    ui->setupUi(this);
}

colourManagerDialog::~colourManagerDialog()
{
    delete ui;
}

void colourManagerDialog::on_addButton_clicked()
{
    QLabel *label = new QLabel();
    label->setAutoFillBackground(true);

    QPalette palette = label->palette();
    palette.setColor(label->backgroundRole(), Qt::black);
    palette.setColor(label->foregroundRole(), Qt::yellow);
    label->setPalette(palette);

    QListWidgetItem *item = new QListWidgetItem();
    item->setSizeHint(QSize(0, 30));

    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(label);

    QWidget *widget = new QWidget();
    widget->setLayout(layout);

    ui->listWidget->addItem(item);
    ui->listWidget->setItemWidget(item,widget);
}

void colourManagerDialog::on_removeButton_clicked()
{

}
