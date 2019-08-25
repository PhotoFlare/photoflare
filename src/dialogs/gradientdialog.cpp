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

// Gradient dialog class for the PhotoFlare application.

#include <QMouseEvent>
#include <QColorDialog>
#include <QSettings>

#include "gradientdialog.h"
#include "ui_gradientdialog.h"
#include "../Settings.h"

GradientDialog::GradientDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GradientDialog)
{
    ui->setupUi(this);
    setFixedSize(size());

    for(int i=Qt::white; i <= Qt::yellow; i++)
    {
        QPixmap pixmap(QSize(ui->startColorComboBox->width(),ui->startColorComboBox->height()));
        pixmap.fill(static_cast<Qt::GlobalColor>(i));
        ui->startColorComboBox->addItem(QString(), pixmap);
    }

    for(int i=Qt::white; i <= Qt::yellow; i++)
    {
        QPixmap pixmap(QSize(ui->stopColorComboBox->width(),ui->stopColorComboBox->height()));
        pixmap.fill(static_cast<Qt::GlobalColor>(i));
        ui->stopColorComboBox->addItem(QString(), pixmap);
    }

    ui->startColorComboBox->setOnClickHandler(this);
    ui->stopColorComboBox->setOnClickHandler(this);

    ui->opacityValue->setText("50%");
    ui->opacityValue_2->setText("50%");

    if(SETTINGS->getMemParamsEnabled())
    {
        readSettings(this);
    }
}

GradientDialog::~GradientDialog()
{
    delete ui;
}

// For reading the direction from settings
void GradientDialog::setDirection(int direction)
{
    if(direction == 0)
    {
        ui->directionN->setChecked(true);
    }
    else if(direction == 1)
    {
        ui->directionNE->setChecked(true);
    }
    else if(direction == 2)
    {
        ui->directionE->setChecked(true);
    }
    else if(direction == 3)
    {
        ui->directionSE->setChecked(true);
    }
    else if(direction == 4)
    {
        ui->directionS->setChecked(true);
    }
    else if(direction == 5)
    {
        ui->directionSW->setChecked(true);
    }
    else if(direction == 6)
    {
        ui->directionW->setChecked(true);
    }
    else if(direction == 7)
    {
        ui->directionNW->setChecked(true);
    }
}

void GradientDialog::applyDirection()
{
    if(ui->directionN->isChecked())
    {
        m_direction = N;
    }
    else if(ui->directionNE->isChecked())
    {
        m_direction = NE;
    }
    else if(ui->directionE->isChecked())
    {
        m_direction = E;
    }
    else if(ui->directionSE->isChecked())
    {
        m_direction = SE;
    }
    else if(ui->directionS->isChecked())
    {
        m_direction = S;
    }
    else if(ui->directionSW->isChecked())
    {
        m_direction = SW;
    }
    else if(ui->directionW->isChecked())
    {
        m_direction = W;
    }
    else if(ui->directionNW->isChecked())
    {
        m_direction = NW;
    }
}

void GradientDialog::on_buttonBox_accepted()
{
    applyDirection();
    if(SETTINGS->getMemParamsEnabled())
    {
        writeSettings(this);
    }
}

void GradientDialog::on_startOpacity_valueChanged(int value)
{
    ui->opacityValue->setText(QString("%1%").arg(value));
}

void GradientDialog::on_stopOpacity_valueChanged(int value)
{
    ui->opacityValue_2->setText(QString("%1%").arg(value));
}

void GradientDialog::mousePressEvent(QComboBox* obj, QMouseEvent *e)
{
    if(e->x() < obj->width() - 20)
    {
        QColor selectedColor = QColorDialog::getColor(Qt::white, this);
        if (selectedColor.isValid())
        {
            setComboColor(obj, selectedColor);
        }
        e->ignore();
    }
    else
    {
        e->accept();
    }
}

void GradientDialog::setComboColor(QComboBox *obj, QColor c)
{
    QPixmap pixmap(QSize(obj->width(),obj->height()));
    pixmap.fill(c);
    obj->insertItem(0, QString(), pixmap);
    obj->setCurrentIndex(0);
}

QColor GradientDialog::startColor()
{
    QImage img = (QImage)ui->startColorComboBox->currentData().value<QImage>();
    QColor color = img.pixel(0,0);
    return QColor(color.red(), color.green(), color.blue(), ui->startOpacity->value()*255.0/100.0);
}

QColor GradientDialog::stopColor()
{
    if(ui->duatone->isChecked())
    {
        QImage img = (QImage)ui->stopColorComboBox->currentData().value<QImage>();
        QColor color = img.pixel(0,0);
        return QColor(color.red(), color.green(), color.blue(), ui->stopOpacity->value()*255.0/100.0);
    }
    else
    {
        QImage img = (QImage)ui->startColorComboBox->currentData().value<QImage>();
        QColor color = img.pixel(0,0);
        return QColor(color.red(), color.green(), color.blue(), 0);
    }
}

QColor GradientDialog::startColorName()
{
    QImage img = (QImage)ui->startColorComboBox->currentData().value<QImage>();
    QColor color = img.pixel(0,0);
    return color.name();
}

QColor GradientDialog::stopColorName()
{
    QImage img = (QImage)ui->stopColorComboBox->currentData().value<QImage>();
    QColor color = img.pixel(0,0);
    return color.name();
}

void GradientDialog::on_monochromatic_toggled(bool checked)
{
    ui->stopColorGroupBox->setEnabled(!checked);
}

void GradientDialog::writeSettings(QWidget* window)
{
    QSettings settings;

    settings.beginGroup(window->objectName());
    settings.setValue("pos", window->pos());
    settings.setValue("startcolor", startColorName());
    settings.setValue("stopcolor", stopColorName());
    settings.setValue("direction", m_direction);
    settings.setValue("startopacity", ui->startOpacity->value());
    settings.setValue("stopopacity", ui->stopOpacity->value());
    settings.setValue("mono", ui->monochromatic->isChecked());
    settings.endGroup();
}

void GradientDialog::readSettings(QWidget* window)
{
    QSettings settings;

    settings.beginGroup(window->objectName());
    QVariant value = settings.value("pos");
    if (!value.isNull())
    {
        window->move(settings.value("pos").toPoint());
        setComboColor(ui->startColorComboBox, settings.value("startcolor").value<QColor>());
        ui->startOpacity->setValue(settings.value("startopacity").toInt());
        setComboColor(ui->stopColorComboBox, settings.value("stopcolor").value<QColor>());
        ui->stopOpacity->setValue(settings.value("stopopacity").toInt());
        setDirection(settings.value("direction").toInt());
        ui->monochromatic->setChecked(settings.value("mono").toBool());
    }
    settings.endGroup();
}

void GradientDialog::on_directionN_clicked(bool checked)
{
    if(!checked)
    {
        ui->directionN->setChecked(true);
    }
}

void GradientDialog::on_directionNE_clicked(bool checked)
{
    if(!checked)
    {
        ui->directionNE->setChecked(true);
    }
}

void GradientDialog::on_directionE_clicked(bool checked)
{
    if(!checked)
    {
        ui->directionE->setChecked(true);
    }
}

void GradientDialog::on_directionSE_clicked(bool checked)
{
    if(!checked)
    {
        ui->directionSE->setChecked(true);
    }
}

void GradientDialog::on_directionS_clicked(bool checked)
{
    if(!checked)
    {
        ui->directionS->setChecked(true);
    }
}

void GradientDialog::on_directionSW_clicked(bool checked)
{
    if(!checked)
    {
        ui->directionSW->setChecked(true);
    }
}

void GradientDialog::on_directionW_clicked(bool checked)
{
    if(!checked)
    {
        ui->directionW->setChecked(true);
    }
}

void GradientDialog::on_directionNW_clicked(bool checked)
{
    if(!checked)
    {
        ui->directionNW->setChecked(true);
    }
}
