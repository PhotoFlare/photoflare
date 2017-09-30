/*

  Transparent dialog class.

*/

#include <QSettings>

#include "transparentdialog.h"
#include "ui_transparentdialog.h"
#include "../Settings.h"

TransparentDialog::TransparentDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TransparentDialog)
{
    ui->setupUi(this);
    setFixedSize(size());

    setColor(Qt::white);

    if(SETTINGS->getMemParamsEnabled())
    {
        readSettings(this);
    }
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
    if(SETTINGS->getMemParamsEnabled())
    {
        writeSettings(this);
    }
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

void TransparentDialog::readSettings(QWidget* window)
{
    QSettings settings;

    settings.beginGroup(window->objectName());
    QVariant value = settings.value("pos");
    if (!value.isNull())
    {
        window->move(settings.value("pos").toPoint());
        window->resize(settings.value("size").toSize());
        ui->horizontalSlider->setValue(settings.value("tolerance").toInt());
    }
    settings.endGroup();
}

void TransparentDialog::writeSettings(QWidget* window)
{
    QSettings settings;

    settings.beginGroup(window->objectName());
    settings.setValue("pos", window->pos());
    settings.setValue("size", window->size());
    settings.setValue("tolerance", tolerance());
    settings.endGroup();
}
