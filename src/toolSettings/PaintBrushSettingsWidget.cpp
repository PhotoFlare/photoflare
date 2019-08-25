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

// Widget for PaintBrushTool settings.

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
    connect(ui->pressureSlider, &QSlider::valueChanged, this, &PaintBrushSettingsWidget::settingsChanged);
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

void PaintBrushSettingsWidget::on_pressureSlider_valueChanged(int value)
{
    ui->labelPressureValue->setText(QString::number(value));
}

int PaintBrushSettingsWidget::brushPressure() const
{
    return ui->pressureSlider->value();
}
