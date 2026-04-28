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

// Widget for AdvancedPaintbrushTool settings.

#include "PaintBrushAdvSettingsWidget.h"
#include "ui_PaintBrushAdvSettingsWidget.h"
#include "../Settings.h"

PaintBrushAdvSettingsWidget::PaintBrushAdvSettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PaintBrushAdvSettingsWidget)
{
    ui->setupUi(this);

    // Populate brush styles
    for(int i=1; i < 10; i++)
    {
        QPixmap pixmap = QPixmap (QString(":/brushes/assets/brushes/type_%1.png").arg(i));
        ui->comboBox->addItem(QString(), pixmap);
    }

    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SIGNAL(settingsChanged()));
    connect(ui->pressureSlider, SIGNAL(valueChanged(int)), this, SIGNAL(settingsChanged()));
    connect(ui->stepSlider, SIGNAL(valueChanged(int)), this, SIGNAL(settingsChanged()));
    connect(ui->fadeCheckBox, SIGNAL(clicked()), this, SIGNAL(settingsChanged()));
}

PaintBrushAdvSettingsWidget::~PaintBrushAdvSettingsWidget()
{
    delete ui;
}

QPixmap PaintBrushAdvSettingsWidget::brushPixmap()
{
    return ui->comboBox->itemData(ui->comboBox->currentIndex(), Qt::UserRole).value<QPixmap>();
}

int PaintBrushAdvSettingsWidget::brushWidth() const
{
    return ui->comboBox->height();
}

int PaintBrushAdvSettingsWidget::pressure() const
{
    return ui->pressureSlider->value();
}

bool PaintBrushAdvSettingsWidget::fade() const
{
    return ui->fadeCheckBox->isChecked();
}

int PaintBrushAdvSettingsWidget::step() const
{
    return ui->stepSlider->value();
}

void PaintBrushAdvSettingsWidget::saveSettings() const
{
    SETTINGS->setValue("toolSettings/paintBrushAdv/brushType", ui->comboBox->currentIndex());
    SETTINGS->setValue("toolSettings/paintBrushAdv/pressure", pressure());
    SETTINGS->setValue("toolSettings/paintBrushAdv/fade", fade());
    SETTINGS->setValue("toolSettings/paintBrushAdv/step", step());
}

void PaintBrushAdvSettingsWidget::loadSettings()
{
    ui->comboBox->setCurrentIndex(SETTINGS->value("toolSettings/paintBrushAdv/brushType", 0).toInt());
    ui->pressureSlider->setValue(SETTINGS->value("toolSettings/paintBrushAdv/pressure", 50).toInt());
    ui->fadeCheckBox->setChecked(SETTINGS->value("toolSettings/paintBrushAdv/fade", false).toBool());
    ui->stepSlider->setValue(SETTINGS->value("toolSettings/paintBrushAdv/step", 1).toInt());
}
