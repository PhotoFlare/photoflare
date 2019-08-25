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

// Widget for LineTool settings.

#include <QGraphicsPixmapItem>

#include "LineSettingsWidget.h"
#include "ui_LineSettingsWidget.h"

LineSettingsWidget::LineSettingsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LineSettingsWidget)
{
    ui->setupUi(this);

    for(int i=1; i <= 6; i++)
    {
        QPixmap pixmap = QPixmap (QString(":/lines/assets/lines/type_%1.png").arg(i));
        ui->styleComboBox->addItem(QString(), pixmap);
    }

    connect(ui->styleComboBox, SIGNAL(currentIndexChanged(int)), this, SIGNAL(settingsChanged()));
    connect(ui->widthSpinBox, SIGNAL(valueChanged(int)), this, SIGNAL(settingsChanged()));
    connect(ui->opacitySpinBox, SIGNAL(valueChanged(int)), this, SIGNAL(settingsChanged()));
    connect(ui->antialiasCheckBox, SIGNAL(clicked()), this, SIGNAL(settingsChanged()));

    connect(ui->arrowCheckBox, SIGNAL(clicked()), this, SIGNAL(settingsChanged()));
    connect(ui->arrowCheckBox2, SIGNAL(clicked()), this, SIGNAL(settingsChanged()));
}

LineSettingsWidget::~LineSettingsWidget()
{
    delete ui;
}

int LineSettingsWidget::width()
{
    return ui->widthSpinBox->value();
}

int LineSettingsWidget::opacity()
{
    return ui->opacitySpinBox->value();
}

bool LineSettingsWidget::antialias()
{
    return ui->antialiasCheckBox->isChecked();
}

int LineSettingsWidget::style()
{
    return ui->styleComboBox->currentIndex();
}

int LineSettingsWidget::arrowStyle()
{
    if(ui->arrowCheckBox2->isChecked())
        return 2;
    else if(ui->arrowCheckBox->isChecked())
        return 1;
    else
        return 0;
}

void LineSettingsWidget::on_arrowCheckBox_clicked(bool checked)
{
    Q_UNUSED(checked);
    ui->arrowCheckBox2->setChecked(false);
}

void LineSettingsWidget::on_arrowCheckBox2_clicked(bool checked)
{
    Q_UNUSED(checked);
    ui->arrowCheckBox->setChecked(false);
}
