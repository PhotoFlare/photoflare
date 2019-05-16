/*

  Compression dialog to give user option to set compression on save.

*/

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
