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
    connect(ui->buttonRoundCap, &QToolButton::clicked, this, &EraserSettingsWidget::settingsChanged);
    connect(ui->buttonSquareCap, &QToolButton::clicked, this, &EraserSettingsWidget::settingsChanged);
}

EraserSettingsWidget::~EraserSettingsWidget()
{
    delete ui;
}

void EraserSettingsWidget::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    if (e->type() == QEvent::LanguageChange)
        ui->retranslateUi(this);
}

int EraserSettingsWidget::radius() const
{
    return ui->radiusSlider->value();
}

Qt::PenCapStyle EraserSettingsWidget::capStyle() const
{
    return ui->buttonRoundCap->isChecked() ? Qt::RoundCap : Qt::SquareCap;
}

void EraserSettingsWidget::on_radiusSlider_valueChanged(int value)
{
    ui->radiusValueLabel->setText(QString::number(value));
}

void EraserSettingsWidget::on_buttonRoundCap_clicked()
{
    ui->buttonRoundCap->setChecked(true);
    ui->buttonSquareCap->setChecked(false);
}

void EraserSettingsWidget::on_buttonSquareCap_clicked()
{
    ui->buttonRoundCap->setChecked(false);
    ui->buttonSquareCap->setChecked(true);
}

void EraserSettingsWidget::saveSettings() const
{
    SETTINGS->setValue("toolSettings/eraser/radius", radius());
    SETTINGS->setValue("toolSettings/eraser/roundCap", capStyle() == Qt::RoundCap);
}

void EraserSettingsWidget::loadSettings()
{
    ui->radiusSlider->setValue(SETTINGS->value("toolSettings/eraser/radius", 10).toInt());
    // The eraser has always been square, so keep that as the default.
    const bool roundCap = SETTINGS->value("toolSettings/eraser/roundCap", false).toBool();
    ui->buttonRoundCap->setChecked(roundCap);
    ui->buttonSquareCap->setChecked(!roundCap);
}
