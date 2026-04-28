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

// Widget for StampTool settings.

#include "StampSettingsWidget.h"
#include "ui_StampSettingsWidget.h"
#include "../Settings.h"

StampSettingsWidget::StampSettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StampSettingsWidget)
{
    ui->setupUi(this);

    ui->fixedCheckBox->setChecked(false);
    ui->stepSlider->setEnabled(false);

    connect(ui->radiusSpinBox, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &StampSettingsWidget::settingsChanged);
    connect(ui->pressureSlider, &QSlider::valueChanged, this, &StampSettingsWidget::settingsChanged);
    connect(ui->stepSlider, &QSlider::valueChanged, this, &StampSettingsWidget::settingsChanged);
    connect(ui->fixedCheckBox, &QCheckBox::clicked, this, &StampSettingsWidget::settingsChanged);
    connect(ui->preciseCheckBox, &QCheckBox::clicked, this, &StampSettingsWidget::settingsChanged);
    connect(ui->diffuseCheckBox, &QCheckBox::clicked, this, &StampSettingsWidget::settingsChanged);
    connect(ui->fixedCheckBox, &QCheckBox::clicked, this, &StampSettingsWidget::onFixedChanged);
}

StampSettingsWidget::~StampSettingsWidget()
{
    delete ui;
}

int StampSettingsWidget::radius() const
{
    return ui->radiusSpinBox->value();
}

int StampSettingsWidget::pressure() const
{
    return ui->pressureSlider->value();
}

int StampSettingsWidget::step() const
{
    return ui->stepSlider->value();
}

bool StampSettingsWidget::fixed() const
{
    return ui->fixedCheckBox->isChecked();
}

bool StampSettingsWidget::diffuse() const
{
    return ui->diffuseCheckBox->isChecked();
}

bool StampSettingsWidget::precise() const
{
    return ui->preciseCheckBox->isChecked();
}

void StampSettingsWidget::onFixedChanged()
{
    ui->stepSlider->setEnabled(ui->fixedCheckBox->isChecked());
}

void StampSettingsWidget::saveSettings() const
{
    SETTINGS->setValue("toolSettings/stamp/radius", radius());
    SETTINGS->setValue("toolSettings/stamp/pressure", pressure());
    SETTINGS->setValue("toolSettings/stamp/step", step());
    SETTINGS->setValue("toolSettings/stamp/fixed", fixed());
    SETTINGS->setValue("toolSettings/stamp/diffuse", diffuse());
    SETTINGS->setValue("toolSettings/stamp/precise", precise());
}

void StampSettingsWidget::loadSettings()
{
    ui->radiusSpinBox->setValue(SETTINGS->value("toolSettings/stamp/radius", 10).toInt());
    ui->pressureSlider->setValue(SETTINGS->value("toolSettings/stamp/pressure", 50).toInt());
    ui->fixedCheckBox->setChecked(SETTINGS->value("toolSettings/stamp/fixed", false).toBool());
    ui->stepSlider->setValue(SETTINGS->value("toolSettings/stamp/step", 1).toInt());
    ui->stepSlider->setEnabled(ui->fixedCheckBox->isChecked());
    ui->diffuseCheckBox->setChecked(SETTINGS->value("toolSettings/stamp/diffuse", false).toBool());
    ui->preciseCheckBox->setChecked(SETTINGS->value("toolSettings/stamp/precise", false).toBool());
}
