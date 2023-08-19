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

#include "pointersettingswidget.h"
#include "ui_pointersettingswidget.h"

PointerSettingsWidget::PointerSettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PointerSettingsWidget)
{
    ui->setupUi(this);

    connect(ui->checkBoxStroke, SIGNAL(clicked()), this, SIGNAL(settingsChanged()));
    connect(ui->checkBoxFill, SIGNAL(clicked()), this, SIGNAL(settingsChanged()));

    ui->checkBoxStroke->setHidden(true);
    ui->square_sel->setHidden(true); // Hide until we have more selection types available
    ui->circle_sel->setHidden(true);
}

PointerSettingsWidget::~PointerSettingsWidget()
{
    delete ui;
}

bool PointerSettingsWidget::stroke() const
{
    return ui->checkBoxStroke->isChecked();
}

bool PointerSettingsWidget::fill() const
{
    return ui->checkBoxFill->isChecked();
}

void PointerSettingsWidget::on_square_sel_clicked(bool checked)
{
    if(checked)
    {
        ui->square_sel->setChecked(true);
        ui->circle_sel->setChecked(false);
    }
}

void PointerSettingsWidget::on_circle_sel_clicked(bool checked)
{
    if(checked)
    {
        ui->square_sel->setChecked(false);
        ui->circle_sel->setChecked(true);
    }
}
