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

#include "erasersettingswidget.h"
#include "ui_erasersettingswidget.h"
#include "../Settings.h"

EraserSettingsWidget::EraserSettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EraserSettingsWidget)
{
    ui->setupUi(this);

    connect(ui->radiusSlider, &QSlider::valueChanged, this, &EraserSettingsWidget::settingsChanged);
}

EraserSettingsWidget::~EraserSettingsWidget()
{
    delete ui;
}

int EraserSettingsWidget::radius() const
{
    return ui->radiusSlider->value();
}

void EraserSettingsWidget::on_radiusSlider_valueChanged(int value)
{
    ui->radiusValueLabel->setText(QString::number(value));
}

void EraserSettingsWidget::saveSettings() const
{
    SETTINGS->setValue("toolSettings/eraser/radius", radius());
}

void EraserSettingsWidget::loadSettings()
{
    ui->radiusSlider->setValue(SETTINGS->value("toolSettings/eraser/radius", 10).toInt());
}
