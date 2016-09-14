/*

  Widget for paintbrush settings.

*/


#include "PaintBrushSettingsWidget.h"
#include "ui_PaintBrushSettingsWidget.h"

PaintBrushSettingsWidget::PaintBrushSettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PaintBrushSettingsWidget)
{
    ui->setupUi(this);

    connect(ui->buttonRoundCap, &QToolButton::clicked, this, &PaintBrushSettingsWidget::settingsChanged);
    connect(ui->buttonSquareCap, &QToolButton::clicked, this, &PaintBrushSettingsWidget::settingsChanged);

    connect(ui->sliderRadius, &QSlider::valueChanged, this, &PaintBrushSettingsWidget::setLabelRadiusValue);
    connect(ui->sliderRadius, &QSlider::valueChanged, this, &PaintBrushSettingsWidget::settingsChanged);
    connect(ui->cbAntialias, &QCheckBox::clicked, this, &PaintBrushSettingsWidget::settingsChanged);
    setLabelRadiusValue(ui->sliderRadius->value());
}

PaintBrushSettingsWidget::~PaintBrushSettingsWidget()
{
    delete ui;
}

void PaintBrushSettingsWidget::setAntialiasing(bool value)
{
    ui->cbAntialias->setChecked(value);
}

bool PaintBrushSettingsWidget::antialiasing() const
{
    return ui->cbAntialias->isChecked();
}

void PaintBrushSettingsWidget::setBrushWidth(int width)
{
    ui->sliderRadius->setValue(width);
}

int PaintBrushSettingsWidget::brushWidth() const
{
    return ui->sliderRadius->value();
}

void PaintBrushSettingsWidget::setLabelRadiusValue(int value)
{
    ui->labelRadiusValue->setText(QString::number(value));
}

Qt::PenCapStyle PaintBrushSettingsWidget::brushCapStyle()
{
    if(ui->buttonRoundCap->isChecked())
        return Qt::RoundCap;
    else
        return Qt::SquareCap;
}

void PaintBrushSettingsWidget::on_buttonRoundCap_clicked()
{
    ui->buttonRoundCap->setChecked(true);
    ui->buttonSquareCap->setChecked(false);
}

void PaintBrushSettingsWidget::on_buttonSquareCap_clicked()
{
    ui->buttonRoundCap->setChecked(false);
    ui->buttonSquareCap->setChecked(true);
}
