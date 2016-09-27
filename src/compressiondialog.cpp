#include "compressiondialog.h"
#include "ui_compressiondialog.h"

CompressionDialog::CompressionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CompressionDialog)
{
    ui->setupUi(this);
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
