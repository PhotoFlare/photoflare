#include "dropshadowdialog.h"
#include "ui_dropshadowdialog.h"

#include <QColorDialog>

dropshadowDialog::dropshadowDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dropshadowDialog)
{
    ui->setupUi(this);

    for(int i=Qt::white; i <= Qt::yellow; i++)
    {
        QPixmap pixmap(QSize(ui->colorComboBox->width(),ui->colorComboBox->height()));
        pixmap.fill(static_cast<Qt::GlobalColor>(i));
        ui->colorComboBox->addItem(QString(), pixmap);
    }

    ui->colorComboBox->setOnClickHandler(this);
}

dropshadowDialog::~dropshadowDialog()
{
    delete ui;
}

void dropshadowDialog::mousePressEvent(QComboBox* obj, QMouseEvent *e)
{
    if(e->x() < obj->width() - 20)
    {
        QColor selectedColor = QColorDialog::getColor(Qt::white, this);
        if (selectedColor.isValid())
        {
            QPixmap pixmap(QSize(obj->width(),obj->height()));
            pixmap.fill(selectedColor);
            obj->insertItem(0, QString(), pixmap);
            obj->setCurrentIndex(0);
        }
        e->ignore();
    }
    else
    {
        e->accept();
    }
}

QColor dropshadowDialog::color() const
{
    QImage img = (QImage)ui->colorComboBox->currentData().value<QImage>();
    return img.pixel(0,0);
}

int dropshadowDialog::radius() const
{
    return ui->radiusValue->value();
}

int dropshadowDialog::offset() const
{
    return ui->offsetValue->value();
}
