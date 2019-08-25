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

// Widget for SmudgeTool settings.

#include "SmudgeSettingsWidget.h"
#include "ui_smudgesettingswidget.h"

SmudgeSettingsWidget::SmudgeSettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SmudgeSettingsWidget)
{
    ui->setupUi(this);

    connect(ui->radiusSpinBox, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), this, &SmudgeSettingsWidget::settingsChanged);
    connect(ui->pressureSlider, &QSlider::valueChanged, this, &SmudgeSettingsWidget::settingsChanged);
}

SmudgeSettingsWidget::~SmudgeSettingsWidget()
{
    delete ui;
}

int SmudgeSettingsWidget::radius() const
{
    return ui->radiusSpinBox->value();
}

int SmudgeSettingsWidget::pressure() const
{
    return ui->pressureSlider->value();
}
