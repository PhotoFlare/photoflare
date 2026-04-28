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

// Widget for BlurTool settings.

#include "BlurSettingsWidget.h"
#include "ui_BlurSettingsWidget.h"
#include "../Settings.h"

BlurSettingsWidget::BlurSettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BlurSettingsWidget)
{
    ui->setupUi(this);

    connect(ui->radiusSpinBox, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &BlurSettingsWidget::settingsChanged);
    connect(ui->pressureSlider, &QSlider::valueChanged, this, &BlurSettingsWidget::settingsChanged);
}

BlurSettingsWidget::~BlurSettingsWidget()
{
    delete ui;
}

int BlurSettingsWidget::radius() const
{
    return ui->radiusSpinBox->value();
}

int BlurSettingsWidget::pressure() const
{
    return ui->pressureSlider->value();
}

void BlurSettingsWidget::saveSettings() const
{
    SETTINGS->setValue("toolSettings/blur/radius", radius());
    SETTINGS->setValue("toolSettings/blur/pressure", pressure());
}

void BlurSettingsWidget::loadSettings()
{
    ui->radiusSpinBox->setValue(SETTINGS->value("toolSettings/blur/radius", 10).toInt());
    ui->pressureSlider->setValue(SETTINGS->value("toolSettings/blur/pressure", 50).toInt());
}
