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

// Outerframe dialog class for the PhotoFlare application.

#include <QColorDialog>

#include "outerframedialog.h"
#include "ui_outerframedialog.h"

OuterFrameDialog::OuterFrameDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OuterFrameDialog)
{
    ui->setupUi(this);
    setFixedSize(size());

    for(int i=Qt::white; i <= Qt::yellow; i++)
    {
        QPixmap pixmap(QSize(ui->colorComboBox->width(),ui->colorComboBox->height()));
        pixmap.fill(static_cast<Qt::GlobalColor>(i));
        ui->colorComboBox->addItem(QString(), pixmap);
    }

    ui->colorComboBox->setOnClickHandler(this);
}

OuterFrameDialog::~OuterFrameDialog()
{
    delete ui;
}

void OuterFrameDialog::mousePressEvent(QComboBox* obj, QMouseEvent *e)
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

QColor OuterFrameDialog::color() const
{
    QImage img = (QImage)ui->colorComboBox->currentData().value<QImage>();
    return img.pixel(0,0);
}

int OuterFrameDialog::width() const
{
    return ui->colorValue->value();
}
