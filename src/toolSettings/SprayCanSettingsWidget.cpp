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

// Widget for SprayCanTool settings.

#include "SprayCanSettingsWidget.h"
#include "ui_SprayCanSettingsWidget.h"

SprayCanSettingsWidget::SprayCanSettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SprayCanSettingsWidget)
{
    ui->setupUi(this);

    connect(ui->radiusSpinBox, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &SprayCanSettingsWidget::settingsChanged);
    connect(ui->pressureSlider, &QSlider::valueChanged, this, &SprayCanSettingsWidget::settingsChanged);
    connect(ui->dispersionSlider, &QSlider::valueChanged, this, &SprayCanSettingsWidget::settingsChanged);
    connect(ui->rainbowCheckBox, &QCheckBox::clicked, this, &SprayCanSettingsWidget::settingsChanged);
}

SprayCanSettingsWidget::~SprayCanSettingsWidget()
{
    delete ui;
}

int SprayCanSettingsWidget::radius() const
{
    return ui->radiusSpinBox->value();
}

int SprayCanSettingsWidget::pressure() const
{
    return ui->pressureSlider->value();
}

int SprayCanSettingsWidget::dispersion() const
{
    return ui->dispersionSlider->value();
}

bool SprayCanSettingsWidget::rainbow() const
{
    return ui->rainbowCheckBox->isChecked();
}
