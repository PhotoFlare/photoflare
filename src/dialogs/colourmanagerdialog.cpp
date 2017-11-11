/*

  Dialog for managing colour palettes

*/

//#include <QDebug>

#include <QBoxLayout>
#include <QLabel>
#include <QWidget>
#include <QColorDialog>

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

QColor colourManagerDialog::getColorFromLabel(QWidget *colorLabel) const
{
    return colorLabel->palette().color(colorLabel->backgroundRole());
}

void colourManagerDialog::addColourToPalette()
{
    QColor selectedColor = QColorDialog::getColor(Qt::black, this);
    if (selectedColor.isValid())
    {

    }
}

void colourManagerDialog::on_addButton_clicked()
{
    QLabel *text = new QLabel();
    text->setText("NewPalette");
    text->setFixedSize(80,26);

    QLabel *label1 = new QLabel();
    label1->setStyleSheet("background: black;");
    label1->setFixedSize(26,26);

    QLabel *label2 = new QLabel();
    label2->setStyleSheet("background: red;");
    label2->setFixedSize(26,26);

    QLabel *label3 = new QLabel();
    label3->setStyleSheet("background: blue;");
    label3->setFixedSize(26,26);

    QPushButton *button = new QPushButton();
    button->setFixedSize(26,26);
    button->setIcon(QIcon::fromTheme("add"));
    connect(button, SIGNAL (released()),this, SLOT (addColourToPalette()));

    QListWidgetItem *item = new QListWidgetItem();
    item->setSizeHint(QSize(0, 50));

    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(text);
    layout->addWidget(label1);
    layout->addWidget(label2);
    layout->addWidget(label3);
    layout->addWidget(button);

    QWidget *widget = new QWidget();
    widget->setLayout(layout);

    ui->listWidget->addItem(item);
    ui->listWidget->setItemWidget(item,widget);
}

void colourManagerDialog::on_removeButton_clicked()
{
    qDeleteAll(ui->listWidget->selectedItems());
}
