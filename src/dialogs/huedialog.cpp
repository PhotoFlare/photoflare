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

// Hue dialog class for the Photoflare application.

#include <QMouseEvent>
#include <QColorDialog>
#include <QSettings>

#include "huedialog.h"
#include "ui_huedialog.h"
#include "../Settings.h"

HueDialog::HueDialog(QWidget *parent, QImage preview) :
    QDialog(parent),
    ui(new Ui::HueDialog)
{
    ui->setupUi(this);
    setFixedSize(size());

    ui->spectrumLabel->installEventFilter(this);
    ui->spectrumLabel->setStyleSheet("background-color: qlineargradient(x1: 0, y1: 0, x2: 1, y2: 0, stop: 0 red, stop: 0.16 yellow, stop: 0.32 green, \
                                    stop: 0.48 cyan, stop: 0.64 blue, stop: 0.8 magenta, stop: 1 red);");
    ui->colorLabel->installEventFilter(this);

    if(SETTINGS->getMemParamsEnabled())
    {
        readSettings(this);
    }
    m_preview = preview.scaled(240,180);
    setPreviewImage(m_preview);
}

HueDialog::~HueDialog()
{
    delete ui;
}

void HueDialog::setColor(const QColor &color, QWidget *colorLabel)
{
    QPalette palette = colorLabel->palette();
    palette.setColor(colorLabel->backgroundRole(), color);
    colorLabel->setPalette(palette);

    m_color = color;

    // Hide degrees as we don't know how to convert a color to a degree value
    ui->horizontalSlider->setValue(0);
    ui->valueLabel->setText("");

    emit huePreviewChanged(m_preview, ui->method1->isChecked(), color, 0);
}

QColor HueDialog::getColorFromLabel(QWidget *colorLabel) const
{
    return colorLabel->palette().color(colorLabel->backgroundRole());
}

void HueDialog::showColorDialog(QWidget *colorLabel)
{
    QColor selectedColor = QColorDialog::getColor(getColorFromLabel(colorLabel), this);
    if (selectedColor.isValid()) {
        setColor(selectedColor, colorLabel);
    }
}

bool HueDialog::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == ui->spectrumLabel)
    {
        QMouseEvent *mevent = (QMouseEvent*)event;
        if(mevent->button() == Qt::LeftButton)
        {
            QLabel* label = (QLabel*)obj;
            QPixmap pxm = label->grab();
            QColor color = QColor(pxm.toImage().pixel(mevent->x(),mevent->y()));
            QPalette palette = ui->colorLabel->palette();
            palette.setColor(ui->colorLabel->backgroundRole(), color);
            ui->colorLabel->setPalette(palette);
            int value = (int)(359.0f*((float)mevent->x()/(float)label->width()));
            ui->valueLabel->setText(QString::number(value) + "°");
            ui->horizontalSlider->setValue(value);
            m_color = color;
            m_degrees = value;
            emit huePreviewChanged(m_preview, ui->method1->isChecked(), color, value);
            return true;
        }
    }
    if(obj == ui->colorLabel) {
        if (event->type() == QEvent::MouseButtonRelease) {
            showColorDialog(static_cast<QWidget *>(obj));
            return true;
        }
    }
    return QWidget::eventFilter(obj, event);
}

void HueDialog::on_horizontalSlider_valueChanged(int value)
{
    ui->valueLabel->setText(QString::number(value) + "°");
    QPixmap pxm = ui->spectrumLabel->grab();
    int x = value*ui->spectrumLabel->width()/360.0f;
    QColor color = QColor(pxm.toImage().pixel(x,5));
    QPalette palette = ui->colorLabel->palette();
    palette.setColor(ui->colorLabel->backgroundRole(), color);
    ui->colorLabel->setPalette(palette);
    m_color = color;
    m_degrees = value;
    emit huePreviewChanged(m_preview, ui->method1->isChecked(), color, value);
}

void HueDialog::setPreviewImage(const QImage& image)
{
    ui->previewLabel->setPixmap(QPixmap::fromImage(image));
    ui->previewLabel->setScaledContents(true);
    if(SETTINGS->getMemParamsEnabled())
    {
        writeSettings(this);
    }
}

void HueDialog::on_method1_toggled()
{
    on_horizontalSlider_valueChanged(ui->horizontalSlider->value());
}

bool HueDialog::isColorizeMethod() const
{
    return ui->method1->isChecked();
}

void HueDialog::writeSettings(QWidget* window)
{
    QSettings settings;

    settings.beginGroup(window->objectName());
    settings.setValue("pos", window->pos());
    settings.setValue("color", m_color.name());
    settings.setValue("degrees", m_degrees);
    settings.setValue("hue", ui->method2->isChecked());
    settings.endGroup();
}

void HueDialog::readSettings(QWidget* window)
{
    QSettings settings;

    settings.beginGroup(window->objectName());
    QVariant value = settings.value("pos");
    if (!value.isNull())
    {
        window->move(settings.value("pos").toPoint());
        setColor(settings.value("color").value<QColor>(), ui->colorLabel);
        int value = settings.value("degrees").toInt();
        ui->valueLabel->setText(QString::number(value) + "°");
        ui->horizontalSlider->setValue(value);
        ui->method2->setChecked(settings.value("hue").toBool());
    }
    settings.endGroup();
}
