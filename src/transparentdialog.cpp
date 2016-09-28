#include "transparentdialog.h"
#include "ui_transparentdialog.h"

TransparentDialog::TransparentDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TransparentDialog)
{
    ui->setupUi(this);

    setColor(Qt::white);
}

TransparentDialog::~TransparentDialog()
{
    delete ui;
}

void TransparentDialog::on_horizontalSlider_sliderMoved(int value)
{
}

void TransparentDialog::setColor(QRgb color)
{
    m_color = QColor(color);
    QPalette palette = ui->color->palette();
    palette.setColor(ui->color->backgroundRole(), m_color);
    ui->color->setPalette(palette);
}

void TransparentDialog::on_horizontalSlider_valueChanged(int value)
{
    ui->toleranceValue->setText(QString::number(value));
}

int TransparentDialog::tolerance()
{
    return ui->horizontalSlider->value();
}

void TransparentDialog::on_pushButton_clicked()
{
    emit previewTransparent(m_color, ui->horizontalSlider->value());
}

void TransparentDialog::on_pushButton_2_clicked()
{
    emit dialogAccepted();
    accept();
    emit dialogFinished(0);
}

void TransparentDialog::on_pushButton_3_clicked()
{
    emit dialogRejected();
    reject();
    emit dialogFinished(0);
}

void TransparentDialog::closeEvent (QCloseEvent *event)
{
    emit dialogRejected();
    reject();
    emit dialogFinished(0);
    QDialog::closeEvent(event);
}
