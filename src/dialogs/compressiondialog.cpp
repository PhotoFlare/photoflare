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

// Compression dialog to give user option to set compression on save.

#include "compressiondialog.h"
#include "ui_compressiondialog.h"

CompressionDialog::CompressionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CompressionDialog)
{
    ui->setupUi(this);
    setFixedSize(size());
}

CompressionDialog::~CompressionDialog()
{
    delete ui;
}

void CompressionDialog::on_horizontalSlider_sliderMoved(int position)
{
    ui->valueLabel->setText(QString::number(position));
}

int CompressionDialog::quality() const
{
    return ui->horizontalSlider->value();
}

void CompressionDialog::on_buttonBox_accepted()
{
    this->enableSaveImage = true;
}

void CompressionDialog::on_buttonBox_rejected()
{
    this->enableSaveImage = false;
}

void CompressionDialog::on_horizontalSlider_valueChanged(int value)
{
    ui->valueLabel->setText(QString::number(value));
}
